/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: system.cpp
---------------------------------------------------
*/

#include "system.h"

//Dependencies
#include "render_window.h"
#include "string_utility.h"

#if defined(_SYS_WIN_)

//Win dependencies
#include <shellapi.h>

#endif

namespace lunar {

//Public

//File
//Public

system::file::file():_Size(0),_Read_only(false),_Hidden(false) {
	//Empty
}

system::file::~file() {
	//Empty
}

const std::string &system::file::name() const {
	return _Name;
}

unsigned long long system::file::size() const {
	return _Size;
}

bool system::file::read_only() const {
	return _Read_only;
}

bool system::file::hidden() const {
	return _Hidden;
}

//Battery
//Public

system::battery::battery():_Percent(0.0f),_Lifetime(0.0f),
	_Full_lifetime(0.0f),_Charging(false) {
	//Empty
}

system::battery::~battery() {
	//Empty
}

float system::battery::percent() const {
	return _Percent;
}

float system::battery::lifetime() const {
	return _Lifetime;
}

float system::battery::full_lifetime() const {
	return _Full_lifetime;
}

bool system::battery::charging() const {
	return _Charging;
}

//System
//Private

#if defined(_SYS_WIN_)

DWORD_PTR system::_Thread_mask=0;

//Swap control extensions

system::PFNWGLEXTEXTENSIONSTRINGPROC system::wglGetExtensionsStringEXT=0;
system::PFNWGLEXTSWAPCONTROLPROC system::wglSwapIntervalEXT=0;
system::PFNWGLEXTGETSWAPINTERVALPROC system::wglGetSwapIntervalEXT=0;

#endif

system::system() {

#if defined(_SYS_WIN_)

DWORD_PTR _Proc_mask;
DWORD_PTR _Sys_mask;
GetProcessAffinityMask(GetCurrentProcess(),&_Proc_mask,&_Sys_mask);

	if (_Proc_mask==0) _Proc_mask=1;

_Thread_mask=1;

	//Find the lowest core that this process uses
	while ((_Thread_mask & _Proc_mask)==0) _Thread_mask<<=1;

#endif

}

system::~system() {
	//Empty
}

void system::init_extensions() {

//Get graphics card extensions
char *_Extensions=(char*)glGetString(GL_EXTENSIONS);

	//No extensions found
	if (!_Extensions) return;

#if defined(_SYS_WIN_)

//Get addresses of function
wglGetExtensionsStringEXT=(PFNWGLEXTEXTENSIONSTRINGPROC)
	wglGetProcAddress("wglGetExtensionsStringEXT");
const char *_Win_extensions=0;

	if (wglGetExtensionsStringEXT)
		_Win_extensions=wglGetExtensionsStringEXT();

	//Initialize swap control extensions
	if (!wglSwapIntervalEXT && !wglGetSwapIntervalEXT) {
	
		//WGL_EXT_swap_control supported
		if (strstr(_Extensions,"WGL_EXT_swap_control") ||
			(_Win_extensions && strstr(_Win_extensions, "WGL_EXT_swap_control"))) {
		
		//Get addresses of both functions
		wglSwapIntervalEXT=(PFNWGLEXTSWAPCONTROLPROC)
			wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT=(PFNWGLEXTGETSWAPINTERVALPROC)
			wglGetProcAddress("wglGetSwapIntervalEXT");
		}
	}

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

}

void system::swap_control(bool _Swap_control) {

#if defined(_SYS_WIN_)

	if (wglSwapIntervalEXT) wglSwapIntervalEXT((int)_Swap_control);

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

}

bool system::swap_control() {

//If swap control is not supported, return true to force sync
bool _Swap_control=true;

#if defined(_SYS_WIN_)

	if (wglGetSwapIntervalEXT) _Swap_control=(wglGetSwapIntervalEXT()!=0);

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

return _Swap_control;
}

//Public

unsigned int system::ticks() {

unsigned int _Ticks=0;

#if defined(_SYS_WIN_)

_Ticks=GetTickCount();

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

return _Ticks;
}

long long system::cpu_ticks() {

long long _Ticks=0;

#if defined(_SYS_WIN_)

//Set affinity to the first core
HANDLE _Thread=GetCurrentThread();
DWORD_PTR _Mask=SetThreadAffinityMask(_Thread,_Thread_mask);

LARGE_INTEGER _Large_int;

	if (QueryPerformanceCounter(&_Large_int))
		_Ticks=_Large_int.QuadPart;

SetThreadAffinityMask(_Thread,_Mask); //Reset affinity

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

return _Ticks;
}

long long system::cpu_frequency() {

long long _Frequency=0;

#if defined(_SYS_WIN_)

//Set affinity to the first core
HANDLE _Thread=GetCurrentThread();
DWORD_PTR _Mask=SetThreadAffinityMask(_Thread,_Thread_mask);

LARGE_INTEGER _Large_int;

	if (QueryPerformanceFrequency(&_Large_int))
		_Frequency=_Large_int.QuadPart;

SetThreadAffinityMask(_Thread,_Mask); //Reset affinity

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

return _Frequency;
}

void system::execute(const std::string &_Path) {

#if defined(_SYS_WIN_)

ShellExecute(render_window::instance()->_Window_handle,
	"open",_Path.c_str(),0,0,SW_SHOWNORMAL);

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

}

void system::clipboard(const std::string &_Data) {

#if defined(_SYS_WIN_)

	//Could not open clipboard
	if (OpenClipboard(render_window::instance()->_Window_handle)==0) return;

EmptyClipboard();

HGLOBAL _Clipbuffer=GlobalAlloc(GMEM_DDESHARE,_Data.size()+1);
char *_Buffer=(char*)GlobalLock(_Clipbuffer);

	//Copy to clipboard
	if (_Buffer) {
	
	strcpy(_Buffer,_Data.c_str());
	SetClipboardData(CF_TEXT,_Clipbuffer); //Clipboard contains text
	}

GlobalUnlock(_Clipbuffer);
CloseClipboard();

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

}

std::string system::clipboard() {

std::string _Data;

#if defined(_SYS_WIN_)

	//Could not open clipboard
	if (OpenClipboard(render_window::instance()->_Window_handle)==0)
		return _Data;

HANDLE _Clipbuffer=GetClipboardData(CF_TEXT);
char *_Buffer=(char*)GlobalLock(_Clipbuffer);

	//Copy from clipboard
	if (_Buffer) _Data=_Buffer;

GlobalUnlock(_Clipbuffer);
CloseClipboard();

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

return _Data;
}

void system::files(std::string _Directory,file_list &_Files) {

#if defined(_SYS_WIN_)

string_utility::format_path(_Directory,"\\"); //Win path
_Directory.push_back('*'); //Find all file types

WIN32_FIND_DATA _Data;
HANDLE _Handle=FindFirstFile(_Directory.c_str(),&_Data);

	if (_Handle!=INVALID_HANDLE_VALUE) {
	
		do {
		
			if ((_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0) {
			
			file _File;
			_File._Name=_Data.cFileName;
			_File._Size=((unsigned long long)_Data.nFileSizeHigh<<32)+
				(unsigned long long)_Data.nFileSizeLow;
			_File._Read_only=(_Data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)!=0;
			_File._Hidden=(_Data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0;
			_Files.push_back(_File);
			}
		
		} while (FindNextFile(_Handle,&_Data));
	
	FindClose(_Handle);
	}

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

}

bool system::battery_status(battery &_Battery) {

#if defined(_SYS_WIN_)

SYSTEM_POWER_STATUS _System_power;

	//Power status available
	if (GetSystemPowerStatus(&_System_power)!=0) {
	
		//Battery status available
		if (_System_power.ACLineStatus==0) {
		
			//Battery percentage is known
			if (_System_power.BatteryLifePercent!=255) {
			
			_Battery._Percent=(float)_System_power.BatteryLifePercent/100;
			_Battery._Lifetime=(float)_System_power.BatteryLifeTime; //Seconds
			_Battery._Full_lifetime=(float)_System_power.BatteryFullLifeTime; //Seconds
			}
		
		_Battery._Charging=(_System_power.BatteryFlag==8); //Charging
		return true;
		}
	}

#elif defined(_SYS_UNIX_)

//Unix code

#elif defined(_SYS_MAC_)

//Mac code

#endif

return false;
}

} //namespace lunar