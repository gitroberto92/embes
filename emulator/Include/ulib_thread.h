
#ifndef __ULIB_THREAD_H__
#define __ULIB_THREAD_H__

#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#endif
#include "dbg.h"
namespace ULIB {

// 스레드의 동기적인 중지처리는 사용하는 모듈(이 클라스의 파생클라스 혹은 이 클라스 인스턴스를 생성한 모듈)에서 조직해야 한다.
// 즉 사용하는 모듈에서 구현한 스레드함수에서 처리중지 조건을 감시하다가 중지조건이 성립되면
// 스레드 함수를 끝내야 한다. 
// 이 클라스의 Stop()함수는 강제로 스레드를 중지시키지 않으며 스레드함수가 끝나기를 무한히 기다린다.
// 따라서 스레드를 중지시키려고 할때 적당한 시점에서 중지 조건을 설정해야 하며 스레드함수가 이 중지조건을 
// 의식하지 못하고 dead lock에 빠지지 않게 주의해야 한다.
// 사실상 이 클라스의 Stop()함수는 WaitForThreadStopAndFinishProcedure()나 동일하다.
class Thread
{
public:
	typedef int (*ThreadProc) (void*);

public:
	// this is the minimum stack size a thread will be spawned with
	// if this value is too small, the OS will allocate memory on demand and
	// thus might lead to dropouts in realtime threads
	// TODO: should be up for testing to get a reasonable good value
	static const int MIN_STACK_SIZE = 524288;

public:
	Thread(bool RealTime, ThreadProc lpfnProc, void* lpProcParameter, int PriorityMax = 0, int PriorityDelta = -4)
		: m_isRunning(false)
		, m_isRealTime(RealTime)
		, m_lpfnProc(lpfnProc)
		, m_lpProcParameter(lpProcParameter)
		, m_priorityMax(PriorityMax)
		, m_priorityDelta(PriorityDelta)
	{
		#if defined(WIN32)
			m_hThread = NULL;
			m_lpThreadId = 0;
		#else
			memset(&m_thread_attr, 0, sizeof(m_thread_attr));
			m_thread_id = 0;
			m_thread_destructor_key = 0;
		#endif

		#if !defined(WIN32)
		m_thread_destructor_key = 0;
		pthread_attr_init(&m_thread_attr);
		#endif
	}

	virtual ~Thread()
	{
		if (IsRunning()){
			Stop();
		}
		#if !defined(WIN32)
	    pthread_attr_destroy(&m_thread_attr);
		#endif
	}

	/**
	 * Returns @c true in case the thread is currently running.
	 */
	bool IsRunning()
	{
	    return m_isRunning;
	}

	void SetProcFinished()
	{
#if defined(WIN32)
		if (m_hThread != NULL)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
		m_lpThreadId = 0;
#endif
		m_isRunning = false;
	}

	virtual int Start()
	{
		if(!m_isRunning)
		{
			return SignalStartThread();
		}
		return 0;
	}

	// 스레드가 끝날때까지 기다린다.
	virtual int Stop()
	{
		SignalStopThread();
		return 0;
	}

#if defined(WIN32)
	// 스레드를 강제 완료
	virtual int ForceStop()
	{
		TerminateThread(m_hThread, 0); // we set ExitCode to 0
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_isRunning = false;
		return 0;
	}
#endif


protected:

