/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: generic_utility.cpp
---------------------------------------------------
*/

#include "generic_utility.h"

//Dependencies
#include <algorithm>
#include <utility>

#include "file_manager.h"
#include "render_node.h"
#include "manageable.h"

#include "text.h"
#include "font.h"
#include "texture.h"
#include "animation.h"
#include "particle_system.h"
#include "scene_manager.h"

#include "any_cast.h"
#include "string_utility.h"

namespace lunar {

//Private

utility::utility() {
	//Empty
}

utility::~utility() {
	//Empty
}

//Other

std::string utility::attachable_type(attachable *_Attachable) {

std::string _Result;

	if (_Attachable) {
	
		if (dynamic_cast<text*>(_Attachable))
			_Result="<text>";
		else if (dynamic_cast<texture*>(_Attachable))
			_Result="<texture>";
		else if (dynamic_cast<animation*>(_Attachable))
			_Result="<animation>";
		else if (dynamic_cast<particle_system*>(_Attachable))
			_Result="<particle_system>";
		
		else if (dynamic_cast<border*>(_Attachable))
			_Result="<border>";
		else if (dynamic_cast<rectangle*>(_Attachable))
			_Result="<rectangle>";
		else if (dynamic_cast<curve*>(_Attachable))
			_Result="<curve>";
		else if (dynamic_cast<ellipse*>(_Attachable))
			_Result="<ellipse>";
		else if (dynamic_cast<polygon*>(_Attachable))
			_Result="<polygon>";
	}

return _Result;
}

std::string utility::manageable_data(manageable *_Manageable) {

std::string _Result;

	//Nothing to do
	if (_Manageable) {
	
	text *_Text=dynamic_cast<text*>(_Manageable);
	
		if (_Text && _Text->text_font() && _Text->text_font()->data())
			_Result=_Text->text_font()->data()->name();
		else if (_Manageable->data())
			_Result=_Manageable->data()->name();
	}

return _Result;
}

//Public
//Conversions

std::string utility::base(unsigned int _Value,unsigned int _Base) {

static const char _Arr[]={'0','1','2','3','4','5','6','7','8','9',
						  'A','B','C','D','E','F','G','H','I','J',
						  'K','L','M','N','O','P','Q','R','S','T',
						  'U','V','W','X','Y','Z'};
static const unsigned int _Size=sizeof(_Arr)/sizeof(_Arr[0]);

//Decimal to any base conversion
std::string _Result;

	if (_Base>=2 && _Base<=_Size) {
	
		do _Result=_Arr[_Value%_Base]+_Result;
		while (_Value/=_Base);
	}

return _Result;
}

vector2 utility::position(const vector2 &_Position,float _Width,
	float _Height,const rectangle::ORIGIN &_From,
		const rectangle::ORIGIN &_To) {

	//Nothing to do
	if (_From==_To) return _Position;

vector2 _New_position=_Position;

	//To CENTER x
	switch (_From) {
	
		case rectangle::TOP_LEFT:
		case rectangle::CENTER_LEFT:
		case rectangle::BOTTOM_LEFT:
		_New_position.x-=_Width*0.5f;
		break;
		
		case rectangle::TOP_RIGHT:
		case rectangle::CENTER_RIGHT:
		case rectangle::BOTTOM_RIGHT:
		_New_position.x+=_Width*0.5f;
		break;
	}
	
	//To CENTER y
	switch (_From) {
	
		case rectangle::TOP_LEFT:
		case rectangle::TOP_CENTER:
		case rectangle::TOP_RIGHT:
		_New_position.y+=_Height*0.5f;
		break;
		
		case rectangle::BOTTOM_LEFT:
		case rectangle::BOTTOM_CENTER:
		case rectangle::BOTTOM_RIGHT:
		_New_position.y-=_Height*0.5f;
		break;
	}
	
	//From CENTER x
	switch (_To) {
	
		case rectangle::TOP_LEFT:
		case rectangle::CENTER_LEFT:
		case rectangle::BOTTOM_LEFT:
		_New_position.x+=_Width*0.5f;
		break;
		
		case rectangle::TOP_RIGHT:
		case rectangle::CENTER_RIGHT:
		case rectangle::BOTTOM_RIGHT:
		_New_position.x-=_Width*0.5f;
		break;
	}
	
	//From CENTER y
	switch (_To) {
	
		case rectangle::TOP_LEFT:
		case rectangle::TOP_CENTER:
		case rectangle::TOP_RIGHT:
		_New_position.y-=_Height*0.5f;
		break;
		
		case rectangle::BOTTOM_LEFT:
		case rectangle::BOTTOM_CENTER:
		case rectangle::BOTTOM_RIGHT:
		_New_position.y+=_Height*0.5f;
		break;
	}

return _New_position;
}

vector2 utility::origin(const vector2 &_Position,float _Width,
	float _Height,const rectangle::ORIGIN &_From,
		const rectangle::ORIGIN &_To) {

	//Nothing to do
	if (_From==_To) return _Position;

vector2 _New_position=_Position;

	//To CENTER x
	switch (_From) {
	
		case rectangle::TOP_LEFT:
		case rectangle::CENTER_LEFT:
		case rectangle::BOTTOM_LEFT:
		_New_position.x+=_Width*0.5f;
		break;
		
		case rectangle::TOP_RIGHT:
		case rectangle::CENTER_RIGHT:
		case rectangle::BOTTOM_RIGHT:
		_New_position.x-=_Width*0.5f;
		break;
	}
	
	//To CENTER y
	switch (_From) {
	
		case rectangle::TOP_LEFT:
		case rectangle::TOP_CENTER:
		case rectangle::TOP_RIGHT:
		_New_position.y-=_Height*0.5f;
		break;
		
		case rectangle::BOTTOM_LEFT:
		case rectangle::BOTTOM_CENTER:
		case rectangle::BOTTOM_RIGHT:
		_New_position.y+=_Height*0.5f;
		break;
	}
	
	//From CENTER x
	switch (_To) {
	
		case rectangle::TOP_LEFT:
		case rectangle::CENTER_LEFT:
		case rectangle::BOTTOM_LEFT:
		_New_position.x-=_Width*0.5f;
		break;
		
		case rectangle::TOP_RIGHT:
		case rectangle::CENTER_RIGHT:
		case rectangle::BOTTOM_RIGHT:
		_New_position.x+=_Width*0.5f;
		break;
	}
	
	//From CENTER y
	switch (_To) {
	
		case rectangle::TOP_LEFT:
		case rectangle::TOP_CENTER:
		case rectangle::TOP_RIGHT:
		_New_position.y+=_Height*0.5f;
		break;
		
		case rectangle::BOTTOM_LEFT:
		case rectangle::BOTTOM_CENTER:
		case rectangle::BOTTOM_RIGHT:
		_New_position.y-=_Height*0.5f;
		break;
	}

return _New_position;
}

//Other

bool utility::dump_graph(const std::string &_Filename,
	render_node *_Parent_node,const DUMP_MODE &_Mode) {

	//Use root node as default parent
	if (!_Parent_node)
		_Parent_node=render_manager::instance()->root_node();

struct _Node {

render_node *_Render_node;
render_node::childnodes_iter _Iter;
unsigned int _X;
unsigned int _Y;
};

typedef std::vector<_Node*> _Graph;
typedef std::vector<_Node*> _Nodes;
typedef std::vector<_Nodes> _Canvas;

_Graph _Mygraph;
_Nodes _Mynodes;
_Canvas _Mycanvas;

//Add parent node
_Node *_Mynode=new _Node;
_Mynode->_Render_node=_Parent_node;
_Mynode->_Iter=_Parent_node->childnodes_begin();
_Mynode->_X=0;
_Mynode->_Y=0;
_Mygraph.push_back(_Mynode);
_Mynodes.push_back(_Mynode);
_Mycanvas.push_back(_Mynodes);

_Nodes _Placed_nodes;
render_node *_Render_node;

	//Build canvas
	while (!_Mygraph.empty()) {
	
	_Mynode=_Mygraph.back(); //Increase speed
	_Render_node=_Mynode->_Render_node;
	
		//Add new node to graph
		if (_Mynode->_Iter!=_Render_node->childnodes_end()) {
		
		unsigned int _Column=_Mynode->_X;
		unsigned int _Row=_Mynode->_Y+1;
		bool _Left_most=(_Mynode->_Iter==_Render_node->childnodes_begin());
		
			//Left most child
			//Place all child nodes of this parent node
			//Resize canvas and move branch if necessary
			if (_Left_most) {
			
			//Step 1
			//Find number of child nodes
			unsigned int _Count=(unsigned int)std::distance(
				_Mynode->_Iter,_Render_node->childnodes_end());
			
			unsigned int _Width=_Count/2;
			unsigned int _Off=0;
			
				//Step 2
				//Increase canvas height
				if (_Mycanvas.size()<=_Mygraph.size())
					_Mycanvas.push_back(_Nodes(_Mycanvas.back().size(),0));
				
				//Step 3
				//Check for collisions
				else {
				
				unsigned int _Leftmost_x=(_Mynode->_X>_Width?
					_Mynode->_X-_Width:0);
				
					for (size_t i=_Mycanvas[_Row].size();i>_Leftmost_x;--i) {
					
						//Collision detected
						if (_Mycanvas[_Row][i-1]) {
						
						_Off=(unsigned int)i-(_Mynode->_X-_Width)+1;
						break;
						}
					}
				}
			
			//Step 4
			//Increase canvas width
			
				//Make room on left side
				if (_Width>_Mynode->_X+_Off) {
				
					for (size_t i=0;i<_Mycanvas.size();++i)
						_Mycanvas[i].insert(_Mycanvas[i].begin(),
							_Width-(_Mynode->_X+_Off),0);
				
				unsigned int _X=_Mynode->_X;
				
					//Update node positions
					for (_Nodes::const_iterator _Iter=_Mynodes.begin(),
						_End=_Mynodes.end();_Iter!=_End;++_Iter)
							(*_Iter)->_X+=_Width-(_X+_Off);
				}
				
				//Make room on right side
				if (_Mynode->_X+_Width+_Off+1>_Mycanvas[_Mynode->_Y].size()) {
				
					for (size_t i=0;i<_Mycanvas.size();++i)
						_Mycanvas[i].insert(_Mycanvas[i].end(),
							(_Mynode->_X+_Width+_Off+1)-
								_Mycanvas[i].size(),0);
				}
				
				//Collision detected
				if (_Off>0) {
				
				_Node *_Branch_node;
				
					//Step 5
					//Backtrack branch and update positions
					for (_Graph::const_reverse_iterator _Riter=_Mygraph.rbegin(),
						_Rend=_Mygraph.rend();_Riter!=_Rend;++_Riter) {
					
					_Branch_node=(*_Riter); //Increase speed
					unsigned int _X=_Branch_node->_X;
					unsigned int _Y=_Branch_node->_Y;
					
						if (_Branch_node!=_Mynode) {
						
						unsigned int _Leftmost_x=_X;
						unsigned int _Rightmost_x=_X;
						
							//Find left most node
							for (size_t i=0;i<_Mycanvas[_Y+1].size();++i) {
							
								if (_Mycanvas[_Y+1][i] && _Mycanvas[_Y+1][i]->
									_Render_node->parent_node()==_Branch_node->
										_Render_node) {
								
								_Leftmost_x=_Mycanvas[_Y+1][i]->_X;
								break;
								}
							}
							
							//Find right most node
							for (size_t i=_Mycanvas[_Y+1].size();i>0;--i) {
							
								if (_Mycanvas[_Y+1][i-1] && _Mycanvas[_Y+1][i-1]->
									_Render_node->parent_node()==_Branch_node->
										_Render_node) {
								
								_Rightmost_x=_Mycanvas[_Y+1][i-1]->_X;
								break;
								}
							}
						
						_Off=(_Leftmost_x+_Rightmost_x)/2-_X;
						}
						
						if (_Off>0) {
						
							//Move nodes by off amount
							for (size_t i=(_Branch_node->_Render_node==_Parent_node?
								_X+1:_Mycanvas[_Y].size());i>_X;--i) {
							
								//No node
								if (!_Mycanvas[_Y][i-1]) continue;
								
								//Make even more room on right side
								if (i+_Off>_Mycanvas[_Y].size()) {
								
									for (size_t j=0;j<_Mycanvas.size();++j)
										_Mycanvas[j].insert(_Mycanvas[j].end(),
											i+_Off-_Mycanvas[j].size(),0);
								}
							
							std::swap(_Mycanvas[_Y][i-1],_Mycanvas[_Y][i-1+_Off]);
							_Mycanvas[_Y][i-1+_Off]->_X+=_Off; //Update referenced variable
							}
						}
					}
				}
			
			_Column=_Mynode->_X-_Width; //Update row
			
			unsigned int _X=_Column;
			bool _Even=(_Count%2==0);
			
				//Step 6
				//Place all child nodes
				for (render_node::childnodes_iter _Iter=_Mynode->_Iter,
					_End=_Render_node->childnodes_end();_Iter!=_End;
						++_Iter,++_X) {
				
					//Skip center
					if (_Even && _X==_Mynode->_X) ++_X;
				
				_Render_node=(*_Iter);
				
				//Add child node
				_Node *_Mychild_node=new _Node;
				_Mychild_node->_Render_node=_Render_node;
				_Mychild_node->_Iter=_Render_node->childnodes_begin();
				_Mychild_node->_X=_X;
				_Mychild_node->_Y=_Row;
				
				_Mycanvas[_Row][_X]=_Mychild_node;
				_Mynodes.push_back(_Mychild_node);
				
					//Placed but not yet traversed
					//Do not add the left most child
					if (_X!=_Column) _Placed_nodes.push_back(_Mychild_node);
				}
			}
		
		_Render_node=(*_Mygraph.back()->_Iter++);
		
			//Newly placed
			if (_Left_most) _Mynode=_Mycanvas[_Row][_Column];
			else {
			
				//Find already placed node
				for (_Nodes::iterator _Iter=_Placed_nodes.begin(),
					_End=_Placed_nodes.end();_Iter!=_End;++_Iter) {
				
				_Mynode=(*_Iter);
				
					if (_Mynode->_Render_node==_Render_node) {
					
					_Placed_nodes.erase(_Iter); //Traversed
					break;
					}
				}
			}
		
		_Mygraph.push_back(_Mynode);
		continue;
		}
	
	_Mygraph.pop_back();
	}

//Write graph to file

unsigned int _Id=0;
unsigned int _Id_base=36;
std::vector<std::string> _Node_template;

	if (_Mode==PROPERTIES_IN_LIST)
		_Node_template.push_back(std::string(base(
			(unsigned int)_Mynodes.size()-1,_Id_base).size(),'0'));
	else if (_Mode==PROPERTIES_IN_NODE) {
	
	_Node_template.push_back(" _________________ ");
	_Node_template.push_back("|                 |");
	_Node_template.push_back("|_________________|");
	_Node_template.push_back("|                 |");
	_Node_template.push_back("|                 |");
	_Node_template.push_back("|                 |");
	_Node_template.push_back("|                 |");
	_Node_template.push_back("|_________________|");
	}
	else _Node_template.push_back("O");

unsigned int _Node_width=(unsigned int)_Node_template.front().size();
unsigned int _Node_height=(unsigned int)_Node_template.size();
unsigned int _Canvas_width=(unsigned int)_Mycanvas.front().size();
unsigned int _Canvas_height=(unsigned int)_Mycanvas.size();

unsigned int _Columns=_Canvas_width*_Node_width+_Canvas_width;
unsigned int _Rows=_Canvas_height*_Node_height+(_Canvas_height-1)*2;
std::string _Data(_Columns*_Rows,' ');

	//Rows
	for (size_t _Y=0,_Row=0;_Y<_Mycanvas.size();++_Y,
		_Row+=_Columns*(_Node_height+2)) {
	
		//Columns
		for (size_t _X=0,_Column=0;_X<_Mycanvas[_Y].size();++_X,
			_Column+=_Node_width+1) {
		
			//No node
			if (!_Mycanvas[_Y][_X]) continue;
		
		_Render_node=_Mycanvas[_Y][_X]->_Render_node; //Increase speed
		
		render_node::attachables_iter _Iter=_Render_node->attachables_begin();
		attachable *_Attachable=0;
		
			//Add node template
			for (unsigned int i=0;i<_Node_height;++i) {
			
				if (_Mode==PROPERTIES_IN_LIST)
					_Data.replace(_Row+_Column+(_Columns*i),
						_Node_template[i].size(),string_utility::fill(
							base(_Id,_Id_base),_Node_template[i].size(),'0'));
				else if (_Mode==PROPERTIES_IN_NODE) {
				
				std::string _Template=_Node_template[i];
				size_t _Width=_Template.size()-2;
				
					//Visibility and draw order
					if (i==1) {
					
					_Template.replace(1,_Width,string_utility::field(
						any_cast(_Render_node->draw_order()),_Width));
					
						if (_Render_node->visible())
							_Template.replace(1,1,"x");
					}
					//Attachables
					else if (i>2 && i<7) {
					
					std::string _Template_data;
					
						//Manageable data
						if (_Attachable) {
						
						manageable *_Manageable=dynamic_cast<manageable*>(_Attachable);
						
							if (_Manageable)
								_Template_data=manageable_data(_Manageable);
						
						++_Iter;
						_Attachable=0;
						}
						
						//Attachable type
						if (_Template_data.empty() &&
							_Iter!=_Render_node->attachables_end()) {
						
						_Attachable=(*_Iter);
						_Template_data=attachable_type(_Attachable);
						}
						
						if (!_Template_data.empty())
							_Template.replace(1,_Width,string_utility::
								field(_Template_data,_Width));
					}
				
				_Data.replace(_Row+_Column+(_Columns*i),
					_Template.size(),_Template);
				}
				else _Data.replace(_Row+_Column+(_Columns*i),
					_Node_template[i].size(),_Node_template[i]);
			}
		
		++_Id;
		
			//Add path between parent and child node
			if (_Render_node->childnodes_begin()!=
				_Render_node->childnodes_end()) {
			
				//Add horizontal line between siblings
				if (std::distance(_Render_node->childnodes_begin(),
					_Render_node->childnodes_end())>1) {
				
				unsigned int _Leftmost_x=(unsigned int)_X;
				unsigned int _Rightmost_x=(unsigned int)_X;
				
					//Find left most node
					for (size_t j=0;j<_Mycanvas[_Y+1].size();++j) {
					
						if (_Mycanvas[_Y+1][j] && _Mycanvas[_Y+1][j]->
							_Render_node->parent_node()==_Render_node) {
						
						_Leftmost_x=_Mycanvas[_Y+1][j]->_X;
						break;
						}
					}
					
					//Find right most node
					for (size_t j=_Mycanvas[_Y+1].size();j>0;--j) {
					
						if (_Mycanvas[_Y+1][j-1] && _Mycanvas[_Y+1][j-1]->
							_Render_node->parent_node()==_Render_node) {
						
						_Rightmost_x=_Mycanvas[_Y+1][j-1]->_X;
						break;
						}
					}
				
				unsigned int _Sibling_range=_Rightmost_x-_Leftmost_x;
				unsigned int _Count=_Sibling_range*_Node_width+_Sibling_range-1;
				_Data.replace(_Row+_Column+(_Columns*_Node_height)+(_Node_width/2)-
					(_Count/2)+(_Sibling_range%2==1?(_Node_width+1)/2+
						(_Node_width+1)%2:0),_Count,std::string(_Count,'_'));
				}
			
			_Data.replace(_Row+_Column+(_Columns*_Node_height)+
				(_Node_template.back().size()/2),1,"|");
			}
			
			//Add path between child and parent node
			if (_Y>0) _Data.replace(_Row+_Column-_Columns+
				(_Node_template.back().size()/2),1,"|");
		}
	}
	
	//Add line breaks
	for (size_t i=0;i<_Rows;++i)
		_Data.replace(_Columns*(i+1)-1,1,"\n");
	
	//Print out node properties
	if (_Mode==PROPERTIES_IN_LIST) {
	
	unsigned int _Draworder_width=0;
	
		//Find max draw order width
		for (size_t i=0;i<_Mynodes.size();++i) {
		
		unsigned int _Width=(unsigned int)any_cast(_Mynodes[i]->
			_Render_node->draw_order()).size();
		
			if (_Width>_Draworder_width) _Draworder_width=_Width;
		}
	
	std::string _Property_list;
	_Id=0; //Reset Id
	
		//Rows
		for (size_t _Y=0,_Row=0;_Y<_Mycanvas.size();++_Y,
			_Row+=_Columns*(_Node_height+2)) {
		
			//Columns
			for (size_t _X=0,_Column=0;_X<_Mycanvas[_Y].size();++_X,
				_Column+=_Node_width+1) {
			
				//No node
				if (!_Mycanvas[_Y][_X]) continue;
			
			_Render_node=_Mycanvas[_Y][_X]->_Render_node; //Increase speed
			
			//Id, visibility and draw order
			_Property_list+="\n"+string_utility::fill(base(
				_Id++,_Id_base),_Node_template.front().size(),'0')+
					" | "+(_Render_node->visible()?"x":" ")+" | "+
						string_utility::fill(any_cast(_Render_node->
							draw_order()),_Draworder_width)+" |";
			
				//Attachables
				for (render_node::attachables_iter _Iter=_Render_node->attachables_begin(),
					_End=_Render_node->attachables_end();_Iter!=_End;++_Iter) {
				
				attachable *_Attachable=(*_Iter); //Increase speed
				manageable *_Manageable=dynamic_cast<manageable*>(_Attachable);
				
				_Property_list+=" "+attachable_type(_Attachable);
				
					//Manageable
					if (_Manageable) {
					
					std::string _Data=manageable_data(_Manageable);
					
						if (!_Data.empty()) _Property_list+=" "+_Data;
					}
					
					//More attachables
					if (++render_node::attachables_iter(_Iter)!=_End)
						_Property_list+=",";
				}
			}
		}
	
	_Data+=_Property_list;
	}
	
	//Clean up nodes
	for (size_t i=0;i<_Mynodes.size();++i)
		delete _Mynodes[i];

return file_manager::save(_Filename,_Data);
}

} //namespace lunar