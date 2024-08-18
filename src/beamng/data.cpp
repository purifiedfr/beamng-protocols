#include "data.h"

using namespace beamng;

bool motion_sim_t::is_motion_sim( ) const {
    return  this->magic[ 0 ] == 'B' &&
            this->magic[ 1 ] == 'N' &&
            this->magic[ 2 ] == 'G' &&
            this->magic[ 3 ] == '1';
}

bool outgauge_t::is_outgauge( ) const {
    return  this->magic[ 0 ] == 'b' &&
            this->magic[ 1 ] == 'e' &&
            this->magic[ 2 ] == 'a' &&
            this->magic[ 3 ] == 'm';
}

bool outgauge_t::prefers_mph( ) const {
    return !( this->flags & og_km );
}