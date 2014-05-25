#include "efc_waitcondition.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "posix/efc_waitcondition_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/efc_waitcondition_p.h"
#endif
