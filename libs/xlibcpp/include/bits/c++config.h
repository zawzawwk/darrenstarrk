// Predefined symbols and macros -*- C++ -*-

// Copyright (C) 1997-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/c++config.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _XLIBCXX_CXX_CONFIG_H
#define _XLIBCXX_CXX_CONFIG_H 1

// The current version of the C++ library in compressed ISO date format.
#define __XLIBCXX__ 20141030

// Macros for various attributes.
//   _XLIBCXX_PURE
//   _XLIBCXX_CONST
//   _XLIBCXX_NORETURN
//   _XLIBCXX_NOTHROW
//   _XLIBCXX_VISIBILITY
#ifndef _XLIBCXX_PURE
# define _XLIBCXX_PURE __attribute__ ((__pure__))
#endif

#ifndef _XLIBCXX_CONST
# define _XLIBCXX_CONST __attribute__ ((__const__))
#endif

#ifndef _XLIBCXX_NORETURN
# define _XLIBCXX_NORETURN __attribute__ ((__noreturn__))
#endif

// See below for C++
#ifndef _XLIBCXX_NOTHROW
# ifndef __cplusplus
#  define _XLIBCXX_NOTHROW __attribute__((__nothrow__))
# endif
#endif

// Macros for visibility attributes.
//   _XLIBCXX_HAVE_ATTRIBUTE_VISIBILITY
//   _XLIBCXX_VISIBILITY
# define _XLIBCXX_HAVE_ATTRIBUTE_VISIBILITY 1

#if _XLIBCXX_HAVE_ATTRIBUTE_VISIBILITY
# define _XLIBCXX_VISIBILITY(V) __attribute__ ((__visibility__ (#V)))
#else
// If this is not supplied by the OS-specific or CPU-specific
// headers included below, it will be defined to an empty default.
# define _XLIBCXX_VISIBILITY(V) _XLIBCXX_PSEUDO_VISIBILITY(V)
#endif

// Macros for deprecated attributes.
//   _XLIBCXX_USE_DEPRECATED
//   _XLIBCXX_DEPRECATED
//   _XLIBCXX17_DEPRECATED
#ifndef _XLIBCXX_USE_DEPRECATED
# define _XLIBCXX_USE_DEPRECATED 1
#endif

#if defined(__DEPRECATED) && (__cplusplus >= 201103L)
# define _XLIBCXX_DEPRECATED __attribute__ ((__deprecated__))
#else
# define _XLIBCXX_DEPRECATED
#endif

#if defined(__DEPRECATED) && (__cplusplus >= 201703L)
# define _XLIBCXX17_DEPRECATED [[__deprecated__]]
#else
# define _XLIBCXX17_DEPRECATED
#endif

// Macros for ABI tag attributes.
#ifndef _XLIBCXX_ABI_TAG_CXX11
# define _XLIBCXX_ABI_TAG_CXX11 __attribute ((__abi_tag__ ("cxx11")))
#endif


#if __cplusplus

// Macro for constexpr, to support in mixed 03/0x mode.
#ifndef _XLIBCXX_CONSTEXPR
# if __cplusplus >= 201103L
#  define _XLIBCXX_CONSTEXPR constexpr
#  define _XLIBCXX_USE_CONSTEXPR constexpr
# else
#  define _XLIBCXX_CONSTEXPR
#  define _XLIBCXX_USE_CONSTEXPR const
# endif
#endif

#ifndef _XLIBCXX14_CONSTEXPR
# if __cplusplus >= 201402L
#  define _XLIBCXX14_CONSTEXPR constexpr
# else
#  define _XLIBCXX14_CONSTEXPR
# endif
#endif

#ifndef _XLIBCXX17_CONSTEXPR
# if __cplusplus > 201402L
#  define _XLIBCXX17_CONSTEXPR constexpr
# else
#  define _XLIBCXX17_CONSTEXPR
# endif
#endif

#ifndef _XLIBCXX17_INLINE
# if __cplusplus > 201402L
#  define _XLIBCXX17_INLINE inline
# else
#  define _XLIBCXX17_INLINE
# endif
#endif

// Macro for noexcept, to support in mixed 03/0x mode.
#ifndef _XLIBCXX_NOEXCEPT
# if __cplusplus >= 201103L
#  define _XLIBCXX_NOEXCEPT noexcept
#  define _XLIBCXX_NOEXCEPT_IF(_COND) noexcept(_COND)
#  define _XLIBCXX_USE_NOEXCEPT noexcept
#  define _XLIBCXX_THROW(_EXC)
# else
#  define _XLIBCXX_NOEXCEPT
#  define _XLIBCXX_NOEXCEPT_IF(_COND)
#  define _XLIBCXX_USE_NOEXCEPT throw()
#  define _XLIBCXX_THROW(_EXC) throw(_EXC)
# endif
#endif

#ifndef _XLIBCXX_NOTHROW
# define _XLIBCXX_NOTHROW _XLIBCXX_USE_NOEXCEPT
#endif

#ifndef _XLIBCXX_THROW_OR_ABORT
# if __EXCEPTIONS
#  define _XLIBCXX_THROW_OR_ABORT(_EXC) (throw (_EXC))
# else
#  define _XLIBCXX_THROW_OR_ABORT(_EXC) (__builtin_abort())
# endif
#endif

#if __cpp_noexcept_function_type
#define _XLIBCXX_NOEXCEPT_PARM , bool _NE
#define _XLIBCXX_NOEXCEPT_QUAL noexcept (_NE)
#else
#define _XLIBCXX_NOEXCEPT_PARM
#define _XLIBCXX_NOEXCEPT_QUAL
#endif

// Macro for extern template, ie controling template linkage via use
// of extern keyword on template declaration. As documented in the g++
// manual, it inhibits all implicit instantiations and is used
// throughout the library to avoid multiple weak definitions for
// required types that are already explicitly instantiated in the
// library binary. This substantially reduces the binary size of
// resulting executables.
// Special case: _XLIBCXX_EXTERN_TEMPLATE == -1 disallows extern
// templates only in basic_string, thus activating its debug-mode
// checks even at -O0.
// # define _XLIBCXX_EXTERN_TEMPLATE 1

/*
  Outline of libstdc++ namespaces.
  namespace std
  {
    namespace __debug { }
    namespace __parallel { }
    namespace __profile { }
    namespace __cxx1998 { }
    namespace __detail { }
    namespace rel_ops { }
    namespace tr1
    {
      namespace placeholders { }
      namespace regex_constants { }
      namespace __detail { }
    }
    namespace tr2 { }

    namespace decimal { }
    namespace chrono { }
    namespace placeholders { }
    namespace regex_constants { }
    namespace this_thread { }
    namespace experimental { }
  }
  namespace abi { }
  namespace __gnu_cxx
  {
    namespace __detail { }
  }
  For full details see:
  http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/namespaces.html
*/
namespace std
{
  typedef __SIZE_TYPE__ 	size_t;
  typedef __PTRDIFF_TYPE__	ptrdiff_t;

#if __cplusplus >= 201103L
  typedef decltype(nullptr)	nullptr_t;
#endif
}


// Defined if inline namespaces are used for versioning.
# define _XLIBCXX_INLINE_VERSION 0

// Inline namespace for symbol versioning.
#if _XLIBCXX_INLINE_VERSION

namespace std
{
  inline namespace __7 { }

  namespace rel_ops { inline namespace __7 { } }

  namespace tr1
  {
    inline namespace __7 { }
    namespace placeholders { inline namespace __7 { } }
    namespace regex_constants { inline namespace __7 { } }
    namespace __detail { inline namespace __7 { } }
  }

  namespace tr2
  { inline namespace __7 { } }

  namespace decimal { inline namespace __7 { } }

  namespace chrono { inline namespace __7 { } }
  namespace placeholders { inline namespace __7 { } }
  namespace regex_constants { inline namespace __7 { } }
  namespace this_thread { inline namespace __7 { } }

  namespace experimental { inline namespace __7 { } }

  namespace __detail { inline namespace __7 { } }
}

