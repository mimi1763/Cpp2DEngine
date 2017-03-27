/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: mersenne_twister.cpp
---------------------------------------------------
*/

#include "mersenne_twister.h"

namespace lunar {

//Mersenne Twister 32-bit
//Private

void mersenne_twister32::copy(const mersenne_twister32 &_Mersenne_twister) {

	for (unsigned int i=0;i<_N;++i)
		_Mt[i]=_Mersenne_twister._Mt[i];

_Mti=_Mersenne_twister._Mti;
}

//Public

mersenne_twister32::mersenne_twister32():
	_Mti(_N+1) { //Unitialized
	//Empty
}

mersenne_twister32::mersenne_twister32(unsigned int _Seed) {
	init(_Seed);
}

mersenne_twister32::mersenne_twister32(unsigned int _Key[],int _Size) {
	init(_Key,_Size);
}

mersenne_twister32::mersenne_twister32(const mersenne_twister32 &_Mersenne_twister) {
	copy(_Mersenne_twister);
}

mersenne_twister32::~mersenne_twister32() {
	//Empty
}

mersenne_twister32 &mersenne_twister32::operator=(const mersenne_twister32 &_Mersenne_twister) {

	//Worth doing
	if (this!=&_Mersenne_twister) copy(_Mersenne_twister);

return *this;
}

void mersenne_twister32::init() {
	init(5489); //A default initial seed is used
}

void mersenne_twister32::init(unsigned int _Seed) {

_Mt[0]=_Seed & 0xffffffff;

	for (_Mti=1;_Mti<_N;++_Mti) {
	
	_Mt[_Mti]=(1812433253*(_Mt[_Mti-1]^(_Mt[_Mti-1]>>30))+_Mti); 
	_Mt[_Mti]&=0xffffffff; //For WORDSIZE > 32 bit machines
	}
}

void mersenne_twister32::init(unsigned int _Key[],int _Size) {

init(19650218);

int _I=1;
int _J=0;
int _K=(_N>_Size?_N:_Size);

	for (;_K>0;--_K) {
	
	_Mt[_I]=(_Mt[_I]^((_Mt[_I-1]^(_Mt[_I-1]>>30))*1664525))+
		_Key[_J]+_J; //Non linear
	_Mt[_I]&=0xffffffff; //For WORDSIZE > 32 machines
	++_I; ++_J;
	
		if (_I>=_N) {
		
		_Mt[0]=_Mt[_N-1];
		_I=1;
		}
		
		if (_J>=_Size) _J=0;
	}
	
	for (_K=_N-1;_K>0;--_K) {
	
	_Mt[_I]=(_Mt[_I]^((_Mt[_I-1]^(_Mt[_I-1]>>30))*1566083941))-
		_I; //Non linear
	_Mt[_I]&=0xffffffff; //For WORDSIZE > 32 machines
	++_I;
	
		if (_I>=_N) {
		
		_Mt[0]=_Mt[_N-1];
		_I=1;
		}
	}

_Mt[0]=0x80000000; //MSB is 1; assuring non-zero initial array
}

unsigned int mersenne_twister32::generate_int() {

static unsigned int _Mag01[2]={0x0, _Matrix_a};
//_Mag01[_X]=_X*_Matrix_a  for _X=0,1

unsigned int _X;

	//Generate _N words at once
	if (_Mti>=_N) {
	
		//Initialize with default seed
		if (_Mti==_N+1) init();
	
	int _I;
	
		for (_I=0;_I<_N-_M;++_I) {
		
		_X=(_Mt[_I] & _Upper_mask) | (_Mt[_I+1] & _Lower_mask);
		_Mt[_I]=_Mt[_I+_M]^(_X>>1)^_Mag01[_X & 0x1];
		}
		
		for (;_I<_N-1;++_I) {
		
		_X=(_Mt[_I] & _Upper_mask) | (_Mt[_I+1] & _Lower_mask);
		_Mt[_I]=_Mt[_I+(_M-_N)]^(_X>>1)^_Mag01[_X & 0x1];
		}
	
	_X=(_Mt[_N-1] & _Upper_mask) | (_Mt[0] & _Lower_mask);
	_Mt[_N-1]=_Mt[_M-1]^(_X>>1)^_Mag01[_X & 0x1];
	
	_Mti=0;
	}

_X=_Mt[_Mti++];

//Tempering
_X^=(_X>>11);
_X^=(_X<<7) & 0x9d2c5680;
_X^=(_X<<15) & 0xefc60000;
_X^=(_X>>18);

return _X;
}

double mersenne_twister32::generate_real() {
	return generate_int()*(1.0/4294967295.0); //Divided by 2^32-1
}

//Mersenne Twister 64-bit
//Private

void mersenne_twister64::copy(const mersenne_twister64 &_Mersenne_twister) {

	for (unsigned int i=0;i<_N;++i)
		_Mt[i]=_Mersenne_twister._Mt[i];

_Mti=_Mersenne_twister._Mti;
}

//Public

mersenne_twister64::mersenne_twister64():
	_Mti(_N+1) { //Unitialized
	//Empty
}

mersenne_twister64::mersenne_twister64(unsigned long long _Seed) {
	init(_Seed);
}

mersenne_twister64::mersenne_twister64(unsigned long long _Key[],
	int _Size) {
	init(_Key,_Size);
}

mersenne_twister64::mersenne_twister64(const mersenne_twister64 &_Mersenne_twister) {
	copy(_Mersenne_twister);
}

mersenne_twister64::~mersenne_twister64() {
	//Empty
}

mersenne_twister64 &mersenne_twister64::operator=(const mersenne_twister64 &_Mersenne_twister) {

	//Worth doing
	if (this!=&_Mersenne_twister) copy(_Mersenne_twister);

return *this;
}

void mersenne_twister64::init() {
	init(5489ULL); //A default initial seed is used
}

void mersenne_twister64::init(unsigned long long _Seed) {

_Mt[0]=_Seed;

	for (_Mti=1;_Mti<_N;++_Mti)
		_Mt[_Mti]=(6364136223846793005ULL*(_Mt[_Mti-1]^(_Mt[_Mti-1]>>62))+_Mti);
}

void mersenne_twister64::init(unsigned long long _Key[],int _Size) {

init(19650218ULL);

int _I=1;
int _J=0;
int _K=(_N>_Size?_N:_Size);

	for (;_K>0;--_K) {
	
	_Mt[_I]=(_Mt[_I]^((_Mt[_I-1]^(_Mt[_I-1]>>62))*3935559000370003845ULL))+
		_Key[_J]+_J; //Non linear
	++_I; ++_J;
	
		if (_I>=_N) {
		
		_Mt[0]=_Mt[_N-1];
		_I=1;
		}
		
		if (_J>=_Size) _J=0;
	}
	
	for (_K=_N-1;_K>0;--_K) {
	
	_Mt[_I]=(_Mt[_I]^((_Mt[_I-1]^(_Mt[_I-1]>>62))*2862933555777941757ULL))-
		_I; //Non linear
	++_I;
	
		if (_I>=_N) {
		
		_Mt[0]=_Mt[_N-1];
		_I=1;
		}
	}

_Mt[0]=1ULL<<63; //MSB is 1; assuring non-zero initial array
}

unsigned long long mersenne_twister64::generate_int() {

static unsigned long long _Mag01[2]={0ULL, _Matrix_a};
//_Mag01[_X]=_X*_Matrix_a  for _X=0,1

unsigned long long _X;

	//Generate _N words at once
	if (_Mti>=_N) {
	
		//Initialize with default seed
		if (_Mti==_N+1) init();
	
	int _I;
	
		for (_I=0;_I<_N-_M;++_I) {
		
		_X=(_Mt[_I] & _Upper_mask) | (_Mt[_I+1] & _Lower_mask);
		_Mt[_I]=_Mt[_I+_M]^(_X>>1)^_Mag01[(int)(_X & 1ULL)];
		}
		
		for (;_I<_N-1;++_I) {
		
		_X=(_Mt[_I] & _Upper_mask) | (_Mt[_I+1] & _Lower_mask);
		_Mt[_I]=_Mt[_I+(_M-_N)]^(_X>>1)^_Mag01[(int)(_X & 1ULL)];
		}
	
	_X=(_Mt[_N-1] & _Upper_mask) | (_Mt[0] & _Lower_mask);
	_Mt[_N-1]=_Mt[_M-1]^(_X>>1)^_Mag01[(int)(_X & 1ULL)];
	
	_Mti=0;
	}

_X=_Mt[_Mti++];

//Tempering
_X^=(_X>>29) & 0x5555555555555555ULL;
_X^=(_X<<17) & 0x71d67fffeda60000ULL;
_X^=(_X<<37) & 0xfff7eee000000000ULL;
_X^=(_X>>43);

return _X;
}

double mersenne_twister64::generate_real() {
	return (generate_int()>>11)*(1.0/9007199254740991.0); //Divided by 2^53-1
}

//Mersenne Twister
//Protected

void mersenne_twister::copy(const mersenne_twister &_Mersenne_twister) {

_Mersenne_twister32=_Mersenne_twister._Mersenne_twister32;
_Mersenne_twister64=_Mersenne_twister._Mersenne_twister64;
}

//Public

mersenne_twister::mersenne_twister() {
	//Empty
}

mersenne_twister::mersenne_twister(const mersenne_twister &_Mersenne_twister) {
	copy(_Mersenne_twister);
}

mersenne_twister::~mersenne_twister() {
	//Empty
}

mersenne_twister &mersenne_twister::operator=(const mersenne_twister &_Mersenne_twister) {

	//Worth doing
	if (this!=&_Mersenne_twister) copy(_Mersenne_twister);

return *this;
}

unsigned int mersenne_twister::generate_int32() {
	return _Mersenne_twister32.generate_int();
}

unsigned long long mersenne_twister::generate_int64() {
	return _Mersenne_twister64.generate_int();
}

float mersenne_twister::generate_single() {
	return (float)_Mersenne_twister32.generate_real();
}

double mersenne_twister::generate_double() {
	return _Mersenne_twister64.generate_real();
}

mersenne_twister32 &mersenne_twister::mt32() {
	return _Mersenne_twister32;
}

mersenne_twister64 &mersenne_twister::mt64() {
	return _Mersenne_twister64;
}

} //namespace lunar