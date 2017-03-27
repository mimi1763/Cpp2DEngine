/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture_manager.cpp
---------------------------------------------------
*/

#include "texture_manager.h"

//Dependencies
#include <cstring>
#include "system.h"

#include "FreeImage.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
texture_manager *texture_manager::_Instance=0;

//Protected

void texture_manager::copy(const texture_manager &_Texture_manager) {
	_Texture_container=_Texture_manager._Texture_container;
}

//FreeImage and GL texture specific

bool texture_manager::create_gltexture(texture_data *_Texture_data,
	unsigned int _Texture_width,unsigned int _Texture_height) {

FIBITMAP *_Bitmap=0;

	//Image texture
	if (_Texture_data) {
	
	std::string _Bytes;
	
		//Make new bitmap from data file (memory)
		if (_Texture_container->load_bytes(_Texture_data,_Bytes)) {
		
		FIMEMORY *_Memory=FreeImage_OpenMemory((BYTE*)_Bytes.c_str(),
			(DWORD)_Bytes.size());
		FREE_IMAGE_FORMAT _Fileformat=FreeImage_GetFileTypeFromMemory(_Memory);
		_Bitmap=FreeImage_LoadFromMemory(_Fileformat,_Memory);
		FreeImage_CloseMemory(_Memory);
		
			if (_Bitmap==0) _Bytes.clear();
		}
		
		//Make new bitmap from file (filename)
		if (_Bytes.empty()) {
		
		FREE_IMAGE_FORMAT _Fileformat=FreeImage_GetFileType(
			_Texture_data->filename().c_str());
		_Bitmap=FreeImage_Load(_Fileformat,_Texture_data->filename().c_str());
		}
		
		if (_Bitmap==0) return false;
	}

//Get texture properties
unsigned int _Bitdepth=(_Bitmap?FreeImage_GetBPP(_Bitmap):32);
unsigned int _Format=(_Bitdepth==32?GL_RGBA:GL_RGB);
unsigned int _Width=(_Bitmap?FreeImage_GetWidth(_Bitmap):_Texture_width);
unsigned int _Height=(_Bitmap?FreeImage_GetHeight(_Bitmap):_Texture_height);

unsigned int _Glwidth=2;
unsigned int _Glheight=2;

	//Make sure each gltexture is power of 2
	while (_Glwidth<_Width) _Glwidth*=2;
	while (_Glheight<_Height) _Glheight*=2;
	
	if (_Bitmap && _Bitdepth<24) {
	
	_Bitmap=FreeImage_ConvertTo24Bits(_Bitmap);
	_Bitdepth=24;
	}

unsigned int _Size=_Width*_Height;
unsigned int _Glsize=_Glwidth*_Glheight;
unsigned int _Components=_Bitdepth/8;

unsigned char *_Pixels=(_Bitmap?(unsigned char*)FreeImage_GetBits(_Bitmap):0);
unsigned char *_Data=new unsigned char[_Glsize*_Components];
memset(_Data,0,_Glsize*_Components); //Clear buffer with color 0,0,0,0

	if (_Pixels) {
	
	//FreeImage uses a 32-bit boundaries for the scanlines
	//The pitch must be a multiple of 32 bits (4 bytes)
	unsigned int _Padding=FreeImage_GetPitch(_Bitmap)-(_Width*_Components);
	
		//BGRA to RGBA conversion
		for (unsigned int i=0,_Data_index=0,_Pixel_index=0;i<_Size;++i,
			_Data_index+=_Components,_Pixel_index+=_Components) {
		
			//Row complete
			if (i>0 && i%_Width==0) {
			
				//Align pixels for power of 2 gltexture
				if (_Width<_Glwidth) _Data_index+=
					(_Glwidth-_Width)*_Components;
			
			_Pixel_index+=_Padding;
			}
		
		#ifndef _BIG_ENDIAN_
		
		_Data[_Data_index+0]=_Pixels[_Pixel_index+2];
		_Data[_Data_index+1]=_Pixels[_Pixel_index+1];
		_Data[_Data_index+2]=_Pixels[_Pixel_index+0];
		#else
		
		_Data[_Data_index+0]=_Pixels[_Pixel_index+0];
		_Data[_Data_index+1]=_Pixels[_Pixel_index+1];
		_Data[_Data_index+2]=_Pixels[_Pixel_index+2];
		#endif
		
			if (_Bitdepth==32) _Data[_Data_index+3]=_Pixels[_Pixel_index+3];
		}
		
		//Texture is non power of 2 (less than allocated)
		//Fill pixels outside texture with edge color
		if (_Size<_Glsize) {
		
		unsigned int _Row_count=_Height;
		unsigned int _Column_count=_Glwidth-_Width;
		
			//Fill top right
			if (_Row_count*_Column_count>0) {
			
				for (unsigned int i=0,_Data_index=_Width*_Components;
					i<_Row_count;++i,_Data_index+=_Width*_Components) {
				
				unsigned int _Color_index=_Data_index-_Components;
				
					for (unsigned int j=0;j<_Column_count;++j,
						_Data_index+=_Components) {
					
					_Data[_Data_index+0]=_Data[_Color_index+0];
					_Data[_Data_index+1]=_Data[_Color_index+1];
					_Data[_Data_index+2]=_Data[_Color_index+2];
					
						if (_Bitdepth==32) _Data[_Data_index+3]=_Data[_Color_index+3];
					}
				}
			}
		
		_Row_count=_Glheight-_Height;
		_Column_count=_Width;
		
			//Fill bottom left
			if (_Row_count*_Column_count>0) {
			
				for (unsigned int i=0,_Data_index=_Glwidth*_Height*_Components;
					i<_Column_count;++i,_Data_index=_Glwidth*_Height*_Components) {
				
				_Data_index+=i*_Components;
				unsigned int _Color_index=_Data_index-(_Glwidth*_Components);
				
					for (unsigned int j=0;j<_Row_count;++j,
						_Data_index+=_Glwidth*_Components) {
					
					_Data[_Data_index+0]=_Data[_Color_index+0];
					_Data[_Data_index+1]=_Data[_Color_index+1];
					_Data[_Data_index+2]=_Data[_Color_index+2];
					
						if (_Bitdepth==32) _Data[_Data_index+3]=_Data[_Color_index+3];
					}
				}
			}
		
		_Row_count=_Glheight-_Height;
		_Column_count=_Glwidth-_Width;
		
			//Fill bottom right
			if (_Row_count*_Column_count) {
			
			unsigned int _Color_index=((_Glwidth*_Height)-(_Glwidth-_Width))*_Components;
			
				//Fill bottom right
				for (unsigned int i=0,_Data_index=((_Glwidth*_Height)+_Width)*_Components;
					i<_Row_count;++i,_Data_index+=_Width*_Components) {
				
					for (unsigned int j=0;j<_Column_count;++j,
						_Data_index+=_Components) {
					
					_Data[_Data_index+0]=_Data[_Color_index+0];
					_Data[_Data_index+1]=_Data[_Color_index+1];
					_Data[_Data_index+2]=_Data[_Color_index+2];
					
						if (_Bitdepth==32) _Data[_Data_index+3]=_Data[_Color_index+3];
					}
				}
			}
		}
	}
	
	if (_Bitmap) FreeImage_Unload(_Bitmap);

bool _Tileable=(_Texture_data?_Texture_data->tileable():false);

//Make OpenGL texture
unsigned int _Gltexture;
glGenTextures(1,&_Gltexture);
glBindTexture(GL_TEXTURE_2D,(GLuint)_Gltexture);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//Clamp texture edges to fix potensial artifacts (edge to edge interpolation)
	if (!_Tileable) {
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	}

glPixelStorei(GL_UNPACK_ALIGNMENT,1);
glTexImage2D(GL_TEXTURE_2D,0,(GLint)_Format,_Glwidth,_Glheight,
	0,(GLenum)_Format,GL_UNSIGNED_BYTE,_Data);

delete[] _Data;

_Created_gltexture _Mycreated_gltexture;
_Mycreated_gltexture._Texture_data=_Texture_data;
_Mycreated_gltexture._Gltexture=_Gltexture;
_Mycreated_gltexture._Texture_width=_Width;
_Mycreated_gltexture._Texture_height=_Height;
_Mycreated_gltexture._Gltexture_width=_Glwidth;
_Mycreated_gltexture._Gltexture_height=_Glheight;
_Mycreated_gltexture._Tileable=_Tileable;
_Mycreated_gltextures.push_front(_Mycreated_gltexture); //Faster access to newly added elements
return true;
}

