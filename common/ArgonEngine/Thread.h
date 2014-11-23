/**
 * @brief This file provides utilities for cross platform threading.
 * @file Thread.h
 * @author Skyler Saleh
 **/
#ifndef __ARTHREAD__
#define __ARTHREAD__ 1
#include "config.h"
#include <cstddef>
#ifdef USE_PTHREADS
#include <pthread.h>
#endif
namespace Argon
{
#ifdef USE_PTHREADS
//! @brief Cross platform Mutex object.
struct Mutex
{
    pthread_mutex_t mutex;
    //! Creates the mutex object.
    Mutex(){
        pthread_mutex_init(&mutex, NULL);
    }
    //! Locks the mutex. Returns when the mutex has been locked.
    void lock(){
        pthread_mutex_lock(&mutex);
    }
    //! Tries to lock the mutex. This will fail if the mutex is already locked.
    //! Returns false if the mutex was locked successfully. Returns True if the mutex
    //! was contended and couldn't be locked.
    bool lockOrSkip(){
        return !pthread_mutex_trylock(&mutex);
    }
    //! Unlocks the mutex.
    void unlock(){
        pthread_mutex_unlock(&mutex);
    }
    //! Destroys the mutex.
    ~Mutex(){
        pthread_mutex_destroy(&mutex);
    }
};
//! @brief Cross platform RecursiveMutex Object
//! @details A recursive mutex does not cause a deadlock if a thread that already has locked
//! the mutex calls the lock() function again, and it unlocks the mutex once it has been unlocked
//! the same number of times as it has been locked.
struct RecursiveMutex
{
    pthread_mutex_t mutex;
    //! Creates a RecursiveMutex
    RecursiveMutex(){
        pthread_mutexattr_t mta;

        pthread_mutexattr_init(&mta);
        pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex, &mta);
    }
    //! Locks the mutex.
    void lock(){
        pthread_mutex_lock(&mutex);
    }
    //! Tries to lock the mutex. This will fail if the mutex is already locked.
    //! Returns false if the mutex was locked successfully. Returns True if the mutex
    //! was contended and couldn't be locked.
    bool lockOrSkip(){
        return !pthread_mutex_trylock(&mutex);
    }
    //! Unlocks the mutex.
    void unlock(){
        pthread_mutex_unlock(&mutex);
    }
    //! Destroys the mutex.
    ~RecursiveMutex(){
        pthread_mutex_destroy(&mutex);
    }
};
//! @brief This class provides a cross platform RWLock implementation.
//! @details
//! A RWLock allows for locking read/write operations individually.
struct RWLock
{
    pthread_rwlock_t rwlock;
    RWLock(){
        pthread_rwlock_init(&rwlock, NULL);
    }
    //! Lock both read and write.
    void lock(){
        pthread_rwlock_wrlock(&rwlock);
    }
    //! Lock read.
    void lock_read(){
        pthread_rwlock_rdlock(&rwlock);
    }
    //! Lock write.
    void lock_write(){
        pthread_rwlock_wrlock(&rwlock);
    }
    //! Unlock read.
    void unlock_read(){
        pthread_rwlock_unlock(&rwlock);
    }
    //! Unlock write.
    void unlock_write(){
        pthread_rwlock_unlock(&rwlock);
    }
    ~RWLock(){
        pthread_rwlock_destroy(&rwlock);
    }
};
#elif PLATFORM_WINDOWS==1
struct Mutex
{
    void* mutex;
    Mutex();
    void lock();
    bool lockOrSkip();
    void unlock();
    ~Mutex();
};
typedef Mutex RecursiveMutex;
struct RWLock
{
    Mutex read;
    Mutex write;
    RWLock(){}
    void lock(){
        read.lock(); write.lock();
    }
    void lock_read(){
        read.lock();
    }
    void lock_write(){
        write.lock();
    }
    void unlock_read(){
        read.unlock();
    }
    void unlock_write(){
        write.unlock();
    }
};
#endif
//! @brief This class is used to create a thread in an OS independent way.
class Thread
{
private:
    void * threadPrimitive=NULL;
    void *data=NULL;
public:
    //! @brief This static function is used to sleep the callers thread in a platform independent way.
    //! It is the equivalent of calling usleep, or Sleep.
    //! @param secs The number of seconds to sleep for.
    static void sleep(float secs);
    //! @brief Creates and launches a thread.
    //! @param function The function for the thread to call after it is created.
    //! @param data The data to give the function when it is first called.
    Thread(void *(*function)(void*), void* data);
    //! Creates a thread but does not launch it.
    Thread() {}
    //! @brief Launches a thread.
    //! @param function The function for the thread to call after it is launched.
    //! @param data The data to call the function with.
    void launch(void *(*function)(void*), void* data);
    //! Destroys the thread. Calls join() before thread is deleted.
    ~Thread();
    //! This function does not return until the thread has terminated.
    void join();

};
//! @brief This class provides a platform independent high resolution timer.
class Timer {
private:
    long timeA,timeB;
    double delta_time_;
public:
    Timer();
    Timer(const Timer &t) : timeA(t.timeA),timeB(t.timeB),delta_time_(t.delta_time_){}
    //! @brief Returns the current time in seconds. This may be the system time, uptime, or time since
    //! the application has launched.
    double time();
    //! @brief Returns the time that has passed since the last call to delta_time() on the timer
    //! object in seconds.
    double delta_time();
};
}
#endif