namespace __gnu_cxx
{
  inline namespace __7 { }
  namespace __detail { inline namespace __7 { } }
}
# define _XLIBCXX_BEGIN_NAMESPACE_VERSION namespace __7 {
# define _XLIBCXX_END_NAMESPACE_VERSION }
#else
# define _XLIBCXX_BEGIN_NAMESPACE_VERSION
# define _XLIBCXX_END_NAMESPACE_VERSION
#endif


// Inline namespaces for special modes: debug, parallel, profile.
#if defined(_XLIBCXX_DEBUG) || defined(_XLIBCXX_PARALLEL) \
    || defined(_XLIBCXX_PROFILE)
namespace std
{
  // Non-inline namespace for components replaced by alternates in active mode.
  namespace __cxx1998
  {
#if _XLIBCXX_INLINE_VERSION
 inline namespace __7 { }
#endif
  }

  // Inline namespace for debug mode.
# ifdef _XLIBCXX_DEBUG
  inline namespace __debug { }
# endif

  // Inline namespaces for parallel mode.
# ifdef _XLIBCXX_PARALLEL
  inline namespace __parallel { }
# endif

  // Inline namespaces for profile mode
# ifdef _XLIBCXX_PROFILE
  inline namespace __profile { }
# endif
}

// Check for invalid usage and unsupported mixed-mode use.
# if defined(_XLIBCXX_DEBUG) && defined(_XLIBCXX_PARALLEL)
#  error illegal use of multiple inlined namespaces
# endif
# if defined(_XLIBCXX_PROFILE) && defined(_XLIBCXX_DEBUG)
#  error illegal use of multiple inlined namespaces
# endif
# if defined(_XLIBCXX_PROFILE) && defined(_XLIBCXX_PARALLEL)
#  error illegal use of multiple inlined namespaces
# endif

// Check for invalid use due to lack for weak symbols.
# if __NO_INLINE__ && !__GXX_WEAK__
#  warning currently using inlined namespace mode which may fail \
   without inlining due to lack of weak symbols
# endif
#endif

// Macros for namespace scope. Either namespace std:: or the name
// of some nested namespace within it corresponding to the active mode.
// _XLIBCXX_STD_A
// _XLIBCXX_STD_C
//
// Macros for opening/closing conditional namespaces.
// _XLIBCXX_BEGIN_NAMESPACE_ALGO
// _XLIBCXX_END_NAMESPACE_ALGO
// _XLIBCXX_BEGIN_NAMESPACE_CONTAINER
// _XLIBCXX_END_NAMESPACE_CONTAINER
#if defined(_XLIBCXX_DEBUG) || defined(_XLIBCXX_PROFILE)
# define _XLIBCXX_STD_C __cxx1998
# define _XLIBCXX_BEGIN_NAMESPACE_CONTAINER \
	 namespace _XLIBCXX_STD_C { _XLIBCXX_BEGIN_NAMESPACE_VERSION
# define _XLIBCXX_END_NAMESPACE_CONTAINER \
	 _XLIBCXX_END_NAMESPACE_VERSION }
# undef _XLIBCXX_EXTERN_TEMPLATE
# define _XLIBCXX_EXTERN_TEMPLATE -1
#endif

#ifdef _XLIBCXX_PARALLEL
# define _XLIBCXX_STD_A __cxx1998
# define _XLIBCXX_BEGIN_NAMESPACE_ALGO \
	 namespace _XLIBCXX_STD_A { _XLIBCXX_BEGIN_NAMESPACE_VERSION
# define _XLIBCXX_END_NAMESPACE_ALGO \
	 _XLIBCXX_END_NAMESPACE_VERSION }
#endif

#ifndef _XLIBCXX_STD_A
# define _XLIBCXX_STD_A std
#endif

#ifndef _XLIBCXX_STD_C
# define _XLIBCXX_STD_C std
#endif

#ifndef _XLIBCXX_BEGIN_NAMESPACE_ALGO
# define _XLIBCXX_BEGIN_NAMESPACE_ALGO
#endif

#ifndef _XLIBCXX_END_NAMESPACE_ALGO
# define _XLIBCXX_END_NAMESPACE_ALGO
#endif

#ifndef _XLIBCXX_BEGIN_NAMESPACE_CONTAINER
# define _XLIBCXX_BEGIN_NAMESPACE_CONTAINER
#endif

#ifndef _XLIBCXX_END_NAMESPACE_CONTAINER
# define _XLIBCXX_END_NAMESPACE_CONTAINER
#endif

// GLIBCXX_ABI Deprecated
// Define if compatibility should be provided for -mlong-double-64.
#undef _XLIBCXX_LONG_DOUBLE_COMPAT

// Inline namespace for long double 128 mode.
#if defined _XLIBCXX_LONG_DOUBLE_COMPAT && defined __LONG_DOUBLE_128__
namespace std
{
  inline namespace __gnu_cxx_ldbl128 { }
}
# define _XLIBCXX_NAMESPACE_LDBL __gnu_cxx_ldbl128::
# define _XLIBCXX_BEGIN_NAMESPACE_LDBL namespace __gnu_cxx_ldbl128 {
# define _XLIBCXX_END_NAMESPACE_LDBL }
#else
# define _XLIBCXX_NAMESPACE_LDBL
# define _XLIBCXX_BEGIN_NAMESPACE_LDBL
# define _XLIBCXX_END_NAMESPACE_LDBL
#endif