	/**
	 *  Starts the thread. This method will signal to start the thread and
	 *  return immediately. Note that the thread might not yet run when this
	 *  method returns! The abstract method Main() is the entry point for the
	 *  new thread. You have to implement the Main() method in your subclass.
	 *
	 *  @see StartThread()
	 */
	int SignalStartThread() {
	#if defined(WIN32)
	    m_hThread = CreateThread(
	               NULL, // no security attributes
	               MIN_STACK_SIZE,
	               (LPTHREAD_START_ROUTINE)__win32thread_launcher,
	               this,
	               0,
	               &m_lpThreadId);
	    if(m_hThread == NULL)
	    {
	        m_isRunning = false;
	        return -1;
	    }

	    return 0;
	#else
	    int res = 0;

		#ifndef ANDROID
	    	// prepare the thread properties
	    	res = pthread_attr_setinheritsched(&m_thread_attr, PTHREAD_EXPLICIT_SCHED);
	    	if (res)
	    	{
	    		m_isRunning = false;
	    		return res;
	    	}

	    	res = pthread_attr_setstacksize(&m_thread_attr, MIN_STACK_SIZE);
	    	if (res)
	    	{
				m_isRunning = false;
				return res;
	    	}
		#endif /*!ANDROID*/

	    res = pthread_attr_setdetachstate(&m_thread_attr, PTHREAD_CREATE_JOINABLE);
	    if (res)
	    {
			m_isRunning = false;
	        return res;
	    }

	    res = pthread_attr_setscope(&m_thread_attr, PTHREAD_SCOPE_SYSTEM);
	    if (res)
	    {
			m_isRunning = false;
	        return res;
	    }

	    // Create and run the thread
	    res = pthread_create(&this->m_thread_id, &m_thread_attr, __pthread_launcher, this);
	    switch (res)
	    {
	        case 0: // Success
	            break;
	        case EAGAIN:
				m_isRunning = false;
	            break;
	        case EPERM:
				m_isRunning = false;
	            break;
	        default:
				m_isRunning = false;
	            break;
	    }

	    return res;
	#endif /*!WIN32*/
	}

	/**
	 *  Stops the thread. This method will signal to stop the thread and return
	 *  immediately. Note that the thread might still run when this method
	 *  returns!
	 *
	 *  @see StopThread()
	 */
	int SignalStopThread()
	{
	    //FIXME: segfaults when thread is not yet running
		#if defined(WIN32)
			if (m_hThread != NULL)
			{
				// 스레드가 끝날때까지 기다린다.
				WaitForSingleObject(m_hThread, INFINITE);
			}

		#else
			int status;
			pthread_join(m_thread_id, (void**)&status);
		#endif

		// 이시점은 m_isRunning = false인 상태이다.
		// m_isRunning = false;

	    return 0;
	}

#if defined(WIN32)
	static DWORD _stdcall __win32thread_launcher(LPVOID thread)
	{
		Thread* t;
		t = (Thread*) thread;
		ThreadProc proc = t->m_lpfnProc;
		if (proc == NULL)
			return 0;

		t->SetSchedulingPriority();
		t->EnableDestructor();

		proc(t->m_lpProcParameter);
		t->SetProcFinished();

		return 0;
	}
#else
	///< Callback function for the POSIX thread API
	static void* __pthread_launcher(void* thread)
	{
		//#warning "pthread_setcanceltype function is not at the ndk library."
		// pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // let the thread be killable under any circumstances

		Thread* t;
		t = (Thread*) thread;
		ThreadProc proc = t->m_lpfnProc;
		if (proc == NULL)
			return 0;

		t->SetSchedulingPriority();
		t->EnableDestructor();

		proc(t->m_lpProcParameter);
		t->SetProcFinished();

		return NULL;
	}
#endif /*!WIN32*/

#if !defined(WIN32)
	///< Callback function for the POSIX thread API
	static void __pthread_destructor(void* thread)
	{
		Thread* t;
		t = (Thread*) thread;
		t->Destructor();
	}
#endif

	/**
	 *  Registers thread destructor callback function which will be executed when
	 *  the thread stops it's execution and sets the 'Running' flag to true. This
	 *  method will be called by the __pthread_launcher callback function, DO NOT
	 *  CALL THIS METHOD YOURSELF!
	 */
	void EnableDestructor()
	{
		m_isRunning = true;
		return;
	}

