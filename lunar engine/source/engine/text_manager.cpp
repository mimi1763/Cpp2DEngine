/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: text_manager.cpp
---------------------------------------------------
*/

#include "text_manager.h"

//Dependencies
#include <stack>
#include <cctype>
#include <algorithm>
#include <sstream>

#include "string_utility.h"
#include "lexical_cast.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
text_manager *text_manager::_Instance=0;

//Protected

void text_manager::copy(const text_manager &_Text_manager) {
	_Font_manager=_Text_manager._Font_manager;
}

//Text specific

void text_manager::init_text(text *_Text) const {

_Text->_Mylines.clear(); //Clean up

std::string _Data=_Text->_Text_data; //Don't change original data
text::_Elements _Myelements;

	//Format text into elements based on different styles
	if (_Text->_Styles_enabled) parse_styles(_Data,_Myelements);
	//Only one element required
	else {
	
	text::element _Element;
	_Element._Data=_Data;
	_Element._Width=0;
	_Myelements.push_back(_Element);
	}

_Text->_Width=width(_Data,_Text->_Font);

	//Nothing to do
	if (_Data.empty()) return;
	
	//If wrap width is specified and text is too long, use word wrap
	if (_Text->_Wrap_width>0 && _Text->_Width>_Text->_Wrap_width) {
	
	word_wrap(_Data,_Text->_Font,_Text->_Wrap_width);
	adjust_elements(_Data,_Myelements);
	}

break_elements(_Myelements,_Text->_Font,_Text->_Mylines);
}

void text_manager::parse_styles(std::string &_Data,text::_Elements &_Myelements) {

	//Nothing to do
	if (_Data.empty()) return;

static std::string _Style_begin="<style ";
static std::string _Style_end="</style>";
std::stack<text::style> _Styles;

	//Check for tags
	for (size_t i=0;i<_Data.size();++i) {
	
	size_t _Pos=_Data.find(_Style_begin,i);
	size_t _End=0;
	
		if (!_Styles.empty()) {
		
		_End=_Data.find(_Style_end,i);
		
			//Found a begin tag or an end tag
			if (_Pos!=std::string::npos || _End!=std::string::npos) {
			
			_Myelements.back()._Data=_Data.substr(i,
				(_Pos<_End?_Pos:_End)-i);
			
				//The end tag is closest 
				if (_End<_Pos) {
				
				_Styles.pop();
				
					if (!_Styles.empty()) {
					
					text::element _Element;
					_Element._Width=0;
					_Element._Style=_Styles.top();
					_Myelements.push_back(_Element);
					}
				
				i=_End+_Style_end.size()-1;
				continue;
				}
			}
		}
		
		if (_Styles.empty() && _Pos>i) {
		
		text::element _Element;
		_Element._Data=_Data.substr(i,_Pos-i);
		_Element._Width=0;
		_Myelements.push_back(_Element);
		}
		
		//Found a begin tag
		if (_Pos!=std::string::npos) {
		
		_Pos=_Data.find('>',i=_Pos+_Style_begin.size());
		
			//Found the end of the begin tag
			if (_Pos!=std::string::npos) {
			
			text::element _Element;
			_Element._Width=0;
			
			string_utility::tokens _Types;
			string_utility::tokenize(_Data.substr(i,_Pos-i),",",_Types);
			
				//Check style types
				for (size_t j=0;j<_Types.size();++j) {
				
				string_utility::tokens _Values;
				string_utility::tokenize(_Types[j],"=",_Values);
				
					//Check type values
					if (_Values.size()>1) {
					
						if (_Values[0]=="color") _Element._Style.
							color_value(lexical_cast<color>(_Values[1]));
					}
				}
			
			_Styles.push(_Element._Style);
			_Myelements.push_back(_Element);
			}
		}
		
		//No more tags
		if (_Pos==std::string::npos) break;
	
	i=_Pos;
	}

_Data.clear();

	for (text::_Elements::iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;) {
	
	text::element &_Element=(*_Iter); //Increase speed
	
		if (_Element._Data.empty()) _Iter=_Myelements.erase(_Iter);
		else {
		
		_Data+=_Element._Data;
		++_Iter;
		}
	}
}

void text_manager::word_wrap(std::string &_Data,font *_Font,
	unsigned int _Wrap_width) {

unsigned int _Width=_Wrap_width;
unsigned int _Linebreak=-1;

	for (unsigned int i=0;i<_Data.size();++i) {
	
		//Force new line
		if (_Data[i]=='\n') {
		
		_Width=_Wrap_width;
		_Linebreak=-1;
		continue;
		}
		
		//Use space as default line break
		if (_Data[i]==' ') _Linebreak=i;
	
	unsigned int _Charwidth=width(_Data[i],_Font);
	
		//Make new line
		if (_Charwidth>_Width) {
		
			//Line must contain at least one character
			if (_Width==_Wrap_width) {
			
				if (i<_Data.size()-1) _Data.insert(++i,"\n");
			}
			//No line break, break inside word
			else if (_Linebreak==-1) _Data.insert(i,"\n");
			//Break at line break
			else {
			
			_Data[_Linebreak]='\n';
			i=_Linebreak;
			}
		
		_Width=_Wrap_width;
		_Linebreak=-1;
		}
		else _Width-=_Charwidth; //More width left
	}
}

