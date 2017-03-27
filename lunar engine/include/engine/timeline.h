/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: timeline.h
---------------------------------------------------
*/

#ifndef _TIMELINE_
#define _TIMELINE_

namespace lunar {

class stopwatch {

public:

	struct time {
	
	float total;
	float interval;
	};

private:

	time _Time;
	bool _Enabled;
	
	long long _Ticks;
	
	void copy(const stopwatch &_Stopwatch);

public:

	//Constructor
	stopwatch();
	
	//Copy constructor
	stopwatch(const stopwatch &_Stopwatch);
	
	//Destructor
	~stopwatch();
	
	stopwatch &operator=(const stopwatch &_Stopwatch);
	
	void start();
	void stop(bool _Reset=true);
	
	const time &elapsed();
	
	bool enabled() const;
};

//Forward declarations
class core;
class timer;

class timeline {

	friend core; //Grant core exclusive access to initialize this

private:

	static stopwatch _Stopwatch;
	static long long _Frequency;
	
	//Private constructor
	timeline();
	
	//Private destructor
	~timeline();
	
	static void update(timer *_Timer);
	static void frequency(timer *_Timer);

public:

	static void reset();
	
	//Returns time elapsed in seconds
	static float elapsed();
	
	static bool high_resolution();
	
	//Utility functions
	
	static long long ticks();
	
	//Returns time elapsed between [from, to] in seconds
	static float elapsed(long long _From_ticks,
		long long _To_ticks);
};

} //namespace lunar

#endif