void texture_manager::init_texture(texture *_Texture,
	const _Created_gltexture &_Mycreated_gltexture) {

_Texture->_Gltexture=_Mycreated_gltexture._Gltexture;
_Texture->_Texture_width=_Mycreated_gltexture._Texture_width;
_Texture->_Texture_height=_Mycreated_gltexture._Texture_height;
_Texture->_Gltexture_width=_Mycreated_gltexture._Gltexture_width;
_Texture->_Gltexture_height=_Mycreated_gltexture._Gltexture_height;
_Texture->_Tileable=_Mycreated_gltexture._Tileable;

	//Don't stretch as default
	if (_Texture->_Tileable) {
	
	_Texture->_Stretch_width=false;
	_Texture->_Stretch_height=false;
	}
}

//Public

texture_manager::texture_manager():_Texture_container(
	texture_container::instance()) {
	//Empty
}

texture_manager::texture_manager(const texture_manager &_Texture_manager):
	manager(_Texture_manager) {
	copy(_Texture_manager);
}

texture_manager::~texture_manager() {

tidy(); //Call before clearing cache
clear_cache();
}

texture_manager &texture_manager::operator=(const texture_manager &_Texture_manager) {

	//Worth doing
	if (this!=&_Texture_manager) {
	
	manager::operator=(_Texture_manager); //Invoke base operator=
	copy(_Texture_manager);
	}

return *this;
}

