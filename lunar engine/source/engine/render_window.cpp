/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: render_window.cpp
---------------------------------------------------
*/

#include "render_window.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
render_window *render_window::_Instance=0;

//Protected

render_window::render_window(const std::string &_Title,unsigned int _Width,
	unsigned int _Height,unsigned int _Color_depth,
		bool _Fullscreen):_Instance_handle(0),_Window_handle(0),
			_Device_context(0),_Rendering_context(0),_Title(_Title),
				_Width(_Width),_Height(_Height),_Color_depth(_Color_depth),
					_Fullscreen(_Fullscreen),_Left(0),_Top(0),_Active(false),
						_Stretch_width(1),_Stretch_height(1),_Min_width(256),
							_Min_height(256),_Resizable(true),_Stretch(false),
								_Mouse_visible(true),_Mode_lock(false),
									_Windowed_left(0),_Windowed_top(0),
										_Windowed_width(0),_Windowed_height(0) {

memset(&_Device_mode,0,sizeof(_Device_mode));
_Device_mode.dmSize=sizeof(_Device_mode);
_Device_mode.dmBitsPerPel=_Color_depth;	
_Device_mode.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

fullscreen_size(_Width,_Height);
}

render_window::render_window(const render_window &_Render_window) {
	copy(_Render_window);
}

render_window::~render_window() {
	close_glwindow();
}

render_window &render_window::operator=(const render_window &_Render_window) {

	//Worth doing
	if (this!=&_Render_window) copy(_Render_window);

return *this;
}

void render_window::copy(const render_window &_Render_window) {
	//Empty - override default memcpy behaviour
}

LRESULT	CALLBACK render_window::static_windowproc(HWND _Window_handle,
	UINT _Message,WPARAM _Wparam,LPARAM _Lparam) {

render_window *_Render_window=0;

	if (_Message==WM_NCCREATE) {
	
	_Render_window=(render_window*)((LPCREATESTRUCT)
		_Lparam)->lpCreateParams;
	SetWindowLong(_Window_handle,GWL_USERDATA,(LONG)(LONG_PTR)
		_Render_window); //Set user data
	}

_Render_window=(render_window*)(LONG_PTR)GetWindowLong(
	_Window_handle,GWL_USERDATA); //Get user data

return (_Render_window?_Render_window->windowproc(_Window_handle,
	_Message,_Wparam,_Lparam):DefWindowProc(_Window_handle,_Message,
		_Wparam,_Lparam));
}

LRESULT	CALLBACK render_window::windowproc(HWND _Window_handle,
	UINT _Message,WPARAM _Wparam,LPARAM _Lparam) {

bool _Received_message=false;

	//Handle internal messages before sending out to the listeners
	switch (_Message) {
	
		case WM_ACTIVATE:
		_Active=(LOWORD(_Wparam)!=0);		
		_Received_message=true;
		break;
		
		case WM_CLOSE:
		PostQuitMessage(0);
		_Received_message=true;
		break;
		
		case WM_SYSKEYDOWN:
		
			//Don't register repeated keydowns (ALT+ENTER)
			if ((_Lparam & 0x40000000)==0 && _Wparam==VK_RETURN) {
			
			fullscreen(!_Fullscreen);
			_Received_message=true;
			}
		
		break;
		
		case WM_MOVE:
		
		_Left=(short)LOWORD(_Lparam);
		_Top=(short)HIWORD(_Lparam);
		_Received_message=true;
		break;
		
		case WM_SIZE: {
		resize_opengl(LOWORD(_Lparam),HIWORD(_Lparam));
		
		//Send custom message with the OpenGL adjusted size
		LPARAM _Glsize=(_Stretch?MAKELPARAM(_Stretch_width,
			_Stretch_height):_Lparam);
		PostMessage(_Window_handle,message_listener::WM_GLSIZE,
			_Wparam,_Glsize);
		
		_Received_message=true;
		break;
		}
		
		case WM_GETMINMAXINFO: {
		
		RECT _Min_size;
		_Min_size.left=0;
		_Min_size.top=0;
		_Min_size.right=_Min_width;
		_Min_size.bottom=_Min_height;
		AdjustWindowRectEx(&_Min_size,windowed_style(),false,
			windowed_exstyle());
		
		MINMAXINFO *_Info=(MINMAXINFO*)_Lparam;
		_Info->ptMinTrackSize.x=_Min_size.right-_Min_size.left;
		_Info->ptMinTrackSize.y=_Min_size.bottom-_Min_size.top;
		_Received_message=true;
		break;
		}
		
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL: {
		
			if (_Message==WM_MOUSEWHEEL) {
			
			unsigned int _X=LOWORD(_Lparam);
			unsigned int _Y=HIWORD(_Lparam);
			
				//Cancel this message if cursor is outside window
				if ((int)_X<_Left || (int)_X>_Left+(int)_Width ||
					(int)_Y<_Top || (int)_Y>_Top+(int)_Height) return 0;
			
			//Adjust to window coordinates
			_Lparam=MAKELPARAM(_X-_Left,_Y-_Top);
			}
			
			//OpenGL adjusted mouse coordinates
			if (_Stretch) _Lparam=MAKELPARAM(_Stretch_width*(
				(float)LOWORD(_Lparam)/_Width),_Stretch_height*(
					(float)HIWORD(_Lparam)/_Height));
		
		_Received_message=true;
		break;
		}
	}
	
	for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
		_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
	
		//At least one have received the message
		if ((*_Iter)->message(_Window_handle,_Message,_Wparam,_Lparam))
			_Received_message=true;
	}

