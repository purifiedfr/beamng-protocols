#pragma once
#include <string>
#include <string_view>
#include "../beamng/data.h"

namespace format {

    struct unit_speed_t {
        std::string_view unit_name;
        float value;

        std::string to_string( uint32_t float_precision = 0 ) const;
    };

    std::string format_gear( uint8_t gear );
    unit_speed_t format_outgauge_speed( beamng::outgauge_t outgauge );
}
