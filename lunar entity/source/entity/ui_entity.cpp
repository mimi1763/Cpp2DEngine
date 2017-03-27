/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_entity.cpp
---------------------------------------------------
*/

#include "ui_entity.h"

//Dependencies
#include <cmath>

//Dependencies (engine)
#include "scene_manager.h"
#include "render_node.h"
#include "texturable.h"
#include "text.h"
#include "font.h"

#include "math_utility.h"
#include "generic_utility.h"

//Dependencies (entity)
#include "ui_manager.h"

//Public

//Part
//Public

const std::string ui_entity::part::CENTER="CENTER";
const std::string ui_entity::part::BORDER="BORDER";

const std::string ui_entity::part::TOP_LEFT="TOP_LEFT";
const std::string ui_entity::part::TOP="TOP";
const std::string ui_entity::part::TOP_RIGHT="TOP_RIGHT";
const std::string ui_entity::part::LEFT="LEFT";
const std::string ui_entity::part::RIGHT="RIGHT";
const std::string ui_entity::part::BOTTOM_LEFT="BOTTOM_LEFT";
const std::string ui_entity::part::BOTTOM="BOTTOM";
const std::string ui_entity::part::BOTTOM_RIGHT="BOTTOM_RIGHT";

//Additional
//Public

const std::string ui_entity::additional::CAPTION="CAPTION";

//Element
//Protected

ui_entity::ui_entity(ui_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible):entity(_Owner,_Parent_node,_Draw_order,_Visible),
			_Positioning(STATIC),_Sizing(STATIC),_Generate_mode(MODIFY_TEXCOORDS),
				_Caption_width(_Width<0.0f?0.0f:_Width),_Caption_height(
					_Height<0.0f?0.0f:_Height),_Caption_origin(CENTER),
						_Static_width(0.0f),_Static_height(0.0f),
							_Update_positioning(false),_Update_sizing(false),
								_Part_status(OKAY),_Update_caption(false) {

_Area=_Owner->external().scene_manager_ptr()->
	create_rectangle(_Width,_Height,color::WHITE,false);
_Node->attach(_Area);

_Positioning_unit[0]=_Positioning_unit[1]=PIXEL;
_Sizing_unit[0]=_Sizing_unit[1]=PIXEL;
}

ui_entity::ui_entity(const ui_entity &_Ui_entity):entity(_Ui_entity) {

_Area=_Owner->external().scene_manager_ptr()->
	create_rectangle(*_Ui_entity._Area);
_Node->attach(_Area);

_Update_caption=true; //Not copyable
copy(_Ui_entity);
}

ui_entity::ui_entity(ui_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,const ui_entity &_Ui_entity):
		entity(_Owner,_Parent_node,_Draw_order,_Ui_entity) {

_Area=_Owner->external().scene_manager_ptr()->
	create_rectangle(*_Ui_entity._Area);
_Node->attach(_Area);

_Update_caption=true; //Not copyable
copy(_Ui_entity);
}

ui_entity::~ui_entity() {
	_Owner->external().scene_manager_ptr()->
		remove_renderable(_Area);
}

void ui_entity::copy(const ui_entity &_Ui_entity) {

_Positioning=_Ui_entity._Positioning;
_Sizing=_Ui_entity._Sizing;
_Positioning_unit[0]=_Ui_entity._Positioning_unit[0];
_Positioning_unit[1]=_Ui_entity._Positioning_unit[1];
_Sizing_unit[0]=_Ui_entity._Sizing_unit[0];
_Sizing_unit[1]=_Ui_entity._Sizing_unit[1];
_Positioning_area=_Ui_entity._Positioning_area;

_Auto_size=_Ui_entity._Auto_size;
_Generate_mode=_Ui_entity._Generate_mode;

_Caption=_Ui_entity._Caption;
_Caption_width=_Ui_entity._Caption_width;
_Caption_height=_Ui_entity._Caption_height;
_Caption_offset=_Ui_entity._Caption_offset;
_Caption_origin=_Ui_entity._Caption_origin;

_Static_position=_Ui_entity._Static_position;
_Static_width=_Ui_entity._Static_width;
_Static_height=_Ui_entity._Static_height;
_Update_positioning=_Ui_entity._Update_positioning;
_Update_sizing=_Ui_entity._Update_sizing;

_Part_status=_Ui_entity._Part_status; //Inherit status
}

void ui_entity::object_created(object *_Object) {

	//Generate potentially new parts
	if (_Generate_mode!=FORCE_OFF) _Part_status=GENERATE;
}

void ui_entity::client_changed(float _Width,float _Height) {

update_positioning(_Width,_Height);
update_sizing(_Width,_Height);
_Update_positioning=false;
_Update_sizing=false;
}

