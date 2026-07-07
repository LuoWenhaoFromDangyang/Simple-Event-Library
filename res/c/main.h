#pragma once
#ifndef SEL_C
    #include <stddef.h> // for size_t
    #include <stdarg.h> // for va_list
    #include "../common/ver.txt" // for SEL_C
    #ifdef _WIN32
        #include <windows.h> // for HANDLE, CreateMutex, CloseHandle
        #define SEL_impl_NewMutexLock(lock) HANDLE lock
        #define SEL_impl_CreateMutexLock(lock, on_failed) \
            do {\
                lock = CreateMutex(NULL, TRUE, NULL); \
                if (lock == NULL) {on_failed} \
            } while(0)
        #define SEL_impl_DestroyMutexLock(lock) \
            do { \
                ReleaseMutex(lock); \
                CloseHandle(lock); \
            } while(0)
        #define SEL_impl_LockMutex(lock) WaitForSingleObject(lock, INFINITE)
        #define SEL_impl_UnlockMutex(lock) ReleaseMutex(lock)
    #elif __unix__
        #include <pthread.h> // for pthread_mutex_t
        #define SEL_impl_NewMutexLock(lock) pthread_mutex_t lock
        #define SEL_impl_CreateMutexLock(lock, on_failed) \
            do { \
                pthread_mutexattr_t attr; \
                if (ipthread_mutexattr_init(&attr)) { on_failed; } \
                pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
                if (pthread_mutex_init(&lock, &attr) != 0) { on_failed; } \
                pthread_mutexattr_destroy(&attr); \
                pthread_mutex_lock(&lock); \
            } while(0)
        #define SEL_impl_DestroyMutexLock(lock) \
            do { \
                pthread_mutex_unlock(&lock); \
                pthread_mutex_destroy(&lock); \
            } while(0)
        #define SEL_impl_LockMutex(lock) pthread_mutex_lock(&lock)
        #define SEL_impl_UnlockMutex(lock) pthread_mutex_unlock(&lock)
    #endif
    #if defined(SEL_C_USE_IMPL)&&!defined(SEL_C_USED_IMPL)
        #define SEL_C_USED_IMPL
        #define SEL_NewMutexLock(lock) SEL_impl_NewMutexLock(lock)
        #define SEL_CreateMutexLock(lock, on_failed) SEL_impl_CreateMutexLock(lock, on_failed)
        #define SEL_DestroyMutexLock(lock) SEL_impl_DestroyMutexLock(lock)
        #define SEL_LockMutex(lock) SEL_impl_LockMutex(lock)
        #define SEL_UnlockMutex(lock) SEL_impl_UnlockMutex(lock)
    #endif
    #ifdef __cplusplus
        extern "C" {
    #endif
            extern char** SEL_EventNames;
            extern void(***SEL_EventFuncs)(va_list);
            extern size_t* SEL_EventFuncCount; // elem
            extern size_t SEL_EventCount; // elem
            extern size_t SEL_EventCapacity; // byte
            extern int SEL_Init(void);
            extern int SEL_AddEvent(const char* name);
            extern int SEL_RemoveEvent(const char* name);
            extern int SEL_ClearEvents(void);
            extern int SEL_Bind(const char* name, void(*func)(va_list));
            extern int SEL_Unbind(const char* name, void(*func)(va_list));
            extern int SEL_Trigger(const char* name, va_list args);
            extern int SEL_TriggerV(const char* name, ...);
            extern int SEL_Free(void);
    #ifdef __cplusplus
        }
        namespace SEL {
            class Events {
            public:
                int Init() noexcept { return SEL_Init(); }
                int AddEvent(const char* name) noexcept { return SEL_AddEvent(name); }
                int RemoveEvent(const char* name) noexcept { return SEL_RemoveEvent(name); }
                int ClearEvents() noexcept { return SEL_ClearEvents(); }
                int Bind(const char* name, void(*func)(va_list)) noexcept { return SEL_Bind(name, func); }
                int Unbind(const char* name, void(*func)(va_list)) noexcept { return SEL_Unbind(name, func); }
                int Trigger(const char* name, va_list args) noexcept { return SEL_Trigger(name, args); }
                int Trigger(const char* name, ...) noexcept {
                    va_list args;
                    va_start(args, name);
                    int ret = SEL_Trigger(name, args);
                    va_end(args);
                    return ret;
                }
                int Free() noexcept { return SEL_Free(); }
            };
        }
    #endif
#endif