/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzbasetypes.h												          |
| Desc: standard lib includes, core definitions and types, etc                | 
|     																		  |
| This program is free software: you can redistribute it and/or modify		  |
| it under the terms of the GNU General Public License as published by		  |
| the Free Software Foundation, either version 3 of the License, or			  |
| (at your option) any later version.										  |
| 																			  |
| This program is distributed in the hope that it will be useful,			  |
| but WITHOUT ANY WARRANTY; without even the implied warranty of			  |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				  |
| GNU General Public License for more details.								  |
| 																			  |
| You should have received a copy of the GNU General Public License			  |
| along with this program.  If not, see <http://www.gnu.org/licenses/>.		  |
******************************************************************************/
#ifndef __KZBASETYPES_H__ 
#define __KZBASETYPES_H__ 

//standard common library includes 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <stddef.h>
#include <assert.h>
#include <ctype.h>   
#include <string>  

#if (_MSC_VER >= 1200)
#  pragma warning(push)
#endif
#  pragma warning(disable:4201) // anonymous unions warning
#if defined(_X86_) || defined(_IA64_)
#  pragma pack(4)
#endif 

#if defined(__APPLE__)
#  if defined(__clang__)
#    pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  elif defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#  endif
#endif

#pragma warning(disable : 4996)  //code uses type marked as deprecated 
#pragma warning(disable : 4244)  //conversion to smaller type, possible loss of data
#pragma warning(disable : 26495) //member var not initialized by constructor  
#pragma warning(disable : 26812) //enum is unscoped, prefer enum class 
  
#if defined(__GNUC__)
#  define kz_attribute(x) __attribute__(x)
#else
#  define kz_attribute(x)  
#endif 

#if defined(_WIN32) 
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN  
#  endif  
   //get rid of ms defines
#  undef min
#  undef max 
#  undef ERROR
#  undef OK
#  undef DELETE
#  undef Error 
#  undef MessageBox
#  undef FindText
#endif 

namespace kz { 

	typedef bool               Bool;       //boolean (true or false)
	typedef unsigned char      Byte;       //unsigned 8-bit integer  
	typedef char               Char;       //8-bit int or ANSI char 
	typedef double             Double;     //64-bit floating point
	typedef unsigned long      Dword;      //32-bit unsigned integer  
	typedef float 	           Float;      //32-bit float 
	typedef float 	           Float32;    //32-bit float 
	typedef double             Float64;    //64-bit float 
	typedef char               Int8;       //8-bit integer or ANSI char 
	typedef short              Int16;      //16-bit signed integer
	typedef int                Int32;      //32-bit signed integer  
	typedef long long          Int64;      //64-bit signed integer 
	typedef long               Long;       //64-bit integer
	typedef const char*        Lpcstr;     //ptr to const null 8-bit chars
	typedef const void*        Lpcvoid;    //pointer to constant of any type
	typedef const wchar_t*     Lpcwstr;    //ptr to const null 16-bit chars
	typedef const char**       Lppcstr;    //ptr to const null 8-bit chars 
	typedef char*              Lpstr;      //pointer to array of 8-bit chars
	typedef void*              Lpvoid;     //data or ptr to any type
	typedef const wchar_t*     Lpwstr;     //ptr to const null 16-bit chars  
	typedef size_t             SizeT;      //unsigned integer
	typedef unsigned char      Uchar;      //an unsigned 8-bit integer 
	typedef unsigned char      Uint8;      //an unsigned 8-bit integer 
	typedef unsigned short     Uint16;     //an unsigned 16-bit integer
	typedef unsigned int       Uint32;     //unsigned 32-bit int
	typedef unsigned long long Uint64;     //64-bit unsigned integer 
	typedef void               Void;       //no parameters or data returned 
	typedef wchar_t            Wchar;      //16-bit unicode character 

#if defined(UNICODE)
	typedef wchar_t*     Lptstr;
	typedef wchar_t*     Pctstr;
	typedef std::wstring String;
#else
	typedef char* 	    Lptstr;
	typedef const char* Pctstr;
	typedef std::string String;
#endif 

	template<class T> inline T Max(T x, T y) {
		return (x > y) ? x : y;
	}
	template<class T> inline T Min(T x, T y) {
		return (x < y) ? x : y;
	}
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/