void ui_entity::update_positioning(float _Width,float _Height) {

	//Do not reposition
	if (_Positioning==STATIC) return;

float _Client_width=(float)static_cast<ui_manager*>(
	_Owner)->client_width();
float _Client_height=(float)static_cast<ui_manager*>(
	_Owner)->client_height();
vector2 _New_position=_Node->position();

	//Dynamic x
	if (_Positioning==DYNAMIC_X_AXIS || _Positioning==DYNAMIC_XY_AXIS) {
	
		//In percent
		if (_Positioning_unit[0]==PERCENT) {
		
		float _Parent_position=_Node->parent_node()->world_position().x;
		
			//Based on positioning area 
			if (!_Positioning_area.empty()) {
			
			float _Size=_Positioning_area.size().x;
			float _Minimum=_Positioning_area.minimum().x;
			_New_position.x=(_Size+(_Width-_Client_width))*
				((_Parent_position+_Static_position.x-
					_Minimum)/_Size)-_Minimum-_Parent_position;
			}
			else _New_position.x=(_Width*((_Parent_position+
				_Static_position.x)/_Client_width))-_Parent_position;
		}
		//In pixel
		else _New_position.x=_Static_position.x+
			(_Width-_Client_width);
	
	_New_position.x=math::round(_New_position.x);
	}
	
	//Dynamic y
	if (_Positioning==DYNAMIC_Y_AXIS || _Positioning==DYNAMIC_XY_AXIS) {
	
		//In percent
		if (_Positioning_unit[1]==PERCENT) {
		
		float _Parent_position=_Node->parent_node()->world_position().y;
		
			//Based on positioning area 
			if (!_Positioning_area.empty()) {
			
			float _Size=_Positioning_area.size().y;
			float _Minimum=_Positioning_area.minimum().y;
			_New_position.y=(_Size+(_Height-_Client_height))*
				((_Parent_position+_Static_position.y-
					_Minimum)/_Size)+_Minimum-_Parent_position;
			}
			else _New_position.y=(_Height*((_Parent_position+
				_Static_position.y)/_Client_height))-_Parent_position;
		}
		//In pixel
		else _New_position.y=_Static_position.y+
			(_Height-_Client_height);
	
	_New_position.y=math::round(_New_position.y);
	}

_Node->position(_New_position);
}

void ui_entity::update_sizing(float _Width,float _Height) {

	//Do not resize
	if (_Sizing==STATIC) return;

float _Client_width=(float)static_cast<ui_manager*>(
	_Owner)->client_width();
float _Client_height=(float)static_cast<ui_manager*>(
	_Owner)->client_height();
float _New_width=_Area->width();
float _New_height=_Area->height();

	//Dynamic width
	if (_Sizing==DYNAMIC_X_AXIS || _Sizing==DYNAMIC_XY_AXIS) {
	
		//In percent
		if (_Sizing_unit[0]==PERCENT)
			_New_width=_Width*(_Static_width/_Client_width);
		//In pixel
		else _New_width=_Static_width+(_Width-_Client_width);
	
	_New_width=math::round(_New_width);
	}
	
	//Dynamic height
	if (_Sizing==DYNAMIC_Y_AXIS || _Sizing==DYNAMIC_XY_AXIS) {
	
		//In percent
		if (_Sizing_unit[1]==PERCENT)
			_New_height=_Height*(_Static_height/_Client_height);
		//In pixel
		else _New_height=_Static_height+(_Height-_Client_height);
	
	_New_height=math::round(_New_height);
	}

size(_New_width,_New_height);
}

//Parts

rectangle *ui_entity::rectangle_pointer(object *_Object) {

	//Nothing to do
	if (!_Object) return 0;

rectangle *_Rectangle=0;
attachable *_Attachable;

	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Attachable=(*_Iter)->data(); //Increase speed
	
		if (_Attachable &&
			(_Rectangle=dynamic_cast<rectangle*>(_Attachable)))
				break;
	}

return _Rectangle;
}

void ui_entity::modify_part(object *_Object,bool _Flip_horizontal,
	bool _Flip_vertical,int _Rotation) {

	//Nothing to do
	if (!_Object) return;

component *_Component;

	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Component=(*_Iter); //Increase speed
	
		if (_Component->data() &&
			(_Component->type()==component::TEXTURE ||
			_Component->type()==component::ANIMATION)) {
		
		texturable *_Texturable=static_cast<texturable*>(_Component->data());
		_Texturable->flip(_Flip_horizontal^_Texturable->flip_horizontal(),
			_Flip_vertical^_Texturable->flip_vertical());
		
		int _New_rotation=(int)_Texturable->rotation()+_Rotation;
		
			if (_New_rotation>180) _New_rotation-=360;
			
			if (_New_rotation==90)
				_Texturable->rotation(texturable::ROTATE_90CCW);
			else if (_New_rotation==-90)
				_Texturable->rotation(texturable::ROTATE_90CW);
			else if (abs(_New_rotation)==180)
				_Texturable->rotation(texturable::ROTATE_180);
			else _Texturable->rotation(texturable::ROTATE_NONE);
		}
	}
}

void ui_entity::reposition_part(object *_Object,const vector2 &_Position) {

	//Nothing to do
	if (!_Object) return;

_Object->node()->position(_Object->node()->position()+_Position);
}

