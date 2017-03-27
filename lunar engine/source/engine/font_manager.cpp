/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font_manager.cpp
---------------------------------------------------
*/

#include "font_manager.h"

//Dependencies
#include "system.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
font_manager *font_manager::_Instance=0;

//Protected

void font_manager::copy(const font_manager &_Font_manager) {
	_Font_container=_Font_manager._Font_container;
}

//FreeType specific

bool font_manager::init_freetype() {

_Library=0;
return (FT_Init_FreeType(&_Library)==0);
}

bool font_manager::create_face(font_data *_Font_data) {

FT_Face _Face;
std::string _Bytes;

	//Make new face from data file (memory)
	if (_Font_container->load_bytes(_Font_data,_Bytes)) {
	
		if (FT_New_Memory_Face(_Library,(const FT_Byte*)_Bytes.c_str(),
			(FT_Long)_Bytes.size(),(FT_Long)_Font_data->face(),&_Face)!=0)
				_Bytes.clear();
	}
	
	//Make new face from file (filename)
	if (_Bytes.empty() && FT_New_Face(_Library,_Font_data->filename().c_str(),
		(FT_Long)_Font_data->face(),&_Face)!=0) return false;

//If this fails, just continue
FT_Set_Char_Size(_Face,(FT_F26Dot6)_Font_data->width()*64,
	(FT_F26Dot6)_Font_data->height()*64,(FT_UInt)_Font_data->
		horizontal_res(),(FT_UInt)_Font_data->vertical_res());

unsigned int _Glyphs=(_Font_data->encoding()==font_data::ASCII?128:256);

_Created_face _Mycreated_face;
_Mycreated_face._Font_data=_Font_data;
_Mycreated_face._Listbase=glGenLists(_Glyphs);
_Mycreated_face._Glyphs=_Glyphs;
_Mycreated_face._Height=0; //Initial height
_Mycreated_face._Advance=new unsigned int[_Glyphs];
_Mycreated_face._Gltexture=new unsigned int[_Glyphs];
glGenTextures(_Glyphs,_Mycreated_face._Gltexture);

	for (unsigned int i=0;i<_Glyphs;++i) {
	
		//Load glyph, if failed continue to the next glyph
		if (FT_Load_Glyph(_Face,FT_Get_Char_Index(_Face,(FT_ULong)i),
			FT_LOAD_DEFAULT)!=0) {
		
		_Mycreated_face._Advance[i]=0; //Don't advance
		continue;
		}
		
		//Render glyph, if failed continue to next glyph
		if (FT_Render_Glyph(_Face->glyph,FT_RENDER_MODE_NORMAL)!=0) {
		
		_Mycreated_face._Advance[i]=0; //Don't advance
		continue;
		}
	
	_Mycreated_face._Advance[i]=_Face->glyph->advance.x;
	
		//Update height if the height of the glyph is higher than current height
		if (_Mycreated_face._Height<(unsigned int)_Face->glyph->bitmap.rows)
			_Mycreated_face._Height=_Face->glyph->bitmap.rows;
	
	int _Width=2;
	int _Height=2;
	
		//Make sure each glyph's gltexture is power of 2
		while (_Width<_Face->glyph->bitmap.width) _Width*=2;
		while (_Height<_Face->glyph->bitmap.rows) _Height*=2;
	
	unsigned char *_Data=new unsigned char[_Width*_Height*2];
	
		//Convert FreeType bitmap data to OpenGL texture data
		for (int y=0;y<_Height;++y) {
		
			for (int x=0;x<_Width;++x) {
			
			_Data[2*(x+y*_Width)]=255; //Anti-aliasing fix
			_Data[2*(x+y*_Width)+1]=(x>=_Face->glyph->bitmap.width ||
				y>=_Face->glyph->bitmap.rows?0:_Face->glyph->bitmap.
					buffer[x+_Face->glyph->bitmap.width*y]);
			}
		}
	
	//Make OpenGL texture
	glBindTexture(GL_TEXTURE_2D,(GLuint)_Mycreated_face._Gltexture[i]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,_Width,_Height,0,GL_LUMINANCE_ALPHA,
		GL_UNSIGNED_BYTE,_Data);
	delete[] _Data;
	
	//Make display list for this texture
	glNewList((GLuint)_Mycreated_face._Listbase+i,GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D,(GLuint)_Mycreated_face._Gltexture[i]);
	
	glPushMatrix();
	glTranslatef((GLfloat)_Face->glyph->bitmap_left,0,0);
	glTranslatef(0,(GLfloat)_Face->glyph->bitmap_top-_Face->glyph->bitmap.rows,0);
	
	//Texture coordinates
	float _S=(float)_Face->glyph->bitmap.width/_Width;
	float _T=(float)_Face->glyph->bitmap.rows/_Height;
	
	//Note:
	//The texture coordinates follows [0,0] -> [width,height] (GUI coordinate system)
	//The vertices follows [0,height] -> [width,0] (normal coordinate system)
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2i(0,_Face->glyph->bitmap.rows);
	glTexCoord2f(0,_T); glVertex2i(0,0);
	glTexCoord2f(_S,_T); glVertex2i(_Face->glyph->bitmap.width,0);
	glTexCoord2f(_S,0); glVertex2i(_Face->glyph->bitmap.width,_Face->glyph->bitmap.rows);
	glEnd();
	
	glPopMatrix();
	glTranslatef((GLfloat)_Face->glyph->advance.x/64,0,0); //Translate relative
	glEndList();
	}
	
	//Use default height if no visual face glyph found
	if (_Mycreated_face._Height==0) _Mycreated_face._Height=_Font_data->height();

FT_Done_Face(_Face);
_Mycreated_faces.push_front(_Mycreated_face); //Faster access to newly added elements
return true;
}

