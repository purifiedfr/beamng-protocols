#include <cstdlib>
#include <iostream>
#include "console.h"
#include "../os_check.h"

#if BP_LINUX
	#include <signal.h>

    // https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c

	void exit_handler( int ) {
		console::enable_cursor( );

		// I'm sure it'd be better to call the previously-set signal handler,
		// however, since I set so many, it'd be kinda tedious to keep track of
		// all of them. I'll just do this for this example
		exit( EXIT_SUCCESS );
	}

    void console::enable_cursor( ) {
		std::cout << "\e[?25h\n";
    }

    void console::disable_cursor( ) {
        std::cout << "\e[?25l\n";
    }

    void console::register_enable_handler( ) {
		// To re-enable the cursor
		// I'm setting a lot of these because why not
		signal( SIGINT, &exit_handler );
		signal( SIGTERM, &exit_handler );
		signal( SIGKILL, &exit_handler );
		signal( SIGTSTP, &exit_handler );
		signal( SIGSTOP, &exit_handler );
		signal( SIGQUIT, &exit_handler );
    }


#elif BP_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
	
    // https://stackoverflow.com/questions/16110898/how-can-i-hide-the-mouse-cursor

	BOOL WINAPI console_handler( DWORD signal ) {
		if ( signal == CTRL_C_EVENT ) {
			enable_cursor( );
			exit( EXIT_SUCCESS );
		}

		return true;
	}

    void console::enable_cursor( ) {
        SetCursor( original_cursor );
    }

    void console::disable_cursor( ) {
		original_cursor = GetCursor( );
		SetCursor( nullptr );
    }

    void console::register_enable_handler( ) {
		SetConsoleCtrlHandler( &console_handler, true );
    }

#endif