/*
 * libjingle
 * Copyright 2004--2008, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ULIB_EVENT_H__
#define __ULIB_EVENT_H__

#if defined(WIN32)
#include <windows.h>  // NOLINT: consider this a system header.
#else
#include <pthread.h>
#endif

#include <assert.h>

//
// [2014/01/10 11:15 KSH]iOS support of gettimeofday() function.
//
#ifdef IOS
#include <sys/time.h>
#endif

namespace Sync {

class Event {
public:

#if defined(WIN32)

         Event(bool manual_reset = false, bool initially_signaled = false)
		 : is_manual_reset_(manual_reset),
		 is_initially_signaled_(initially_signaled) {
			 event_handle_ = ::CreateEvent(NULL,                 // Security attributes.
				 is_manual_reset_,
				 is_initially_signaled_,
				 NULL);                // Name.
	 }

         ~Event() {
		 CloseHandle(event_handle_);
	 }

         void Set() {
		 SetEvent(event_handle_);
	 }

         void Reset() {
		 ResetEvent(event_handle_);
	 }

         bool Wait(int cms) {
		 DWORD ms = (cms == -1)? INFINITE : cms;
		 return (WaitForSingleObject(event_handle_, ms) == WAIT_OBJECT_0);
	 }

#else
	 static const int kForever = -1;

	 Event(bool manual_reset, bool initially_signaled)
		 : is_manual_reset_(manual_reset),
		 event_status_(initially_signaled) {
			 int result;
			 result = pthread_mutex_init(&event_mutex_, NULL);
			 assert(result == 0);
			 result = pthread_cond_init(&event_cond_, NULL);
			 assert(result == 0);
	 }

	 ~Event() {
		 pthread_mutex_destroy(&event_mutex_);
		 pthread_cond_destroy(&event_cond_);
	 }

	 void Set() {
		 pthread_mutex_lock(&event_mutex_);
		 event_status_ = true;
		 pthread_cond_broadcast(&event_cond_);
		 pthread_mutex_unlock(&event_mutex_);
	 }

	 void Reset() {
		 pthread_mutex_lock(&event_mutex_);
		 event_status_ = false;
		 pthread_mutex_unlock(&event_mutex_);
	 }

	 bool Wait(int cms) {
		 pthread_mutex_lock(&event_mutex_);
		 int error = 0;

		 if (cms != kForever) {
			 // Converting from seconds and microseconds (1e-6) plus
			 // milliseconds (1e-3) to seconds and nanoseconds (1e-9).

			 struct timeval tv;
			 gettimeofday(&tv, NULL);

			 struct timespec ts;
			 ts.tv_sec = tv.tv_sec + (cms / 1000);
			 ts.tv_nsec = tv.tv_usec * 1000 + (cms % 1000) * 1000000;

			 // Handle overflow.
			 if (ts.tv_nsec >= 1000000000) {
				 ts.tv_sec++;
				 ts.tv_nsec -= 1000000000;
			 }

			 while (!event_status_ && error == 0)
				 error = pthread_cond_timedwait(&event_cond_, &event_mutex_, &ts);
		 } else {
			 while (!event_status_ && error == 0)
				 error = pthread_cond_wait(&event_cond_, &event_mutex_);
		 }

		 // NOTE(liulk): Exactly one thread will auto-reset this event. All
		 // the other threads will think it's unsignaled.  This seems to be
		 // consistent with auto-reset events in WIN32.
		 if (error == 0 && !is_manual_reset_)
			 event_status_ = false;

		 pthread_mutex_unlock(&event_mutex_);

		 return (error == 0);
	 }

#endif

 private:
  bool is_manual_reset_;

#if defined(WIN32)
  bool is_initially_signaled_;
  HANDLE event_handle_;
#else
  bool event_status_;
  pthread_mutex_t event_mutex_;
  pthread_cond_t event_cond_;
#endif
};

}	// namespace sync

#endif  // __ULIB_EVENT_H__
