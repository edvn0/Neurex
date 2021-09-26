#pragma once

#ifdef NX_PT_WIN
	#ifdef NX_BUILD_DLL
		#define NX_API __declspec(dllexport)
	#else
		#define NX_API __declspec(dllimport)
	#endif
#else
	#error We only support Windows.
#endif


constexpr auto BIT(int x) { return 1 << x; }