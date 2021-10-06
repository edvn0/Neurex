#pragma once

#ifdef NX_DEBUG
	#define NX_ALLOW_ASSERTS
#endif

#ifdef NX_PT_WIN
	#ifdef NX_ALLOW_ASSERTS
		#define NX_ASSERT(x, ...) { if (!(x)) { NX_ERROR("User: Assertion fail. {0}", __VA_ARGS__); __debugbreak(); } }
		#define NX_CORE_ASSERT(x, ...) { if (!(x)) { NX_CORE_ERROR("Core: Assertion fail. {0}", __VA_ARGS__); __debugbreak(); } }
	#else
		#define NX_ASSERT(x, ...)
		#define NX_CORE_ASSERT(x, ...)
	#endif

	#define glCreateVA(n, x) glCreateVertexArrays(n, x)
	#define glCreateVB(n, x) glCreateVertexBuffers(n, x)

#elif defined(NX_PT_OSX)
	#ifdef NX_ALLOW_ASSERTS
		#define NX_ASSERT(x, ...) { if (!(x)) { NX_ERROR("User: Assertion fail: {0}", __VA_ARGS__); } }
		#define NX_CORE_ASSERT(x, ...) { if (!(x)) { NX_CORE_ERROR("Core: Assertion fail. {0}", __VA_ARGS__); } }
	#else
		#define NX_ASSERT(x, ...)
		#define NX_CORE_ASSERT(x, ...)
	#endif

	#define glCreateVA(n, x) glGenVertexArrays(n, x)
	#define glCreateVB(n, x) glGenBuffers(n, x)

#else
	#define NX_ASSERT(x, ...)
	#define NX_CORE_ASSERT(x, ...)	
#endif

#define BEFn(x) std::bind(&x, this, std::placeholders::_1)
#define BIT(x) (1 << x)
