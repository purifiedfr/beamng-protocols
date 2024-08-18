#pragma once
#if BP_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

namespace console {

    #if BP_WINDOWS
        static HCURSOR original_cursor = { };
    #endif

    void enable_cursor( );
    void disable_cursor( );

    void register_enable_handler( );

    class cursor_hider_t {
        bool m_run_callback = false;
        inline static bool s_registered_handler = false;
    public:
        cursor_hider_t( ) {
            this->m_run_callback = true;

            disable_cursor( );
            if ( !s_registered_handler ) {
                register_enable_handler( );
                s_registered_handler = true;
            }
        }

        cursor_hider_t( const cursor_hider_t & ) = delete;
        cursor_hider_t( cursor_hider_t &&other ) {
            other.m_run_callback = false;
            this->m_run_callback = true;
        }

        template < typename t >
        t operator=( t ) = delete;
    
        ~cursor_hider_t( ) {
            disable_cursor( );
        }
    };
}