return (_Received_message?0:DefWindowProc(_Window_handle,_Message,
	_Wparam,_Lparam));
}

bool render_window::create_glwindow() {

_Instance_handle=GetModuleHandle(0);

WNDCLASS _Window_class;
_Window_class.style=CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
_Window_class.lpfnWndProc=(WNDPROC)static_windowproc;
_Window_class.cbClsExtra=0;
_Window_class.cbWndExtra=0;
_Window_class.hInstance=_Instance_handle;
_Window_class.hIcon= LoadIcon(0,IDI_WINLOGO);
_Window_class.hCursor=LoadCursor(0,IDC_ARROW);
_Window_class.hbrBackground=0;			
_Window_class.lpszMenuName=0;		
_Window_class.lpszClassName="OpenGL";

	if (!RegisterClass(&_Window_class)) return false;
	
	if (_Width<_Min_width) _Width=_Min_width;
	if (_Height<_Min_height) _Height=_Min_height;

RECT _Desktop;
GetWindowRect(GetDesktopWindow(),&_Desktop);

RECT _Window;
_Window.left=0;
_Window.top=0;
_Window.right=_Width;
_Window.bottom=_Height;

//Adjust for windowed mode
AdjustWindowRectEx(&_Window,windowed_style(),false,windowed_exstyle());

//Set default window left/top to desktop center
_Windowed_left=(_Desktop.right-_Desktop.left)/2-
	(_Window.right-_Window.left)/2-_Window.left;
_Windowed_top=(_Desktop.bottom-_Desktop.top)/2-
	(_Window.bottom-_Window.top)/2-_Window.top;
_Windowed_width=_Width;
_Windowed_height=_Height;

	if (_Fullscreen && ChangeDisplaySettings(&_Device_mode,
		CDS_FULLSCREEN!=DISP_CHANGE_SUCCESSFUL)) _Fullscreen=false;

DWORD _Style=(_Fullscreen?fullscreen_style():windowed_style());
DWORD _Exstyle=(_Fullscreen?fullscreen_exstyle():windowed_exstyle());

_Window.left=0;
_Window.top=0;
_Window.right=_Width;
_Window.bottom=_Height;

//Adjust for current mode
AdjustWindowRectEx(&_Window,_Style,false,_Exstyle);

_Left=(_Fullscreen?0:_Windowed_left);
_Top=(_Fullscreen?0:_Windowed_top);

	if (!(_Window_handle=CreateWindowEx(_Exstyle,"OpenGL",_Title.c_str(),
		_Style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,(_Fullscreen?_Left:
			_Left+_Window.left),(_Fullscreen?_Top:_Top+_Window.top),
				_Window.right-_Window.left,_Window.bottom-_Window.top,0,0,
					_Instance_handle,(LPVOID)this))) {
	
	close_glwindow();
	return false;
	}

PIXELFORMATDESCRIPTOR _Pfd;
_Pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
_Pfd.nVersion=1;
_Pfd.dwFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
_Pfd.iPixelType=PFD_TYPE_RGBA;
_Pfd.cColorBits=(BYTE)_Color_depth;
_Pfd.cRedBits=0;
_Pfd.cRedShift=0;
_Pfd.cGreenBits=0;
_Pfd.cGreenShift=0;
_Pfd.cBlueBits=0;
_Pfd.cBlueShift=0;
_Pfd.cAlphaBits=8; //Enable alpha in frame buffer
_Pfd.cAlphaShift=0;
_Pfd.cAccumBits=0;
_Pfd.cAccumRedBits=0;
_Pfd.cAccumGreenBits=0;
_Pfd.cAccumBlueBits=0;
_Pfd.cAccumAlphaBits=0;
_Pfd.cDepthBits=16;
_Pfd.cStencilBits=0;
_Pfd.cAuxBuffers=0;
_Pfd.iLayerType=PFD_MAIN_PLANE;
_Pfd.bReserved=0;
_Pfd.dwLayerMask=0;
_Pfd.dwVisibleMask=0;
_Pfd.dwDamageMask=0;

	if (!(_Device_context=GetDC(_Window_handle))) {
	
	close_glwindow();
	return false;
	}

int _Pixel_format;

	if (!(_Pixel_format=ChoosePixelFormat(_Device_context,&_Pfd))) {
	
	close_glwindow();
	return false;
	}
	
	if (!SetPixelFormat(_Device_context,_Pixel_format,&_Pfd)) {
	
	close_glwindow();
	return false;
	}
	
	if (!(_Rendering_context=wglCreateContext(_Device_context))) {
	
	close_glwindow();
	return false;
	}
	
	if (!wglMakeCurrent(_Device_context,_Rendering_context)) {
	
	close_glwindow();
	return false;
	}

resize_opengl(_Width,_Height);

ShowWindow(_Window_handle,SW_SHOW);
SetForegroundWindow(_Window_handle);
SetFocus(_Window_handle);
return true;
}

