/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: color.h
---------------------------------------------------
*/

#ifndef _COLOR_
#define _COLOR_

namespace lunar {

class color {

private:

	void copy(const color &_Color);

public:

	//Color values in range [0.0, 1.0]
	//For direct use with OpenGL
	union {
	
		struct {
		
		float r;
		float g;
		float b;
		float a;
		};
	
	float ptr[4];
	};
	
	static const color ZERO;
	static const color BLACK;
	static const color WHITE;
	static const color RED;
	static const color GREEN;
	static const color BLUE;
	
	//Constructor
	color();
	
	//Alternative constructor
	explicit color(unsigned int _Rgba);
	
	//Alternative constructor
	color(unsigned char _Red,unsigned char _Green,
		unsigned char _Blue,float _Alpha=1.0f);
	
	//Copy constructor
	color(const color &_Color);
	
	//Destructor
	~color();
	
	color &operator=(const color &_Color);
	color &operator=(unsigned int _Rgba);
	
	bool operator==(const color &_Color) const;
	bool operator!=(const color &_Color) const;
	
	void rgba(unsigned int _Rgba);
	void rgba(unsigned char _Red,unsigned char _Green,
		unsigned char _Blue,float _Alpha=1.0f);
	
	color mix(const color &_Color,float _Amount=0.5f) const;
	
	void clamp();
	void inverse();
};

} //namespace lunar

#endif