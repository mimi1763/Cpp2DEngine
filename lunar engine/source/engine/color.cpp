/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: color.cpp
---------------------------------------------------
*/

#include "color.h"

//Dependencies
#include "math_utility.h"

namespace lunar {

//Private

void color::copy(const color &_Color) {

r=_Color.r;
g=_Color.g;
b=_Color.b;
a=_Color.a;
}

//Public

const color color::ZERO(0,0,0,0.0f);
const color color::BLACK(0,0,0);
const color color::WHITE(255,255,255);
const color color::RED(255,0,0);
const color color::GREEN(0,255,0);
const color color::BLUE(0,0,255);

color::color():r(1.0f),g(1.0f),b(1.0f),a(1.0f) {
	//Empty
}

color::color(unsigned int _Rgba) {
	rgba(_Rgba);
}

color::color(unsigned char _Red,unsigned char _Green,
	unsigned char _Blue,float _Alpha) {
	rgba(_Red,_Green,_Blue,_Alpha);
}

color::color(const color &_Color) {
	copy(_Color);
}

color::~color() {
	//Empty
}

color &color::operator=(const color &_Color) {

	//Worth doing
	if (this!=&_Color) copy(_Color);

return *this;
}

color &color::operator=(unsigned int _Rgba) {

rgba(_Rgba);
return *this;
}

bool color::operator==(const color &_Color) const {
	return (r==_Color.r && g==_Color.g && b==_Color.b && a==_Color.a);
}

bool color::operator!=(const color &_Color) const {
	return !(*this==_Color);
}

void color::rgba(unsigned int _Rgba) {

r=(float)(_Rgba>>24)/255;
g=(float)(_Rgba>>16 & 0xff)/255;
b=(float)(_Rgba>>8 & 0xff)/255;
a=(float)(_Rgba & 0xff)/255;
}

void color::rgba(unsigned char _Red,unsigned char _Green,
	unsigned char _Blue,float _Alpha) {

r=(float)_Red/255;
g=(float)_Green/255;
b=(float)_Blue/255;

	if (_Alpha<0.0f) a=0.0f;
	else if (_Alpha>1.0f) a=1.0f;
	else a=_Alpha;
}

color color::mix(const color &_Color,float _Amount) const {

color _Mixed_color;
_Mixed_color.r=math::lerp(r,_Color.r,_Amount);
_Mixed_color.g=math::lerp(g,_Color.g,_Amount);
_Mixed_color.b=math::lerp(b,_Color.b,_Amount);
_Mixed_color.a=math::lerp(a,_Color.a,_Amount);

return _Mixed_color;
}

void color::clamp() {

	if (r>1.0f) r=1.0f;
	else if (r<0.0f) r=0.0f;
	
	if (g>1.0f) g=1.0f;
	else if (g<0.0f) g=0.0f;
	
	if (b>1.0f) b=1.0f;
	else if (b<0.0f) b=0.0f;
	
	if (a>1.0f) a=1.0f;
	else if (a<0.0f) a=0.0f;
}

void color::inverse() {

r=1.0f-r;
g=1.0f-g;
b=1.0f-b;
}

} //namespace lunar