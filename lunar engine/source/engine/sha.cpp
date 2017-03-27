/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sha.cpp
---------------------------------------------------
*/

#include "sha.h"

//Dependencies
#include <cstring> //memcpy

namespace lunar {

//Private

const unsigned int sha::_Sha256init[8]={
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

const unsigned int sha::_Sha256round[64]={ 
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

const unsigned char sha::_Padding[64]={
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void sha::bytetoword(const unsigned char *_Byte,unsigned int _Size,unsigned int *_Word) {

	//Big-endian
	for (unsigned int i=0,j=0;j<_Size;++i,j+=4) _Word[i]=(unsigned int)(_Byte[j]<<24) | 
		(unsigned int)(_Byte[j+1]<<16) | (unsigned int)(_Byte[j+2]<<8) | 
			(unsigned int)_Byte[j+3];
}

void sha::wordtobyte(const unsigned int *_Word,unsigned int _Size,unsigned char *_Byte) {

	//Big-endian
	for (unsigned int i=_Size,j=0;i>0;--i,j+=4) {
	
	_Byte[j]=(unsigned char)((_Word[i-1]>>24) & 0xff);
	_Byte[j+1]=(unsigned char)((_Word[i-1]>>16) & 0xff);
	_Byte[j+2]=(unsigned char)((_Word[i-1]>>8) & 0xff);
	_Byte[j+3]=(unsigned char)(_Word[i-1] & 0xff);
	}
}

void sha::transform(const unsigned char *_Byte) {

unsigned int _Word[64];
bytetoword(_Byte,64,_Word);

unsigned int _S0=0;
unsigned int _S1=0;
unsigned int _T1=0;
unsigned int _T2=0;
unsigned int _Maj=0;
unsigned int _Ch=0;

	//Extend the sixteen 32 bit words into sixty-four 32 bit words
	for (unsigned int i=16;i<64;++i) {
	
	_S0=rightrotate(_Word[i-15],7)^rightrotate(_Word[i-15],18)^(_Word[i-15]>>3);
	_S1=rightrotate(_Word[i-2],17)^rightrotate(_Word[i-2],19)^(_Word[i-2]>>10);
	_Word[i]=_Word[i-16]+_S0+_Word[i-7]+_S1;
	}

unsigned int _Hash[8];

	for (unsigned int i=0;i<8;++i) _Hash[i]=_Sha256[i];
	
	//Main SHA256 loop
	for (unsigned int i=0;i<64;++i) {
	
    _S0=rightrotate(_Hash[0],2)^rightrotate(_Hash[0],13)^rightrotate(_Hash[0],22);
    _Maj=(_Hash[0] & _Hash[1])^(_Hash[0] & _Hash[2])^(_Hash[1] & _Hash[2]);
    _T2=_S0+_Maj;
    _S1=rightrotate(_Hash[4],6)^rightrotate(_Hash[4],11)^rightrotate(_Hash[4],25);
    _Ch=(_Hash[4] & _Hash[5])^(~_Hash[4] & _Hash[6]);
    _T1=_Hash[7]+_S1+_Ch+_Sha256round[i]+_Word[i];
    
    _Hash[7]=_Hash[6];
    _Hash[6]=_Hash[5];
    _Hash[5]=_Hash[4];
    _Hash[4]=_Hash[3]+_T1;
    _Hash[3]=_Hash[2];
    _Hash[2]=_Hash[1];
    _Hash[1]=_Hash[0];
    _Hash[0]=_T1+_T2;
	}
	
	//Update current hash
	for (unsigned int i=0;i<8;++i) _Sha256[i]+=_Hash[i];
}

//Public

sha::sha() {
	reset();
}

sha::~sha() {
	//Empty
}

const unsigned int *sha::hash(const unsigned char *_Byte,unsigned int _Size) {

reset();
update(_Byte,_Size);
return final();
}

void sha::update(const unsigned char *_Byte,unsigned int _Size) {

unsigned int _Index=(_Bitcount[0]>>3) & 0x3F;

	//Update number of bits
	if ((_Bitcount[0]+=_Size<<3)<_Size<<3) ++_Bitcount[1];

_Bitcount[1]+=_Size>>29;
unsigned int _Partsize=64-_Index;
unsigned int _Idx=0;

	//Transform as many times as possible
	if (_Size>=_Partsize) {
	
	memcpy(&_Buffer[_Index],_Byte,_Partsize);
	transform(_Buffer);
	
		for (_Idx=_Partsize;_Idx+63<_Size;_Idx+=64) transform(&_Byte[_Idx]);
	
	_Index=0;
	}

memcpy(&_Buffer[_Index],&_Byte[_Idx],_Size-_Idx); //Buffer remaining data
}

const unsigned int *sha::final() {

unsigned char _Bits[8];
wordtobyte(_Bitcount,2,_Bits); //Save number of bits

unsigned int _Index=(_Bitcount[0]>>3) & 0x3f;
unsigned int _Paddingsize=(_Index<56?56-_Index:120-_Index); //Pad out to 56 mod 64

update(_Padding,_Paddingsize);
update(_Bits,8);
return _Sha256;
}

void sha::reset() {

	for (unsigned int i=0;i<8;++i) _Sha256[i]=_Sha256init[i];

_Bitcount[0]=0;
_Bitcount[1]=0;
}

} //namespace lunar