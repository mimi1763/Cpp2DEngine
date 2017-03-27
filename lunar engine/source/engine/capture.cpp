/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: capture.cpp
---------------------------------------------------
*/

#include "capture.h"

//Dependencies
#include <cmath>
#include "system.h"

#include "texture.h"
#include "framebuffer.h"
#include "render_node.h"
#include "render_window.h"

#include "support.h"
#include "generic_utility.h"

namespace lunar {

//Protected

capture *capture::_Active=0;

void capture::copy(const capture &_Capture) {

_Width=_Capture._Width;
_Height=_Capture._Height;
_Position=_Capture._Position;

_Destination_framebuffer=_Capture._Destination_framebuffer;
_Destination_texture=_Capture._Destination_texture;
_Technique=_Capture._Technique;
}

void capture::unsubscribed(interface_base *_Interface) {

//Capture destination has been unsubscribed, notify capture

	//Remove capture destination
	if (_Destination_framebuffer==_Interface) _Destination_framebuffer=0;
	else if (_Destination_texture==_Interface) _Destination_texture=0;
}

void capture::manageable_changed(manageable *_Manageable) {
	//Empty - override if needed
}

void capture::manageable_statuschanged(manageable *_Manageable) {
	//Empty - override if needed
}

void capture::manageable_removed(manageable *_Manageable) {

//Capture destination has been destroyed, notify capture

capture::unsubscribed(_Manageable); //Reuse functionality
}

void capture::attach_framebuffer() {

//Can't add secure listeners because capture doesn't own framebuffer
//Secure listeners will not be broken before framebuffer is out of scope
//If public link is boken the capture will remove framebuffer

	//Add listener
	if (_Destination_framebuffer) _Destination_framebuffer->subscribe(this);
}

void capture::detach_framebuffer() {

_Listening=false; //Ignore notifications

	//Remove listener
	if (_Destination_framebuffer) _Destination_framebuffer->unsubscribe(this);

_Listening=true;
}

void capture::attach_texture() {

//Can't add secure listeners because capture doesn't own texture
//Secure listeners will not be broken before texture is out of scope
//If public link is boken the capture will remove texture

	//Add listener
	if (_Destination_texture) _Destination_texture->subscribe(this);
}

void capture::detach_texture() {

_Listening=false; //Ignore notifications

	//Remove listener
	if (_Destination_texture) _Destination_texture->unsubscribe(this);

_Listening=true;
}

void capture::start(framebuffer *_Framebuffer) {

render_window *_Render_window=render_window::instance();
float _Viewport_width=(float)_Render_window->width();
float _Viewport_height=(float)_Render_window->height();

	if (_Render_window->stretch()) {
	
		//Adjust viewport width to ortho width
		if (_Render_window->stretch_width()!=_Render_window->width()) {
		
		_Viewport_width=_Render_window->stretch_width();
		_Adjust_viewport=true;
		}
		
		//Adjust viewport height to ortho height
		if (_Render_window->stretch_height()!=_Render_window->height()) {
		
		_Viewport_height=_Render_window->stretch_height();
		_Adjust_viewport=true;
		}
	}

unsigned int _Framebuffer_width=_Framebuffer->width();
unsigned int _Framebuffer_height=_Framebuffer->height();

	//Capture width larger than framebuffer width
	//Need to adjust viewport
	if (_Width>_Framebuffer_width) {
	
	_Viewport_width*=_Framebuffer_width/_Width;
	_Adjusted_width=floor(_Viewport_width)*
		(_Framebuffer_width/_Viewport_width);
	_Adjust_viewport=true;
	}
	
	//Capture height larger than framebuffer height
	//Need to adjust viewport
	if (_Height>_Framebuffer_height) {
	
	_Viewport_height*=_Framebuffer_height/_Height;
	_Adjusted_height=floor(_Viewport_height)*
		(_Framebuffer_height/_Viewport_height);
	_Adjust_viewport=true;
	}

glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,_Framebuffer->glframebuffer());

	//Change viewport size
	if (_Adjust_viewport) glViewport(0,0,(GLsizei)
		_Viewport_width,(GLsizei)_Viewport_height);
}

