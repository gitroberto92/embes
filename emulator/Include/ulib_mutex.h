/***************************************************************************
 *                                                                         *
 *   LinuxSampler - modular, streaming capable sampler                     *
 *                                                                         *
 *   Copyright (C) 2003, 2004 by Benno Senoner and Christian Schoenebeck   *
 *   Copyright (C) 2005, 2006 Christian Schoenebeck                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 ***************************************************************************/

#ifndef __MUTEX_H__
#define __MUTEX_H__

#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif
#include "ulib_isync.h"

namespace ULIB {

/** @brief Mutual exclusive objects
 *
 * This class provides the classical thread / process synchronisation
 * technique called Mutex. It is used to protect critical sections, that is
 * resources (typically data structures) from being used at the same time by
 * different threads or processes which otherwise might turn into undefined
 * and of course undesired behavior.
 *
 * Note: as this technique might block the calling thread and also implies
 * a system call, this should not be used directly in realtime sensitive
 * threads!
 */
class Mutex : public ISync {
    public:
		Mutex()
			: isOpend(false)
		{
		}

		virtual ~Mutex()
		{
			Close();
		}

		bool Create(LPCSTR lpName = NULL)
		{
			Close();
			#if defined(WIN32)
			hMutex = CreateMutexA(NULL, FALSE, lpName);
			if (hMutex == NULL) {
				OutputDebugStringA("Mutex Create: Fatal error - CreateMutex error \n");
				return false;
			}
			#elif ANDROID
			pthread_mutex_init(&__posix_mutex, NULL);
			#else
			pthread_mutexattr_init(&__posix_mutexattr);

			// the following function call only works on UNIX98 compatible systems
			#if (_XOPEN_SOURCE > 500) || defined(__APPLE__)
			// Mac OS X Note: 10.4 (and later) does support PTHREAD_MUTEX_ERRORCHECK, and
			// actually LinuxSampler does not work if this call is omitted. However,
			// defining _XOPEN_SOURCE macro seems to cause other problems. As a workaround,
			// the symbol __APPLE__ is checked here. There should be a better solution
			// to this problem. (Toshi Nagata, 27 Mar 2007)
			if (pthread_mutexattr_settype(&__posix_mutexattr, PTHREAD_MUTEX_ERRORCHECK)) {
				TRACEX("Mutex Create: Fatal error - unable to pthread_mutexattr_settype(PTHREAD_MUTEX_ERRORCHECK)\n";
				return false;
			}
			#endif
			pthread_mutex_init(&__posix_mutex, &__posix_mutexattr);
			#endif

			isOpend = true;
			return true;
		}

		bool Open(LPCSTR lpName = NULL)
		{
			Close();
			#if defined(WIN32)
			hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, lpName);
			if (hMutex == NULL) {
				return false;
			}
			#elif ANDROID
			pthread_mutex_init(&__posix_mutex, NULL);
			#else
			pthread_mutexattr_init(&__posix_mutexattr);

			// the following function call only works on UNIX98 compatible systems
			#if (_XOPEN_SOURCE > 500) || defined(__APPLE__)
			// Mac OS X Note: 10.4 (and later) does support PTHREAD_MUTEX_ERRORCHECK, and
			// actually LinuxSampler does not work if this call is omitted. However,
			// defining _XOPEN_SOURCE macro seems to cause other problems. As a workaround,
			// the symbol __APPLE__ is checked here. There should be a better solution
			// to this problem. (Toshi Nagata, 27 Mar 2007)
			if (pthread_mutexattr_settype(&__posix_mutexattr, PTHREAD_MUTEX_ERRORCHECK)) {
				TRACEX("Mutex Open: Fatal error - unable to pthread_mutexattr_settype(PTHREAD_MUTEX_ERRORCHECK)\n";
				return false;
			}
			#endif
			pthread_mutex_init(&__posix_mutex, &__posix_mutexattr);
			#endif

			isOpend = true;
			return true;
		}

		bool Close()
		{
			if (!isOpend)
				return true;

			#if defined(WIN32)
			CloseHandle(hMutex);
			hMutex = NULL;
			#elif ANDROID
			pthread_mutex_destroy(&__posix_mutex);
			#else
			pthread_mutex_destroy(&__posix_mutex);
			pthread_mutexattr_destroy(&__posix_mutexattr);
			#endif

			isOpend = false;
			return true;
		}

        /** @brief Lock this Mutex.
         *
         * If this Mutex object is currently be locked by another thread,
         * then the calling thread will be blocked until the other thread
         * unlocks this Mutex object. The calling thread though can safely
         * call this method several times without danger to be blocked
         * himself.
         *
         * The calling thread should call Unlock() as soon as the critical
         * section was left.
         */      
		virtual void Lock()
		{
			if (!isOpend) return;

			#if defined(WIN32)
				WaitForSingleObject(hMutex, INFINITE);
			#else
				pthread_mutex_lock(&__posix_mutex);
			#endif
		}

        /** @brief Try to lock this Mutex.
         *
         * Same as Lock() except that this method won't block the calling
         * thread in case this Mutex object is currently locked by another
         * thread. So this call will always immediately return and the
         * return value has to be checked if the locking request was
         * successful or not.
         *
         * @returns  true if the Mutex object could be locked, false if the
         *           Mutex is currently locked by another thread
         */
        bool TryLock()
        {
			if (!isOpend) return false;

			#if defined(WIN32)
				if( WaitForSingleObject(hMutex, 0) == WAIT_TIMEOUT) return false;
				return true;
			#else
				if (pthread_mutex_trylock(&__posix_mutex) == EBUSY)
					return false;
				return true;
			#endif
		}

        /** @brief Unlock this Mutex.
         *
         * If other threads are currently blocked and waiting due to a
         * Lock() call, one of them will be awaken.
         */
        virtual void UnLock()
        {
			if (!isOpend) return;

			#if defined(WIN32)
				ReleaseMutex(hMutex);
			#else
				pthread_mutex_unlock(&__posix_mutex);
			#endif
		}
        
    protected:
    	#if defined(WIN32)
        	HANDLE hMutex;
    	#elif ANDROID
        	pthread_mutex_t     __posix_mutex;
		#else
        	pthread_mutex_t     __posix_mutex;
        	pthread_mutexattr_t __posix_mutexattr;
    	#endif

		bool isOpend;
};

} // namespace sync

#endif // __MUTEX_H__