void render_window::close_glwindow() {

	if (_Fullscreen) ChangeDisplaySettings(0,0);
	
	if (_Rendering_context) {
	
	wglMakeCurrent(0,0);
	wglDeleteContext(_Rendering_context);
	_Rendering_context=0;
	}
	
	if (_Device_context) {
	
	ReleaseDC(_Window_handle,_Device_context);
	_Device_context=0;
	}
	
	if (_Window_handle) {
	
	DestroyWindow(_Window_handle);
	_Window_handle=0;
	}
	
	if (_Instance_handle) {
	
	UnregisterClass("OpenGL",_Instance_handle);
	_Instance_handle=0;
	}
}

void render_window::resize_opengl(unsigned int _Width,
	unsigned int _Height) {

glViewport(0,0,_Width,_Height);

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0,(_Stretch?_Stretch_width:_Width),0.0,(_Stretch?
	_Stretch_height:_Height),-1,1); //Normal coordinate system

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

this->_Width=_Width;
this->_Height=_Height;
}

bool render_window::init() {

	//Already initialized
	if (_Window_handle!=0) return true;

return create_glwindow();
}

//Public

render_window *render_window::instance() {
	return _Instance;
}

void render_window::move(unsigned int _Left,unsigned int _Top) {

	//No window created
	if (_Window_handle==0) return;
	
	//In fullscreen mode
	if (_Fullscreen) {
	
	_Windowed_left=_Left;
	_Windowed_top=_Top;
	return;
	}

RECT _Window;
GetWindowRect(_Window_handle,&_Window);

MoveWindow(_Window_handle,_Left,_Top,_Window.right-_Window.left,
	_Window.bottom-_Window.top,1);
}

void render_window::resize(unsigned int _Width,
	unsigned int _Height) {

	//No window created
	if (_Window_handle==0) {
	
	this->_Width=_Width;
	this->_Height=_Height;
	return;
	}
	
	//In fullscreen mode
	if (_Fullscreen) {
	
	_Windowed_width=_Width;
	_Windowed_height=_Height;
	return;
	}

RECT _Window,_Client;
GetWindowRect(_Window_handle,&_Window);
GetClientRect(_Window_handle,&_Client);

int _Xdifference=(_Window.right-_Window.left)-_Client.right;
int _Ydifference=(_Window.bottom-_Window.top)-_Client.bottom;

MoveWindow(_Window_handle,_Window.left,_Window.top,
	_Width+_Xdifference,_Height+_Ydifference,1);
resize_opengl(_Width,_Height);
}

void render_window::title(const std::string &_Title) {

this->_Title=_Title;

	//No window created
	if (_Window_handle==0) return;

SetWindowText(_Window_handle, _Title.c_str());
}

const std::string &render_window::title() const {
	return _Title;
}

unsigned int render_window::width() const {
	return _Width;
}

unsigned int render_window::height() const {
	return _Height;
}

unsigned int render_window::color_depth() const {
	return _Color_depth;
}

int render_window::left() const {
	return _Left;
}

int render_window::top() const {
	return _Top;
}

bool render_window::active() const {
	return _Active;
}

