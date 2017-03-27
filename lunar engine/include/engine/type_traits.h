/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: type_traits.h
---------------------------------------------------
*/

#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

#include <string>

#include "color.h"
#include "vector2.h"

namespace lunar {

//Tag dispatching

struct tags {

	struct type_tag {};
	
	//Signed types
	struct bool_tag:public type_tag {};
	struct char_tag:public type_tag {};
	struct wchar_tag:public type_tag {};
	struct short_tag:public type_tag {};
	struct int_tag:public type_tag {};
	struct long_tag:public type_tag {};
	struct longlong_tag:public type_tag {};
	
	//Unsigned types
	struct uchar_tag:public type_tag {};
	struct ushort_tag:public type_tag {};
	struct uint_tag:public type_tag {};
	struct ulong_tag:public type_tag {};
	struct ulonglong_tag:public type_tag {};
	
	//Floating types
	struct float_tag:public type_tag {};
	struct double_tag:public type_tag {};
	struct longdouble_tag:public type_tag {};
	
	//Std types
	struct string_tag:public type_tag {};
	
	//Engine types
	struct color_tag:public type_tag {};
	struct vector2_tag:public type_tag {};
};

//Template traits

template <typename _Ty>
struct type_traits {
	typedef tags::type_tag category;
};

template <>
struct type_traits<bool> {
	typedef tags::bool_tag category;
};

template <>
struct type_traits<char> {
	typedef tags::char_tag category;
};

template <>
struct type_traits<wchar_t> {
	typedef tags::wchar_tag category;
};

template <>
struct type_traits<short> {
	typedef tags::short_tag category;
};

template <>
struct type_traits<int> {
	typedef tags::int_tag category;
};

template <>
struct type_traits<long> {
	typedef tags::long_tag category;
};

template <>
struct type_traits<long long> {
	typedef tags::longlong_tag category;
};

template <>
struct type_traits<unsigned char> {
	typedef tags::uchar_tag category;
};

template <>
struct type_traits<unsigned short> {
	typedef tags::ushort_tag category;
};

template <>
struct type_traits<unsigned int> {
	typedef tags::uint_tag category;
};

template <>
struct type_traits<unsigned long> {
	typedef tags::ulong_tag category;
};

template <>
struct type_traits<unsigned long long> {
	typedef tags::ulonglong_tag category;
};

template <>
struct type_traits<float> {
	typedef tags::float_tag category;
};

template <>
struct type_traits<double> {
	typedef tags::double_tag category;
};

template <>
struct type_traits<long double> {
	typedef tags::longdouble_tag category;
};

template <>
struct type_traits<std::string> {
	typedef tags::string_tag category;
};

template <>
struct type_traits<color> {
	typedef tags::color_tag category;
};

template <>
struct type_traits<vector2> {
	typedef tags::vector2_tag category;
};

} //namespace lunar

#endif