void ui_entity::resize_part(object *_Object,float _Width,
	float _Height) {

	//Nothing to do
	if (!_Object) return;

bool _Swap=math::equal(fabs(math::radian(
	_Object->node()->rotation())),90.0f,0.1f);

component *_Component;

	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Component=(*_Iter); //Increase speed
	
		if (_Component->data() &&
			(_Component->type()==component::TEXTURE ||
			_Component->type()==component::ANIMATION)) {
		
		texturable *_Texturable=static_cast<texturable*>(_Component->data());
		_Texturable->prepare();
		_Texturable->auto_size(false);
		
			//Swap width and height
			if (_Width==0.0f && _Height==0.0f) {
			
				if (_Swap || abs((int)_Texturable->rotation())==90)
					_Texturable->size(_Texturable->height(),
						_Texturable->width());
			}
			//Resize swapped
			else if (_Swap)
				_Texturable->size(_Texturable->width()+_Height,
					_Texturable->height()+_Width);
			//Resize normally
			else _Texturable->size(_Texturable->width()+_Width,
				_Texturable->height()+_Height);
		}
	}
}

void ui_entity::resize_parts(object *_Center,object *_Border,
	object *_Top_left,object *_Top_right,object *_Bottom_left,
		object *_Bottom_right,object *_Top,object *_Bottom,
			object *_Left,object *_Right,float _Width,
				float _Height) {

	//Nothing to do
	if (_Width==0.0f && _Height==0.0f) return;
	
	//Nothing to resize
	if (!_Center && !_Border) return;

rectangle *_Rectangle=0;

	if (_Border) _Rectangle=rectangle_pointer(_Border);
	if (!_Rectangle && _Center) _Rectangle=rectangle_pointer(_Center);
	if (!_Rectangle) _Rectangle=_Area;

rectangle::ORIGIN _Origin=_Rectangle->origin();
vector2 _Direction;

	//Find expansion direction
	switch (_Origin) {
	
		case rectangle::TOP_LEFT:
		_Direction=vector2(1.0f,-1.0f);
		break;
		
		case rectangle::TOP_CENTER:
		_Direction=vector2(0.0f,-1.0f);
		break;
		
		case rectangle::TOP_RIGHT:
		_Direction=vector2(-1.0f,-1.0f);
		break;
		
		case rectangle::CENTER_LEFT:
		_Direction=vector2(1.0f,0.0f);
		break;
		
		case rectangle::CENTER_RIGHT:
		_Direction=vector2(-1.0f,0.0f);
		break;
		
		case rectangle::BOTTOM_LEFT:
		_Direction=vector2(1.0f,1.0f);
		break;
		
		case rectangle::BOTTOM_CENTER:
		_Direction=vector2(0.0f,1.0f);
		break;
		
		case rectangle::BOTTOM_RIGHT:
		_Direction=vector2(-1.0f,1.0f);
		break;
	}
	
	//Resize only
	if (_Center) resize_part(_Center,_Width,_Height);
	if (_Border) resize_part(_Border,_Width,_Height);
	
	//Corners
	//Reposition only
	if (_Top_left) reposition_part(_Top_left,vector2(
		_Direction.x<=0.0f?-_Width*(_Direction.x==0.0f?0.5f:1.0f):0.0f,
		_Direction.y>=0.0f?_Height*(_Direction.y==0.0f?0.5f:1.0f):0.0f));
	
	if (_Top_right) reposition_part(_Top_right,vector2(
		_Direction.x>=0.0f?_Width*(_Direction.x==0.0f?0.5f:1.0f):0.0f,
		_Direction.y>=0.0f?_Height*(_Direction.y==0.0f?0.5f:1.0f):0.0f));
	
	if (_Bottom_left) reposition_part(_Bottom_left,vector2(
		_Direction.x<=0.0f?-_Width*(_Direction.x==0.0f?0.5f:1.0f):0.0f,
		_Direction.y<=0.0f?-_Height*(_Direction.y==0.0f?0.5f:1.0f):0.0f));
	
	if (_Bottom_right) reposition_part(_Bottom_right,vector2(
		_Direction.x>=0.0f?_Width*(_Direction.x==0.0f?0.5f:1.0f):0.0f,
		_Direction.y<=0.0f?-_Height*(_Direction.y==0.0f?0.5f:1.0f):0.0f));
	
	//Sides
	//Resize and reposition
	if (_Top) {
	
		if (_Width!=0.0f) resize_part(_Top,_Width,0.0f);
	
	reposition_part(_Top,vector2(0.0f,_Direction.y>=0.0f?
		_Height*(_Direction.y==0.0f?0.5f:1.0f):0.0f));
	}
	
	if (_Bottom) {
	
		if (_Width!=0.0f) resize_part(_Bottom,_Width,0.0f);
	
	reposition_part(_Bottom,vector2(0.0f,_Direction.y<=0.0f?
		-_Height*(_Direction.y==0.0f?0.5f:1.0f):0.0f));
	}
	
	if (_Left) {
	
		if (_Height!=0.0f) resize_part(_Left,0.0f,_Height);
	
	reposition_part(_Left,vector2(_Direction.x<=0.0f?
		-_Width*(_Direction.x==0.0f?0.5f:1.0f):0.0f,0.0f));
	}
	
	if (_Right) {
	
		if (_Height!=0.0f) resize_part(_Right,0.0f,_Height);
	
	reposition_part(_Right,vector2(_Direction.x>=0.0f?
		_Width*(_Direction.x==0.0f?0.5f:1.0f):0.0f,0.0f));
	}
}