void render_window::fullscreen(bool _Fullscreen) {

	//Nothing to do
	if (this->_Fullscreen==_Fullscreen) return;
	
	//No window created
	if (_Window_handle==0) {
	
	this->_Fullscreen=_Fullscreen;
	return;
	}
	
	//If mode is locked or window is already in correct mode
	if (_Mode_lock || this->_Fullscreen==_Fullscreen) return;
	
	if (_Fullscreen) {
	
	_Windowed_left=_Left;
	_Windowed_top=_Top;
	_Windowed_width=_Width;
	_Windowed_height=_Height;
	
		if (ChangeDisplaySettings(&_Device_mode,CDS_FULLSCREEN)!=
			DISP_CHANGE_SUCCESSFUL) return;
	}
	else ChangeDisplaySettings(0,0);

DWORD _Style=(_Fullscreen?fullscreen_style():windowed_style());
DWORD _Exstyle=(_Fullscreen?fullscreen_exstyle():windowed_exstyle());

RECT _Window;
_Window.left=0;
_Window.top=0;
_Window.right=(_Fullscreen?_Device_mode.dmPelsWidth:_Windowed_width);
_Window.bottom=(_Fullscreen?_Device_mode.dmPelsHeight:_Windowed_height);
AdjustWindowRectEx(&_Window,_Style,false,_Exstyle);

SetWindowLong(_Window_handle,GWL_STYLE,_Style);
SetWindowLong(_Window_handle,GWL_EXSTYLE,_Exstyle);
SetWindowPos(_Window_handle,0,(_Fullscreen?0:_Windowed_left+_Window.left),
	(_Fullscreen?0:_Windowed_top+_Window.top),_Window.right-_Window.left,
		_Window.bottom-_Window.top,SWP_NOZORDER | SWP_FRAMECHANGED |
			SWP_SHOWWINDOW);

this->_Fullscreen=_Fullscreen;
}

bool render_window::fullscreen() const {
	return _Fullscreen;
}

void render_window::fullscreen_size(unsigned int _Width,
	unsigned int _Height) {

_Device_mode.dmPelsWidth=(_Width<_Min_width?_Min_width:_Width);
_Device_mode.dmPelsHeight=(_Height<_Min_height?_Min_height:_Height);
}

unsigned int render_window::fullscreen_width() const {
	return _Device_mode.dmPelsWidth;
}

unsigned int render_window::fullsceen_height() const {
	return _Device_mode.dmPelsHeight;
}

void render_window::stretch_size(float _Stretch_width,
	float _Stretch_height) {

	if (this->_Stretch_width!=_Stretch_width ||
		this->_Stretch_height!=_Stretch_height) {
	
	this->_Stretch_width=_Stretch_width;
	this->_Stretch_height=_Stretch_height;
	
		//Update screen visual
		if (_Window_handle && _Stretch) resize_opengl(_Width,_Height);
	}
}

float render_window::stretch_width() const {
	return _Stretch_width;
}

float render_window::stretch_height() const {
	return _Stretch_height;
}

void render_window::min_size(unsigned int _Min_width,
	unsigned int _Min_height) {

this->_Min_width=_Min_width;
this->_Min_height=_Min_height;
}

unsigned int render_window::min_width() const {
	return _Min_width;
}

unsigned int render_window::min_height() const {
	return _Min_height;
}

void render_window::resizable(bool _Resizable) {

	//Nothing to do
	if (this->_Resizable==_Resizable) return;

this->_Resizable=_Resizable;

	//No window created or in fullscreen mode
	if (_Window_handle==0 || _Fullscreen) return;

RECT _Client;
GetClientRect(_Window_handle,&_Client);
AdjustWindowRectEx(&_Client,windowed_style(),false,windowed_exstyle());

SetWindowLong(_Window_handle,GWL_STYLE,windowed_style());
SetWindowLong(_Window_handle,GWL_EXSTYLE,windowed_exstyle());
SetWindowPos(_Window_handle,0,0,0,_Client.right-_Client.left,
	_Client.bottom-_Client.top,SWP_NOMOVE | SWP_NOZORDER |
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

bool render_window::resizable() const {
	return _Resizable;
}

void render_window::stretch(bool _Stretch) {

	if (this->_Stretch!=_Stretch) {
	
	this->_Stretch=_Stretch;
	
		//Update screen visual
		if (_Window_handle) resize_opengl(_Width,_Height);
	}
}

bool render_window::stretch() const {
	return _Stretch;
}

void render_window::mouse_visible(bool _Visible) {

	if (_Mouse_visible!=_Visible) {
	
	ShowCursor((BOOL)_Visible);
	_Mouse_visible=_Visible;
	}
}

bool render_window::mouse_visible() const {
	return _Mouse_visible;
}

void render_window::mode_lock(bool _Mode_lock) {
	this->_Mode_lock=_Mode_lock;
}

bool render_window::mode_lock() const {
	return _Mode_lock;
}

float render_window::ortho_width() const {
	return (_Stretch?_Stretch_width:_Width);
}

float render_window::ortho_height() const {
	return (_Stretch?_Stretch_height:_Height);
}

void render_window::subscribe_messagelistener(message_listener *_Message_listener,
	bool _Secured) {
	subscribe(_Message_listener,_Secured);
}

void render_window::unsubscribe_messagelistener(message_listener *_Message_listener) {
	unsubscribe(_Message_listener);
}

void render_window::clear_messagelisteners() {
	clear_listeners();
}

render_window::messagelisteners_iter render_window::messagelistener_begin() const {
	return listeners_begin();
}

render_window::messagelisteners_iter render_window::messagelistener_end() const {
	return listeners_end();
}

} //namespace lunar