void font_manager::init_font(font *_Font,const _Created_face &_Mycreated_face) {

_Font->_Listbase=_Mycreated_face._Listbase;
_Font->_Glyphs=_Mycreated_face._Glyphs;
_Font->_Height=_Mycreated_face._Height;
_Font->_Advance=_Mycreated_face._Advance;
}

//Public

font_manager::font_manager():_Font_container(
	font_container::instance()) {
	_Supported=init_freetype();
}

font_manager::font_manager(const font_manager &_Font_manager):
	manager(_Font_manager) {

_Supported=init_freetype();
copy(_Font_manager);
}

font_manager::~font_manager() {

tidy(); //Call before clearing cache
clear_cache();

	if (_Supported) FT_Done_FreeType(_Library);
}

font_manager &font_manager::operator=(const font_manager &_Font_manager) {

	//Worth doing
	if (this!=&_Font_manager) {
	
	manager::operator=(_Font_manager); //Invoke base operator=
	copy(_Font_manager);
	}

return *this;
}

font_manager *font_manager::instance() {
	return _Instance;
}

void font_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

font *_Font=dynamic_cast<font*>(_Manageable);

	//Invalid type for this manager
	if (!_Font) return;
	
	//Not supported
	if (!_Supported) {
	
	status(_Font,manageable::FAILED);
	return;
	}

font_data *_Font_data=static_cast<font_data*>(
	_Manageable->data());

	//No data
	if (!_Font_data) {
	
	status(_Font,manageable::FAILED);
	return;
	}
	
	for (_Created_faces::const_iterator _Iter=_Mycreated_faces.begin(),
		_End=_Mycreated_faces.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)._Font_data==_Font_data) {
		
		init_font(_Font,*_Iter);
		status(_Font,manageable::OKAY); //No need for update anymore
		return;
		}
	}
	
	//Could not create face
	if (!create_face(_Font_data)) {
	
	status(_Font,manageable::FAILED);
	return;
	}

init_font(_Font,_Mycreated_faces.front());
status(_Font,manageable::OKAY); //No need for update anymore
}

void font_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

void font_manager::font_source(font_container *_Font_container) {
	this->_Font_container=_Font_container;
}

font_container *font_manager::font_source() const {
	return _Font_container;
}

//Font functions

font *font_manager::create_font(const std::string &_Name,bool _Secured) {
	return create_font(static_cast<font_data*>(
		_Font_container->data(_Name)),_Secured);
}

font *font_manager::create_font(const font_data &_Font_data,bool _Secured) {
	return create_font(_Font_container->add_data(
		_Font_data),_Secured);
}

font *font_manager::create_font(font_data *_Font_data,bool _Secured) {

font *_Myfont=new font(this,_Font_data);
add(_Myfont,_Secured);
return _Myfont;
}

font *font_manager::create_font(const font &_Font,bool _Secured) {

font *_Myfont=new font(this,_Font);
add(_Myfont,_Secured);
return _Myfont;
}

void font_manager::clear_cache() {

	for (_Created_faces::const_iterator _Iter=_Mycreated_faces.begin(),
		_End=_Mycreated_faces.end();_Iter!=_End;++_Iter) {
	
	const _Created_face &_Mycreated_face=(*_Iter); //Increase speed
	
	glDeleteLists((GLuint)_Mycreated_face._Listbase,_Mycreated_face._Glyphs);
	glDeleteTextures(_Mycreated_face._Glyphs,_Mycreated_face._Gltexture);
	
	delete[] _Mycreated_face._Advance;
	delete[] _Mycreated_face._Gltexture;
	}

_Mycreated_faces.clear();

	//Update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) status(
			*_Iter,manageable::NEED_UPDATE);
}

} //namespace lunar