object *ui_entity::generate_part(const std::string &_Name,object *_Object,
	object *_Parent_object,const vector2 &_From,const vector2 &_To) {

	//Nothing to do
	if (!_Object || !_Parent_object) return 0;
	
	//Invalid parent
	if (object_parent(_Object->name())!=_Parent_object)
		return 0;

light *_Light=dynamic_cast<light*>(_Object);
object *_Generated_object=(_Light?create_light(_Name,_Parent_object,
	_Light->draw_order(),*_Light):create_object(_Name,_Parent_object,
		_Object->draw_order(),*_Object));

	//Could not create object
	if (!_Generated_object) return 0;

rectangle *_Rectangle=0;

	if (_Parent_object) _Rectangle=rectangle_pointer(_Parent_object);
	if (!_Rectangle) _Rectangle=_Area;

//Convert origin position to center position
vector2 _Origin_position=utility::position(
	_Rectangle->parent_node()->position(),_Rectangle->width(),
		_Rectangle->height(),rectangle::CENTER,_Rectangle->origin());

//Convert position to center position
_Rectangle=rectangle_pointer(_Generated_object);

	if (_Rectangle) _Rectangle->prepare();

vector2 _Position=(_Rectangle?utility::position(
	_Generated_object->node()->position(),_Rectangle->width(),
		_Rectangle->height(),rectangle::CENTER,_Rectangle->origin()):
			_Generated_object->node()->position());

bool _Corner=(_From.squared_length()>=2.0f);
float _Degrees=math::radian(atan2(_To.y,_To.x)-
	atan2(_From.y,_From.x));

	if (_Degrees>180.0f) _Degrees-=360.0f;
	else if (_Degrees<-180.0f) _Degrees+=360.0f;
	
	if (math::equal(_Degrees,90.0f,0.1f)) {
	
		if (_Corner) {
		
			if (_Generate_mode==MODIFY_TEXCOORDS)
				modify_part(_Generated_object,(_From.y==_To.y),(_From.x==_To.x));
			else _Generated_object->node()->rotate(math::degree(90.0f));
			
			if (_From.y==_To.y)
				_Position.x=_Origin_position.x-(_Position.x-_Origin_position.x);
			else _Position.y=_Origin_position.y-(_Position.y-_Origin_position.y);
		}
		else {
		
			if (_Generate_mode==MODIFY_TEXCOORDS)
				modify_part(_Generated_object,false,false,90);
			else _Generated_object->node()->rotate(math::degree(90.0f));
		
		_Position=vector2(_Origin_position.x-(_Position.y-_Origin_position.y),
			_Origin_position.y+(_Position.x-_Origin_position.x));
		}
	}
	else if (math::equal(_Degrees,-90.0f,0.1f)) {
	
		if (_Corner) {
		
			if (_Generate_mode==MODIFY_TEXCOORDS)
				modify_part(_Generated_object,(_From.y==_To.y),(_From.x==_To.x));
			else _Generated_object->node()->rotate(math::degree(-90.0f));
			
			if (_From.y==_To.y)
				_Position.x=_Origin_position.x-(_Position.x-_Origin_position.x);
			else _Position.y=_Origin_position.y-(_Position.y-_Origin_position.y);
		}
		else {
		
			if (_Generate_mode==MODIFY_TEXCOORDS)
				modify_part(_Generated_object,false,false,-90);
			else _Generated_object->node()->rotate(math::degree(-90.0f));
		
		_Position=vector2(_Origin_position.x+(_Position.y-_Origin_position.y),
			_Origin_position.y-(_Position.x-_Origin_position.x));
		}
	}
	else if (math::equal(fabs(_Degrees),180.0f,0.1f)) {
	
		if (_Corner) {
		
			if (_Generate_mode==MODIFY_TEXCOORDS)
				modify_part(_Generated_object,true,true);
			else _Generated_object->node()->rotate(math::degree(180.0f));
		
		_Position=_Origin_position-(_Position-_Origin_position);
		}
		else {
		
			if (_Generate_mode==MODIFY_TEXCOORDS)
				modify_part(_Generated_object,(_From.x!=0.0f),(_From.y!=0.0f));
			else _Generated_object->node()->rotate(math::degree(180.0f));
			
			if (_From.x!=0.0f)
				_Position.x=_Origin_position.x-(_Position.x-_Origin_position.x);
			else _Position.y=_Origin_position.y-(_Position.y-_Origin_position.y);
		}
	}

//Swap width and height if needed
resize_part(_Generated_object,0.0f,0.0f);

//Convert position back from center position
_Generated_object->node()->position(_Rectangle?utility::
	position(_Position,_Rectangle->width(),_Rectangle->height(),
		_Rectangle->origin()):_Position);

return _Generated_object;
}

