/*
<Project name>

Written by <Name>
game.h
*/

#ifndef _GAME_
#define _GAME_

#include <vector>

//Dependencies (engine)
#include "core.h"

using namespace lunar;

class game final :
	public frame_listener, public message_listener,
	public key_listener, public mouse_listener
{
	protected:

		core engine;

		bool update(float time) override;
		bool post_update(float time) override;
		
		bool message(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) override;
		
		void key_pressed(unsigned int key) override;
		void key_released(unsigned int key) override;
		void char_pressed(char character) override;
		
		void mouse_pressed(unsigned int button, unsigned int x, unsigned int y) override;
		void mouse_released(unsigned int button, unsigned int x, unsigned int y) override;
		void mouse_moved(unsigned int x, unsigned int y) override;
		void mouse_scrolled(int delta, unsigned int x, unsigned int y) override;

		void load_settings(std::vector<unsigned int> &settings) const;

	public:

		//Constructor
		game() noexcept;
		
		//Destructor
		~game();
		
		int start();
};

#endif