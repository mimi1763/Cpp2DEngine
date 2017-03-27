/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_hud.cpp
---------------------------------------------------
*/

#include "ui_hud.h"

//Dependencies
#include <cmath>
#include <algorithm>
#include <limits>

//Dependencies (engine)
#include "input_manager.h"
#include "framebuffer_manager.h"
#include "render_window.h"

#include "capture.h"
#include "render_node.h"
#include "texture.h"

#include "generic_utility.h"

//Dependencies (entity)
#include "ui_manager.h"
#include "node_manager.h"
#include "lighting_manager.h"

//Public

//Region
//Private

ui_hud::region::region(ui_groupbox *_Groupbox,const MODE &_Mode):
	_Groupbox(_Groupbox),_Mode(_Mode),_Update_elements(false) {
	//Empty
}

ui_hud::region::region(const region &_Region):
	_Groupbox(_Region._Groupbox) {

_Update_elements=false; //Not copyable
copy(_Region);
}

ui_hud::region::region(ui_groupbox *_Groupbox,const region &_Region):
	_Groupbox(_Groupbox) {

_Update_elements=false; //Not copyable
copy(_Region);
}

ui_hud::region::~region() {
	//Empty
}

void ui_hud::region::copy(const region &_Region) {

_Mode=_Region._Mode;

	if (!_Region._Myelements.empty()) {
	
		//Add elements
		for (_Elements::const_iterator _Iter=_Region._Myelements.begin(),
			_End=_Region._Myelements.end();_Iter!=_End;++_Iter)
				_Myelements.push_back(static_cast<ui_groupbox*>(
					element_pointer((*_Iter)->name())));
	
	_Update_elements=true;
	}
}

//Elements

void ui_hud::region::update_elements() {

vector2 _Position;
vector2 _Region_offset=utility::position(vector2::ZERO,
	_Groupbox->area()->width(),_Groupbox->area()->height(),
		_Mode==VERTICAL?rectangle::TOP_CENTER:rectangle::CENTER_LEFT,
			_Groupbox->area()->origin());

bool _Full=false;
ui_groupbox *_Element;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	_Element=(*_Iter); //Increase speed
	
		//Region is not full
		if (!_Full) {
		
		vector2 _Element_offset=utility::position(vector2::ZERO,
			_Element->area()->width(),_Element->area()->height(),
				_Element->area()->origin(),_Mode==VERTICAL?
					rectangle::TOP_CENTER:rectangle::CENTER_LEFT);
		
		vector2 _Pos=_Position+_Region_offset+_Element_offset;
		_Element->node()->position(_Pos);
		
			//Horizontal
			if (_Mode==HORIZONTAL) {
			
				if (_Position.x+_Element->width()<=_Groupbox->area()->width())
					_Element->size(_Element->width(),_Groupbox->area()->height());
				else _Full=true;
			
			_Position.x+=_Element->width();
			}
			//Vertical
			else {
			
				if (_Position.y-_Element->height()>=-_Groupbox->area()->height())
					_Element->size(_Groupbox->area()->width(),_Element->height());
				else _Full=true;
			
			_Position.y-=_Element->height();
			}
		}
		
		//Hud is full, no room for more elements
		if (_Full) {
		
			if (_Element->visible()) _Element->visible(false);
		}
		else {
		
			if (!_Element->visible() &&
				_Element->owner()->node()->visible())
					_Element->visible(true);
		}
	}
}

void ui_hud::region::swap_elements(ui_groupbox *_Element,ui_groupbox *_Element2) {

	if (_Element==_Element2) return;

_Elements::iterator _Iter=std::find(_Myelements.begin(),
	_Myelements.end(),_Element);

	//Could not find element in region
	if (_Iter==_Myelements.end()) return;

_Elements::iterator _Iter2=std::find(_Myelements.begin(),
	_Myelements.end(),_Element2);

	//Could not find element in region
	if (_Iter2==_Myelements.end()) return;

std::swap(*_Iter,*_Iter2);
_Update_elements=true;
}

//Public

ui_hud::region &ui_hud::region::operator=(const region &_Region) {

	//Worth doing
	if (this!=&_Region) {
	
	_Myelements.clear();
	copy(_Region);
	}

return *this;
}