void ui_entity::update_parts(const std::string &_Name) {

object *_Center=object_pointer(part::CENTER+_Name);
object *_Border=object_pointer(part::BORDER+_Name);

	//Nothing to do
	if (!_Center && !_Border) return;

//Corners
object *_Top_left=object_pointer(part::TOP_LEFT+_Name);
object *_Top_right=object_pointer(part::TOP_RIGHT+_Name);
object *_Bottom_left=object_pointer(part::BOTTOM_LEFT+_Name);
object *_Bottom_right=object_pointer(part::BOTTOM_RIGHT+_Name);

//Sides
object *_Top=object_pointer(part::TOP+_Name);
object *_Bottom=object_pointer(part::BOTTOM+_Name);
object *_Left=object_pointer(part::LEFT+_Name);
object *_Right=object_pointer(part::RIGHT+_Name);

object *_Corner[4]={_Top_left,_Top_right,_Bottom_left,_Bottom_right};
object *_Side[4]={_Top,_Bottom,_Left,_Right};

	//Generate border parts
	if (_Generate_mode!=FORCE_OFF &&
		_Part_status==GENERATE && _Border) {
	
		if (!_Top_left) {
		
		object *_Object=(_Top_right?_Top_right:_Bottom_left?
			_Bottom_left:_Bottom_right?_Bottom_right:0); //Based on
		
			if (_Object) _Corner[0]=generate_part(part::TOP_LEFT+_Name,
				_Object,_Border,(_Top_right?vector2(1.0f,1.0f):_Bottom_left?
					vector2(-1.0f,-1.0f):vector2(1.0f,-1.0f)),vector2(-1.0f,1.0f));
		}
		
		if (!_Top_right) {
		
		object *_Object=(_Top_left?_Top_left:_Bottom_right?
			_Bottom_right:_Bottom_left?_Bottom_left:0); //Based on
		
			if (_Object) _Corner[1]=generate_part(part::TOP_RIGHT+_Name,
				_Object,_Border,(_Top_left?vector2(-1.0f,1.0f):_Bottom_right?
					vector2(1.0f,-1.0f):vector2(-1.0f,-1.0f)),vector2(1.0f,1.0f));
		}
		
		if (!_Bottom_left) {
		
		object *_Object=(_Bottom_right?_Bottom_right:_Top_left?
			_Top_left:_Top_right?_Top_right:0); //Based on
		
			if (_Object) _Corner[2]=generate_part(part::BOTTOM_LEFT+_Name,
				_Object,_Border,(_Bottom_right?vector2(1.0f,-1.0f):_Top_left?
					vector2(-1.0f,1.0f):vector2(1.0f,1.0f)),vector2(-1.0f,-1.0f));
		}
		
		if (!_Bottom_right) {
		
		object *_Object=(_Bottom_left?_Bottom_left:_Top_right?
			_Top_right:_Top_left?_Top_left:0); //Based on
		
			if (_Object) _Corner[3]=generate_part(part::BOTTOM_RIGHT+_Name,
				_Object,_Border,(_Bottom_left?vector2(-1.0f,-1.0f):_Top_right?
					vector2(1.0f,1.0f):vector2(-1.0f,1.0f)),vector2(1.0f,-1.0f));
		}
		
		if (!_Top) {
		
		object *_Object=(_Bottom?_Bottom:_Left?_Left:_Right?_Right:0); //Based on
		
			if (_Object) _Side[0]=generate_part(part::TOP+_Name,
				_Object,_Border,(_Bottom?vector2(0.0f,-1.0f):_Left?
					vector2(-1.0f,0.0f):vector2(1.0f,0.0f)),vector2(0.0f,1.0f));
		}
		
		if (!_Bottom) {
		
		object *_Object=(_Top?_Top:_Right?_Right:_Left?_Left:0); //Based on
		
			if (_Object) _Side[1]=generate_part(part::BOTTOM+_Name,
				_Object,_Border,(_Top?vector2(0.0f,1.0f):_Right?
					vector2(1.0f,0.0f):vector2(-1.0f,0.0f)),vector2(0.0f,-1.0f));
		}
		
		if (!_Left) {
		
		object *_Object=(_Right?_Right:_Top?_Top:_Bottom?_Bottom:0); //Based on
		
			if (_Object) _Side[2]=generate_part(part::LEFT+_Name,
				_Object,_Border,(_Right?vector2(1.0f,0.0f):_Top?
					vector2(0.0f,1.0f):vector2(0.0f,-1.0f)),vector2(-1.0f,0.0f));
		}
		
		if (!_Right) {
		
		object *_Object=(_Left?_Left:_Bottom?_Bottom:_Top?_Top:0); //Based on
		
			if (_Object) _Side[3]=generate_part(part::RIGHT+_Name,
				_Object,_Border,(_Left?vector2(-1.0f,0.0f):_Bottom?
					vector2(0.0f,-1.0f):vector2(0.0f,1.0f)),vector2(1.0f,0.0f));
		}
	}
	
	//Auto size
	if (_Auto_size!=vector2::ZERO &&
		_Auto_size!=vector2::UNIT_SCALE) {
	
	vector2 _Size(_Area->width(),_Area->height());
	vector2 _Percent(_Auto_size.x-1.0f,_Auto_size.y-1.0f);
	vector2 _Previous_size(_Size*_Percent);
	
	resize_parts(_Center,_Border,_Corner[0],_Corner[1],
		_Corner[2],_Corner[3],_Side[0],_Side[1],_Side[2],_Side[3],
			_Size.x-_Previous_size.x,_Size.y-_Previous_size.y);
	}
}

