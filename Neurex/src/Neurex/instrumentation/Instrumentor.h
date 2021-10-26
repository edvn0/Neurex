#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "Neurex/core/Logger.h"

namespace Neurex {

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
	std::string Name;

	FloatingPointMicroseconds Start;
	std::chrono::microseconds ElapsedTime;
	std::thread::id ThreadID;
};

struct InstrumentationSession {
	std::string Name;
};

class Instrumentor {
public:
	Instrumentor(const Instrumentor&) = delete;
	Instrumentor(Instrumentor&&) = delete;

	void begin_session(
		const std::string& name, const std::string& filepath = "results.json")
	{
		std::lock_guard lock(instrumentor_lock);
		if (current_session) {
			// If there is already a current session, then close it before
			// beginning new one. Subsequent profiling output meant for the
			// original session will end up in the newly opened session instead.
			// That's better than having badly formatted profiling output.
			if (Logger::get_core_logger()) // Edge case: begin_session() might
										   // be before Log::Init()
			{
				NX_CORE_ERROR("Instrumentor::begin_session('{0}') when session "
							  "'{1}' already open.",
					name, current_session->Name);
			}
			internal_end_session();
		}
		output_stream.open(filepath);

		if (output_stream.is_open()) {
			current_session = new InstrumentationSession({ name });
			write_header();
		} else {
			if (Logger::get_core_logger()) // Edge case: begin_session() might
										   // be before Log::Init()
			{
				NX_CORE_ERROR("Instrumentor could not open results file '{0}'.",
					filepath);
			}
		}
	}

	void end_session()
	{
		std::lock_guard lock(instrumentor_lock);
		internal_end_session();
	}

	void write_profile(const ProfileResult& result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":\"" << result.ThreadID << "\",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		std::lock_guard lock(instrumentor_lock);
		if (current_session) {
			output_stream << json.str();
			output_stream.flush();
		}
	}

	static Instrumentor& get()
	{
		static Instrumentor instance;
		return instance;
	}

private:
	Instrumentor()
		: current_session(nullptr)
	{
	}

	~Instrumentor() { end_session(); }

	void write_header()
	{
		output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
		output_stream.flush();
	}

	void write_footer()
	{
		output_stream << "]}";
		output_stream.flush();
	}

	// Note: you must already own lock on instrumentor_lock before
	// calling internal_end_session()
	void internal_end_session()
	{
		if (current_session) {
			write_footer();
			output_stream.close();
			delete current_session;
			current_session = nullptr;
		}
	}

private:
	std::mutex instrumentor_lock;
	InstrumentationSession* current_session;
	std::ofstream output_stream;
};

class InstrumentationTimer {
public:
	InstrumentationTimer(const char* name_)
		: name(name_)
		, stopped(false)
	{
		start_time_point = std::chrono::steady_clock::now();
	}

	~InstrumentationTimer()
	{
		if (!stopped)
			stop();
	}

	void stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart
			= FloatingPointMicroseconds{ start_time_point.time_since_epoch() };
		auto elapsedTime
			= std::chrono::time_point_cast<std::chrono::microseconds>(
				  endTimepoint)
				  .time_since_epoch()
			- std::chrono::time_point_cast<std::chrono::microseconds>(
				start_time_point)
				  .time_since_epoch();

		Instrumentor::get().write_profile(
			{ name, highResStart, elapsedTime, std::this_thread::get_id() });

		stopped = true;
	}

private:
	const char* name;
	std::chrono::time_point<std::chrono::steady_clock> start_time_point;
	bool stopped;
};

namespace InstrumentorUtils {

	template <size_t N> struct ChangeResult {
		char data[N];
	};

	template <size_t N, size_t K>
	constexpr auto cleanup_output_string(
		const char (&expr)[N], const char (&remove)[K])
	{
		ChangeResult<N> result = {};

		size_t src_index = 0;
		size_t destination_index = 0;
		while (src_index < N) {
			size_t matching_index = 0;
			while (matching_index < K - 1 && src_index + matching_index < N - 1
				&& expr[src_index + matching_index] == remove[matching_index])
				matching_index++;
			if (matching_index == K - 1)
				src_index += matching_index;
			result.data[destination_index++]
				= expr[src_index] == '"' ? '\'' : expr[src_index];
			src_index++;
		}
		return result;
	}
}
}

#define NX_PROFILE 1
#if NX_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000))       \
	|| (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define NX_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define NX_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define NX_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600))                 \
	|| (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define NX_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define NX_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define NX_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define NX_FUNC_SIG __func__
#else
#define NX_FUNC_SIG "NX_FUNC_SIG unknown!"
#endif

#define NX_PROFILE_BEGIN_SESSION(name, filepath)                               \
	::Neurex::Instrumentor::get().begin_session(name, filepath)
#define NX_PROFILE_END_SESSION() ::Neurex::Instrumentor::get().end_session()
#define NX_PROFILE_SCOPE_LINE2(name, line)                                     \
	constexpr auto fixedName##line                                             \
		= ::Neurex::InstrumentorUtils::cleanup_output_string(                  \
			name, "__cdecl ");                                                 \
	::Neurex::InstrumentationTimer timer##line(fixedName##line.data)
#define NX_PROFILE_SCOPE_LINE(name, line) NX_PROFILE_SCOPE_LINE2(name, line)
#define NX_PROFILE_SCOPE(name) NX_PROFILE_SCOPE_LINE(name, __LINE__)
#define NX_PROFILE_FUNCTION() NX_PROFILE_SCOPE(NX_FUNC_SIG)
#else
#define NX_PROFILE_BEGIN_SESSION(name, filepath)
#define NX_PROFILE_END_SESSION()
#define NX_PROFILE_SCOPE(name)
#define NX_PROFILE_FUNCTION()
#endif