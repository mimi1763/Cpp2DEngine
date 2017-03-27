/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sha.h
---------------------------------------------------
*/

#ifndef _SHA_
#define _SHA_

namespace lunar {

class sha {

private:

	static const unsigned int _Sha256init[8];
	static const unsigned int _Sha256round[64];
	static const unsigned char _Padding[64];
	
	unsigned int _Sha256[8];
	unsigned int _Bitcount[2];
	unsigned char _Buffer[64];
	
	inline unsigned int rightrotate(unsigned int _Value,int _Bits) const {
		return (_Value>>_Bits) | (_Value<<(32-_Bits));
	}
	
	void bytetoword(const unsigned char *_Byte,unsigned int _Size,unsigned int *_Word);
	void wordtobyte(const unsigned int *_Word,unsigned int _Size,unsigned char *_Byte);
	
	void transform(const unsigned char *_Byte);

public:

	//Constructor
	sha();
	
	//Destructor
	~sha();
	
	const unsigned int *hash(const unsigned char *_Byte,unsigned int _Size);
	
	void update(const unsigned char *_Byte,unsigned int _Size);
	const unsigned int *final();
	
	void reset();
};

} //namespace lunar

#endif