//Caption

void ui_entity::update_caption() {

object *_Object=object_pointer(additional::CAPTION);

	if (!_Object) return;

text *_Text=0;
attachable *_Data;

	//Find first text component in caption object
	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Data=(*_Iter)->data(); //Increase speed
	
		if (_Data && (_Text=dynamic_cast<text*>(_Data)))
			break;
	}
	
	//Caption text not found
	if (!_Text) return;

//Update caption data

_Text->text_font()->update(); //Update font

	//Font update failed
	if (_Text->text_font()->status()!=manageable::OKAY)
		return;

unsigned int _Max_lines=_Text->line_capacity((unsigned int)_Caption_height);

	if (_Max_lines==0) _Text->text_data("");
	else _Text->text_data(_Caption);

_Text->wrap_width((unsigned int)_Caption_width);
_Text->max_lines(_Max_lines);
_Text->update();

	//Text update failed
	if (_Text->status()!=manageable::OKAY)
		return;
	
	//Too small width for a single character
	if (_Text->width()>_Caption_width) {
	
	_Text->text_data("");
	_Text->update(); //Undo
	}
	
	//Update alignment
	switch (_Caption_origin) {
	
		case LEFT:
		case TOP_LEFT:
		case BOTTOM_LEFT:
		case OUTSIDE_RIGHT:
		case OUTSIDE_TOP_RIGHT:
		case OUTSIDE_BOTTOM_RIGHT:
		_Text->alignment(text::LEFT);
		break;
		
		case RIGHT:
		case TOP_RIGHT:
		case BOTTOM_RIGHT:
		case OUTSIDE_LEFT:
		case OUTSIDE_TOP_LEFT:
		case OUTSIDE_BOTTOM_LEFT:
		_Text->alignment(text::RIGHT);
		break;
		
		default:
		_Text->alignment(text::CENTER);
		break;
	}

vector2 _Position;

	//Update x
	switch (_Caption_origin) {
	
		case LEFT:
		case TOP_LEFT:
		case BOTTOM_LEFT:
		_Position.x=-_Area->width()*0.5f;
		break;
		
		case RIGHT:
		case TOP_RIGHT:
		case BOTTOM_RIGHT:
		_Position.x=-_Caption_width+_Area->width()*0.5f;
		break;
		
		case OUTSIDE_LEFT:
		case OUTSIDE_TOP_LEFT:
		case OUTSIDE_BOTTOM_LEFT:
		_Position.x=-_Caption_width-_Area->width()*0.5f;
		break;
		
		case OUTSIDE_RIGHT:
		case OUTSIDE_TOP_RIGHT:
		case OUTSIDE_BOTTOM_RIGHT:
		_Position.x=_Area->width()*0.5f;
		break;
		
		default:
		_Position.x=-_Caption_width*0.5f;
		break;
	}
	
	//Update y
	switch (_Caption_origin) {
	
		case TOP_LEFT:
		case TOP_CENTER:
		case TOP_RIGHT:
		case OUTSIDE_TOP_LEFT:
		case OUTSIDE_TOP_RIGHT:
		_Position.y=_Area->height()*0.5f;
		break;
		
		case BOTTOM_LEFT:
		case BOTTOM_CENTER:
		case BOTTOM_RIGHT:
		case OUTSIDE_BOTTOM_LEFT:
		case OUTSIDE_BOTTOM_RIGHT:
		_Position.y=-_Area->height()*0.5f+(float)_Text->height();
		break;
		
		default:
		_Position.y=(float)_Text->height()*0.5f;
		break;
	}

_Position=utility::position(_Position,_Area->width(),
	_Area->height(),rectangle::CENTER,_Area->origin());
_Object->node()->position(vector2(ceil(_Position.x),
	floor(_Position.y)+2.0f)+_Caption_offset);
}

//Public

ui_entity &ui_entity::operator=(const ui_entity &_Ui_entity) {

	//Worth doing
	if (this!=&_Ui_entity) {
	
	entity::operator=(_Ui_entity); //Invoke base operator=
	
	copy(_Ui_entity);
	*_Area=*_Ui_entity._Area;
	_Update_caption=true;	
	}

return *this;
}