void ui_hud::region::update(float _Time) {

	if (_Update_elements) {
	
	update_elements();
	_Update_elements=false;
	}
}

void ui_hud::region::mode(const MODE &_Mode) {

	if (this->_Mode!=_Mode) {
	
	this->_Mode=_Mode;
	_Update_elements=true; //Mode changed
	}
}

ui_hud::region::MODE ui_hud::region::mode() const {
	return _Mode;
}

void ui_hud::region::groupbox(ui_groupbox *_Groupbox) {

	if (this->_Groupbox!=_Groupbox &&
		this->_Groupbox->owner()==_Groupbox->owner()) {
	
	this->_Groupbox=_Groupbox;
	_Update_elements=true; //Groupbox changed
	}
}

ui_groupbox *ui_hud::region::groupbox() const {
	return _Groupbox;
}

//Elements

void ui_hud::region::add_element(ui_groupbox *_Element) {

	//Nothing to add
	if (!_Element) return;
	
	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)==_Element) return;
	}
	
	//Adopt element
	if (_Element->node()->parent_node()!=_Groupbox->node())
		_Groupbox->add_element(_Element);

_Myelements.push_back(_Element);
_Update_elements=true; //Element added
}

void ui_hud::region::remove_element(ui_groupbox *_Element) {

	for (_Elements::iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Element) {
		
		_Myelements.erase(_Iter);
		
			if (!_Myelements.empty())
				_Update_elements=true; //Element removed
		
		return;
		}
	}
}

void ui_hud::region::clear_elements() {
	_Myelements.clear();
}

ui_hud::region::elements_iter ui_hud::region::elements_begin() const {
	return _Myelements.begin();
}

ui_hud::region::elements_iter ui_hud::region::elements_end() const {
	return _Myelements.end();
}

ui_groupbox *ui_hud::region::element_pointer(const std::string &_Name) const {

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		//Element exist, return pointer
		if ((*_Iter)->name()==_Name) return (*_Iter);
	}

return 0;
}

//Hud
//Protected

ui_hud::ui_hud(ui_manager *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_menu(_Owner,_Name,_Parent_node,_Draw_order,_Width,_Height,_Visible),
				_Swapped(false),_Over_framebuffer(0),_Over_node(0),_Drag_element(0),
					_Drag_framebuffer(0),_Drag_node(0),_Capture(0),_Capture_node(0),
						_Pre_capturenode(0),_Post_capturenode(0) {
	//Empty
}

ui_hud::ui_hud(const ui_hud &_Hud):ui_menu(_Hud),_Swapped(false),
	_Over_framebuffer(0),_Over_node(0),_Drag_element(0),_Drag_framebuffer(0),
		_Drag_node(0),_Capture(0),_Capture_node(0),_Pre_capturenode(0),
			_Post_capturenode(0) {
	copy(_Hud);
}

ui_hud::ui_hud(ui_manager *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,const ui_hud &_Hud):
		ui_menu(_Owner,_Name,_Parent_node,_Draw_order,_Hud),_Swapped(false),
			_Over_framebuffer(0),_Over_node(0),_Drag_element(0),_Drag_framebuffer(0),
				_Drag_node(0),_Capture(0),_Capture_node(0),_Pre_capturenode(0),
					_Post_capturenode(0) {
	copy(_Hud);
}

ui_hud::~ui_hud() {

clear_regions();

	if (_Over_framebuffer) _Over_framebuffer->
		owner()->remove(_Over_framebuffer);
	
	if (_Drag_framebuffer) _Drag_framebuffer->
		owner()->remove(_Drag_framebuffer);
	
	if (_Capture) delete _Capture;
}

void ui_hud::copy(const ui_hud &_Hud) {

	if (!_Hud._Myregions.empty()) {
	
	region *_Region;
	
		//Add regions
		for (_Regions::const_iterator _Iter=_Hud._Myregions.begin(),
			_End=_Hud._Myregions.end();_Iter!=_End;++_Iter) {
		
		_Region=(*_Iter); //Increase speed
		create_region(region_pointer(_Region->_Groupbox->name())->_Groupbox,*_Region);
		}
	}
}

void ui_hud::client_changed(float _Width,float _Height) {

ui_menu::client_changed(_Width,_Height);

	//Nothing to do
	if (_Myelements.empty() || _Myregions.empty()) return;

bool _Update_regions=false;
ui_element *_Element;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	_Element=(*_Iter); //Increase speed
	
		//Element uses dynamic positioning or sizing
		if (_Element->positioning()!=ui_entity::STATIC ||
			_Element->sizing()!=ui_entity::STATIC) {
		
		ui_groupbox *_Groupbox=dynamic_cast<ui_groupbox*>(_Element);
		
			//Potentially a region or region element
			if (_Groupbox) {
			
			_Update_regions=true;
			break;
			}
		}
	}
	
	//Update region
	if (_Update_regions) {
	
		for (_Regions::const_iterator _Iter=_Myregions.begin(),
			_End=_Myregions.end();_Iter!=_End;++_Iter)
				(*_Iter)->_Update_elements=true;
	}
}

