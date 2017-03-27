/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: system.h
---------------------------------------------------
*/

#ifndef _SYSTEM_
#define _SYSTEM_

#include <string>
#include <list>

#define _SYS_WIN_
//#define _SYS_UNIX_
//#define _SYS_MAC_

#if defined(_SYS_WIN_)

//Windows headers
#include <windows.h>
#include <gl/glew.h>

//This value must be defined to handle the WM_MOUSEHWHEEL on 
//Windows 2000 and Windows XP
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#elif defined(_SYS_UNIX_)

//Unix/linux headers

#elif defined(_SYS_MAC_)

//Mac headers

#endif

namespace lunar {

//Forward declarations
class core;

class system {

	friend core;

public:

	struct file {
	
		friend system;
	
	private:
	
		std::string _Name;
		unsigned long long _Size;
		
		bool _Read_only;
		bool _Hidden;
	
	public:
	
		//Constructor
		file();
		
		//Destructor
		~file();
		
		const std::string &name() const;
		unsigned long long size() const;
		
		bool read_only() const;
		bool hidden() const;
	};
	
	struct battery {
	
		friend system;
	
	private:
	
		float _Percent;
		float _Lifetime;
		float _Full_lifetime;
		
		bool _Charging;
	
	public:
	
		//Constructor
		battery();
		
		//Destructor
		~battery();
		
		float percent() const;
		float lifetime() const;
		float full_lifetime() const;
		
		bool charging() const;
	};

private:

	#if defined(_SYS_WIN_)
	
	static DWORD_PTR _Thread_mask;
	
	//Swap control extensions
	
	typedef const char *(APIENTRY *PFNWGLEXTEXTENSIONSTRINGPROC)();
	typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC)(int);
	typedef int (*PFNWGLEXTGETSWAPINTERVALPROC)();
	
	static PFNWGLEXTEXTENSIONSTRINGPROC wglGetExtensionsStringEXT;
	static PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT;
	static PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT;
	
	#endif
	
	//Private constructor
	system();
	
	//Private destructor
	~system();
	
	static void init_extensions();
	
	static void swap_control(bool _Swap_control);
	static bool swap_control();

public:

	typedef std::list<file> file_list;
	
	//Returns number of milliseconds since system boot
	static unsigned int ticks();
	
	//Returns number of cpu ticks since system boot
	static long long cpu_ticks();
	
	//Returns the current cpu frequency in hertz
	static long long cpu_frequency();
	
	static void execute(const std::string &_Path);
	
	static void clipboard(const std::string &_Data);
	static std::string clipboard();
	
	static void files(std::string _Directory,file_list &_Files);
	
	static bool battery_status(battery &_Battery);
};

} //namespace lunar

#endif