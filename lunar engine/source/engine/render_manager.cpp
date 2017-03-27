/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: render_manager.cpp
---------------------------------------------------
*/

#include "render_manager.h"

//Dependencies
#include "system.h"

#include "render_node.h"
#include "attachable.h"
#include "shaderprogram_manager.h"
#include "capture.h"

#include "support.h"
#include "math_utility.h"

#include "timeline.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
render_manager *render_manager::_Instance=0;

//Protected

render_manager::render_manager():_Root_node(new render_node(0)),
	_Active_rendernode(0),_Active_attachable(0) {
	//Empty
}

render_manager::render_manager(const render_manager &_Render_manager) {

_Root_node=new render_node(0); //Not copyable
_Active_rendernode=0; //Not copyable
_Active_attachable=0; //Not copyable

copy(_Render_manager);
}

render_manager::~render_manager() {
	delete _Root_node;
}

render_manager &render_manager::operator=(const render_manager &_Render_manager) {

	//Worth doing
	if (this!=&_Render_manager) copy(_Render_manager);

return *this;
}

void render_manager::copy(const render_manager &_Render_manager) {
	//Empty - override default memcpy behaviour
}

void render_manager::render(attachable *_Attachable) const {

_Attachable->pre_draw();

	//Render attachable with one or more shader programs
	if (_Attachable->_Shaderprograms_enabled && !_Attachable->
		_Myshaderprograms.empty() && support::shader()!=support::NONE) {
	
	shaderprogram *_Shaderprogram;
	
		//Render attachable in one or multiple passes
		for (unsigned int i=0;i<_Attachable->_Passes;++i) {
		
			for (attachable::_Shaderprograms::const_iterator _Iter=_Attachable->
				_Myshaderprograms.begin(),_End=_Attachable->_Myshaderprograms.end();
					_Iter!=_End;++_Iter) {
			
			_Shaderprogram=(*_Iter); //Increase speed
			
			_Shaderprogram->update();
			
				if (_Shaderprogram->status()!=manageable::OKAY) continue;
				
				//Enable shader program
				if (support::shader()==support::OPENGL)
					glUseProgram(_Shaderprogram->glprogram());
				else if (support::shader()==support::ARB)
					glUseProgramObjectARB(_Shaderprogram->glprogram());
			
			static_cast<shaderprogram_manager*>(_Shaderprogram->owner())->
				update_shadervariables(_Shaderprogram);
			
			blend(_Shaderprogram->blending());
			_Attachable->draw();
			}
		}
		
		//Disable programs
		if (support::shader()==support::OPENGL) glUseProgram(0);
		else if (support::shader()==support::ARB) glUseProgramObjectARB(0);
	}
	//Render attachable with fixed functionality
	else {
	
	blend(_Attachable->_Blending);
	
		//Render attachable in one or multiple passes
		for (unsigned int i=0;i<_Attachable->_Passes;++i)
			_Attachable->draw();
	}

_Attachable->post_draw();
}

void render_manager::blend(const BLENDING &_Blending) const {

	switch (_Blending) {
	
		case DEFAULT_BLEND:
		glBlendFunc(GL_ONE,GL_ZERO);
		break;
		
		case DEFAULT_ALPHA_BLEND:
		
			//Improved alpha blending
			if (support::blend_separate()==support::OPENGL)
				glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
					GL_SRC_ALPHA,GL_ONE);
			//Normal alpha blending
			else glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		
		break;
		
		case ADDITIVE_BLEND:
		glBlendFunc(GL_ONE,GL_ONE);
		break;
		
		case ADDITIVE_ALPHA_BLEND:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
		
		case LIGHT_BLEND:
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
}

//Public

render_manager *render_manager::instance() {
	return _Instance;
}