void ui_entity::update(float _Time) {

	//Only update if entity is visible
	if (visible()) {
	
		if (_Part_status!=OKAY) {
		
		update_parts();
		
			if (_Auto_size!=vector2::ZERO)
				_Auto_size=vector2::UNIT_SCALE;
		
		_Part_status=OKAY;
		}
		
		if (_Update_positioning || _Update_sizing) {
		
		float _Width=render_window::instance()->ortho_width();
		float _Height=render_window::instance()->ortho_height();
		
			if (_Update_positioning) {
			
			update_positioning(_Width,_Height);
			_Update_positioning=false;
			}
			
			if (_Update_sizing) {
			
			update_sizing(_Width,_Height);
			_Update_sizing=false;
			}
		}
		
		if (_Update_caption) {
		
		update_caption();
		_Update_caption=false;
		}
	}

entity::update(_Time); //Use base functionality
}

//Area

void ui_entity::size(float _Width,float _Height) {

float _New_width=(_Width<0.0f?0.0f:_Width);
float _New_height=(_Height<0.0f?0.0f:_Height);

	if (_New_width!=_Area->width() || _New_height!=_Area->height()) {
	
		//Resize parts
		if (_Auto_size!=vector2::ZERO) {
		
		vector2 _Percent=vector2(_Area->width()/_New_width,
			_Area->height()/_New_height);
		
			//Add resize percent
			if (_Auto_size==vector2::UNIT_SCALE)
				_Auto_size+=_Percent;
			//Multiply with next percent
			else _Auto_size=(_Auto_size-1.0f)*_Percent+1.0f;
			
			//Update parts
			if (_Part_status==OKAY)
				_Part_status=NEED_UPDATE;
		}
		
		//Resize caption
		if (_Caption_width==_Area->width() &&
			_Caption_height==_Area->height())
				caption_size(_New_width,_New_height);
	
	_Area->size(_New_width,_New_height);
	_Update_caption=true; //Size changed
	}
}

float ui_entity::width() const {
	return _Area->width();
}

float ui_entity::height() const {
	return _Area->height();
}

rectangle *ui_entity::area() {

_Update_caption=true; //Area size may change
return _Area;
}

//Layout

void ui_entity::positioning(const LAYOUT &_Positioning) {

	if (this->_Positioning!=_Positioning) {
	
		//Change from static X positioning
		if (this->_Positioning!=DYNAMIC_X_AXIS &&
			this->_Positioning!=DYNAMIC_XY_AXIS &&
				//To dynamic X positioning
				(_Positioning==DYNAMIC_X_AXIS ||
				_Positioning==DYNAMIC_XY_AXIS))
					_Static_position.x=_Node->position().x;
		
		//Change from static Y positioning
		if (this->_Positioning!=DYNAMIC_Y_AXIS &&
			this->_Positioning!=DYNAMIC_XY_AXIS &&
				//To dynamic Y positioning
				(_Positioning==DYNAMIC_Y_AXIS ||
				_Positioning==DYNAMIC_XY_AXIS))
					_Static_position.y=_Node->position().y;
		
		//Change to static X positioning
		if (_Positioning!=DYNAMIC_X_AXIS &&
			_Positioning!=DYNAMIC_XY_AXIS &&
				//From dynamic X positioning
				(this->_Positioning==DYNAMIC_X_AXIS ||
				this->_Positioning==DYNAMIC_XY_AXIS)) {
		
		_Node->position(vector2(_Static_position.x,
			_Node->position().y));
		_Static_position.x=0.0f;
		}
		
		//Change to static Y positioning
		if (_Positioning!=DYNAMIC_Y_AXIS &&
			_Positioning!=DYNAMIC_XY_AXIS &&
				//From dynamic Y positioning
				(this->_Positioning==DYNAMIC_Y_AXIS ||
				this->_Positioning==DYNAMIC_XY_AXIS)) {
		
		_Node->position(vector2(_Node->position().x,
			_Static_position.y));
		_Static_position.y=0.0f;
		}
	
	this->_Positioning=_Positioning;
	
		if (_Positioning!=STATIC) _Update_positioning=true;
	}
}

ui_entity::LAYOUT ui_entity::positioning() const {
	return _Positioning;
}

void ui_entity::sizing(const LAYOUT &_Sizing) {

	if (this->_Sizing!=_Sizing) {
	
		//Change from static X sizing
		if (this->_Sizing!=DYNAMIC_X_AXIS &&
			this->_Sizing!=DYNAMIC_XY_AXIS &&
				//To dynamic X sizing
				(_Sizing==DYNAMIC_X_AXIS ||
				_Sizing==DYNAMIC_XY_AXIS))
					_Static_width=_Area->width();
		
		//Change from static Y sizing
		if (this->_Sizing!=DYNAMIC_Y_AXIS &&
			this->_Sizing!=DYNAMIC_XY_AXIS &&
				//To dynamic Y sizing
				(_Sizing==DYNAMIC_Y_AXIS ||
				_Sizing==DYNAMIC_XY_AXIS))
					_Static_height=_Area->height();
		
		//Change to static X sizing
		if (_Sizing!=DYNAMIC_X_AXIS &&
			_Sizing!=DYNAMIC_XY_AXIS &&
				//From dynamic X sizing
				(this->_Sizing==DYNAMIC_X_AXIS ||
				this->_Sizing==DYNAMIC_XY_AXIS)) {
		
		size(_Static_width,_Area->height());
		_Static_width=0.0f;
		}
		
		//Change to static Y sizing
		if (_Sizing!=DYNAMIC_Y_AXIS &&
			_Sizing!=DYNAMIC_XY_AXIS &&
				//From dynamic Y sizing
				(this->_Sizing==DYNAMIC_Y_AXIS ||
				this->_Sizing==DYNAMIC_XY_AXIS)) {
		
		size(_Area->width(),_Static_height);
		_Static_height=0.0f;
		}
	
	this->_Sizing=_Sizing;
	
		if (_Sizing!=STATIC) _Update_sizing=true;
	}
}

