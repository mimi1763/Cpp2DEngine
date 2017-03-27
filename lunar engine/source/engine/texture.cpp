/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture.cpp
---------------------------------------------------
*/

#include "texture.h"

//Dependencies
#include "texture_manager.h"

namespace lunar {

//Protected

texture::texture(texture_manager *_Owner,texture_data *_Texture_data,
	bool _Visible):manageable(_Owner,_Texture_data),
		texturable(_Visible),_Gltexture(0),_Type(IMAGE) {
	//Empty
}

texture::texture(texture_manager *_Owner,unsigned int _Width,
	unsigned int _Height,bool _Visible):
		manageable(_Owner,0),texturable(_Visible),_Gltexture(0),
			_Type(CUSTOM) {

_Texture_width=_Width; //Must store width for custom texture
_Texture_height=_Height; //Must store height for custom texture
}

texture::texture(const texture &_Texture):manageable(_Texture),
	texturable(_Texture) {

	//Not copyable if custom
	if (_Texture._Type==CUSTOM) _Gltexture=0;

copy(_Texture);
}

texture::texture(texture_manager *_Owner,const texture &_Texture):
	manageable(_Owner,_Texture),texturable(_Texture) {

	//Request gltexture if custom
	if (_Texture._Type==CUSTOM) _Gltexture=0;

copy(_Texture);
}

texture::~texture() {
	//Empty
}

void texture::copy(const texture &_Texture) {

_Type=_Texture._Type;

	//Image texture
	if (_Type==IMAGE) {
	
	_Gltexture=_Texture._Gltexture;
	status(_Texture._Status); //Inherit status
	}
}

void texture::pre_draw() {

	//Use base functionality
	if (_Status==OKAY) texturable::pre_draw();
	//Skip texturable functionality
	else rectangle::pre_draw();
}

void texture::post_draw() {

	//Use base functionality
	if (_Status==OKAY) texturable::post_draw();
	//Skip texturable functionality
	else rectangle::post_draw();
}

//Public

texture &texture::operator=(const texture &_Texture) {

	//Worth doing
	if (this!=&_Texture) {
	
	manageable::operator=(_Texture); //Invoke base operator=
	texturable::operator=(_Texture); //Invoke base operator=
	copy(_Texture);
	}

return *this;
}

bool texture::prepare() {

	//Update if needed (increase speed)
	if (_Status==NEED_UPDATE) update();
	
	//Use base functionality
	if (_Status==OKAY) return texturable::prepare();

//Fallback on rectangle if update failed
//Skip texturable functionality
return rectangle::prepare();
}

void texture::data(texture_data *_Texture_data) {
	manageable::data(_Texture_data); //Data contains the correct derived data type
}

texture_data *texture::data() const {
	return static_cast<texture_data*>(manageable::data());
}

unsigned int texture::gltexture() const {
	return _Gltexture;
}

texture::TYPE texture::type() const {
	return _Type;
}

} //namespace lunar