texture_manager *texture_manager::instance() {
	return _Instance;
}

void texture_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

texture *_Texture=dynamic_cast<texture*>(_Manageable);

	//Invalid type for this manager
	if (!_Texture) return;

texture_data *_Texture_data=static_cast<texture_data*>(
	_Manageable->data());

	//No data for image texture
	if (!_Texture_data && _Texture->type()==texture::IMAGE) {
	
	status(_Texture,manageable::FAILED);
	return;
	}
	
	//Reuse gltexture for image texture
	if (_Texture->_Type==texture::IMAGE) {
	
		for (_Created_gltextures::const_iterator _Iter=_Mycreated_gltextures.begin(),
			_End=_Mycreated_gltextures.end();_Iter!=_End;++_Iter) {
		
			if ((*_Iter)._Texture_data==_Texture_data) {
			
			init_texture(_Texture,*_Iter);
			status(_Texture,manageable::OKAY); //No need for update anymore
			return;
			}
		}
	}
	
	//Could not create gltexture
	if (!create_gltexture(_Texture_data,_Texture->_Texture_width,
		_Texture->_Texture_height)) {
	
	status(_Texture,manageable::FAILED);
	return;
	}

init_texture(_Texture,_Mycreated_gltextures.front());
status(_Texture,manageable::OKAY); //No need for update anymore
}

void texture_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

void texture_manager::texture_source(texture_container *_Texture_container) {
	this->_Texture_container=_Texture_container;
}

texture_container *texture_manager::texture_source() const {
	return _Texture_container;
}

//Texture functions

texture *texture_manager::create_texture(const std::string &_Name,
	bool _Visible,bool _Secured) {
	return create_texture(static_cast<texture_data*>(
		_Texture_container->data(_Name)),_Visible,_Secured);
}

texture *texture_manager::create_texture(const texture_data &_Texture_data,
	bool _Visible,bool _Secured) {
	return create_texture(_Texture_container->add_data(
		_Texture_data),_Visible,_Secured);
}

texture *texture_manager::create_texture(texture_data *_Texture_data,
	bool _Visible,bool _Secured) {

texture *_Mytexture=new texture(this,_Texture_data,_Visible);
add(_Mytexture,_Secured);
return _Mytexture;
}

texture *texture_manager::create_texture(unsigned int _Width,
	unsigned int _Height,bool _Visible,bool _Secured) {

texture *_Mytexture=new texture(this,_Width,_Height,_Visible);
add(_Mytexture,_Secured);
return _Mytexture;
}

texture *texture_manager::create_texture(const texture &_Texture,
	bool _Secured) {

texture *_Mytexture=new texture(this,_Texture);
add(_Mytexture,_Secured);
return _Mytexture;
}

void texture_manager::clear_cache() {

	for (_Created_gltextures::const_iterator _Iter=_Mycreated_gltextures.begin(),
		_End=_Mycreated_gltextures.end();_Iter!=_End;++_Iter)
			glDeleteTextures(1,&(*_Iter)._Gltexture);

_Mycreated_gltextures.clear();

	//Update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) status(
			*_Iter,manageable::NEED_UPDATE);
}

} //namespace lunar