	/**
	 *  Will be called by the kernel when the thread stops it's execution.
	 */
	int Destructor()
	{
		#if !defined(WIN32)
			pthread_key_delete(m_thread_destructor_key);
		#endif

		return 0;
	}

	/**
	 *  Sets the process SCHED_FIFO policy,  if max=1 then set at max priority,
	 *  else use min priority. delta is added to the priority so that we can
	 *  for example set 3 SCHED_FIFO tasks to different priorities by specifying
	 *  delta  0 , -1 , -2  ( 0 = highest priority because -1 is subtracted to the
	 *  current priority).
	 */
	int SetSchedulingPriority()
	{
		#if defined(WIN32)
			DWORD dwPriorityClass;
			int nPriority;

			if(m_isRealTime) {
				dwPriorityClass = REALTIME_PRIORITY_CLASS;
				if (m_priorityMax == 1) {
					if(m_priorityDelta == 0) nPriority = THREAD_PRIORITY_TIME_CRITICAL;
					else nPriority = 7 + m_priorityDelta;
				}
				else nPriority = THREAD_PRIORITY_NORMAL + m_priorityDelta;
			}
			else {
				dwPriorityClass = NORMAL_PRIORITY_CLASS;
				nPriority = THREAD_PRIORITY_NORMAL + m_priorityDelta;
			}

			return 0;
		#else

		#if !defined(__APPLE__)
			int policy;
			const char* policyDescription = NULL;
			if (m_isRealTime) { // becomes a RT thread
				policy = SCHED_FIFO;
				policyDescription = "realtime";
			} else { // 'normal', non-RT thread
				policy = SCHED_OTHER;
				policyDescription = "normal (non-RT)";
			}

			// set selected scheduling policy and priority
			struct sched_param schp;
			memset(&schp, 0, sizeof(schp));

			if (m_isRealTime) // it is not possible to change priority for the SCHED_OTHER policy
			{
				if (m_priorityMax == 1) {
					schp.sched_priority = sched_get_priority_max(policy) + m_priorityDelta;
				}
				if (this->m_priorityMax == -1) {
					schp.sched_priority = sched_get_priority_min(policy) + m_priorityDelta;
				}
			}

			if (pthread_setschedparam(m_thread_id, policy, &schp) != 0)
			{
				return -1;
			}

		#endif /*!__APPLE__*/
	    return 0;
		#endif /*!WIN32*/
	}

private:

	#if defined(WIN32)
		HANDLE	m_hThread;
        DWORD	m_lpThreadId;
	#else
        pthread_attr_t  m_thread_attr;
        pthread_t       m_thread_id;
        pthread_key_t   m_thread_destructor_key;
	#endif

	bool 	m_isRunning;
	bool 	m_isRealTime;
	int 	m_priorityMax;
	int		m_priorityDelta;

public:
	ThreadProc 	m_lpfnProc;
	void*		m_lpProcParameter;
};

class IRunnable {
public:
	virtual ~IRunnable() { }
	virtual void Run() = 0;
};

class Threadalbe : public IRunnable
{
protected:
	virtual void Run() = 0;

private:
	Thread* thread;

	static int MainProc(void* param)
	{
		Threadalbe* p = (Threadalbe*)param;
		if (p != nullptr)
		{
			p->Run();
		}

		return 0;
	}

public:
	Threadalbe(bool RealTime = false, int PriorityMax = 0, int PriorityDelta = -4)
	{
		thread = new Thread(RealTime, MainProc, this, PriorityMax, PriorityDelta);
	}

	virtual ~Threadalbe()
	{
	    Stop();
		delete thread;
	}

	/**
	 * Returns @c true in case the thread is currently running.
	 */
	inline bool IsRunning()
	{
		return thread->IsRunning();
	}

	virtual int Start()
	{
		return thread->Start();
	}

	virtual int Stop()
	{
		return thread->Stop();
	}
};

} /*!namespace ULIB*/

#endif // __ULIB_THREAD_H__
