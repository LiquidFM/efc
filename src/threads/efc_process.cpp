#include "efc_process.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "posix/efc_process_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/efc_process_p.h"
#endif