void capture::stop(framebuffer *_Framebuffer,bool _Crop) {

	//Change viewport back to original size
	if (_Adjust_viewport) {
	
	glViewport(0,0,render_window::instance()->width(),
		render_window::instance()->height());
	_Adjust_viewport=false;
	}

glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);

	//Crop framebuffer texture
	if (_Crop) {
	
	_Framebuffer->framebuffer_texture()->auto_crop(false);
	_Framebuffer->framebuffer_texture()->crop_size(
		_Adjusted_width,_Adjusted_height);
	_Framebuffer->framebuffer_texture()->crop_position(vector2::ZERO);
	}
}

void capture::start(texture *_Texture) {

render_window *_Render_window=render_window::instance();
float _Viewport_width=(float)_Render_window->width();
float _Viewport_height=(float)_Render_window->height();

	if (_Render_window->stretch()) {
	
		//Might need to adjust viewport
		if (_Render_window->stretch_width()!=_Render_window->width()) {
		
			//Ortho width larger than window width
			//Capture width might need to adjust if region is outside window
			if (_Render_window->stretch_width()>_Render_window->width() &&
				_Position.x+_Width>_Render_window->width()) {
			
			_Adjusted_width=_Width*(_Render_window->width()/
				_Render_window->stretch_width());
			_Adjusted_position.x=_Render_window->width()*
				(_Position.x/_Render_window->stretch_width());
			}
			else {
			
			_Viewport_width=_Render_window->stretch_width();
			_Adjust_viewport=true;
			}
		}
		
		//Might need to adjust viewport
		if (_Render_window->stretch_height()!=_Render_window->height()) {
		
			//Ortho height larger than window height
			//Capture height might need to adjust if region is outside window
			if (_Render_window->stretch_height()>_Render_window->height() &&
				_Position.y+_Height>_Render_window->height()) {
			
			_Adjusted_height=_Height*(_Render_window->height()/
				_Render_window->stretch_height());
			_Adjusted_position.y=_Render_window->height()*
				(_Position.y/_Render_window->stretch_height());
			}
			else {
			
			_Viewport_height=_Render_window->stretch_height();
			_Adjust_viewport=true;
			}
		}
	}

unsigned int _Gltexture_width=_Texture->gltexture_width();
unsigned int _Gltexture_height=_Texture->gltexture_height();

	//Capture width larger than gl texture width
	//Need to adjust viewport
	if (_Adjusted_width>_Gltexture_width) {
	
	float _Ortho_width=_Viewport_width;
	_Viewport_width*=_Gltexture_width/_Adjusted_width;
	
	//Re-adjust capture size/position to new viewport width
	_Adjusted_width=floor(_Viewport_width)*
		(_Gltexture_width/_Viewport_width);
	_Adjusted_position.x=floor(_Viewport_width)*
		(_Adjusted_position.x/_Ortho_width);
	
	_Adjust_viewport=true;
	}
	
	//Capture height larger than gl texture height
	//Need to adjust viewport
	if (_Adjusted_height>_Gltexture_height) {
	
	float _Ortho_height=_Viewport_height;
	_Viewport_height*=_Gltexture_height/_Adjusted_height;
	
	//Re-adjust capture size/position to new viewport height
	_Adjusted_height=floor(_Viewport_height)*
		(_Gltexture_height/_Viewport_height);
	_Adjusted_position.y=floor(_Viewport_height)*
		(_Adjusted_position.y/_Ortho_height);
	
	_Adjust_viewport=true;
	}
	
	//Change viewport size
	if (_Adjust_viewport) glViewport(0,0,(GLsizei)
		_Viewport_width,(GLsizei)_Viewport_height);
}

void capture::stop(texture *_Texture,bool _Crop) {

glBindTexture(GL_TEXTURE_2D,_Texture->gltexture());
glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,(GLint)floor(_Adjusted_position.x),
	(GLint)floor(_Adjusted_position.y),_Texture->gltexture_width(),
		_Texture->gltexture_height());

	//Change viewport back to original size
	if (_Adjust_viewport) {
	
	glViewport(0,0,render_window::instance()->width(),
		render_window::instance()->height());
	_Adjust_viewport=false;
	}
	
	//Crop texture
	if (_Crop) {
	
	_Texture->auto_crop(false);
	_Texture->crop_size(_Adjusted_width,_Adjusted_height);
	_Texture->crop_position(vector2(_Adjusted_position.x-floor(
		_Adjusted_position.x),_Adjusted_position.y-floor(
			_Adjusted_position.y)));
	}
}