//Elements

void ui_hud::element_added(ui_element *_Element) {
	//Empty
}

void ui_hud::element_removed(ui_element *_Element) {

ui_groupbox *_Groupbox=dynamic_cast<ui_groupbox*>(_Element);

	if (_Groupbox) {
	
	region *_Region;
	
		for (_Regions::const_iterator _Iter=_Myregions.begin(),
			_End=_Myregions.end();_Iter!=_End;++_Iter) {
		
		_Region=(*_Iter); //Increase speed
		
			//Remove region
			if (_Groupbox==_Region->_Groupbox) {
			
			remove_region(_Region);
			break;
			}
		
		size_t _Size=_Region->_Myelements.size();
		_Region->remove_element(_Groupbox);
		
			//Element has been removed
			if (_Size!=_Region->_Myelements.size()) break;
		}
	}
}

ui_element *ui_hud::intersect_element(const vector2 &_Position) const {

region *_Region;
region *_Intersect_region=0;

	for (_Regions::const_iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter) {
	
	_Region=(*_Iter); //Increase speed
	
		if (!_Region->_Groupbox->enabled() ||
			!_Region->_Groupbox->visible()) continue;
	
	_Region->_Groupbox->area()->prepare(); //Prepare bounds
	
		//Check if cursor is intersecting a region
		if (_Region->_Groupbox->area()->world_orientedboundingbox().
			intersects(_Position)) {
		
			if (!_Intersect_region || _Region->_Groupbox->node()->draw_order()>=
				_Intersect_region->_Groupbox->node()->draw_order())
					_Intersect_region=_Region;
		}
	}

ui_groupbox *_Element;
ui_groupbox *_Intersect_element=0;

	if (_Intersect_region) {
	
		for (region::_Elements::const_iterator _Iter=_Intersect_region->
			_Myelements.begin(),_End=_Intersect_region->_Myelements.end();
				_Iter!=_End;++_Iter) {
		
		_Element=(*_Iter); //Increase speed
		
			if (!_Element->enabled() || !_Element->visible()) continue;
		
		_Element->area()->prepare(); //Prepare bounds
		
			//Check if cursor is intersecting a element
			if (_Element->area()->world_orientedboundingbox().
				intersects(_Position)) {
			
			_Intersect_element=_Element;
			break;
			}
		}
	}

return (_Intersect_element?_Intersect_element:
	ui_menu::intersect_element(_Position));
}

ui_hud::region *ui_hud::element_owner(ui_groupbox *_Element) const {

region *_Region;

	for (_Regions::const_iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter) {
	
	_Region=(*_Iter); //Increase speed
	
		for (region::_Elements::const_iterator _Eiter=_Region->_Myelements.begin(),
			_Eend=_Region->_Myelements.end();_Eiter!=_Eend;++_Eiter) {
		
			//Region is owner of element
			if ((*_Eiter)==_Element) return _Region;
		}
	}

return 0;
}

//Capture

