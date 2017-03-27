/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shadervariable.h
---------------------------------------------------
*/

#ifndef _SHADERVARIABLE_
#define _SHADERVARIABLE_

#include <string>

namespace lunar {

//Forward declarations
class shaderprogram;
class shaderprogram_manager;

class shadervariable {

public:

	enum DATA_TYPE { SHADER_INT, SHADER_FLOAT };
	enum STATUS { OKAY, NEED_UPDATE, FAILED }; //Doesn't inherit from manageable

protected:

	std::string _Name;
	int _Location;
	
	void *_Data;
	unsigned int _Components;
	DATA_TYPE _Data_type;
	bool _Auto_update;
	
	STATUS _Status;
	bool _Update_data;
	
	//Protected constructor
	explicit shadervariable(const std::string &_Name,
		unsigned int _Components=1);
	
	//Protected constructor (int)
	shadervariable(const std::string &_Name,int *_Data,
		unsigned int _Components=1);
	
	//Protected constructor (float)
	shadervariable(const std::string &_Name,float *_Data,
		unsigned int _Components=1);
	
	//Protected copy constructor
	shadervariable(const shadervariable &_Shadervariable);
	
	//Protected virtual destructor
	virtual ~shadervariable();
	
	void copy(const shadervariable &_Shadervariable);

public:

	shadervariable &operator=(const shadervariable &_Shadervariable);
	
	bool operator==(const shadervariable &_Shadervariable) const;
	bool operator!=(const shadervariable &_Shadervariable) const;
	
	void name(const std::string &_Name);
	const std::string &name() const;
	
	unsigned int location() const;
	
	void data(int *_Data);
	void data(float *_Data);
	void *data() const;
	
	void components(unsigned int _Components);
	unsigned int components() const;
	
	DATA_TYPE data_type() const;
	
	void auto_update(bool _Auto_update);
	bool auto_update() const;
	
	STATUS status() const;
	
	void update_data();
};

} //namespace lunar

#endif