// NOTE: This file is used to import libs into your project in the correct order.
//       Make sure you keep this in mind when you edit this file. The correct order
//       should roughly be:
//         1. MFC.
//         2. C runtime.
//         3. Other system libs. (atlsd.lib, commsup.lib, ...)
//         4. ARLAssert.
//         5. Other libs that need to be globally available.
#pragma once
#ifndef _ALLOW_RTCc_IN_STL
#define _ALLOW_RTCc_IN_STL
#endif
/* Use of the _DEBUG symbol is prohibited if you use ObjectARX. */
#if defined(_DBXAPP) || defined(_ACRXAPP)
#ifdef _DEBUG
#error Do not define the _DEBUG symbol in projects that use ObjectARX. Use SCONDEBUG instead.
#endif // _DEBUG
#endif // {ObjectARX}

/* Enable strict type checking. */
#ifndef STRICT
#define STRICT
#endif // STRICT

/* Exclude rarely-used stuff from Windows headers. */
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // VC_EXTRALEAN

/* Specify minimum Windows version. 0x0601 = Windows 7 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif // _WIN32_WINNT
#ifndef WINVER
#define WINVER 0x0601
#endif // WINVER

/* Prevent unintentional cast operations. */
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

/* Use apartment threading. The default is _ATL_FREE_THREADED. */
#define _ATL_APARTMENT_THREADED

/* Makes ATL push/pop the current warning state when it disables
   compiler warnings in 'atlbase.h', 'atlcom.h' and 'atlcore.h'. */
#define _ATL_ALL_WARNINGS

#ifdef _MANAGED
/*
 * Disable the warning for initializing static or global objects compiled to MSIL. These
 * initializers run from the modules .cctor which executes the first time MSIL code runs
 * for that module.
 */
#pragma warning( disable : 4835 )

/*
 * Ignore 'const has no effect on initonly data member'. This happens when importing managed
 * types from assemblies created with the VC 7.1 compiler that have static data members.
 */
#pragma warning( disable : 4393 )
#endif // _MANAGED

/* C++ common headers. */
#include <typeinfo.h>
#include <math.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <float.h>
#include <memory.h>     
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <search.h>

/* STL */
#include <string>
#include <set>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <queue>
#include <functional>
#include <exception>
#include <utility>
#include <memory>
#include <algorithm>
#include <sstream>
#include <iterator>

/* AcArray and AcString */
#include <acarray.h>
#include <acadstrc.h>
#include <gedblar.h>

/* Disable warning about enumeration in qualified name. */
#pragma warning( disable : 4482 )

/* MFC support. */
#if defined(_AFXDLL) || defined(SCON_MFC_SUPPORT)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifndef SCON_NO_AFXTEMPL_SUPPORT
#include <afxtempl.h>       // MFC template classes
#endif //SCON_NO_AFXTEMPL_SUPPORT

struct _TREEITEM {};				// fix for warning LNK4248
struct _IMAGELIST {};			// fix for warning LNK4248

#include <AcExtensionModule.h>

#endif // MFC support.

/* ATL support. */
#if defined(_ATL_STATIC_REGISTRY) || defined(_ATL_DLL)
#include <atlbase.h>

//#ifdef SCON_ATL_COM_SUPPORT
//#include <atlcom.h>
//#endif

#include <atlstr.h>
#endif // ATL support.

// FIX: See MS-Knowledgebase article KB309801.
#ifdef __CSTRINGT_H__
#include <atlstr.h>
template class __declspec(dllimport) CStringT<TCHAR, StrTraitMFC<TCHAR, ChTraitsCRT<TCHAR> > >;
template class __declspec(dllimport) CSimpleStringT<TCHAR>;
#endif

/* C Runtime functions. */
#include <float.h>
#include <comutil.h> // _bstr_t, _variant_t

/* Standard C library. */
#include <use_ansi.h>

/* .NET Framework types. */
#ifdef _MANAGED
#pragma warning( push )
#pragma warning( disable:4091 ) // 'keyword' : ignored on left of 'type' when no variable is declared
#include <comip.h>
#include <vcclr.h>
#include <msclr/all.h>
#pragma warning( pop )
#endif

/* Windows */
#include <shlwapi.h>

/* 64 vs 32 bit Macros */
#ifdef WIN64
#define _ttol_ptr _tstoi64
#else
#define _ttol_ptr _ttol
#endif

/* Helpful macros. */
#ifndef GUID_ARRAY
#define GUID_ARRAY(name) (mscorlib::Guid*)name.asArrayPtr()
#endif

#pragma warning( push )
#pragma warning( disable: 4100 ) // unreferenced formal parameter.
#pragma warning( disable: 4239 ) // nonstandard extension used : conversion from 'type' to 'type'.
#pragma warning( disable: 4244 ) // conversion from 'type1' to 'type2', possible loss of data.
#pragma warning( disable: 4996 ) // function or variable may be unsafe.
#pragma warning( disable: 4633 ) // badly-formed XML documentation.
#pragma warning( disable: 4635 ) // badly-formed XML documentation.
#pragma warning( disable: 4638 ) // badly-formed XML documentation.

//#define _AMODELER_SUPPORT_
#include <dbxHeaders.h>
#include <arxHeaders.h>


#pragma warning( pop )

class AcDbStub {};				// fix for warning LNK4248


namespace std {

	/* GUID specializations for the hash<> and equal_to<> functors. These are used by unordered_map, unordered_set, etc. */

	template<>
	class hash<GUID> : public unary_function<GUID, size_t>
	{
	public:
		size_t operator()(const GUID& _Keyval) const
		{
			return 0;
		}
	};

	template<>
	struct equal_to<GUID> : public binary_function<GUID, GUID, bool>
	{
		bool operator()(const GUID& left, const GUID& right) const
		{
			return IsEqualGUID(left, right) != 0;
		}
	};

	/* AcDbObjectId specialization for the hash<> functor. This is used by unordered_map, unordered_set, etc. */

	template<>
	class hash<AcDbObjectId>
		: public std::unary_function<AcDbObjectId, size_t>
	{
	public:
		size_t operator()(const AcDbObjectId& keyValue) const
		{
			return inner(reinterpret_cast<uintptr_t>(static_cast<AcDbStub*>(keyValue)));
		}

	private:
		hash<uintptr_t> inner;
	};
} // namespace std