void ui_hud::prepare_capture(framebuffer *_Framebuffer,render_node *_Capture_node) {

	//Capture is already prepared for this frame
	if (this->_Capture_node) return;

//-------------------------------------------------
//1. Build capture aabb and traverse capture branch
//-------------------------------------------------

aabb _Capture_aabb;
render_node *_First_node=_Capture_node;
render_node *_Last_node=_Capture_node;

render_node::render_nodes _Render_nodes;
_Capture_node->traverse(_Render_nodes);

render_node *_Render_node;

	//Find the nodes that are rendered first and last
	for (render_node::render_nodes::const_iterator _Iter=_Render_nodes.begin(),
		_End=_Render_nodes.end();_Iter!=_End;++_Iter) {
	
	_Render_node=(*_Iter); //Increase speed
	
		//Check for new first
		if (_Render_node->draw_order()<_First_node->draw_order())
			_First_node=_Render_node;
		//Check for new last
		else if (_Render_node->draw_order()>_Last_node->draw_order())
			_Last_node=_Render_node;
		
		//Merge capture aabb with attachables aabb
		for (render_node::attachables_iter _Aiter=_Render_node->attachables_begin(),
			_Aend=_Render_node->attachables_end();_Aiter!=_Aend;++_Aiter)
				_Capture_aabb.merge((*_Aiter)->world_boundingbox());
	}
	
	//Make sure capture texture is rendered after last node
	if (_Framebuffer->framebuffer_texture()->parent_node()==_Over_node)
		_Over_node->draw_order(_Last_node->draw_order()+1);

//-------------------------------------
//2. Prepare pre and post capture nodes
//-------------------------------------

//Example
//If the capture branch draw order range is [0,->]
//then the actual capture range should be [<-,-0]

//Example
//Make sure the last node is rendered at:
//-5 (0 (base) - 1 (negative) + -3 (pre light) - 1 (post node))
//-4 for post node, -3 for pre light

int _Draw_order=-_Node->draw_order()-
	(_Last_node->draw_order()-_Node->draw_order());
_Capture_node->draw_order(_Draw_order-1+_Owner->
	lighting_manager_ptr()->capture_layer().from()-1);

	//Initialize pre capture node for first time use
	if (!_Pre_capturenode) {
	
	_Pre_capturenode=_Owner->node_manager_ptr()->create_node(
		_Node,interval(_First_node->draw_order()-1));
	_Pre_capturenode->onpredraw(&ui_hud::pre_capture,this);
	}
	else {
	
	_Pre_capturenode->draw_order(_First_node->draw_order()-1);
	_Pre_capturenode->visible(true); //Enable callback
	}
	
	//Initialize post capture node for first time use
	if (!_Post_capturenode) {
	
	_Post_capturenode=_Owner->node_manager_ptr()->create_node(
		_Node,interval(_Last_node->draw_order()+1));
	_Post_capturenode->onpostdraw(&ui_hud::post_capture,this);
	}
	else {
	
	_Post_capturenode->draw_order(_Last_node->draw_order()+1);
	_Post_capturenode->visible(true); //Enable callback
	}

//--------------------------------
//3. Finalize and set capture data
//--------------------------------

	//Initialize capture for first time use
	if (!_Capture) _Capture=new capture();

//Fix floating point errors
const vector2 &_Min=_Capture_aabb.minimum();
const vector2 &_Max=_Capture_aabb.maximum();
_Capture_aabb.extents(vector2(floor(_Min.x),floor(_Min.y)),
	vector2(ceil(_Max.x),ceil(_Max.y)));

//Set capture size destination
vector2 _Capture_size=_Capture_aabb.size();
_Capture->size(_Capture_size.x,_Capture_size.y);
_Capture->destination(_Framebuffer);

_Capture_origin=_Capture_aabb.minimum();

_Framebuffer->framebuffer_texture()->size(
	_Capture->width(),_Capture->height());
_Framebuffer->framebuffer_texture()->parent_node()->position(
	-_Framebuffer->framebuffer_texture()->parent_node()->
		parent_node()->position()+_Capture_origin);

//Set capture node position to origo (required by FBO capturing)
_Capture_node->position(_Capture_node->position()-_Capture_origin);

this->_Capture_node=_Capture_node; //Start capturing
}

