/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: mersenne_twister.h
---------------------------------------------------
*/

#ifndef _MERSENNE_TWISTER_
#define _MERSENNE_TWISTER_

#include "random_generator.h"

namespace lunar {

class mersenne_twister32 {

private:

	static const int _N=624;
	static const int _M=397;
	
	static const unsigned int _Matrix_a=0x9908b0df; //Constant vector a
	static const unsigned int _Upper_mask=0x80000000; //Most significant w-r bits
	static const unsigned int _Lower_mask=0x7fffffff; //Least significant r bits
	
	unsigned int _Mt[_N]; //The array for the state vector
	int _Mti;
	
	void copy(const mersenne_twister32 &_Mersenne_twister);

public:

	//Constructor
	mersenne_twister32();
	
	//Alternative constructor
	mersenne_twister32(unsigned int _Seed);
	
	//Alternative constructor
	mersenne_twister32(unsigned int _Key[],int _Size);
	
	//Copy constructor
	mersenne_twister32(const mersenne_twister32 &_Mersenne_twister);
	
	//Destructor
	~mersenne_twister32();
	
	mersenne_twister32 &operator=(const mersenne_twister32 &_Mersenne_twister);
	
	//Initializes with a default seed
	void init();
	
	//Initializes with a seed
	void init(unsigned int _Seed);
	
	//Initializes with one or more keys
	void init(unsigned int _Key[],int _Size);
	
	//Generate value in range [0, 4 294 967 295]
	unsigned int generate_int();
	
	//Generate value in range [0.0, 1.0]
	double generate_real();
};

class mersenne_twister64 {

private:

	static const int _N=312;
	static const int _M=156;
	
	static const unsigned long long _Matrix_a=0xb5026f5aa96619e9ULL; //Constant vector a
	static const unsigned long long _Upper_mask=0xffffffff80000000ULL; //Most significant w-r bits
	static const unsigned long long _Lower_mask=0x7fffffffULL; //Least significant r bits
	
	unsigned long long _Mt[_N]; //The array for the state vector
	int _Mti;
	
	void copy(const mersenne_twister64 &_Mersenne_twister);

public:

	//Constructor
	mersenne_twister64();
	
	//Alternative constructor
	mersenne_twister64(unsigned long long _Seed);
	
	//Alternative constructor
	mersenne_twister64(unsigned long long _Key[],int _Size);
	
	//Copy constructor
	mersenne_twister64(const mersenne_twister64 &_Mersenne_twister);
	
	//Destructor
	~mersenne_twister64();
	
	mersenne_twister64 &operator=(const mersenne_twister64 &_Mersenne_twister);
	
	//Initializes with a default seed
	void init();
	
	//Initializes with a seed
	void init(unsigned long long _Seed);
	
	//Initializes with one or more keys
	void init(unsigned long long _Key[],int _Size);
	
	//Generate value in range [0, 18 446 744 073 709 551 615]
	unsigned long long generate_int();
	
	//Generate value in range [0.0, 1.0]
	double generate_real();
};

class mersenne_twister:public random_generator {

protected:

	mersenne_twister32 _Mersenne_twister32;
	mersenne_twister64 _Mersenne_twister64;
	
	void copy(const mersenne_twister &_Mersenne_twister);

public:

	//Constructor
	mersenne_twister();
	
	//Copy constructor
	mersenne_twister(const mersenne_twister &_Mersenne_twister);
	
	//Virtual destructor
	virtual ~mersenne_twister();
	
	mersenne_twister &operator=(const mersenne_twister &_Mersenne_twister);
	
	//Generate value in range [0, 4 294 967 295]
	virtual unsigned int generate_int32();
	
	//Generate value in range [0, 18 446 744 073 709 551 615]
	virtual unsigned long long generate_int64();
	
	//Generate value in range [0.0, 1.0] with single precision
	virtual float generate_single();
	
	//Generate value in range [0.0, 1.0] with double precision
	virtual double generate_double();
	
	mersenne_twister32 &mt32();
	mersenne_twister64 &mt64();
};

} //namespace lunar

#endif

/*
Algorithms used:

Mersenne Twister 32-bit version
Mersenne Twister 64-bit version


Copyright (C) 1997 - 2004, Makoto Matsumoto and Takuji Nishimura,
All rights reserved.                          

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 3. The names of its contributors may not be used to endorse or promote 
    products derived from this software without specific prior written 
    permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/