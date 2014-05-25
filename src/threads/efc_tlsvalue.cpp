#include "efc_tlsvalue.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "posix/efc_tlsvalue_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/efc_tlsvalue_p.h"
#endif
