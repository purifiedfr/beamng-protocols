#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstring>

#include <span>
#include <string_view>
#include <cmath>

#include <bit>

#include "beamng/data.h"


// your code here
void handle_motion_sim( motion_sim_t m ) {
    // ...
    // you could also print stuff here,
    // but I'm currently not doing anything for
    // simplicity
}

// description: formats a gear into a string format
// param: gear - the gear value from OutGauge
// returns: a string (R, N, 1, 2, 3, ...)
std::string format_gear( uint8_t gear ) {
    if ( gear == 0 ) {
        return "R";
    }

    if ( gear == 1 ) {
        return "N";
    }

    return std::to_string( gear - 1 );
}

void handle_outgauge( outgauge_t g ) {
    // in km
    float speed = ( g.speed * 60.f * 60.f ) / 1000.f;
    const char *speed_unit = "km/h";

    constexpr float mile_multiplier = 1.60934f; 

    if ( !( g.flags & og_km ) ) {
        speed *= mile_multiplier;
        speed_unit = "m/h";
    }

    std::cout << '\r' << std::fixed << std::setprecision( 0 ) << speed << ' ' << speed_unit << " @ " << g.rpm << "RPM" << " (" << format_gear( g.gear ) << ')'
    //      this vvvv is a hacky solution to make sure that remaining characters get erased
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

int main( int argc, char **argv ) {

    // Get the requested port
    const uint16_t port = get_port( std::span( &argv[ 0 ], argc ) );

    // Set up the socket
    constexpr int32_t addr_family = AF_INET;
    constexpr int32_t sock_type = SOCK_DGRAM;

    // $ man 2 socket
    //  -> "On error, -1 is returned, ..."
    constexpr int32_t socket_fail = -1;
    const int32_t socket_fd = socket( addr_family, sock_type, 0 );
    if ( socket_fd == socket_fail ) {
        std::cout << "(!) Failed to create a socket\n -> errno = " << errno << '\n';
        return EXIT_FAILURE;
    }

    // Bind to the port
    const sockaddr_in local_addr = {
        .sin_family = addr_family,
        .sin_port = htons( port ),
        .sin_addr = {
            .s_addr = INADDR_ANY,
        },
    };

    // $ man 2 bind
    //  -> "On success, zero is returned."
    constexpr int32_t bind_ok = 0;
    const int32_t bind_result = bind( socket_fd, std::bit_cast< const sockaddr * >( &local_addr ), sizeof( local_addr ) );

    if ( bind_result != bind_ok ) {
        std::cout << "(!): Failed to bind( )\n -> Are you perhaps running another instance of this program?\n -> errno = " << errno << '\n';
        return EXIT_FAILURE;
    }

    //                     this vvvvvv                 could practically be: std::max( sizeof( outgauge_t ), sizeof( motionsim_t ) )
    constexpr size_t max_size = sizeof( buffer_t ); // why isn't it? because this looks cleaner
    constexpr size_t min_size = std::min( sizeof( outgauge_t ), sizeof( motion_sim_t ) );
    // Add 1 byte to check for above-maximum packet sizes
    constexpr size_t buffer_size = max_size + 1;

    while ( true ) {
        uint8_t buffer[ buffer_size ] = { };

        sockaddr from_addr = { };
        uint32_t from_addr_size = { };

        // $ man 2 recv
        //   -> These calls return the number of bytes received, or -1 if an error occurred.
        constexpr int32_t recv_fail = -1;
        int32_t packet_size = recvfrom( socket_fd, buffer, sizeof( buffer ), 0, &from_addr, &from_addr_size );
        if ( packet_size == recv_fail ) {
            return EXIT_FAILURE;
        }
        if ( packet_size < min_size ) {
            return EXIT_FAILURE;
        }

        if ( packet_size > max_size ) {
            return EXIT_FAILURE;
        }

        if ( packet_size < std::min( sizeof( motion_sim_t ), sizeof( outgauge_t ) ) ) {
            return EXIT_FAILURE;
        }

        auto packet_buffer = std::bit_cast< buffer_t * >( &buffer[ 0 ] );
        if ( packet_buffer->outgauge.is_outgauge( ) ) {
            handle_outgauge( packet_buffer->outgauge );
        }

        if ( packet_buffer->motion_sim.is_motion_sim( ) ) {
            handle_motion_sim( packet_buffer->motion_sim );
        }

    }

    return EXIT_SUCCESS;


}