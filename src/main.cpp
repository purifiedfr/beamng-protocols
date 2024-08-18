#include "os_check.h"

#if BP_LINUX
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
	#include <netdb.h>
#elif BP_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <iphlpapi.h>
	#pragma comment(lib, "ws2_32.lib")
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <string>

#include <span>
#include <string_view>
#include <cmath>

#include <bit>

#include "beamng/data.h"
#include "console/console.h"
#include "format/format.h"
#include "conversion/conversion.h"

// your code here
void handle_motion_sim( beamng::motion_sim_t m ) {
    // ...
    // you could also print stuff here,
    // but I'm currently not doing anything for
    // simplicity
}

void handle_outgauge( beamng::outgauge_t g ) {
	const format::unit_speed_t speed = format::format_outgauge_speed( g );

    std::cout << '\r' << speed.to_string( ) << " @ " << std::setprecision( 0 ) << std::fixed << g.rpm << "RPM" << " (" << format::format_gear( g.gear ) << ')'
    //      this vvvv is a hacky solution to make sure that remaining characters get erased (fixme?)
        << "            ";
}

// description: figure out what port to bind to
// param: args - the argument vector
// returns: the port to use
int get_port( std::span< char * > args ) {
    constexpr uint16_t default_port = 4444;
    if ( args.size( ) < 2 ) {
        return default_port;
    }

    if ( args.size( ) > 2 ) {
        std::cout << "(i): you seem to have supplied more than one argument\n -> usage: " << args[ 0 ] << " <[port]>\n\n";
    }

    const uint16_t port = std::stoul( args[ 1 ] );
    if ( port == 0 ) { // invalid number
        std::cout << "(i): you seem to have entered an invalid port\n -> falling back to " << default_port << "\n\n";
        return default_port;
    }

    return port;
}

// TODO: do proper cleanup of sockets, currently it doesn't matter since exiting cleans everything up anyway
int main( int argc, char **argv ) {

	console::cursor_hider_t hider = { };

	#if BP_WINDOWS
	  	{
			// Initialize WinSock   
			WSADATA wsa_data = { };
			if ( const int32_t wsa_result = WSAStartup( MAKEWORD( 2, 2 ), &wsa_data ); wsa_result != 0 ) {
				std::cout << "(!) Failed to initialize WinSock\n -> WSAStartup( ) = " << wsa_result << '\n';
				return EXIT_FAILURE;
			}
		}
	#endif

    // Get the requested port
    const uint16_t port = get_port( std::span( &argv[ 0 ], argc ) );

    // Set up the socket
    constexpr int32_t addr_family = AF_INET;
    constexpr int32_t sock_type = SOCK_DGRAM;

	#if BP_LINUX
		// Bind to the port
		const sockaddr_in local_addr = {
			.sin_family = addr_family,
			.sin_port = htons( port ),
			.sin_addr = {
				.s_addr = INADDR_ANY,
			},
		};

    	// $ man 2 socket
		//  -> "On error, -1 is returned, ..."
		constexpr int32_t socket_fail = -1;
		const int32_t socket_fd = socket( addr_family, sock_type, 0 );
		if ( socket_fd == socket_fail ) {
			std::cout << "(!) Failed to create a socket\n -> errno = " << errno << '\n';
			return EXIT_FAILURE;
		}

	#elif BP_WINDOWS
	 	const addrinfo hints = {
			.ai_flags = AI_PASSIVE,
			.ai_family = addr_family,
			.ai_socktype = sock_type,
			.ai_protocol = IPPROTO_UDP,
		};
		addrinfo *result = { };

		if ( getaddrinfo( nullptr, std::to_string( port ).c_str( ), &hints, &result ) != 0 ) {
			std::cout << "(!) Failed to get address info\n -> WSAGetLastError( ) = " << WSAGetLastError( ) << '\n';
			return EXIT_FAILURE;
		}

		addrinfo *current = result;
		uintptr_t socket_fd = { };

		while ( true ) {
			socket_fd = socket( current->ai_family, current->ai_socktype, current->ai_protocol );

			if ( socket_fd == INVALID_SOCKET ) {
				current = current->ai_next;
			}

			if ( !current ) {
				std::cout << "(!) Failed to create a socket\n -> WSAGetLastError( ) = " << WSAGetLastError( ) << '\n';
				return EXIT_FAILURE;
			}

			break; // at this point, we have a valid socket
		}
	#endif

	#if BP_LINUX
		// $ man 2 bind
		//  -> "On success, zero is returned."
		constexpr int32_t bind_ok = 0;
		const int32_t bind_result = bind( socket_fd, std::bit_cast< const sockaddr * >( &local_addr ), sizeof( local_addr ) );

		if ( bind_result != bind_ok ) {
			std::cout << "(!): Failed to bind( )\n -> Are you perhaps running another instance of this program?\n -> errno = " << errno << '\n';
			return EXIT_FAILURE;
		}
	#elif BP_WINDOWS
		if ( bind( socket_fd, current->ai_addr, static_cast< int >( current->ai_addrlen ) ) == SOCKET_ERROR ) {
			std::cout << "(!): Failed to bind( )\n -> Are you perhaps running another instance of this program?\n -> WSAGetLastError( ) = " << WSAGetLastError( ) << '\n';
			return EXIT_FAILURE;
		}

		freeaddrinfo( result );
	#endif

    //                     this vvvvvv                 could practically be: std::max( sizeof( outgauge_t ), sizeof( motionsim_t ) )
    constexpr size_t max_size = sizeof( beamng::buffer_t ); // why isn't it? because this looks cleaner
    constexpr size_t min_size = std::min( sizeof( beamng::outgauge_t ), sizeof( beamng::motion_sim_t ) );
    // Add 1 byte to check for above-maximum packet sizes
    constexpr size_t buffer_size = max_size + 1;

	// WinSock uses char instead of uint8_t
	#if BP_LINUX
		uint8_t buffer[ buffer_size ] = { };
	#elif BP_WINDOWS
		char buffer[ buffer_size ] = {};
	#endif

    while ( true ) {

        // $ man 2 recv
        //   -> These calls return the number of bytes received, or -1 if an error occurred.
        constexpr int32_t recv_fail = -1;

        int32_t packet_size = recv( socket_fd, buffer, buffer_size, 0 );
        if ( packet_size == recv_fail ) {
            return EXIT_FAILURE;
        }
        if ( packet_size < min_size ) {
            return EXIT_FAILURE;
        }

        if ( packet_size > max_size ) {
            return EXIT_FAILURE;
        }

        if ( packet_size < std::min( sizeof( beamng::motion_sim_t ), sizeof( beamng::outgauge_t ) ) ) {
            return EXIT_FAILURE;
        }

        auto packet_buffer = std::bit_cast< beamng::buffer_t * >( &buffer[ 0 ] );
        if ( packet_buffer->outgauge.is_outgauge( ) ) {
            handle_outgauge( packet_buffer->outgauge );
        }

        if ( packet_buffer->motion_sim.is_motion_sim( ) ) {
            handle_motion_sim( packet_buffer->motion_sim );
        }

    }

    return EXIT_SUCCESS;

}