void ui_hud::pre_capture(render_node *_Render_node) {

_Capture->start();
render_manager::instance()->clear_screen();

_Render_node->visible(false); //Disable pre capture callback
}

void ui_hud::post_capture(render_node *_Render_node) {

_Capture->stop();
render_manager::instance()->clear_screen();

revert_back(_Render_node);
_Render_node->visible(false); //Disable post capture callback
}

void ui_hud::revert_back(render_node *_Render_node) {

//Revert back draw order and position
//WARNING: This only works because capture node is not render node
//We can't change draw order on the node currently being rendered

_Capture_node->draw_order(-_Render_node->draw_order()+
	(_Owner->lighting_manager_ptr()->capture_layer().from()-1));
_Capture_node->position(_Capture_node->position()+_Capture_origin);
_Capture_node=0;
}

//Over/drag framebuffer

void ui_hud::prepare_overframebuffer(render_node *_Capture_node) {

	//Initialize over framebuffer for first time use
	if (!_Over_framebuffer) {
	
	_Over_framebuffer=_Owner->external().framebuffer_manager_ptr()->
		create_framebuffer((unsigned int)render_window::instance()->ortho_width(),
			(unsigned int)render_window::instance()->ortho_height());
	texture *_Over_texture=_Over_framebuffer->framebuffer_texture();
	_Over_texture->color_value(color(200,200,200,0.75f)); //Over color
	_Over_texture->origin(rectangle::BOTTOM_LEFT);
	_Over_texture->auto_size(false);
	
	_Over_node=_Owner->node_manager_ptr()->create_node(
		_Node,_Capture_node->draw_order()+1);
	_Over_node->attach(_Over_texture);
	}
	else _Over_node->visible(true);

prepare_capture(_Over_framebuffer,_Capture_node);
}

void ui_hud::prepare_dragframebuffer(render_node *_Capture_node) {

	//Initialize drag framebuffer for first time use
	if (!_Drag_framebuffer) {
	
	_Drag_framebuffer=_Owner->external().framebuffer_manager_ptr()->
		create_framebuffer((unsigned int)render_window::instance()->ortho_width(),
			(unsigned int)render_window::instance()->ortho_height());
	texture *_Drag_texture=_Drag_framebuffer->framebuffer_texture();
	_Drag_texture->origin(rectangle::BOTTOM_LEFT);
	_Drag_texture->auto_size(false);
	
	_Drag_node=_Owner->node_manager_ptr()->create_node(
		_Node,std::numeric_limits<int>::max()-1001); //Default (cursor-1)
	_Drag_node->attach(_Drag_texture);
	}
	else _Drag_node->visible(true);

prepare_capture(_Drag_framebuffer,_Capture_node);
}

//Public

ui_hud &ui_hud::operator=(const ui_hud &_Hud) {

	//Worth doing
	if (this!=&_Hud) {
	
	ui_menu::operator=(_Hud); //Invoke base operator=
	
	clear_regions();
	copy(_Hud);
	}

return *this;
}

void ui_hud::update(float _Time) {

	//Update regions
	for (_Regions::const_iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter)
			(*_Iter)->update(_Time);

ui_menu::update(_Time);
}

void ui_hud::post_update(float _Time) {

	//Update over element as the newly swapped element
	if (_Swapped) {
	
	_Owner->mouse_moved(input_manager::instance()->mouse_x(),
		input_manager::instance()->mouse_y());
	_Swapped=false;
	}

ui_menu::post_update(_Time);
}

void ui_hud::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return;

ui_menu::mouse_pressed(_Button,_X,_Y);

	//Start drag procedure
	if (_Active_element && !_Drag_element &&
		_Over_node && _Over_node->visible()) {
	
	ui_groupbox *_Element=static_cast<ui_groupbox*>(_Over_element);
	prepare_dragframebuffer(_Over_node);
	
	_Drag_origin=vector2((float)_X,(float)_Y)-_Drag_node->position();
	_Drag_element=_Element;
	}
}

