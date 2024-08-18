#pragma once
#include <cstdint>

namespace beamng {

    struct vec3_t {
        float x, y, z;
    };

    // https://documentation.beamng.com/modding/protocols/

    class motion_sim_t {
        char magic[4]; // fixed value of "BNG1"

    public:
        vec3_t position;
        vec3_t velocity;
        vec3_t acceleration;

        vec3_t up_vector;

        vec3_t angle;
        vec3_t angle_velocity;
        vec3_t angle_acceleration;

        bool is_motion_sim( ) const;
    };

    class outgauge_t {
        uint32_t unused_0;

        char           magic[4]; // fixed value of "beam"
    public:
        uint16_t flags;           // Info (see OG_x below)
        char     gear;            // reverse: 0, neutral: 1, ...
                                // TODO: I'm not sure whether this is char because it's a character, or used as an integral type

    private:
        char unused_1;
    public:
        float speed;       // m/s
        float rpm;         // rpm
        float turbo;       // bar
        float engine_temp; // ˚C
        float fuel;        // 0 -> 1
    private:
        // float oilPressure;     // BAR // N/A, hardcoded to 0
        float unused_2;
    public:
        float oil_temp; // ˚C
        uint32_t dash_light_flags; // Dash lights available (see DL_x below)
        uint32_t dash_lights;
        float throttle; // 0 -> 1
        float brake;    // 0 -> 1
        float clutch;   // 0 -> 1
    private:
        char /* display1 */ unused_3[16];    // Usually Fuel // N/A, hardcoded to ""
        char /* display2 */ unused_4[16];    // Usually Settings // N/A, hardcoded to ""
    public:
        int id; // optional - only if OutGauge ID is specified

        bool is_outgauge( ) const;
        bool prefers_mph( ) const;
    };

    // constexpr uint16_t og_shift = 1 << 0;
    // constexpr uint16_t og_ctrl = 1 << 1;
    constexpr uint16_t og_turbo = 1 << 13;
    constexpr uint16_t og_km    = 1 << 14;
    constexpr uint16_t og_bar   = 1 << 15;

    // todo: implement these later, I don't feel like typing them out :)
    // local DL_SHIFT        = 2 ^ 0    -- shift light
    // local DL_FULLBEAM     = 2 ^ 1    -- full beam
    // local DL_HANDBRAKE    = 2 ^ 2    -- handbrake
    // local DL_PITSPEED     = 2 ^ 3    -- pit speed limiter // N/A
    // local DL_TC           = 2 ^ 4    -- tc active or switched off
    // local DL_SIGNAL_L     = 2 ^ 5    -- left turn signal
    // local DL_SIGNAL_R     = 2 ^ 6    -- right turn signal
    // local DL_SIGNAL_ANY   = 2 ^ 7    -- shared turn signal // N/A
    // local DL_OILWARN      = 2 ^ 8    -- oil pressure warning
    // local DL_BATTERY      = 2 ^ 9    -- battery warning
    // local DL_ABS          = 2 ^ 10   -- abs active or switched off
    // local DL_SPARE        = 2 ^ 11   -- N/A


    union buffer_t {
        outgauge_t outgauge;
        motion_sim_t motion_sim;
    };

}