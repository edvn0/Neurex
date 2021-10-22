#pragma once

#include <memory>

#ifdef NX_DEBUG
#define NX_ALLOW_ASSERTS
#endif

#ifdef NX_PT_WIN
#ifdef NX_ALLOW_ASSERTS
#define NX_ASSERT(x, ...)                                       \
	{                                                           \
		if (!(x)) {                                             \
			NX_ERROR("User: Assertion fail. {0}", __VA_ARGS__); \
			__debugbreak();                                     \
		}                                                       \
	}
#define NX_CORE_ASSERT(x, ...)                                       \
	{                                                                \
		if (!(x)) {                                                  \
			NX_CORE_ERROR("Core: Assertion fail. {0}", __VA_ARGS__); \
			__debugbreak();                                          \
		}                                                            \
	}
#else
#define NX_ASSERT(x, ...)
#define NX_CORE_ASSERT(x, ...)
#endif

#define glCreateVA(n, x) glCreateVertexArrays(n, x)
#define glCreateVB(n, x) glCreateVertexBuffers(n, x)

#elif defined(NX_PT_OSX)
#ifdef NX_ALLOW_ASSERTS
#include <stdlib.h>

void abort(void);

#define NX_ASSERT(x, ...)                                       \
	{                                                           \
		if (!(x)) {                                             \
			NX_ERROR("User: Assertion fail: {0}", __VA_ARGS__); \
			abort();                                            \
		}                                                       \
	}
#define NX_CORE_ASSERT(x, ...)                                       \
	{                                                                \
		if (!(x)) {                                                  \
			NX_CORE_ERROR("Core: Assertion fail. {0}", __VA_ARGS__); \
			abort();                                                 \
		}                                                            \
	}
#else
#define NX_ASSERT(x, ...)
#define NX_CORE_ASSERT(x, ...)
#endif

#define glCreateVA(n, x) glGenVertexArrays(n, x)
#define glCreateVB(n, x) glGenTextures(n, x)
#define glCreateTex(n, x) glGenTextures(n, x)

#else
#define NX_ASSERT(x, ...)
#define NX_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

namespace Neurex {

template <typename T>
using scoped = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr scoped<T> make_scoped(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr ref<T> make_ref(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

}