void capture::snapshot(texture *_Texture,bool _Crop) {

render_window *_Render_window=render_window::instance();

	//May need to adjust size and position
	if (_Render_window->stretch()) {
	
		//Ortho width different than window width
		if (_Render_window->stretch_width()!=_Render_window->width()) {
		
		_Adjusted_width=_Width*(_Render_window->width()/
			_Render_window->stretch_width());
		_Adjusted_position.x=_Render_window->width()*
			(_Position.x/_Render_window->stretch_width());
		}
		
		//Ortho height different than window height
		if (_Render_window->stretch_height()!=_Render_window->height()) {
		
		_Adjusted_height=_Height*(_Render_window->height()/
			_Render_window->stretch_height());
		_Adjusted_position.y=_Render_window->height()*
			(_Position.y/_Render_window->stretch_height());
		}
	}

unsigned int _Gltexture_width=_Texture->gltexture_width();
unsigned int _Gltexture_height=_Texture->gltexture_height();

glBindTexture(GL_TEXTURE_2D,_Texture->gltexture());
glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,(GLint)floor(_Adjusted_position.x),
	(GLint)floor(_Adjusted_position.y),_Gltexture_width,_Gltexture_height);

	//Crop texture
	if (_Crop) {
	
	_Texture->auto_crop(false);
	_Texture->crop_size(_Adjusted_width<_Gltexture_width?_Adjusted_width:
		_Gltexture_width,_Adjusted_height<_Gltexture_height?
			_Adjusted_height:_Gltexture_height);
	_Texture->crop_position(vector2(_Adjusted_position.x-floor(
		_Adjusted_position.x),_Adjusted_position.y-floor(
			_Adjusted_position.y)));
	}
}

//Public

capture::capture():_Width(0.0f),_Height(0.0f),_Destination_framebuffer(0),
	_Destination_texture(0),_Technique(FBO),_Adjusted_width(0.0f),
		_Adjusted_height(0.0f),_Adjust_viewport(false) {
	//Empty
}

capture::capture(const capture &_Capture) {

//Not copyable
_Adjusted_width=0.0f;
_Adjusted_height=0.0f;
_Adjust_viewport=false;

copy(_Capture);
attach_framebuffer();
attach_texture();
}

capture::capture(const rectangle &_Rectangle) {

//Not copyable
_Destination_framebuffer=0;
_Destination_texture=0;
_Technique=FBO;

_Adjusted_width=0.0f;
_Adjusted_height=0.0f;
_Adjust_viewport=false;

*this=_Rectangle;
}

capture::~capture() {

cancel();

detach_framebuffer();
detach_texture();
}

capture &capture::operator=(const capture &_Capture) {

	//Object in use
	if (_Active==this) return *this;
	
	//Worth doing
	if (this!=&_Capture) {
	
	bool _Framebuffer_changed=(_Destination_framebuffer!=_Capture.
		_Destination_framebuffer);
	bool _Texture_changed=(_Destination_texture!=_Capture.
		_Destination_texture);
	
		if (_Framebuffer_changed) detach_framebuffer();
		if (_Texture_changed) detach_texture();
	
	copy(_Capture);
	
		if (_Framebuffer_changed) attach_framebuffer();
		if (_Texture_changed) attach_texture();
	}

return *this;
}

capture &capture::operator=(const rectangle &_Rectangle) {

//No important properties
//Can be called while object is being used

_Width=_Rectangle.width();
_Height=_Rectangle.height();

render_node *_Parent_node=_Rectangle.parent_node();
position(_Parent_node?_Parent_node->world_position():vector2::ZERO,
	_Rectangle.origin());
return *this;
}