ui_entity::LAYOUT ui_entity::sizing() const {
	return _Sizing;
}

void ui_entity::positioning_unit(const LAYOUT_UNIT &_Unit) {
	positioning_unit(_Unit,_Unit);
}

void ui_entity::positioning_unit(const LAYOUT_UNIT &_X,
	const LAYOUT_UNIT &_Y) {

	if (_Positioning_unit[0]!=_X ||
		_Positioning_unit[1]!=_Y) {
	
	_Positioning_unit[0]=_X;
	_Positioning_unit[1]=_Y;
	
		if (_Positioning!=STATIC) _Update_positioning=true;
	}
}

ui_entity::LAYOUT_UNIT ui_entity::positioning_x() const {
	return _Positioning_unit[0];
}

ui_entity::LAYOUT_UNIT ui_entity::positioning_y() const {
	return _Positioning_unit[1];
}

void ui_entity::sizing_unit(const LAYOUT_UNIT &_Unit) {
	sizing_unit(_Unit,_Unit);
}

void ui_entity::sizing_unit(const LAYOUT_UNIT &_X,
	const LAYOUT_UNIT &_Y) {

	if (_Sizing_unit[0]!=_X ||
		_Sizing_unit[1]!=_Y) {
	
	_Sizing_unit[0]=_X;
	_Sizing_unit[1]=_Y;
	
		if (_Sizing!=STATIC) _Update_sizing=true;
	}
}

ui_entity::LAYOUT_UNIT ui_entity::sizing_x() const {
	return _Sizing_unit[0];
}

ui_entity::LAYOUT_UNIT ui_entity::sizing_y() const {
	return _Sizing_unit[1];
}

void ui_entity::positioning_area(const aabb &_Positioning_area) {

	if (this->_Positioning_area!=_Positioning_area) {
	
	this->_Positioning_area=_Positioning_area;
	
		if (_Positioning!=STATIC) _Update_positioning=true;
	}
}

const aabb &ui_entity::positioning_area() const {
	return _Positioning_area;
}

//Parts

void ui_entity::auto_size(bool _Auto_size) {

	if (_Auto_size && this->_Auto_size==vector2::ZERO)
		this->_Auto_size=vector2::UNIT_SCALE;
	else this->_Auto_size=vector2::ZERO;
}

bool ui_entity::auto_size() const {
	return (_Auto_size!=vector2::ZERO);
}

void ui_entity::generate_mode(const GENERATE_MODE &_Generate_mode) {

	if (this->_Generate_mode!=_Generate_mode) {
	
	this->_Generate_mode=_Generate_mode;
	
		//Generate parts
		if (_Generate_mode!=FORCE_OFF) _Part_status=GENERATE;
	}
}

ui_entity::GENERATE_MODE ui_entity::generate_mode() const {
	return _Generate_mode;
}

//Caption

void ui_entity::caption(const std::string &_Caption) {

	if (this->_Caption!=_Caption) {
	
	this->_Caption=_Caption;
	_Update_caption=true;
	}
}

const std::string &ui_entity::caption() const {
	return _Caption;
}

void ui_entity::caption_size(float _Width,float _Height) {

float _New_width=(_Width<0.0f?0.0f:_Width);
float _New_height=(_Height<0.0f?0.0f:_Height);

	if (_Caption_width!=_New_width || _Caption_height!=_New_height) {
	
	_Caption_width=_New_width;
	_Caption_height=_New_height;
	_Update_caption=true;
	}
}

float ui_entity::caption_width() const {
	return _Caption_width;
}

float ui_entity::caption_height() const {
	return _Caption_height;
}

void ui_entity::caption_offset(const vector2 &_Caption_offset) {

	if (this->_Caption_offset!=_Caption_offset) {
	
	this->_Caption_offset=_Caption_offset;
	_Update_caption=true;
	}
}

const vector2 &ui_entity::caption_offset() const {
	return _Caption_offset;
}

void ui_entity::caption_origin(const CAPTION_ORIGIN &_Caption_origin) {

	if (this->_Caption_origin!=_Caption_origin) {
	
	this->_Caption_origin=_Caption_origin;
	_Update_caption=true;
	}
}

ui_entity::CAPTION_ORIGIN ui_entity::caption_origin() const {
	return _Caption_origin;
}