bool ui_hud::mouse_released(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return false;

bool _Result=ui_menu::mouse_released(_Button,_X,_Y);

	//Start drop procedure
	if (_Drag_element) {
	
		if (_Over_element && _Over_node && _Over_node->visible()) {
		
		region *_Region=element_owner(_Drag_element);
		
			if (_Region && _Region->_Groupbox->enabled()) {
			
			_Region->swap_elements(static_cast<ui_groupbox*>(
				_Over_element),_Drag_element);
			_Swapped=true;
			
			_Over_node->visible(false);
			_Over_element=0;
			}
		}
	
	_Drag_node->visible(false);
	_Drag_element=0;
	}

return (_Result || (_Over_node && _Over_node->visible()));
}

bool ui_hud::mouse_moved(unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return false;

ui_element *_Previous_element=_Over_element;
bool _Result=ui_menu::mouse_moved(_X,_Y);

	//Move drag node relative to cursor
	if (_Drag_node && _Drag_node->visible())
		_Drag_node->position(vector2((float)_X,(float)_Y)-_Drag_origin);
	
	//Over element has changed
	if (_Over_element!=_Previous_element) {
	
	ui_groupbox *_Element=dynamic_cast<ui_groupbox*>(_Over_element);
	bool _Exit=true;
	
		//Element is correct type
		if (_Element) {
		
		region *_Region=element_owner(_Element);
		
			if (_Region && _Region->_Groupbox->enabled()) {
			
				//If dragging, drop region must match drag elements owner
				if (!_Drag_element || element_owner(_Drag_element)==_Region) {
				
				prepare_overframebuffer(_Element->node());
				_Exit=false;
				}
			}
		}
		
		if (_Exit && _Over_node && _Over_node->visible())
			_Over_node->visible(false);
	}

return (_Result || (_Over_node && _Over_node->visible()));
}

void ui_hud::clean() {

_Swapped=false;

	if (_Over_node && _Over_node->visible()) {
	
	_Over_node->visible(false);
	//Over element is disabled in ui_menu::clean
	}
	
	if (_Drag_element) {
	
	_Drag_element=0;
	_Drag_node->visible(false);
	}
	
	//Disable pre capture callback
	if (_Pre_capturenode) _Pre_capturenode->visible(false); 
	
	//Disable post capture callback
	if (_Post_capturenode) {
	
		//Capturing, revert back
		if (_Capture_node) revert_back(_Post_capturenode);
	
	_Post_capturenode->visible(false); 
	}

ui_menu::clean();
}

//Regions

ui_hud::region *ui_hud::create_region(ui_groupbox *_Groupbox,
	const region::MODE &_Mode) {

	//Nothing to add
	if (!_Groupbox) return 0;
	
	//Invalid owner
	if (_Groupbox->owner()!=this) return 0;
	
	for (_Regions::const_iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)->_Groupbox==_Groupbox) return 0;
	}

region *_Myregion=new region(_Groupbox,_Mode);
_Myregions.push_back(_Myregion);
return _Myregion;
}

ui_hud::region *ui_hud::create_region(ui_groupbox *_Groupbox,
	const region &_Region) {

	//Nothing to add
	if (!_Groupbox) return 0;
	
	//Invalid owner
	if (_Groupbox->owner()!=this) return 0;
	
	for (_Regions::const_iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)->_Groupbox==_Groupbox) return 0;
	}

region *_Myregion=new region(_Groupbox,_Region);
_Myregions.push_back(_Myregion);
return _Myregion;
}

void ui_hud::remove_region(region *&_Region) {

	for (_Regions::iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Region) {
		
		delete _Region;
		_Region=0; //Set referenced pointer to null
		
		_Myregions.erase(_Iter);
		return;
		}
	}
}

void ui_hud::clear_regions() {

	for (_Regions::iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myregions.clear();
}

ui_hud::regions_iter ui_hud::regions_begin() const {
	return _Myregions.begin();
}

ui_hud::regions_iter ui_hud::regions_end() const {
	return _Myregions.end();
}

ui_hud::region *ui_hud::region_pointer(const std::string &_Name) const {

	for (_Regions::const_iterator _Iter=_Myregions.begin(),
		_End=_Myregions.end();_Iter!=_End;++_Iter) {
	
		//Element exist, return pointer
		if ((*_Iter)->_Groupbox->name()==_Name) return (*_Iter);
	}

return 0;
}