void render_manager::render() {

render_node *_Render_node;
attachable *_Attachable;

	for (render_node::_Graph::const_iterator _Iter=render_node::
		_Mygraph.begin(),_End=render_node::_Mygraph.end();
			_Iter!=_End;++_Iter) {
	
	_Active_rendernode=_Render_node=(*_Iter).second; //Increase speed
	
		if (_Render_node->_Visible) {
		
		//The render node pre/post draws can be called without any attachables
		//They can be used for pure callback reasons, like a dummy node where
		//the visible state can be used as a flag to enable/disable callback
		_Render_node->pre_draw();
		
			//Something to draw. Scale, translate and rotate node
			if (!_Render_node->_Myattachables.empty()) {
			
			const vector2 &_Position=_Render_node->world_position();
			float _Angle=_Render_node->world_rotation();
			const vector2 &_Scaling=_Render_node->world_scaling();
			
			glPushMatrix(); //Push matrix for this node
			glTranslatef(_Position.x,_Position.y,0.0f); //Translate to origin
			
				if (_Angle!=0.0f) glRotatef(math::radian(_Angle),
					0.0f,0.0f,1.0f); //Rotate around center
				if (_Scaling!=vector2::UNIT_SCALE) glScalef(
					_Scaling.x,_Scaling.y,1.0f); //Scale to correct size
				
				for (render_node::_Attachables::const_iterator _Aiter=_Render_node->
					_Myattachables.begin(),_Aend=_Render_node->_Myattachables.end();
						_Aiter!=_Aend;++_Aiter) {
				
				_Active_attachable=_Attachable=(*_Aiter); //Increase speed
				
					//Not visible
					if (!_Attachable->_Visible) continue;
					
					//Could not prepare for rendering
					if (!_Attachable->prepare()) continue;
				
				render(_Attachable);
				
				//For debugging purposes only
				#ifdef _DEBUG
				
					if (_Attachable->_Show_bounds) {
					
					glPopMatrix(); //Draw bounds in world coordinates
					
					_Attachable->world_boundingsphere().draw();
					_Attachable->world_orientedboundingbox().draw();
					_Attachable->world_boundingbox().draw();
					
						//Restore relative matrix for next attachable
						if (++_Aiter!=_Aend) {
						
						glPushMatrix();
						glLoadMatrixf(_Render_node->glmatrix()); 
						}
					
					--_Aiter;
					}
				
				#endif
				}
			
			_Active_attachable=0; //No active attachables
			glPopMatrix(); //Pop matrix for this node
			}
		
		_Render_node->post_draw();
		}
		
		//Use negative draw orders as 'offscreen' rendering
		if (_Render_node->_Draw_order<0) {
		
			//Clear screen between 'offscreen' and 'onscreen' rendering
			if (++_Iter==_End || (*_Iter).first>=0) {
			
				//Cancel current active capture
				if (capture::_Active) capture::_Active->cancel();
			
			clear_screen();
			}
		
		--_Iter;
		}
	}

_Active_rendernode=0; //No active render nodes
}

render_node *render_manager::root_node() const {
	return _Root_node;
}

render_node *render_manager::active_rendernode() const {
	return _Active_rendernode;
}

attachable *render_manager::active_attachable() const {
	return _Active_attachable;
}

render_manager::render_stats render_manager::statistics() const {

render_stats _Render_stats={0};

render_node *_Render_node;
attachable *_Attachable;

	for (render_node::_Graph::const_iterator _Iter=render_node::
		_Mygraph.begin(),_End=render_node::_Mygraph.end();
			_Iter!=_End;++_Iter) {
	
	_Render_node=(*_Iter).second; //Increase speed
	
		if (!_Render_node->_Visible || _Render_node->_Myattachables.empty())
			continue;
		
		for (render_node::_Attachables::const_iterator _Aiter=_Render_node->
			_Myattachables.begin(),_Aend=_Render_node->_Myattachables.end();
				_Aiter!=_Aend;++_Aiter) {
		
		_Attachable=(*_Aiter); //Increase speed
		
			if (!_Attachable->_Visible) continue;
		
		++_Render_stats.objects;
		attachable::geometry_stats _Geometry_stats=_Attachable->statistics();
		_Render_stats.polygons+=_Geometry_stats.polygons;
		_Render_stats.vertices+=_Geometry_stats.vertices;
		}
	}

return _Render_stats;
}

//Utility functions

void render_manager::copy_screen(void *_Data,unsigned int _Width,
	unsigned int _Height,const vector2 &_Position) {

glFlush(); //Flush the render pipe line explicitly
glReadPixels((GLint)_Position.x,(GLint)_Position.y,_Width,_Height,
	GL_RGBA,GL_UNSIGNED_BYTE,_Data);
}

void render_manager::clear_screen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_manager::clear_screen(const color &_Color) {

glClearColor(_Color.r,_Color.g,_Color.b,_Color.a);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} //namespace lunar