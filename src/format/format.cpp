#include <sstream>
#include <iomanip>
#include "../conversion/conversion.h"
#include "format.h"

std::string format::format_gear( uint8_t gear ) {
    if ( gear == 0 ) {
        return "R";
    }

    if ( gear == 1 ) {
        return "N";
    }

    return std::to_string( gear - 1 );
}

std::string format::unit_speed_t::to_string( uint32_t float_precision ) const {
    std::ostringstream value_string;
    value_string.precision( float_precision );
    value_string << std::fixed << this->value;
    return value_string.str( ) + " " + this->unit_name.data( );
}

auto format::format_outgauge_speed( beamng::outgauge_t outgauge ) -> unit_speed_t {
    std::string_view unit_name = "km/h";
    float value = conversion::mps_to_kmph( outgauge.speed );

    if ( outgauge.prefers_mph( ) ) {
        unit_name = "m/h";
        value = conversion::kmph_to_mph( value );
    }

    return unit_speed_t{
        .unit_name = unit_name,
        .value = value,
    };
}