void capture::start() {

	//A concurrent capture task is running
	if (_Active) return;

render_node *_Active_rendernode=render_manager::instance()->active_rendernode();

	//Capturing is only allowed while in 'offscreen' rendering
	//(use snapshot to capture 'onscreen' rendering)
	if (!_Active_rendernode || _Active_rendernode->draw_order()>=0) return;
	
	//Nothing to capture
	if ((int)_Width==0 || (int)_Height==0) return;
	
	//Missing capture destination
	if (!_Destination_framebuffer && !_Destination_texture) return;

_Adjusted_width=_Width;
_Adjusted_height=_Height;
_Adjusted_position=_Position;
_Adjust_viewport=false;

	//Use framebuffer object
	if (_Technique==FBO && _Destination_framebuffer &&
		_Position==vector2::ZERO && support::fbo()!=support::NONE) {
	
	_Destination_framebuffer->update(); //Make sure it's up to date
	
		if (_Destination_framebuffer->status()==manageable::OKAY) {
		
		start(_Destination_framebuffer);
		_Active=this; //Start capture
		return;
		}
	}

texture *_Texture=_Destination_texture;

	//Try to use framebuffer texture
	if (!_Texture && _Destination_framebuffer)
		_Texture=_Destination_framebuffer->framebuffer_texture();
	
	//Use texture
	if (_Texture) {
	
	_Texture->update(); //Make sure it's up to date
	
		if (_Texture->status()==manageable::OKAY) {
		
		start(_Texture);
		_Active=this; //Start capture
		return;
		}
	}
}

void capture::stop(bool _Crop) {

	//Capture not started
	if (_Active!=this) return;
	
	//Use framebuffer object
	if (_Technique==FBO && _Destination_framebuffer &&
		_Position==vector2::ZERO && support::fbo()!=support::NONE) {
	
		if (_Destination_framebuffer->status()==manageable::OKAY) {
		
		stop(_Destination_framebuffer,_Crop);
		_Active=0; //Capture complete
		return;
		}
	}

//Use texture
stop(_Destination_texture?_Destination_texture:
	_Destination_framebuffer->framebuffer_texture(),_Crop);
_Active=0; //Capture complete
}

void capture::cancel() {

	if (_Active==this) {
	
		//Change viewport back to original size
		if (_Adjust_viewport) {
		
		glViewport(0,0,render_window::instance()->width(),
			render_window::instance()->height());
		_Adjust_viewport=false;
		}
		
		//Use framebuffer object
		if (_Technique==FBO && _Destination_framebuffer &&
			_Position==vector2::ZERO && support::fbo()!=support::NONE &&
				_Destination_framebuffer->status()==manageable::OKAY)
					glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	
	_Active=0;
	}
}

void capture::snapshot(bool _Crop) {

render_node *_Active_rendernode=render_manager::instance()->active_rendernode();

	//Snapshots are only allowed while in 'onscreen' rendering
	if (!_Active_rendernode || _Active_rendernode->draw_order()<0) return;
	
	//Nothing to snapshot
	if ((int)_Width==0 || (int)_Height==0) return;
	
	//Missing snapshot destination texture
	if (!_Destination_texture) return;

_Destination_texture->update(); //Make sure it's up to date

	if (_Destination_texture->status()==manageable::OKAY) {
	
	_Adjusted_width=_Width;
	_Adjusted_height=_Height;
	_Adjusted_position=_Position;
	
	snapshot(_Destination_texture,_Crop);
	}
}

void capture::size(float _Width,float _Height) {

this->_Width=(_Width<0.0f?0.0f:_Width);
this->_Height=(_Height<0.0f?0.0f:_Height);
}

float capture::width() const {
	return _Width;
}

float capture::height() const {
	return _Height;
}

void capture::position(const vector2 &_Position,const rectangle::ORIGIN &_Origin) {

//Transform position to bottom left origin
//Make sure capture size is set
this->_Position=utility::position(_Position,_Width,_Height,
	_Origin,rectangle::BOTTOM_LEFT);
}

const vector2 &capture::position() const {
	return _Position;
}

void capture::destination(framebuffer *_Destination) {

	//Destination in use
	if (_Active==this) return;
	
	if (_Destination_framebuffer!=_Destination) {
	
	detach_framebuffer();
	_Destination_framebuffer=_Destination;
	attach_framebuffer();
	}
}

framebuffer *capture::destination_framebuffer() const {
	return _Destination_framebuffer;
}

void capture::destination(texture *_Destination) {

	//Destination in use
	if (_Active==this) return;
	
	//Custom texture required
	if (_Destination && _Destination->type()!=texture::CUSTOM) return;
	
	if (_Destination_texture!=_Destination) {
	
	detach_texture();
	_Destination_texture=_Destination;
	attach_texture();
	}
}

texture *capture::destination_texture() const {
	return _Destination_texture;
}

void capture::technique(const TECHNIQUE &_Technique) {

	//Technique in use
	if (_Active==this) return;

this->_Technique=_Technique;
}

capture::TECHNIQUE capture::technique() const {
	return _Technique;
}

} //namespace lunar