// Assert.
#if !defined(_XLIBCXX_DEBUG) && !defined(_XLIBCXX_PARALLEL)
# define __glibcxx_assert(_Condition)
#else
namespace std
{
  // Avoid the use of assert, because we're trying to keep the <cassert>
  // include out of the mix.
  inline void
  __replacement_assert(const char* __file, int __line,
		       const char* __function, const char* __condition)
  {
    __builtin_printf("%s:%d: %s: Assertion '%s' failed.\n", __file, __line,
		     __function, __condition);
    __builtin_abort();
  }
}
#define __glibcxx_assert(_Condition)				   	 \
  do 									 \
  {							      		 \
    if (! (_Condition))                                                  \
      std::__replacement_assert(__FILE__, __LINE__, __PRETTY_FUNCTION__, \
				#_Condition);				 \
  } while (false)
#endif

// Macros for race detectors.
// _XLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) and
// _XLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A) should be used to explain
// atomic (lock-free) synchronization to race detectors:
// the race detector will infer a happens-before arc from the former to the
// latter when they share the same argument pointer.
//
// The most frequent use case for these macros (and the only case in the
// current implementation of the library) is atomic reference counting:
//   void _M_remove_reference()
//   {
//     _XLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(&this->_M_refcount);
//     if (__gnu_cxx::__exchange_and_add_dispatch(&this->_M_refcount, -1) <= 0)
//       {
//         _XLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(&this->_M_refcount);
//         _M_destroy(__a);
//       }
//   }
// The annotations in this example tell the race detector that all memory
// accesses occurred when the refcount was positive do not race with
// memory accesses which occurred after the refcount became zero.
#ifndef _XLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
# define  _XLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A)
#endif
#ifndef _XLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
# define  _XLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)
#endif

// Macros for C linkage: define extern "C" linkage only when using C++.
# define _XLIBCXX_BEGIN_EXTERN_C extern "C" {
# define _XLIBCXX_END_EXTERN_C }

#else // !__cplusplus
# define _XLIBCXX_BEGIN_EXTERN_C
# define _XLIBCXX_END_EXTERN_C
#endif


// First includes.

// Pick up any OS-specific definitions.
// #include <bits/os_defines.h>

// Pick up any CPU-specific definitions.
// #include <bits/cpu_defines.h>

// If platform uses neither visibility nor psuedo-visibility,
// specify empty default for namespace annotation macros.
#ifndef _XLIBCXX_PSEUDO_VISIBILITY
# define _XLIBCXX_PSEUDO_VISIBILITY(V)
#endif

// Certain function definitions that are meant to be overridable from
// user code are decorated with this macro.  For some targets, this
// macro causes these definitions to be weak.
#ifndef _XLIBCXX_WEAK_DEFINITION
# define _XLIBCXX_WEAK_DEFINITION
#endif


// Conditionally enable annotations for the Transactional Memory TS on C++11.
// Most of the following conditions are due to limitations in the current
// implementation.
#if __cplusplus >= 201103L && _XLIBCXX_USE_CXX11_ABI      \
  && _XLIBCXX_USE_DUAL_ABI && __cpp_transactional_memory >= 201505L \
  &&  !_XLIBCXX_FULLY_DYNAMIC_STRING && _XLIBCXX_USE_WEAK_REF   \
  && _XLIBCXX_USE_ALLOCATOR_NEW
#define _XLIBCXX_TXN_SAFE transaction_safe
#define _XLIBCXX_TXN_SAFE_DYN transaction_safe_dynamic
#else
#define _XLIBCXX_TXN_SAFE
#define _XLIBCXX_TXN_SAFE_DYN
#endif

// The remainder of the prewritten config is automatic; all the
// user hooks are listed above.

// Create a boolean flag to be used to determine if --fast-math is set.
#ifdef __FAST_MATH__
# define _XLIBCXX_FAST_MATH 1
#else
# define _XLIBCXX_FAST_MATH 0
#endif

// This marks string literals in header files to be extracted for eventual
// translation.  It is primarily used for messages in thrown exceptions; see
// src/functexcept.cc.  We use __N because the more traditional _N is used
// for something else under certain OSes (see BADNAMES).
#define __N(msgid)     (msgid)

// For example, <windows.h> is known to #define min and max as macros...
#undef min
#undef max

// End of prewritten config; the settings discovered at configure time follow.
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `acosf' function. */
#define _XLIBCXX_HAVE_ACOSF 1

/* Define to 1 if you have the `acosl' function. */
/* #undef _XLIBCXX_HAVE_ACOSL */

/* Define to 1 if you have the `asinf' function. */
#define _XLIBCXX_HAVE_ASINF 1

/* Define to 1 if you have the `asinl' function. */
/* #undef _XLIBCXX_HAVE_ASINL */

/* Define to 1 if the target assembler supports .symver directive. */
#define _XLIBCXX_HAVE_AS_SYMVER_DIRECTIVE 1

/* Define to 1 if you have the `atan2f' function. */
#define _XLIBCXX_HAVE_ATAN2F 1

/* Define to 1 if you have the `atan2l' function. */
/* #undef _XLIBCXX_HAVE_ATAN2L */

/* Define to 1 if you have the `atanf' function. */
#define _XLIBCXX_HAVE_ATANF 1

/* Define to 1 if you have the `atanl' function. */
/* #undef _XLIBCXX_HAVE_ATANL */

/* Define to 1 if you have the `at_quick_exit' function. */
/* #undef _XLIBCXX_HAVE_AT_QUICK_EXIT */

/* Define to 1 if the target assembler supports thread-local storage. */
/* #undef _XLIBCXX_HAVE_CC_TLS */

/* Define to 1 if you have the `ceilf' function. */
#define _XLIBCXX_HAVE_CEILF 1

/* Define to 1 if you have the `ceill' function. */
/* #undef _XLIBCXX_HAVE_CEILL */

/* Define to 1 if you have the <complex.h> header file. */
#define _XLIBCXX_HAVE_COMPLEX_H 1

/* Define to 1 if you have the `cosf' function. */
#define _XLIBCXX_HAVE_COSF 1

/* Define to 1 if you have the `coshf' function. */
#define _XLIBCXX_HAVE_COSHF 1

/* Define to 1 if you have the `coshl' function. */
/* #undef _XLIBCXX_HAVE_COSHL */

/* Define to 1 if you have the `cosl' function. */
/* #undef _XLIBCXX_HAVE_COSL */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef _XLIBCXX_HAVE_DLFCN_H */

/* Define if EBADMSG exists. */
#define _XLIBCXX_HAVE_EBADMSG 1

/* Define if ECANCELED exists. */
#define _XLIBCXX_HAVE_ECANCELED 1

/* Define if ECHILD exists. */
#define _XLIBCXX_HAVE_ECHILD 1

/* Define if EIDRM exists. */
#define _XLIBCXX_HAVE_EIDRM 1

/* Define to 1 if you have the <endian.h> header file. */
/* #undef _XLIBCXX_HAVE_ENDIAN_H */

/* Define if ENODATA exists. */
#define _XLIBCXX_HAVE_ENODATA 1

/* Define if ENOLINK exists. */
#define _XLIBCXX_HAVE_ENOLINK 1

/* Define if ENOSPC exists. */
#define _XLIBCXX_HAVE_ENOSPC 1

/* Define if ENOSR exists. */
#define _XLIBCXX_HAVE_ENOSR 1

/* Define if ENOSTR exists. */
#define _XLIBCXX_HAVE_ENOSTR 1

/* Define if ENOTRECOVERABLE exists. */
#define _XLIBCXX_HAVE_ENOTRECOVERABLE 1

/* Define if ENOTSUP exists. */
#define _XLIBCXX_HAVE_ENOTSUP 1

/* Define if EOVERFLOW exists. */
#define _XLIBCXX_HAVE_EOVERFLOW 1

/* Define if EOWNERDEAD exists. */
#define _XLIBCXX_HAVE_EOWNERDEAD 1

/* Define if EPERM exists. */
#define _XLIBCXX_HAVE_EPERM 1

/* Define if EPROTO exists. */
#define _XLIBCXX_HAVE_EPROTO 1

/* Define if ETIME exists. */
#define _XLIBCXX_HAVE_ETIME 1

/* Define if ETIMEDOUT exists. */
#define _XLIBCXX_HAVE_ETIMEDOUT 1

/* Define if ETXTBSY exists. */
#define _XLIBCXX_HAVE_ETXTBSY 1

/* Define if EWOULDBLOCK exists. */
#define _XLIBCXX_HAVE_EWOULDBLOCK 1

/* Define to 1 if you have the <execinfo.h> header file. */
/* #undef _XLIBCXX_HAVE_EXECINFO_H */

/* Define to 1 if you have the `expf' function. */
#define _XLIBCXX_HAVE_EXPF 1

/* Define to 1 if you have the `expl' function. */
/* #undef _XLIBCXX_HAVE_EXPL */

/* Define to 1 if you have the `fabsf' function. */
#define _XLIBCXX_HAVE_FABSF 1

/* Define to 1 if you have the `fabsl' function. */
/* #undef _XLIBCXX_HAVE_FABSL */

/* Define to 1 if you have the <fenv.h> header file. */
/* #undef _XLIBCXX_HAVE_FENV_H */

/* Define to 1 if you have the `finite' function. */
/* #undef _XLIBCXX_HAVE_FINITE */

/* Define to 1 if you have the `finitef' function. */
/* #undef _XLIBCXX_HAVE_FINITEF */

/* Define to 1 if you have the `finitel' function. */
/* #undef _XLIBCXX_HAVE_FINITEL */

/* Define to 1 if you have the <float.h> header file. */
#define _XLIBCXX_HAVE_FLOAT_H 1

/* Define to 1 if you have the `floorf' function. */
#define _XLIBCXX_HAVE_FLOORF 1

/* Define to 1 if you have the `floorl' function. */
/* #undef _XLIBCXX_HAVE_FLOORL */

/* Define to 1 if you have the `fmodf' function. */
#define _XLIBCXX_HAVE_FMODF 1

/* Define to 1 if you have the `fmodl' function. */
/* #undef _XLIBCXX_HAVE_FMODL */

/* Define to 1 if you have the `fpclass' function. */
/* #undef _XLIBCXX_HAVE_FPCLASS */

/* Define to 1 if you have the <fp.h> header file. */
/* #undef _XLIBCXX_HAVE_FP_H */

/* Define to 1 if you have the `frexpf' function. */
#define _XLIBCXX_HAVE_FREXPF 1

/* Define to 1 if you have the `frexpl' function. */
/* #undef _XLIBCXX_HAVE_FREXPL */

/* Define if _Unwind_GetIPInfo is available. */
#define _XLIBCXX_HAVE_GETIPINFO 1

/* Define if gets is available in <stdio.h>. */
#define _XLIBCXX_HAVE_GETS 1

/* Define to 1 if you have the `hypot' function. */
#define _XLIBCXX_HAVE_HYPOT 1

/* Define to 1 if you have the `hypotf' function. */
/* #undef _XLIBCXX_HAVE_HYPOTF */

/* Define to 1 if you have the `hypotl' function. */
/* #undef _XLIBCXX_HAVE_HYPOTL */

/* Define if you have the iconv() function. */
#define _XLIBCXX_HAVE_ICONV 1

/* Define to 1 if you have the <ieeefp.h> header file. */
#define _XLIBCXX_HAVE_IEEEFP_H 1

/* Define if int64_t is available in <stdint.h>. */
#define _XLIBCXX_HAVE_INT64_T 1

/* Define if int64_t is a long. */
/* #undef _XLIBCXX_HAVE_INT64_T_LONG */

/* Define if int64_t is a long long. */
#define _XLIBCXX_HAVE_INT64_T_LONG_LONG 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define _XLIBCXX_HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isinf' function. */
/* #undef _XLIBCXX_HAVE_ISINF */

/* Define to 1 if you have the `isinff' function. */
/* #undef _XLIBCXX_HAVE_ISINFF */

/* Define to 1 if you have the `isinfl' function. */
/* #undef _XLIBCXX_HAVE_ISINFL */

/* Define to 1 if you have the `isnan' function. */
/* #undef _XLIBCXX_HAVE_ISNAN */

/* Define to 1 if you have the `isnanf' function. */
/* #undef _XLIBCXX_HAVE_ISNANF */

/* Define to 1 if you have the `isnanl' function. */
/* #undef _XLIBCXX_HAVE_ISNANL */

/* Defined if iswblank exists. */
#define _XLIBCXX_HAVE_ISWBLANK 1

/* Define if LC_MESSAGES is available in <locale.h>. */
#define _XLIBCXX_HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `ldexpf' function. */
#define _XLIBCXX_HAVE_LDEXPF 1

/* Define to 1 if you have the `ldexpl' function. */
/* #undef _XLIBCXX_HAVE_LDEXPL */

/* Define to 1 if you have the <libintl.h> header file. */
/* #undef _XLIBCXX_HAVE_LIBINTL_H */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _XLIBCXX_HAVE_LIMIT_AS */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _XLIBCXX_HAVE_LIMIT_DATA */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _XLIBCXX_HAVE_LIMIT_FSIZE */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _XLIBCXX_HAVE_LIMIT_RSS */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _XLIBCXX_HAVE_LIMIT_VMEM */

/* Define if futex syscall is available. */
/* #undef _XLIBCXX_HAVE_LINUX_FUTEX */

/* Define to 1 if you have the <locale.h> header file. */
#define _XLIBCXX_HAVE_LOCALE_H 1

/* Define to 1 if you have the `log10f' function. */
#define _XLIBCXX_HAVE_LOG10F 1

/* Define to 1 if you have the `log10l' function. */
/* #undef _XLIBCXX_HAVE_LOG10L */

/* Define to 1 if you have the `logf' function. */
#define _XLIBCXX_HAVE_LOGF 1

/* Define to 1 if you have the `logl' function. */
/* #undef _XLIBCXX_HAVE_LOGL */

/* Define to 1 if you have the <machine/endian.h> header file. */
#define _XLIBCXX_HAVE_MACHINE_ENDIAN_H 1

/* Define to 1 if you have the <machine/param.h> header file. */
#define _XLIBCXX_HAVE_MACHINE_PARAM_H 1

/* Define if mbstate_t exists in wchar.h. */
#define _XLIBCXX_HAVE_MBSTATE_T 1

/* Define to 1 if you have the <memory.h> header file. */
/* #undef _XLIBCXX_HAVE_MEMORY_H */

/* Define to 1 if you have the `modf' function. */
/* #undef _XLIBCXX_HAVE_MODF */

/* Define to 1 if you have the `modff' function. */
#define _XLIBCXX_HAVE_MODFF 1

/* Define to 1 if you have the `modfl' function. */
/* #undef _XLIBCXX_HAVE_MODFL */

/* Define to 1 if you have the <nan.h> header file. */
/* #undef _XLIBCXX_HAVE_NAN_H */

/* Define if poll is available in <poll.h>. */
/* #undef _XLIBCXX_HAVE_POLL */

/* Define to 1 if you have the `powf' function. */
#define _XLIBCXX_HAVE_POWF 1

/* Define to 1 if you have the `powl' function. */
/* #undef _XLIBCXX_HAVE_POWL */

/* Define to 1 if you have the `qfpclass' function. */
/* #undef _XLIBCXX_HAVE_QFPCLASS */

/* Define to 1 if you have the `quick_exit' function. */
/* #undef _XLIBCXX_HAVE_QUICK_EXIT */

/* Define to 1 if you have the `setenv' function. */
/* #undef _XLIBCXX_HAVE_SETENV */

/* Define to 1 if you have the `sincos' function. */
/* #undef _XLIBCXX_HAVE_SINCOS */

/* Define to 1 if you have the `sincosf' function. */
/* #undef _XLIBCXX_HAVE_SINCOSF */

/* Define to 1 if you have the `sincosl' function. */
/* #undef _XLIBCXX_HAVE_SINCOSL */

/* Define to 1 if you have the `sinf' function. */
#define _XLIBCXX_HAVE_SINF 1

/* Define to 1 if you have the `sinhf' function. */
#define _XLIBCXX_HAVE_SINHF 1

/* Define to 1 if you have the `sinhl' function. */
/* #undef _XLIBCXX_HAVE_SINHL */

/* Define to 1 if you have the `sinl' function. */
/* #undef _XLIBCXX_HAVE_SINL */

/* Defined if sleep exists. */
#define _XLIBCXX_HAVE_SLEEP 1

/* Define to 1 if you have the `sqrtf' function. */
#define _XLIBCXX_HAVE_SQRTF 1

/* Define to 1 if you have the `sqrtl' function. */
/* #undef _XLIBCXX_HAVE_SQRTL */

/* Define to 1 if you have the <stdalign.h> header file. */
#define _XLIBCXX_HAVE_STDALIGN_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define _XLIBCXX_HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define _XLIBCXX_HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define _XLIBCXX_HAVE_STDLIB_H 1

/* Define if strerror_l is available in <string.h>. */
/* #undef _XLIBCXX_HAVE_STRERROR_L */

/* Define if strerror_r is available in <string.h>. */
#define _XLIBCXX_HAVE_STRERROR_R 1

/* Define to 1 if you have the <strings.h> header file. */
#define _XLIBCXX_HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define _XLIBCXX_HAVE_STRING_H 1

/* Define to 1 if you have the `strtof' function. */
#define _XLIBCXX_HAVE_STRTOF 1

/* Define to 1 if you have the `strtold' function. */
/* #undef _XLIBCXX_HAVE_STRTOLD */

/* Define if strxfrm_l is available in <string.h>. */
/* #undef _XLIBCXX_HAVE_STRXFRM_L */

/* Define to 1 if the target runtime linker supports binding the same symbol
   to different versions. */
/* #undef _XLIBCXX_HAVE_SYMVER_SYMBOL_RENAMING_RUNTIME_SUPPORT */

/* Define to 1 if you have the <sys/filio.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_FILIO_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_IOCTL_H */

/* Define to 1 if you have the <sys/ipc.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_IPC_H */

/* Define to 1 if you have the <sys/isa_defs.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_ISA_DEFS_H */

/* Define to 1 if you have the <sys/machine.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_MACHINE_H */

/* Define to 1 if you have the <sys/param.h> header file. */
#define _XLIBCXX_HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define _XLIBCXX_HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have a suitable <sys/sdt.h> header file */
/* #undef _XLIBCXX_HAVE_SYS_SDT_H */

/* Define to 1 if you have the <sys/sem.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_SEM_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define _XLIBCXX_HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysinfo.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_SYSINFO_H */

/* Define to 1 if you have the <sys/time.h> header file. */
#define _XLIBCXX_HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define _XLIBCXX_HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
/* #undef _XLIBCXX_HAVE_SYS_UIO_H */

/* Define if S_IFREG is available in <sys/stat.h>. */
/* #undef _XLIBCXX_HAVE_S_IFREG */

/* Define if S_IFREG is available in <sys/stat.h>. */
#define _XLIBCXX_HAVE_S_ISREG 1

/* Define to 1 if you have the `tanf' function. */
#define _XLIBCXX_HAVE_TANF 1

/* Define to 1 if you have the `tanhf' function. */
#define _XLIBCXX_HAVE_TANHF 1

/* Define to 1 if you have the `tanhl' function. */
/* #undef _XLIBCXX_HAVE_TANHL */

/* Define to 1 if you have the `tanl' function. */
/* #undef _XLIBCXX_HAVE_TANL */

/* Define to 1 if you have the <tgmath.h> header file. */
#define _XLIBCXX_HAVE_TGMATH_H 1

/* Define to 1 if the target supports thread-local storage. */
/* #undef _XLIBCXX_HAVE_TLS */

/* Define to 1 if you have the <unistd.h> header file. */
#define _XLIBCXX_HAVE_UNISTD_H 1

/* Defined if usleep exists. */
#define _XLIBCXX_HAVE_USLEEP 1

/* Defined if vfwscanf exists. */
#define _XLIBCXX_HAVE_VFWSCANF 1

/* Defined if vswscanf exists. */
#define _XLIBCXX_HAVE_VSWSCANF 1

/* Defined if vwscanf exists. */
#define _XLIBCXX_HAVE_VWSCANF 1

/* Define to 1 if you have the <wchar.h> header file. */
#define _XLIBCXX_HAVE_WCHAR_H 1

/* Defined if wcstof exists. */
#define _XLIBCXX_HAVE_WCSTOF 1

/* Define to 1 if you have the <wctype.h> header file. */
#define _XLIBCXX_HAVE_WCTYPE_H 1

/* Defined if Sleep exists. */
/* #undef _XLIBCXX_HAVE_WIN32_SLEEP */

/* Define if writev is available in <sys/uio.h>. */
/* #undef _XLIBCXX_HAVE_WRITEV */

/* Define to 1 if you have the `_acosf' function. */
/* #undef _XLIBCXX_HAVE__ACOSF */

/* Define to 1 if you have the `_acosl' function. */
/* #undef _XLIBCXX_HAVE__ACOSL */

/* Define to 1 if you have the `_asinf' function. */
/* #undef _XLIBCXX_HAVE__ASINF */

/* Define to 1 if you have the `_asinl' function. */
/* #undef _XLIBCXX_HAVE__ASINL */

/* Define to 1 if you have the `_atan2f' function. */
/* #undef _XLIBCXX_HAVE__ATAN2F */

/* Define to 1 if you have the `_atan2l' function. */
/* #undef _XLIBCXX_HAVE__ATAN2L */

/* Define to 1 if you have the `_atanf' function. */
/* #undef _XLIBCXX_HAVE__ATANF */

/* Define to 1 if you have the `_atanl' function. */
/* #undef _XLIBCXX_HAVE__ATANL */

/* Define to 1 if you have the `_ceilf' function. */
/* #undef _XLIBCXX_HAVE__CEILF */

/* Define to 1 if you have the `_ceill' function. */
/* #undef _XLIBCXX_HAVE__CEILL */

/* Define to 1 if you have the `_cosf' function. */
/* #undef _XLIBCXX_HAVE__COSF */

/* Define to 1 if you have the `_coshf' function. */
/* #undef _XLIBCXX_HAVE__COSHF */

/* Define to 1 if you have the `_coshl' function. */
/* #undef _XLIBCXX_HAVE__COSHL */

/* Define to 1 if you have the `_cosl' function. */
/* #undef _XLIBCXX_HAVE__COSL */

/* Define to 1 if you have the `_expf' function. */
/* #undef _XLIBCXX_HAVE__EXPF */

/* Define to 1 if you have the `_expl' function. */
/* #undef _XLIBCXX_HAVE__EXPL */

/* Define to 1 if you have the `_fabsf' function. */
/* #undef _XLIBCXX_HAVE__FABSF */

/* Define to 1 if you have the `_fabsl' function. */
/* #undef _XLIBCXX_HAVE__FABSL */

/* Define to 1 if you have the `_finite' function. */
/* #undef _XLIBCXX_HAVE__FINITE */

/* Define to 1 if you have the `_finitef' function. */
/* #undef _XLIBCXX_HAVE__FINITEF */

/* Define to 1 if you have the `_finitel' function. */
/* #undef _XLIBCXX_HAVE__FINITEL */

/* Define to 1 if you have the `_floorf' function. */
/* #undef _XLIBCXX_HAVE__FLOORF */

/* Define to 1 if you have the `_floorl' function. */
/* #undef _XLIBCXX_HAVE__FLOORL */

/* Define to 1 if you have the `_fmodf' function. */
/* #undef _XLIBCXX_HAVE__FMODF */

/* Define to 1 if you have the `_fmodl' function. */
/* #undef _XLIBCXX_HAVE__FMODL */

/* Define to 1 if you have the `_fpclass' function. */
/* #undef _XLIBCXX_HAVE__FPCLASS */

/* Define to 1 if you have the `_frexpf' function. */
/* #undef _XLIBCXX_HAVE__FREXPF */

/* Define to 1 if you have the `_frexpl' function. */
/* #undef _XLIBCXX_HAVE__FREXPL */

/* Define to 1 if you have the `_hypot' function. */
/* #undef _XLIBCXX_HAVE__HYPOT */

/* Define to 1 if you have the `_hypotf' function. */
/* #undef _XLIBCXX_HAVE__HYPOTF */

/* Define to 1 if you have the `_hypotl' function. */
/* #undef _XLIBCXX_HAVE__HYPOTL */

/* Define to 1 if you have the `_isinf' function. */
/* #undef _XLIBCXX_HAVE__ISINF */

/* Define to 1 if you have the `_isinff' function. */
/* #undef _XLIBCXX_HAVE__ISINFF */

/* Define to 1 if you have the `_isinfl' function. */
/* #undef _XLIBCXX_HAVE__ISINFL */

/* Define to 1 if you have the `_isnan' function. */
/* #undef _XLIBCXX_HAVE__ISNAN */

/* Define to 1 if you have the `_isnanf' function. */
/* #undef _XLIBCXX_HAVE__ISNANF */

/* Define to 1 if you have the `_isnanl' function. */
/* #undef _XLIBCXX_HAVE__ISNANL */

/* Define to 1 if you have the `_ldexpf' function. */
/* #undef _XLIBCXX_HAVE__LDEXPF */

/* Define to 1 if you have the `_ldexpl' function. */
/* #undef _XLIBCXX_HAVE__LDEXPL */

/* Define to 1 if you have the `_log10f' function. */
/* #undef _XLIBCXX_HAVE__LOG10F */

/* Define to 1 if you have the `_log10l' function. */
/* #undef _XLIBCXX_HAVE__LOG10L */

/* Define to 1 if you have the `_logf' function. */
/* #undef _XLIBCXX_HAVE__LOGF */

/* Define to 1 if you have the `_logl' function. */
/* #undef _XLIBCXX_HAVE__LOGL */

/* Define to 1 if you have the `_modf' function. */
/* #undef _XLIBCXX_HAVE__MODF */

/* Define to 1 if you have the `_modff' function. */
/* #undef _XLIBCXX_HAVE__MODFF */

/* Define to 1 if you have the `_modfl' function. */
/* #undef _XLIBCXX_HAVE__MODFL */

/* Define to 1 if you have the `_powf' function. */
/* #undef _XLIBCXX_HAVE__POWF */

/* Define to 1 if you have the `_powl' function. */
/* #undef _XLIBCXX_HAVE__POWL */

/* Define to 1 if you have the `_qfpclass' function. */
/* #undef _XLIBCXX_HAVE__QFPCLASS */

/* Define to 1 if you have the `_sincos' function. */
/* #undef _XLIBCXX_HAVE__SINCOS */

/* Define to 1 if you have the `_sincosf' function. */
/* #undef _XLIBCXX_HAVE__SINCOSF */

/* Define to 1 if you have the `_sincosl' function. */
/* #undef _XLIBCXX_HAVE__SINCOSL */

/* Define to 1 if you have the `_sinf' function. */
/* #undef _XLIBCXX_HAVE__SINF */

/* Define to 1 if you have the `_sinhf' function. */
/* #undef _XLIBCXX_HAVE__SINHF */

/* Define to 1 if you have the `_sinhl' function. */
/* #undef _XLIBCXX_HAVE__SINHL */

/* Define to 1 if you have the `_sinl' function. */
/* #undef _XLIBCXX_HAVE__SINL */

/* Define to 1 if you have the `_sqrtf' function. */
/* #undef _XLIBCXX_HAVE__SQRTF */

/* Define to 1 if you have the `_sqrtl' function. */
/* #undef _XLIBCXX_HAVE__SQRTL */

/* Define to 1 if you have the `_tanf' function. */
/* #undef _XLIBCXX_HAVE__TANF */

/* Define to 1 if you have the `_tanhf' function. */
/* #undef _XLIBCXX_HAVE__TANHF */

/* Define to 1 if you have the `_tanhl' function. */
/* #undef _XLIBCXX_HAVE__TANHL */

/* Define to 1 if you have the `_tanl' function. */
/* #undef _XLIBCXX_HAVE__TANL */

/* Define to 1 if you have the `__cxa_thread_atexit_impl' function. */
/* #undef _XLIBCXX_HAVE___CXA_THREAD_ATEXIT_IMPL */

/* Define as const if the declaration of iconv() needs const. */
/* #undef _XLIBCXX_ICONV_CONST */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
/* #undef _XLIBCXX_PACKAGE */

/* Define to the address where bug reports for this package should be sent. */
#define _XLIBCXX_PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define _XLIBCXX_PACKAGE_NAME "package-unused"

/* Define to the full name and version of this package. */
#define _XLIBCXX_PACKAGE_STRING "package-unused version-unused"

/* Define to the one symbol short name of this package. */
#define _XLIBCXX_PACKAGE_TARNAME "libstdc++"

/* Define to the home page for this package. */
#define _XLIBCXX_PACKAGE_URL ""

/* Define to the version of this package. */
#define _XLIBCXX_PACKAGE__XLIBCXX_VERSION "version-unused"

/* The size of `char', as computed by sizeof. */
/* #undef SIZEOF_CHAR */

/* The size of `int', as computed by sizeof. */
/* #undef SIZEOF_INT */

/* The size of `long', as computed by sizeof. */
/* #undef SIZEOF_LONG */

/* The size of `short', as computed by sizeof. */
/* #undef SIZEOF_SHORT */

/* The size of `void *', as computed by sizeof. */
/* #undef SIZEOF_VOID_P */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
/* #undef _XLIBCXX_VERSION */

/* Define if the compiler supports C++11 atomics. */
/* #undef _XLIBCXX_ATOMIC_BUILTINS */

/* Define to use concept checking code from the boost libraries. */
/* #undef _XLIBCXX_CONCEPT_CHECKS */

/* Define to 1 if a fully dynamic basic_string is wanted, 0 to disable,
   undefined for platform defaults */
#define _XLIBCXX_FULLY_DYNAMIC_STRING 0

/* Define if gthreads library is available. */
/* #undef _XLIBCXX_HAS_GTHREADS */

/* Define to 1 if a full hosted library is built, or 0 if freestanding. */
#define _XLIBCXX_HOSTED 1

/* Define if compatibility should be provided for -mlong-double-64. */

/* Define if ptrdiff_t is int. */
#define _XLIBCXX_PTRDIFF_T_IS_INT 1

/* Define if using setrlimit to set resource limits during "make check" */
/* #undef _XLIBCXX_RES_LIMITS */

/* Define if size_t is unsigned int. */
#define _XLIBCXX_SIZE_T_IS_UINT 1

/* Define if the compiler is configured for setjmp/longjmp exceptions. */
/* #undef _XLIBCXX_SJLJ_EXCEPTIONS */

/* Define to the value of the EOF integer constant. */
#define _XLIBCXX_STDIO_EOF -1

/* Define to the value of the SEEK_CUR integer constant. */
#define _XLIBCXX_STDIO_SEEK_CUR 1

/* Define to the value of the SEEK_END integer constant. */
#define _XLIBCXX_STDIO_SEEK_END 2

/* Define to use symbol versioning in the shared library. */
/* #undef _XLIBCXX_SYMVER */

/* Define to use darwin versioning in the shared library. */
/* #undef _XLIBCXX_SYMVER_DARWIN */

/* Define to use GNU versioning in the shared library. */
/* #undef _XLIBCXX_SYMVER_GNU */

/* Define to use GNU namespace versioning in the shared library. */
/* #undef _XLIBCXX_SYMVER_GNU_NAMESPACE */

/* Define to use Sun versioning in the shared library. */
/* #undef _XLIBCXX_SYMVER_SUN */

/* Define if C99 functions or macros from <wchar.h>, <math.h>, <complex.h>,
   <stdio.h>, and <stdlib.h> can be used or exposed. */
/* #undef _XLIBCXX_USE_C99 */

/* Define if C99 functions in <complex.h> should be used in <complex>. Using
   compiler builtins for these functions requires corresponding C99 library
   functions to be present. */
/* #undef _XLIBCXX_USE_C99_COMPLEX */

/* Define if C99 functions in <complex.h> should be used in <tr1/complex>.
   Using compiler builtins for these functions requires corresponding C99
   library functions to be present. */
/* #undef _XLIBCXX_USE_C99_COMPLEX_TR1 */

/* Define if C99 functions in <ctype.h> should be imported in <tr1/cctype> in
   namespace std::tr1. */
#define _XLIBCXX_USE_C99_CTYPE_TR1 1

/* Define if C99 functions in <fenv.h> should be imported in <tr1/cfenv> in
   namespace std::tr1. */
/* #undef _XLIBCXX_USE_C99_FENV_TR1 */

/* Define if C99 functions in <inttypes.h> should be imported in
   <tr1/cinttypes> in namespace std::tr1. */
#define _XLIBCXX_USE_C99_INTTYPES_TR1 1

/* Define if wchar_t C99 functions in <inttypes.h> should be imported in
   <tr1/cinttypes> in namespace std::tr1. */
#define _XLIBCXX_USE_C99_INTTYPES_WCHAR_T_TR1 1

/* Define if C99 functions or macros in <math.h> should be imported in <cmath>
   in namespace std. */
#define _XLIBCXX_USE_C99_MATH 1

/* Define if C99 functions or macros in <math.h> should be imported in
   <tr1/cmath> in namespace std::tr1. */
#define _XLIBCXX_USE_C99_MATH_TR1 1

/* Define if C99 types in <stdint.h> should be imported in <tr1/cstdint> in
   namespace std::tr1. */
#define _XLIBCXX_USE_C99_STDINT_TR1 1

/* Defined if clock_gettime syscall has monotonic and realtime clock support.
   */
/* #undef _XLIBCXX_USE_CLOCK_GETTIME_SYSCALL */

/* Defined if clock_gettime has monotonic clock support. */
/* #undef _XLIBCXX_USE_CLOCK_MONOTONIC */

/* Defined if clock_gettime has realtime clock support. */
/* #undef _XLIBCXX_USE_CLOCK_REALTIME */

/* Define if ISO/IEC TR 24733 decimal floating point types are supported on
   this host. */
/* #undef _XLIBCXX_USE_DECIMAL_FLOAT */

/* Define if __float128 is supported on this host. */
/* #undef _XLIBCXX_USE_FLOAT128 */

/* Defined if gettimeofday is available. */
#define _XLIBCXX_USE_GETTIMEOFDAY 1

/* Define if get_nprocs is available in <sys/sysinfo.h>. */
/* #undef _XLIBCXX_USE_GET_NPROCS */

/* Define if __int128 is supported on this host. */
/* #undef _XLIBCXX_USE_INT128 */

/* Define if LFS support is available. */
/* #undef _XLIBCXX_USE_LFS */

/* Define if code specialized for long long should be used. */
#define _XLIBCXX_USE_LONG_LONG 1

/* Defined if nanosleep is available. */
/* #undef _XLIBCXX_USE_NANOSLEEP */

/* Define if NLS translations are to be used. */
/* #undef _XLIBCXX_USE_NLS */

/* Define if pthreads_num_processors_np is available in <pthread.h>. */
/* #undef _XLIBCXX_USE_PTHREADS_NUM_PROCESSORS_NP */

/* Define if /dev/random and /dev/urandom are available for the random_device
   of TR1 (Chapter 5.1). */
/* #undef _XLIBCXX_USE_RANDOM_TR1 */

/* Defined if sched_yield is available. */
/* #undef _XLIBCXX_USE_SCHED_YIELD */

/* Define if _SC_NPROCESSORS_ONLN is available in <unistd.h>. */
/* #undef _XLIBCXX_USE_SC_NPROCESSORS_ONLN */

/* Define if _SC_NPROC_ONLN is available in <unistd.h>. */
/* #undef _XLIBCXX_USE_SC_NPROC_ONLN */

/* Define if sysctl(), CTL_HW and HW_NCPU are available in <sys/sysctl.h>. */
/* #undef _XLIBCXX_USE_SYSCTL_HW_NCPU */

/* Define if obsolescent tmpnam is available in <stdio.h>. */
#define _XLIBCXX_USE_TMPNAM 1

/* Define if code specialized for wchar_t should be used. */
#define _XLIBCXX_USE_WCHAR_T 1

/* Define to 1 if a verbose library is built, or 0 otherwise. */
#define _XLIBCXX_VERBOSE 1

/* Defined if as can handle rdrand. */
/* #undef _XLIBCXX_X86_RDRAND */

/* Define to 1 if mutex_timedlock is available. */
#define _GTHREAD_USE_MUTEX_TIMEDLOCK 1

#if defined (_XLIBCXX_HAVE__ACOSF) && ! defined (_XLIBCXX_HAVE_ACOSF)
# define _XLIBCXX_HAVE_ACOSF 1
# define acosf _acosf
#endif

#if defined (_XLIBCXX_HAVE__ACOSL) && ! defined (_XLIBCXX_HAVE_ACOSL)
# define _XLIBCXX_HAVE_ACOSL 1
# define acosl _acosl
#endif

#if defined (_XLIBCXX_HAVE__ASINF) && ! defined (_XLIBCXX_HAVE_ASINF)
# define _XLIBCXX_HAVE_ASINF 1
# define asinf _asinf
#endif

#if defined (_XLIBCXX_HAVE__ASINL) && ! defined (_XLIBCXX_HAVE_ASINL)
# define _XLIBCXX_HAVE_ASINL 1
# define asinl _asinl
#endif

#if defined (_XLIBCXX_HAVE__ATAN2F) && ! defined (_XLIBCXX_HAVE_ATAN2F)
# define _XLIBCXX_HAVE_ATAN2F 1
# define atan2f _atan2f
#endif

#if defined (_XLIBCXX_HAVE__ATAN2L) && ! defined (_XLIBCXX_HAVE_ATAN2L)
# define _XLIBCXX_HAVE_ATAN2L 1
# define atan2l _atan2l
#endif

#if defined (_XLIBCXX_HAVE__ATANF) && ! defined (_XLIBCXX_HAVE_ATANF)
# define _XLIBCXX_HAVE_ATANF 1
# define atanf _atanf
#endif

#if defined (_XLIBCXX_HAVE__ATANL) && ! defined (_XLIBCXX_HAVE_ATANL)
# define _XLIBCXX_HAVE_ATANL 1
# define atanl _atanl
#endif

#if defined (_XLIBCXX_HAVE__CEILF) && ! defined (_XLIBCXX_HAVE_CEILF)
# define _XLIBCXX_HAVE_CEILF 1
# define ceilf _ceilf
#endif

#if defined (_XLIBCXX_HAVE__CEILL) && ! defined (_XLIBCXX_HAVE_CEILL)
# define _XLIBCXX_HAVE_CEILL 1
# define ceill _ceill
#endif

#if defined (_XLIBCXX_HAVE__COSF) && ! defined (_XLIBCXX_HAVE_COSF)
# define _XLIBCXX_HAVE_COSF 1
# define cosf _cosf
#endif

#if defined (_XLIBCXX_HAVE__COSHF) && ! defined (_XLIBCXX_HAVE_COSHF)
# define _XLIBCXX_HAVE_COSHF 1
# define coshf _coshf
#endif

#if defined (_XLIBCXX_HAVE__COSHL) && ! defined (_XLIBCXX_HAVE_COSHL)
# define _XLIBCXX_HAVE_COSHL 1
# define coshl _coshl
#endif

#if defined (_XLIBCXX_HAVE__COSL) && ! defined (_XLIBCXX_HAVE_COSL)
# define _XLIBCXX_HAVE_COSL 1
# define cosl _cosl
#endif

#if defined (_XLIBCXX_HAVE__EXPF) && ! defined (_XLIBCXX_HAVE_EXPF)
# define _XLIBCXX_HAVE_EXPF 1
# define expf _expf
#endif

#if defined (_XLIBCXX_HAVE__EXPL) && ! defined (_XLIBCXX_HAVE_EXPL)
# define _XLIBCXX_HAVE_EXPL 1
# define expl _expl
#endif

#if defined (_XLIBCXX_HAVE__FABSF) && ! defined (_XLIBCXX_HAVE_FABSF)
# define _XLIBCXX_HAVE_FABSF 1
# define fabsf _fabsf
#endif

#if defined (_XLIBCXX_HAVE__FABSL) && ! defined (_XLIBCXX_HAVE_FABSL)
# define _XLIBCXX_HAVE_FABSL 1
# define fabsl _fabsl
#endif

#if defined (_XLIBCXX_HAVE__FINITE) && ! defined (_XLIBCXX_HAVE_FINITE)
# define _XLIBCXX_HAVE_FINITE 1
# define finite _finite
#endif

#if defined (_XLIBCXX_HAVE__FINITEF) && ! defined (_XLIBCXX_HAVE_FINITEF)
# define _XLIBCXX_HAVE_FINITEF 1
# define finitef _finitef
#endif

#if defined (_XLIBCXX_HAVE__FINITEL) && ! defined (_XLIBCXX_HAVE_FINITEL)
# define _XLIBCXX_HAVE_FINITEL 1
# define finitel _finitel
#endif

#if defined (_XLIBCXX_HAVE__FLOORF) && ! defined (_XLIBCXX_HAVE_FLOORF)
# define _XLIBCXX_HAVE_FLOORF 1
# define floorf _floorf
#endif

#if defined (_XLIBCXX_HAVE__FLOORL) && ! defined (_XLIBCXX_HAVE_FLOORL)
# define _XLIBCXX_HAVE_FLOORL 1
# define floorl _floorl
#endif

#if defined (_XLIBCXX_HAVE__FMODF) && ! defined (_XLIBCXX_HAVE_FMODF)
# define _XLIBCXX_HAVE_FMODF 1
# define fmodf _fmodf
#endif

#if defined (_XLIBCXX_HAVE__FMODL) && ! defined (_XLIBCXX_HAVE_FMODL)
# define _XLIBCXX_HAVE_FMODL 1
# define fmodl _fmodl
#endif

#if defined (_XLIBCXX_HAVE__FPCLASS) && ! defined (_XLIBCXX_HAVE_FPCLASS)
# define _XLIBCXX_HAVE_FPCLASS 1
# define fpclass _fpclass
#endif

#if defined (_XLIBCXX_HAVE__FREXPF) && ! defined (_XLIBCXX_HAVE_FREXPF)
# define _XLIBCXX_HAVE_FREXPF 1
# define frexpf _frexpf
#endif

#if defined (_XLIBCXX_HAVE__FREXPL) && ! defined (_XLIBCXX_HAVE_FREXPL)
# define _XLIBCXX_HAVE_FREXPL 1
# define frexpl _frexpl
#endif

#if defined (_XLIBCXX_HAVE__HYPOT) && ! defined (_XLIBCXX_HAVE_HYPOT)
# define _XLIBCXX_HAVE_HYPOT 1
# define hypot _hypot
#endif

#if defined (_XLIBCXX_HAVE__HYPOTF) && ! defined (_XLIBCXX_HAVE_HYPOTF)
# define _XLIBCXX_HAVE_HYPOTF 1
# define hypotf _hypotf
#endif

#if defined (_XLIBCXX_HAVE__HYPOTL) && ! defined (_XLIBCXX_HAVE_HYPOTL)
# define _XLIBCXX_HAVE_HYPOTL 1
# define hypotl _hypotl
#endif

#if defined (_XLIBCXX_HAVE__ISINF) && ! defined (_XLIBCXX_HAVE_ISINF)
# define _XLIBCXX_HAVE_ISINF 1
# define isinf _isinf
#endif

#if defined (_XLIBCXX_HAVE__ISINFF) && ! defined (_XLIBCXX_HAVE_ISINFF)
# define _XLIBCXX_HAVE_ISINFF 1
# define isinff _isinff
#endif

#if defined (_XLIBCXX_HAVE__ISINFL) && ! defined (_XLIBCXX_HAVE_ISINFL)
# define _XLIBCXX_HAVE_ISINFL 1
# define isinfl _isinfl
#endif

#if defined (_XLIBCXX_HAVE__ISNAN) && ! defined (_XLIBCXX_HAVE_ISNAN)
# define _XLIBCXX_HAVE_ISNAN 1
# define isnan _isnan
#endif

#if defined (_XLIBCXX_HAVE__ISNANF) && ! defined (_XLIBCXX_HAVE_ISNANF)
# define _XLIBCXX_HAVE_ISNANF 1
# define isnanf _isnanf
#endif

#if defined (_XLIBCXX_HAVE__ISNANL) && ! defined (_XLIBCXX_HAVE_ISNANL)
# define _XLIBCXX_HAVE_ISNANL 1
# define isnanl _isnanl
#endif

#if defined (_XLIBCXX_HAVE__LDEXPF) && ! defined (_XLIBCXX_HAVE_LDEXPF)
# define _XLIBCXX_HAVE_LDEXPF 1
# define ldexpf _ldexpf
#endif

#if defined (_XLIBCXX_HAVE__LDEXPL) && ! defined (_XLIBCXX_HAVE_LDEXPL)
# define _XLIBCXX_HAVE_LDEXPL 1
# define ldexpl _ldexpl
#endif

#if defined (_XLIBCXX_HAVE__LOG10F) && ! defined (_XLIBCXX_HAVE_LOG10F)
# define _XLIBCXX_HAVE_LOG10F 1
# define log10f _log10f
#endif

#if defined (_XLIBCXX_HAVE__LOG10L) && ! defined (_XLIBCXX_HAVE_LOG10L)
# define _XLIBCXX_HAVE_LOG10L 1
# define log10l _log10l
#endif

#if defined (_XLIBCXX_HAVE__LOGF) && ! defined (_XLIBCXX_HAVE_LOGF)
# define _XLIBCXX_HAVE_LOGF 1
# define logf _logf
#endif

#if defined (_XLIBCXX_HAVE__LOGL) && ! defined (_XLIBCXX_HAVE_LOGL)
# define _XLIBCXX_HAVE_LOGL 1
# define logl _logl
#endif

#if defined (_XLIBCXX_HAVE__MODF) && ! defined (_XLIBCXX_HAVE_MODF)
# define _XLIBCXX_HAVE_MODF 1
# define modf _modf
#endif

#if defined (_XLIBCXX_HAVE__MODFF) && ! defined (_XLIBCXX_HAVE_MODFF)
# define _XLIBCXX_HAVE_MODFF 1
# define modff _modff
#endif

#if defined (_XLIBCXX_HAVE__MODFL) && ! defined (_XLIBCXX_HAVE_MODFL)
# define _XLIBCXX_HAVE_MODFL 1
# define modfl _modfl
#endif

#if defined (_XLIBCXX_HAVE__POWF) && ! defined (_XLIBCXX_HAVE_POWF)
# define _XLIBCXX_HAVE_POWF 1
# define powf _powf
#endif

#if defined (_XLIBCXX_HAVE__POWL) && ! defined (_XLIBCXX_HAVE_POWL)
# define _XLIBCXX_HAVE_POWL 1
# define powl _powl
#endif

#if defined (_XLIBCXX_HAVE__QFPCLASS) && ! defined (_XLIBCXX_HAVE_QFPCLASS)
# define _XLIBCXX_HAVE_QFPCLASS 1
# define qfpclass _qfpclass
#endif

#if defined (_XLIBCXX_HAVE__SINCOS) && ! defined (_XLIBCXX_HAVE_SINCOS)
# define _XLIBCXX_HAVE_SINCOS 1
# define sincos _sincos
#endif

#if defined (_XLIBCXX_HAVE__SINCOSF) && ! defined (_XLIBCXX_HAVE_SINCOSF)
# define _XLIBCXX_HAVE_SINCOSF 1
# define sincosf _sincosf
#endif

#if defined (_XLIBCXX_HAVE__SINCOSL) && ! defined (_XLIBCXX_HAVE_SINCOSL)
# define _XLIBCXX_HAVE_SINCOSL 1
# define sincosl _sincosl
#endif

#if defined (_XLIBCXX_HAVE__SINF) && ! defined (_XLIBCXX_HAVE_SINF)
# define _XLIBCXX_HAVE_SINF 1
# define sinf _sinf
#endif

#if defined (_XLIBCXX_HAVE__SINHF) && ! defined (_XLIBCXX_HAVE_SINHF)
# define _XLIBCXX_HAVE_SINHF 1
# define sinhf _sinhf
#endif

#if defined (_XLIBCXX_HAVE__SINHL) && ! defined (_XLIBCXX_HAVE_SINHL)
# define _XLIBCXX_HAVE_SINHL 1
# define sinhl _sinhl
#endif

#if defined (_XLIBCXX_HAVE__SINL) && ! defined (_XLIBCXX_HAVE_SINL)
# define _XLIBCXX_HAVE_SINL 1
# define sinl _sinl
#endif

#if defined (_XLIBCXX_HAVE__SQRTF) && ! defined (_XLIBCXX_HAVE_SQRTF)
# define _XLIBCXX_HAVE_SQRTF 1
# define sqrtf _sqrtf
#endif

#if defined (_XLIBCXX_HAVE__SQRTL) && ! defined (_XLIBCXX_HAVE_SQRTL)
# define _XLIBCXX_HAVE_SQRTL 1
# define sqrtl _sqrtl
#endif

#if defined (_XLIBCXX_HAVE__STRTOF) && ! defined (_XLIBCXX_HAVE_STRTOF)
# define _XLIBCXX_HAVE_STRTOF 1
# define strtof _strtof
#endif

#if defined (_XLIBCXX_HAVE__STRTOLD) && ! defined (_XLIBCXX_HAVE_STRTOLD)
# define _XLIBCXX_HAVE_STRTOLD 1
# define strtold _strtold
#endif

#if defined (_XLIBCXX_HAVE__TANF) && ! defined (_XLIBCXX_HAVE_TANF)
# define _XLIBCXX_HAVE_TANF 1
# define tanf _tanf
#endif

#if defined (_XLIBCXX_HAVE__TANHF) && ! defined (_XLIBCXX_HAVE_TANHF)
# define _XLIBCXX_HAVE_TANHF 1
# define tanhf _tanhf
#endif

#if defined (_XLIBCXX_HAVE__TANHL) && ! defined (_XLIBCXX_HAVE_TANHL)
# define _XLIBCXX_HAVE_TANHL 1
# define tanhl _tanhl
#endif

#if defined (_XLIBCXX_HAVE__TANL) && ! defined (_XLIBCXX_HAVE_TANL)
# define _XLIBCXX_HAVE_TANL 1
# define tanl _tanl
#endif

#endif // _XLIBCXX_CXX_CONFIG_H