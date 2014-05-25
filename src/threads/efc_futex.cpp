#include "efc_futex.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "posix/efc_futex_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/efc_futex_p.h"
#endif


namespace EFC {

Futex::Locker::Locker(Futex &futex) :
    m_locked(true),
    m_futex(futex)
{
    m_futex.lock();
}

Futex::Locker::~Locker()
{
    if (m_locked)
        m_futex.unlock();
}

void Futex::Locker::lock()
{
    m_futex.lock();
    m_locked = true;
}

void Futex::Locker::unlock()
{
    m_futex.unlock();
    m_locked = false;
}

Futex::Unlocker::Unlocker(Locker &lock) :
    m_locked(false),
    m_lock(lock)
{
    m_lock.unlock();
}

Futex::Unlocker::~Unlocker()
{
    if (!m_locked)
        m_lock.lock();
}

void Futex::Unlocker::lock()
{
    m_lock.lock();
    m_locked = true;
}

void Futex::Unlocker::unlock()
{
    m_lock.unlock();
    m_locked = false;
}

}
