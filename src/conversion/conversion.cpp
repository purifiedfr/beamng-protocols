#include "conversion.h"

float conversion::mps_to_kmph( float mps ) {
    const float mpm = mps * 60.f;
    const float mph = mpm * 60.f;

    const float kmph = mph / 1000.f;

    return kmph;
}

float conversion::kmph_to_mph( float kmph ) {
    constexpr float mile_in_km = 1.60934f;

    return kmph / mile_in_km;
}