void text_manager::adjust_elements(const std::string &_Data,
	text::_Elements &_Myelements) {

	//Nothing to do
	if (_Myelements.empty()) return;
	
	if (_Myelements.size()==1) {
	
	_Myelements.back()._Data=_Data;
	return;
	}

size_t _Pos=0;

	//Make sure each element is updated with new changes
	for (text::_Elements::iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	text::element &_Element=(*_Iter); //Increase speed
	
		for (size_t i=0;i<_Element._Data.size();++i,++_Pos) {
		
			if (_Element._Data[i]!=_Data[_Pos]) {
			
				if (_Element._Data[i]==' ') _Element._Data[i]=_Data[_Pos];
				else _Element._Data.insert(i,_Data.substr(_Pos,1));
			}
		}
	}
	
	if (_Pos<_Data.size()) _Myelements.back()._Data+=_Data.substr(_Pos);
}

void text_manager::break_elements(const text::_Elements &_Myelements,
	font *_Font,text::_Lines &_Mylines) {

	//Break all elements to multiple elements with new line
	for (text::_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	const text::element &_Element=(*_Iter); //Increase speed
	
	size_t _Off=0;
	size_t _Pos=0;
	
		do {
		
		_Pos=_Element._Data.find('\n',_Off);
		
		text::element _Elem;
		_Elem._Data=_Element._Data.substr(_Off,_Pos-_Off);
		_Elem._Width=width(_Elem._Data,_Font);
		_Elem._Style=_Element._Style;
		
			if (_Off==0 && !_Mylines.empty()) {
			
			_Mylines.back()._Width+=_Elem._Width;
			_Mylines.back()._Myelements.push_back(_Elem);
			_Off=_Pos+1;
			continue;
			}
		
		text::line _Line;
		_Line._Width=_Elem._Width;
		_Line._Myelements.push_back(_Elem);
		_Mylines.push_back(_Line);
		
		_Off=_Pos+1;
		} while (_Pos!=std::string::npos);
	}
}

//Public

text_manager::text_manager():_Font_manager(
	font_manager::instance()) {
	//Empty
}

text_manager::text_manager(font_manager *_Font_manager):
	_Font_manager(_Font_manager) {
	//Empty
}

text_manager::text_manager(const text_manager &_Text_manager):
	manager(_Text_manager) {
	copy(_Text_manager);
}

text_manager::~text_manager() {
	//Empty - let manager::~manager() do the clean up
}

text_manager &text_manager::operator=(const text_manager &_Text_manager) {

	//Worth doing
	if (this!=&_Text_manager) {
	
	manager::operator=(_Text_manager); //Invoke base operator=
	copy(_Text_manager);
	}

return *this;
}

text_manager *text_manager::instance() {
	return _Instance;
}

void text_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

text *_Text=dynamic_cast<text*>(_Manageable);

	//Invalid type for this manager
	if (!_Text) return;
	
	//No font
	if (!_Text->_Font) {
	
	status(_Text,manageable::FAILED);
	return;
	}

_Font_manager->update(_Text->_Font); //Update font

	//Font is invalid
	if (_Text->_Font->status()!=manageable::OKAY) {
	
	status(_Text,manageable::FAILED);
	return;
	}

init_text(_Text);
status(_Text,manageable::OKAY); //No need for update anymore
}

void text_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

unsigned int text_manager::width(unsigned char _Elem,font *_Font) {
	return (_Font->advance() && _Elem<_Font->glyphs()?
		_Font->advance()[_Elem]/64:0);
}

unsigned int text_manager::width(const std::string &_Data,font *_Font) {

	//No data or font advance info
	if (_Data.empty() || !_Font->advance()) return 0;

unsigned int _Width=0;

	for (unsigned int i=0;i<_Data.size();++i) {
	
	unsigned char _Elem=(unsigned char)_Data[i];
	
		//Advance if character is in range
		if (_Elem<_Font->glyphs()) _Width+=_Font->advance()[_Elem];
	}

return _Width/64;
}

std::string text_manager::trim(const std::string &_Data,font *_Font,
	unsigned int _Max_width) {

unsigned int _Width=width(_Data,_Font);

	//Nothing to do
	if (_Width<=_Max_width) return _Data;

unsigned int _Suffix_width=width("...",_Font);

	//Nothing to to
	if (_Suffix_width>_Max_width) return "";
	if (_Suffix_width==_Max_width) return "...";

unsigned int _Count=0;

	//Trim data
	do {
	
	_Width-=width(_Data[_Data.size()-(++_Count)],_Font);
	} while (_Width+_Suffix_width>_Max_width);

return _Data.substr(0,_Data.size()-_Count)+"...";
}

//Text functions

text *text_manager::create_text(const std::string &_Data,
	const std::string &_Font_name,const color &_Color,
		bool _Visible,bool _Secured) {

text *_Mytext=new text(this,_Data,_Font_manager->create_font(
	_Font_name,_Secured),_Color,_Visible);
add(_Mytext,_Secured);
return _Mytext;
}

text *text_manager::create_text(const std::string &_Data,const font_data &_Font_data,
	const color &_Color,bool _Visible,bool _Secured) {
	return create_text(_Data,_Font_manager->font_source()->add_data(
		_Font_data),_Color,_Visible,_Secured);
}

text *text_manager::create_text(const std::string &_Data,font_data *_Font_data,
	const color &_Color,bool _Visible,bool _Secured) {

text *_Mytext=new text(this,_Data,_Font_manager->create_font(
	_Font_data,_Secured),_Color,_Visible);
add(_Mytext,_Secured);
return _Mytext;
}

text *text_manager::create_text(const text &_Text,bool _Secured) {

text *_Mytext=new text(this,_Text,_Text._Font?_Font_manager->
	create_font(*_Text._Font,_Secured):0);
add(_Mytext,_Secured);
return _Mytext;
}

} //namespace lunar