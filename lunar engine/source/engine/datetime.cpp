/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: datetime.cpp
---------------------------------------------------
*/

#include "datetime.h"

//Dependencies
#include <ctime>
#include <cctype>
#include <sstream>

#include "string_utility.h"

namespace lunar {

//Private

const unsigned int datetime::_Day_difference=25569;
const unsigned int datetime::_Seconds_perhour=3600;
const unsigned int datetime::_Seconds_perday=_Seconds_perhour*24;

void datetime::copy(const datetime &_Datetime) {
	_Value=_Datetime._Value;
}

time_t datetime::now() {

time_t _Now;
time(&_Now);
return _Now;
}

//Public

datetime::datetime():_Value(now()) {
	//Empty
}

datetime::datetime(const time_t &_Value):_Value(_Value) {
	//Empty
}

datetime::datetime(const datetime &_Datetime) {
	copy(_Datetime);
}

datetime::~datetime() {
	//Empty
}

datetime::operator time_t() const {
	return _Value;
}

datetime::operator std::string() const {
	return datetime_string();
}

datetime &datetime::operator=(const datetime &_Datetime) {

	//Worth doing
	if (this!=&_Datetime) copy(_Datetime);

return *this;
}

bool datetime::operator==(const datetime &_Datetime) const {
	return (_Value==_Datetime._Value);
}

bool datetime::operator!=(const datetime &_Datetime) const {
	return !(*this==_Datetime);
}

bool datetime::operator<(const datetime &_Datetime) const {
	return (_Value<_Datetime._Value);
}

bool datetime::operator>(const datetime &_Datetime) const {
	return (_Value>_Datetime._Value);
}

bool datetime::operator<=(const datetime &_Datetime) const {
	return (_Value<=_Datetime._Value);
}

bool datetime::operator>=(const datetime &_Datetime) const {
	return (_Value>=_Datetime._Value);
}

//Arithmetic operators

datetime &datetime::operator++() {
	return (*this+=1);
}

datetime &datetime::operator--() {
	return (*this-=1);
}

datetime datetime::operator++(int) {

datetime _Datetime(*this);
++*this;
return _Datetime;
}

datetime datetime::operator--(int) {

datetime _Datetime(*this);
--*this;
return _Datetime;
}

datetime datetime::operator+(const datetime &_Datetime) const {
	return datetime(_Value+_Datetime._Value);
}

datetime datetime::operator-(const datetime &_Datetime) const {
	return datetime(_Value-_Datetime._Value);
}

datetime &datetime::operator+=(const datetime &_Datetime) {

_Value+=_Datetime._Value;
return *this;
}

datetime &datetime::operator-=(const datetime &_Datetime) {

_Value-=_Datetime._Value;
return *this;
}

datetime datetime::operator+(unsigned int _Days) const {
	return datetime(_Value+(_Days*_Seconds_perday));
}

datetime datetime::operator-(unsigned int _Days) const {
	return datetime(_Value-(_Days*_Seconds_perday));
}

datetime &datetime::operator+=(unsigned int _Days) {

_Value+=_Days*_Seconds_perday;
return *this;
}

datetime &datetime::operator-=(unsigned int _Days) {

_Value-=_Days*_Seconds_perday;
return *this;
}

//Common functions

datetime datetime::current_date() {
	return datetime(now());
}

datetime datetime::current_datetime() {
	return datetime(now());
}

datetime datetime::encode_date(unsigned int _Year,
	unsigned int _Month,unsigned int _Day) {
	return encode_datetime(_Year,_Month,_Day,0,0,0);
}

datetime datetime::encode_time(unsigned int _Hour,
	unsigned int _Minute,unsigned int _Second) {
	return encode_datetime(1900,1,1,_Hour,_Minute,_Second);
}

datetime datetime::encode_datetime(unsigned int _Year,
	unsigned int _Month,unsigned int _Day,
		unsigned int _Hour,unsigned int _Minute,
			unsigned int _Second) {

tm _Time;
_Time.tm_year=_Year-1900;
_Time.tm_mon=_Month-1;
_Time.tm_mday=_Day;
_Time.tm_hour=_Hour;
_Time.tm_min=_Minute;
_Time.tm_sec=_Second;
_Time.tm_isdst=0;

return datetime((unsigned int)mktime(&_Time));
}

void datetime::decode_date(unsigned int &_Year,unsigned int &_Month,
	unsigned int &_Day) const {

unsigned int _Hour,_Minute,_Second;
decode_datetime(_Year,_Month,_Day,_Hour,_Minute,_Second);
}

void datetime::decode_time(unsigned int &_Hour,unsigned int &_Minute,
	unsigned int &_Second) const {

unsigned int _Year,_Month,_Day;
decode_datetime(_Year,_Month,_Day,_Hour,_Minute,_Second);
}

void datetime::decode_datetime(unsigned int &_Year,unsigned int &_Month,
	unsigned int &_Day,unsigned int &_Hour,unsigned int &_Minute,
		unsigned int &_Second) const {

struct tm _Time;
localtime_s(&_Time,&_Value);

_Year=_Time.tm_year+1900;
_Month=_Time.tm_mon+1;
_Day=_Time.tm_mday;
_Hour=_Time.tm_hour;
_Minute=_Time.tm_min;
_Second=_Time.tm_sec;
}

std::string datetime::date_string() const {
	return format("yyyymmdd");
}

std::string datetime::time_string() const {
	return format("hh:nn:ss");
}

std::string datetime::datetime_string() const {
	return format("yyyymmdd hh:nn:ss");
}

unsigned int datetime::day_ofweek() const {

struct tm _Time;
localtime_s(&_Time,&_Value);
return _Time.tm_wday;
}

unsigned int datetime::day_ofyear() const {

struct tm _Time;
localtime_s(&_Time,&_Value);
return _Time.tm_yday;
}

//Format functions

std::string datetime::format(const std::string &_Format) const {

std::string _Result=string_utility::lower_case(_Format); //Make lower case

size_t _Year_position=_Result.find("yyyy");
size_t _Year_digit=4;

	if (_Year_position==std::string::npos) {
	
	_Year_position=_Result.find("yy");
	_Year_digit=2;
	}

size_t _Month_position=_Result.find("mm");
size_t _Day_position=_Result.find("dd");
size_t _Hour_position=_Result.find("hh");
size_t _Minute_position=_Result.find("nn");
size_t _Second_position=_Result.find("ss");

_Result=_Format; //Bring back correct case state

unsigned int _Year,_Month,_Day,_Hour,_Minute,_Second;
decode_datetime(_Year,_Month,_Day,_Hour,_Minute,_Second);

std::stringstream _Stringstream;

	if (_Year_position!=std::string::npos) {
	
	_Stringstream<<_Year;
	_Result.replace(_Year_position,_Year_digit,(_Year_digit==4?
		_Stringstream.str():_Stringstream.str().substr(2,2)));
	}
	
	if (_Month_position!=std::string::npos) {
	
	_Stringstream.str("");
	
		if (_Month<10) _Stringstream<<"0";
	
	_Stringstream<<_Month;
	_Result.replace(_Month_position,2,_Stringstream.str());
	}
	
	if (_Day_position!=std::string::npos) {
	
	_Stringstream.str("");
	
		if (_Day<10) _Stringstream<<"0";
	
	_Stringstream<<_Day;
	_Result.replace(_Day_position,2,_Stringstream.str());
	}
	
	if (_Hour_position!=std::string::npos) {
	
	_Stringstream.str("");
	
		if (_Hour<10) _Stringstream<<"0";
	
	_Stringstream<<_Hour;
	_Result.replace(_Hour_position,2,_Stringstream.str());
	}
	
	if (_Minute_position!=std::string::npos) {
	
	_Stringstream.str("");
	
		if (_Minute<10) _Stringstream<<"0";
	
	_Stringstream<<_Minute;
	_Result.replace(_Minute_position,2,_Stringstream.str());
	}
	
	if (_Second_position!=std::string::npos) {
	
	_Stringstream.str("");
	
		if (_Second<10) _Stringstream<<"0";
	
	_Stringstream<<_Second;
	_Result.replace(_Second_position,2,_Stringstream.str());
	}

return _Result;
}

} //namespace lunar