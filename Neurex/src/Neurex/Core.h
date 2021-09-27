#pragma once


#ifdef NX_PT_WIN
#if NX_DYNAMIC_LINK
	#ifdef NX_BUILD_DLL
		#define NX_API __declspec(dllexport)
	#else
		#define NX_API __declspec(dllimport)
	#endif
#else
	#define NX_API
#endif
#else
	#error We only support Windows.
#endif

#ifdef NX_DEBUG
	#define NX_ALLOW_ASSERTS
#endif

#ifdef NX_ALLOW_ASSERTS
	#define NX_ASSERT(x, ...) { if (!(x)) { NX_ERROR("User: Assertion fail. {0}", __VA_ARGS__); __debugbreak(); } }
	#define NX_CORE_ASSERT(x, ...) { if (!(x)) { NX_CORE_ERROR("Core: Assertion fail. {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define NX_ASSERT(x, ...)
	#define NX_CORE_ASSERT(x, ...)
#endif

#define BEFn(x) std::bind(&x, this, std::placeholders::_1)
#define BIT(x) (1 << x)