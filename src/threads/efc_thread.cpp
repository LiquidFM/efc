#include "efc_thread.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "posix/efc_thread_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/efc_thread_p.h"
#endif
