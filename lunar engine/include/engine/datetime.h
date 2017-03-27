/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: datetime.h
---------------------------------------------------
*/

#ifndef _DATETIME_
#define _DATETIME_

#include <string>

namespace lunar {

class datetime {

private:

	time_t _Value;
	
	static const unsigned int _Day_difference;
	static const unsigned int _Seconds_perhour;
	static const unsigned int _Seconds_perday;
	
	void copy(const datetime &_Datetime);
	
	static time_t now();

public:

	//Constructor
	datetime();
	
	//Alternative constructor
	datetime(const time_t &_Value);
	
	//Copy constructor
	datetime(const datetime &_Datetime);
	
	//Destructor
	~datetime();
	
	operator time_t() const;
	operator std::string() const;
	
	datetime &operator=(const datetime &_Datetime);
	
	bool operator==(const datetime &_Datetime) const;
	bool operator!=(const datetime &_Datetime) const;
	
	bool operator<(const datetime &_Datetime) const;
	bool operator>(const datetime &_Datetime) const;
	
	bool operator<=(const datetime &_Datetime) const;
	bool operator>=(const datetime &_Datetime) const;
	
	//Arithmetic operators
	
	datetime &operator++(); //Prefix
	datetime &operator--(); //Prefix
	
	datetime operator++(int); //Postfix
	datetime operator--(int); //Postfix
	
	datetime operator+(const datetime &_Datetime) const;
	datetime operator-(const datetime &_Datetime) const;
	
	datetime &operator+=(const datetime &_Datetime);
	datetime &operator-=(const datetime &_Datetime);
	
	datetime operator+(unsigned int _Days) const;
	datetime operator-(unsigned int _Days) const;
	
	datetime &operator+=(unsigned int _Days);
	datetime &operator-=(unsigned int _Days);
	
	//Common functions
	
	static datetime current_date();
	static datetime current_datetime();
	
	static datetime encode_date(unsigned int _Year,
		unsigned int _Month,unsigned int _Day);
	
	static datetime encode_time(unsigned int _Hour,
		unsigned int _Minute,unsigned int _Second);
	
	static datetime encode_datetime(unsigned int _Year,
		unsigned int _Month,unsigned int _Day,
			unsigned int _Hour,unsigned int _Minute,
				unsigned int _Second);
	
	void decode_date(unsigned int &_Year,unsigned int &_Month,
		unsigned int &_Day) const;
	
	void decode_time(unsigned int &_Hour,unsigned int &_Minute,
		unsigned int &_Second) const;
	
	void decode_datetime(unsigned int &_Year,unsigned int &_Month,
		unsigned int &_Day,unsigned int &_Hour,unsigned int &_Minute,
			unsigned int &_Second) const;
	
	//Default format: yyyymmdd
	std::string date_string() const;
	
	//Default format: hh:nn:ss
	std::string time_string() const;
	
	//Default format: yyyymmdd hh:nn:ss
	std::string datetime_string() const;
	
	unsigned int day_ofweek() const;
	unsigned int day_ofyear() const;
	
	//Format functions
	
	//Format types:
	//
	//yyyy = 4 digit year
	//yy = 2 digit year
	//
	//mm = month
	//dd = day
	//
	//hh = hour
	//nn = minute
	//ss = second
	std::string format(const std::string &_Format) const;
};

} //namespace lunar

#endif