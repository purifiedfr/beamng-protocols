#if defined( __linux__ )
	#define BP_LINUX 1
	#define BP_WINDOWS 0
#elif defined( _WIN32 ) || defined( _WIN64 )
	#define BP_LINUX 0
	#define BP_WINDOWS 1
#else
	#error Unsupported operating system
#endif
