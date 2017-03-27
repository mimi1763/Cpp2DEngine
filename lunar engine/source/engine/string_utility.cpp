/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: string_utility.cpp
---------------------------------------------------
*/

#include "string_utility.h"

//Dependencies
#include <cmath>
#include <cctype>
#include <algorithm>

#include "any_cast.h"

namespace lunar {

//Private

string_utility::string_utility() {
	//Empty
}

string_utility::~string_utility() {
	//Empty
}

//Public

std::string string_utility::trim(const std::string &_String) {
	return trim_left(trim_right(_String));
}

std::string string_utility::trim_left(const std::string &_String) {

size_t _Off=_String.find_first_not_of(" ");
return (_Off!=std::string::npos?_String.substr(_Off):"");
}

std::string string_utility::trim_right(const std::string &_String) {

size_t _Off=_String.find_last_not_of(" ");
return (_Off!=std::string::npos?_String.substr(0,_Off+1):"");
}

std::string string_utility::lower_case(const std::string &_String) {

std::string _Result=_String; //Make copy
std::transform(_Result.begin(),_Result.end(),_Result.begin(),std::tolower);
return _Result;
}

std::string string_utility::upper_case(const std::string &_String) {

std::string _Result=_String; //Make copy
std::transform(_Result.begin(),_Result.end(),_Result.begin(),std::toupper);
return _Result;
}

std::string string_utility::field(const std::string &_String,
	const size_t &_Width,char _Fill_char,bool _Pad_left) {

	//Nothing to do
	if (_String.size()==_Width) return _String;

return (_String.size()>_Width?_String.substr(_Width):
	fill(_String,_Width,_Fill_char,_Pad_left));
}

std::string string_utility::fill(const std::string &_String,
	const size_t &_Width,char _Fill_char,bool _Pad_left) {

	//Nothing to do
	if (_String.size()>=_Width) return _String;

std::string _Padding(_Width-_String.size(),_Fill_char);
return (_Pad_left?_Padding+_String:_String+_Padding);
}

std::string string_utility::format(std::string _Format,
	float _Value) {

std::string _Raw_value=any_cast(_Value);

	//Nothing to do
	if (_Format.empty()) return _Raw_value;

size_t _Decimal_point=std::string::npos;
bool _Thousand_separator=false;

	for (size_t i=0;i<_Format.size();) {
	
		//Check for decimal point and thousand separator
		if (_Decimal_point==std::string::npos &&
			(_Format[i]=='.' || _Format[i]==',')) {
		
			if (!_Thousand_separator && i>0 && _Format[i]==',' &&
				(i+1==_Format.size() || _Format[i+1]=='.' || _Format[i+1]==',')) {
			
			_Thousand_separator=true;
			_Format.erase(i,1);
			continue;
			}
		
		_Decimal_point=i;
		}
		//Erase all non-supported values
		else if (_Format[i]!='#' && _Format[i]!='0') {
		
		_Format.erase(i,1);
		continue;
		}
	
	++i;
	}
	
	//Invalid format
	if (_Format.empty() || (_Decimal_point==0 && _Format.size()==1))
		return _Raw_value;
	
	//Format exponent
	if (_Value>=1e+006f || _Value<=9e-005f) {
	
	size_t _Pos=_Raw_value.find('e');
	
		//Should always be the case
		if (_Pos!=std::string::npos) {
		
		int _Exponent=any_cast<int>(_Raw_value.substr(_Pos+1));
		_Raw_value.erase(_Pos);
		
			//Erase decimal point
			if (_Raw_value[1]=='.') _Raw_value.erase(1,1);
			
			//Build value from exponent
			if (_Exponent>0) _Raw_value+=std::string(_Exponent-(_Raw_value.size()-1),'0');
			else _Raw_value="0."+std::string(abs(_Exponent)-1,'0')+_Raw_value;
		}
	}

size_t _Pos=_Raw_value.find('.');
std::string _Natural=_Raw_value.substr(0,_Pos);
std::string _Formated_value;

size_t _Size=_Format.substr(0,_Decimal_point).find('0');

	if (_Size==std::string::npos) _Size=0;
	else if (_Decimal_point==std::string::npos) _Size=_Format.size();
	else _Size=_Decimal_point-_Size;
	
	//Format the natural part
	if (_Decimal_point>0 && (_Size>0 || _Natural!="0")) {
	
	size_t _Diff=(_Size>_Natural.size()?_Size-_Natural.size():0);
	
		if (_Diff>0) _Formated_value=std::string(_Diff,'0');
		
		//Add blanks for each thousand
		if (_Thousand_separator && _Natural.size()>3) {
		
			for (size_t i=_Natural.size()-3;i>0;i-=(i>3?3:i))
				_Natural.insert(i,1,' ');
		}
	
	_Formated_value+=_Natural;
	}
	
	//Decimals found
	if (_Decimal_point!=std::string::npos && _Decimal_point+1<_Format.size()) {
	
	size_t _Decimal_size=_Format.substr(_Decimal_point).find_last_of('0');
	
		if (_Decimal_size==std::string::npos) _Decimal_size=0;
	
	std::string _Mantissa=(_Pos!=std::string::npos?_Raw_value.substr(_Pos+1):"0");
	
		//Format the mantissa part
		if (_Decimal_size>0 || (!_Mantissa.empty() && _Mantissa!="0")) {
		
		_Formated_value+=_Format[_Decimal_point]; //Add decimal point
		size_t _Diff=(_Decimal_size>_Mantissa.size()?_Decimal_size-_Mantissa.size():0);
		
			if (_Diff>0) _Formated_value+=_Mantissa+std::string(_Diff,'0');
			else _Formated_value+=_Mantissa.substr(0,_Format.size()-_Decimal_point-1);
		}
	}

return _Formated_value;
}

bool string_utility::compare(const std::string &_String,
	const std::string &_String2) {

	if (_String.size()!=_String2.size()) return false;

size_t _Size=_String.size(); //Both has equal size

	for (size_t i=0;i<_Size;++i) {
	
		if (std::tolower((unsigned char)_String[i])!=std::tolower(
			(unsigned char)_String2[i])) return false;
	}

return true;
}

unsigned int string_utility::replace(std::string &_String,
	const std::string &_Elem,const std::string &_Elem2) {

unsigned int _Count=0;
size_t _Off=0;

	while ((_Off=_String.find(_Elem,_Off))!=std::string::npos) {
	
	_String.replace(_Off,_Elem.size(),_Elem2);
	_Off+=_Elem2.size();
	++_Count;
	}

return _Count;
}

void string_utility::format_path(std::string &_Path,
	const std::string &_Delimiter) {

	//Nothing to do
	if (_Path.empty()) return;
	
	if (_Delimiter!="\\") string_utility::replace(_Path,"\\",_Delimiter);
	if (_Delimiter!="/") string_utility::replace(_Path,"/",_Delimiter);
	
	//Add path ending if missing
	if (_Path.size()>=_Delimiter.size() && _Path.substr(
		_Path.size()-_Delimiter.size())!=_Delimiter)
			_Path+=_Delimiter;
}

void string_utility::tokenize(const std::string &_String,
	const std::string &_Delimiter,tokens &_Tokens) {

char *_Data=new char[_String.size()+1];
strcpy(_Data,_String.c_str());

char *_Token=strtok(_Data,_Delimiter.c_str()); //Non-secure strtok

	while (_Token) {
	
	_Tokens.push_back(_Token);
	_Token=strtok(0,_Delimiter.c_str());
	}

delete[] _Data;
}

} //namespace lunar