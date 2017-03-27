/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texturable.cpp
---------------------------------------------------
*/

#include "texturable.h"

//Dependencies
#include <cmath>
#include <utility>
#include "system.h"

#include "math_utility.h"
#include "support.h"

namespace lunar {

//Protected

texturable::texturable(bool _Visible):rectangle(20,20,color::WHITE,
	_Visible),_Texture_width(1),_Texture_height(1),_Gltexture_width(1),
		_Gltexture_height(1),_Tileable(false),_Auto_size(true),
			_Stretch_width(true),_Stretch_height(true),
				_Flip_horizontal(false),_Flip_vertical(false),_Rotation(ROTATE_NONE),
					_Ratio_mode(RATIO_FIT_INSIDE),_Keep_ratio(false),_Cut_mode(CUT_TOP_RIGHT),
						_Crop_width(0.0f),_Crop_height(0.0f),_Auto_crop(true) {
	_Update_texcoords=true; //Update texcoords
}

texturable::texturable(const texturable &_Texturable):
	rectangle(_Texturable) {

_Update_texcoords=true; //Update texcoords
copy(_Texturable);
}

texturable::~texturable() {
	//Empty
}

void texturable::copy(const texturable &_Texturable) {

_Texture_width=_Texturable._Texture_width;
_Texture_height=_Texturable._Texture_height;
_Gltexture_width=_Texturable._Gltexture_width;
_Gltexture_height=_Texturable._Gltexture_height;
_Tileable=_Texturable._Tileable;

_Auto_size=_Texturable._Auto_size;
_Stretch_width=_Texturable._Stretch_width;
_Stretch_height=_Texturable._Stretch_height;

_Flip_horizontal=_Texturable._Flip_horizontal;
_Flip_vertical=_Texturable._Flip_vertical;
_Rotation=_Texturable._Rotation;

_Ratio_mode=_Texturable._Ratio_mode;
_Keep_ratio=_Texturable._Keep_ratio;

_Cut_mode=_Texturable._Cut_mode;
_Crop_width=_Texturable._Crop_width;
_Crop_height=_Texturable._Crop_height;
_Crop_position=_Texturable._Crop_position;
_Auto_crop=_Texturable._Auto_crop;
}

void texturable::pre_draw() {

	if (!_Shaderprograms_enabled || support::shader()==support::NONE) {
	
	glBindTexture(GL_TEXTURE_2D,gltexture());
	glEnable(GL_TEXTURE_2D);
	}

rectangle::pre_draw(); //Use base functionality
}

void texturable::post_draw() {

rectangle::post_draw(); //Use base functionality

	if (!_Shaderprograms_enabled || support::shader()==support::NONE)
		glDisable(GL_TEXTURE_2D);
}

void texturable::update_vertices() {

	//Texture is initialized
	if (_Gltexture_width>1) {

		//Auto size
		if (_Auto_size) {

		_Width=(float)_Texture_width;
		_Height=(float)_Texture_height;
		}
		//Keep ratio
		else if (_Keep_ratio) {

			switch (_Ratio_mode) {

				case RATIO_KEEP_WIDTH:
				_Height=(float)_Texture_height/_Texture_width*_Width;
				break;

				case RATIO_KEEP_HEIGHT:
				_Width=(float)_Texture_width/_Texture_height*_Height;
				break;

				case RATIO_FIT_INSIDE: {

				//Find texture ratio
				float _Texturewidth_ratio=(float)_Texture_width/_Texture_height;
				float _Textureheight_ratio=1.0f/_Texturewidth_ratio;

				//Find ratio
				float _Width_ratio=_Width/_Height;
				float _Height_ratio=1.0f/_Width_ratio;

					//Texture width less than height
					if (_Texture_width<_Texture_height) {

						//Texture ratio less than ratio
						if (_Texturewidth_ratio<_Width_ratio)
							_Width=_Texturewidth_ratio*_Height;
						else
							_Height=_Textureheight_ratio*_Width;
					}
					else {

						//Texture ratio less than ratio
						if (_Textureheight_ratio<_Height_ratio)
							_Height=_Textureheight_ratio*_Width;
						else
							_Width=_Texturewidth_ratio*_Height;
					}

				break;
				}

				case RATIO_FULLY_COVER: {

				//Find texture ratio
				float _Texturewidth_ratio=(float)_Texture_width/_Texture_height;
				float _Textureheight_ratio=1.0f/_Texturewidth_ratio;

				//Find ratio
				float _Width_ratio=_Width/_Height;
				float _Height_ratio=1.0f/_Width_ratio;

					//Texture width greater than height
					if (_Texture_width>_Texture_height) {

						//Texture ratio grater than ratio
						if (_Texturewidth_ratio>_Width_ratio)
							_Width=_Texturewidth_ratio*_Height;
						else
							_Height=_Textureheight_ratio*_Width;
					}
					else {

						//Texture ratio greater than ratio
						if (_Textureheight_ratio>_Height_ratio)
							_Height=_Textureheight_ratio*_Width;
						else
							_Width=_Texturewidth_ratio*_Height;
					}

				break;
				}
			}
		}
	}

rectangle::update_vertices();

	//Tileable size has changed
	//Must update texcoords
	if (_Tileable && (!_Stretch_width || !_Stretch_height))
		_Update_texcoords=true;
}

void texturable::update_texcoords() {

float _S=(_Tileable && !_Stretch_width?_Width:
	_Texture_width)/_Gltexture_width;
float _T=(_Tileable && !_Stretch_height?_Height:
	_Texture_height)/_Gltexture_height;

float _X=0.0f;
float _Y=0.0f;

CUT_MODE _Cut_mode=CUT_TOP_RIGHT;

	//Crop manually
	if (!_Auto_crop) {
	
	_S=((_Tileable && !_Stretch_width) || _Crop_width<_Gltexture_width?
		_Crop_width:_Gltexture_width)/_Gltexture_width;
	_T=((_Tileable && !_Stretch_height) || _Crop_height<_Gltexture_height?
		_Crop_height:_Gltexture_height)/_Gltexture_height;
	
		if (_Crop_position!=vector2::ZERO) {
		
		_X=fabs(_Crop_position.x)/_Gltexture_width;
		_Y=fabs(_Crop_position.y)/_Gltexture_height;
		
			if (_X>1.0f) _X=1.0f;
			if (_Y>1.0f) _Y=1.0f;
		}
		
		if ((_S+=_X)>1.0f) _S=1.0f;
		if ((_T+=_Y)>1.0f) _T=1.0f;
	
	_Cut_mode=this->_Cut_mode;
	}

/*
	0,1--6,7
	|      |
	|      |
	2,3--4,5
*/

unsigned int _Idx[8]={0,1,2,3,4,5,6,7};

	//Flip texcoords horizontally
	if (_Flip_horizontal) {
	
	_Idx[0]=6; _Idx[1]=7;
	_Idx[2]=4; _Idx[3]=5;
	_Idx[4]=2; _Idx[5]=3;
	_Idx[6]=0; _Idx[7]=1;
	}
	
	//Flip texcoords vertically
	if (_Flip_vertical) {
	
	unsigned int _Tmp[8]={_Idx[0],_Idx[1],_Idx[2],_Idx[3],
						  _Idx[4],_Idx[5],_Idx[6],_Idx[7]};
	
	_Idx[0]=_Tmp[2]; _Idx[1]=_Tmp[3];
	_Idx[2]=_Tmp[0]; _Idx[3]=_Tmp[1];
	_Idx[4]=_Tmp[6]; _Idx[5]=_Tmp[7];
	_Idx[6]=_Tmp[4]; _Idx[7]=_Tmp[5];
	}
	
	//Add rotation to texcoords
	if (_Rotation!=ROTATE_NONE) {
	
	unsigned int _Tmp[8]={_Idx[0],_Idx[1],_Idx[2],_Idx[3],
						  _Idx[4],_Idx[5],_Idx[6],_Idx[7]};
	
		//180 degrees
		if (_Rotation==ROTATE_180) {
		
		_Idx[0]=_Tmp[4]; _Idx[1]=_Tmp[5];
		_Idx[2]=_Tmp[6]; _Idx[3]=_Tmp[7];
		_Idx[4]=_Tmp[0]; _Idx[5]=_Tmp[1];
		_Idx[6]=_Tmp[2]; _Idx[7]=_Tmp[3];
		}
		//90 degrees clockwise
		else if (_Rotation==ROTATE_90CW) {
		
		_Idx[0]=_Tmp[6]; _Idx[1]=_Tmp[7];
		_Idx[2]=_Tmp[0]; _Idx[3]=_Tmp[1];
		_Idx[4]=_Tmp[2]; _Idx[5]=_Tmp[3];
		_Idx[6]=_Tmp[4]; _Idx[7]=_Tmp[5];
		}
		//90 degrees counter-clockwise
		else if (_Rotation==ROTATE_90CCW) {
		
		_Idx[0]=_Tmp[2]; _Idx[1]=_Tmp[3];
		_Idx[2]=_Tmp[4]; _Idx[3]=_Tmp[5];
		_Idx[4]=_Tmp[6]; _Idx[5]=_Tmp[7];
		_Idx[6]=_Tmp[0]; _Idx[7]=_Tmp[1];
		}
		
		//Swap texcoords
		if (_Rotation!=ROTATE_180 && _Tileable &&
			(!_Stretch_width || !_Stretch_height))
				std::swap(_S,_T);
	}
	
	//Cut texcoords
	switch (_Cut_mode) {
	
		case CUT_TOP_LEFT:
		_Texcoords[_Idx[0]]=1.0f-_S; _Texcoords[_Idx[1]]=_T;
		_Texcoords[_Idx[2]]=1.0f-_S; _Texcoords[_Idx[3]]=_Y;
		_Texcoords[_Idx[4]]=1.0f-_X; _Texcoords[_Idx[5]]=_Y;
		_Texcoords[_Idx[6]]=1.0f-_X; _Texcoords[_Idx[7]]=_T;
		break;
		
		case CUT_TOP_RIGHT:
		_Texcoords[_Idx[0]]=_X; _Texcoords[_Idx[1]]=_T;
		_Texcoords[_Idx[2]]=_X; _Texcoords[_Idx[3]]=_Y;
		_Texcoords[_Idx[4]]=_S; _Texcoords[_Idx[5]]=_Y;
		_Texcoords[_Idx[6]]=_S; _Texcoords[_Idx[7]]=_T;
		break;
		
		case CUT_BOTTOM_LEFT:
		_Texcoords[_Idx[0]]=1.0f-_S; _Texcoords[_Idx[1]]=1.0f-_Y;
		_Texcoords[_Idx[2]]=1.0f-_S; _Texcoords[_Idx[3]]=1.0f-_T;
		_Texcoords[_Idx[4]]=1.0f-_X; _Texcoords[_Idx[5]]=1.0f-_T;
		_Texcoords[_Idx[6]]=1.0f-_X; _Texcoords[_Idx[7]]=1.0f-_Y;
		break;
		
		case CUT_BOTTOM_RIGHT:
		_Texcoords[_Idx[0]]=_X; _Texcoords[_Idx[1]]=1.0f-_Y;
		_Texcoords[_Idx[2]]=_X; _Texcoords[_Idx[3]]=1.0f-_T;
		_Texcoords[_Idx[4]]=_S; _Texcoords[_Idx[5]]=1.0f-_T;
		_Texcoords[_Idx[6]]=_S; _Texcoords[_Idx[7]]=1.0f-_Y;
		break;
	}
}

//Public

texturable &texturable::operator=(const texturable &_Texturable) {

	//Worth doing
	if (this!=&_Texturable) {
	
	rectangle::operator=(_Texturable); //Invoke base operator=
	copy(_Texturable);
	
	_Update_texcoords=true; //Update texcoords
	}

return *this;
}

//Common functions

unsigned int texturable::texture_width() const {
	return _Texture_width;
}

unsigned int texturable::texture_height() const {
	return _Texture_height;
}

unsigned int texturable::gltexture_width() const {
	return _Gltexture_width;
}

unsigned int texturable::gltexture_height() const {
	return _Gltexture_height;
}

void texturable::auto_size(bool _Auto_size) {

	if (this->_Auto_size!=_Auto_size) {
	
	this->_Auto_size=_Auto_size;
	
		if (_Width!=_Texture_width || _Height!=_Texture_height)
			_Update_vertices=true; //Update vertices
	}
}

bool texturable::auto_size() const {
	return _Auto_size;
}

void texturable::stretch(bool _Stretch_width,bool _Stretch_height) {

	if (this->_Stretch_width!=_Stretch_width ||
		this->_Stretch_height!=_Stretch_height) {
	
	this->_Stretch_width=_Stretch_width;
	this->_Stretch_height=_Stretch_height;
	
		//Only supported if tileable
		//Update texcoords
		if (_Tileable) _Update_texcoords=true;
	}
}

bool texturable::stretch_width() const {
	return _Stretch_width;
}

bool texturable::stretch_height() const {
	return _Stretch_height;
}

void texturable::flip(bool _Flip_horizontal,bool _Flip_vertical) {

	if (this->_Flip_horizontal!=_Flip_horizontal ||
		this->_Flip_vertical!=_Flip_vertical) {
	
	this->_Flip_horizontal=_Flip_horizontal;
	this->_Flip_vertical=_Flip_vertical;
	 _Update_texcoords=true; //Update texcoords
	}
}

bool texturable::flip_horizontal() const {
	return _Flip_horizontal;
}

bool texturable::flip_vertical() const {
	return _Flip_vertical;
}

void texturable::rotation(const ROTATION &_Rotation) {

	if (this->_Rotation!=_Rotation) {
	
	this->_Rotation=_Rotation;
	_Update_texcoords=true; //Update texcoords
	}
}

texturable::ROTATION texturable::rotation() const {
	return _Rotation;
}

//Ratio functions

void texturable::ratio_mode(const RATIO_MODE &_Ratio_mode) {

	if (this->_Ratio_mode!=_Ratio_mode) {
	
	this->_Ratio_mode=_Ratio_mode;
	_Update_vertices=true; //Update vertices
	}
}

void texturable::keep_ratio(bool _Keep_ratio) {
	this->_Keep_ratio=_Keep_ratio;
}

bool texturable::keep_ratio() const {
	return _Keep_ratio;
}

//Cut/crop functions

void texturable::cut_mode(const CUT_MODE &_Cut_mode) {

	if (this->_Cut_mode!=_Cut_mode) {
	
	this->_Cut_mode=_Cut_mode;
	_Update_texcoords=true; //Update texcoords
	}
}

texturable::CUT_MODE texturable::cut_mode() const {
	return _Cut_mode;
}

void texturable::crop_size(float _Crop_width,float _Crop_height) {

float _New_width=(_Crop_width<0.0f?0.0f:_Crop_width);
float _New_height=(_Crop_height<0.0f?0.0f:_Crop_height);

	if (this->_Crop_width!=_New_width || this->_Crop_height!=_New_height) {
	
	this->_Crop_width=_New_width;
	this->_Crop_height=_New_height;
	
		//Update texcoords
		if (!_Auto_crop) _Update_texcoords=true;
	}
}

float texturable::crop_width() const {
	return _Crop_width;
}

float texturable::crop_height() const {
	return _Crop_height;
}

void texturable::crop_position(const vector2 &_Crop_position) {

	if (this->_Crop_position!=_Crop_position) {
	
	this->_Crop_position=_Crop_position;
	
		//Update texcoords
		if (!_Auto_crop) _Update_texcoords=true;
	}
}

const vector2 &texturable::crop_position() const {
	return _Crop_position;
}

void texturable::auto_crop(bool _Auto_crop) {

	if (this->_Auto_crop!=_Auto_crop) {
	
	this->_Auto_crop=_Auto_crop;
	_Update_texcoords=true; //Update texcoords
	}
}

bool texturable::auto_crop() const {
	return _Auto_crop;
}

} //namespace lunar