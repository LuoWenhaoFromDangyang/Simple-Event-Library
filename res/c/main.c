#include <stddef.h> // for size_t
#include <stdarg.h> // for va_list
#include <stdint.h> // for SIZE_MAX
#include <stdlib.h> // for malloc, free, realloc
#include <string.h> // for strcmp, strdup
#ifdef _WIN32
    #define strdup _strdup
#endif
#include "../common/ver.txt"
#ifdef __cplusplus
extern "C" {
#endif
    char** SEL_EventNames;
    void(***SEL_EventFuncs)(va_list);
    size_t* SEL_EventFuncCount; //elem
    size_t SEL_EventCount = 0; // elem
    size_t SEL_EventCapacity = 0; // byte
    int SEL_Init() {
        size_t init_events = 1024 / sizeof(char*);
        size_t bytes_names = init_events * sizeof(char*);
        size_t bytes_funcs = init_events * sizeof(void(**)(va_list));
        size_t bytes_counts = init_events * sizeof(size_t);
        SEL_EventNames = (char**)malloc(bytes_names);
        if (!SEL_EventNames) return -1;
        SEL_EventFuncs = (void(***)(va_list))malloc(bytes_funcs);
        if (!SEL_EventFuncs) {
            free(SEL_EventNames);
            SEL_EventNames = NULL;
            return -1;
        }
        SEL_EventFuncCount = (size_t*)malloc(bytes_counts);
        if (!SEL_EventFuncCount) {
            free(SEL_EventNames);
            SEL_EventNames = NULL;
            free(SEL_EventFuncs);
            SEL_EventFuncs = NULL;
            return -1;
        }
        for (size_t i = 0; i < init_events; i++) {
            SEL_EventFuncCount[i] = 0;
            SEL_EventFuncs[i] = NULL;
            SEL_EventNames[i] = NULL;
        }
        SEL_EventCapacity = bytes_names;
        return 0;
    }
    int SEL_AddEvent(const char* name) {
        if (!name) return -1;
        for (size_t i = 0; i < SEL_EventCount; i++) {
            if (!strcmp(SEL_EventNames[i], name)) return -1;
        }
        size_t max_events = SEL_EventCapacity / sizeof(char*);
        if (SEL_EventCount >= max_events) {
            if (SEL_EventCapacity > SIZE_MAX / 2) return -1;
            size_t new_max = max_events * 2;
            size_t new_bytes_names = new_max * sizeof(char*);
            size_t new_bytes_funcs = new_max * sizeof(void(**)(va_list));
            size_t new_bytes_counts = new_max * sizeof(size_t);
            char** new_names = (char**)realloc(SEL_EventNames, new_bytes_names);
            if (!new_names) return -1;
            void (***new_funcs)(va_list) = (void(***)(va_list))realloc(SEL_EventFuncs, new_bytes_funcs);
            if (!new_funcs) {
                free(new_names);
                return -1;
            }
            size_t* new_func_count = (size_t*)realloc(SEL_EventFuncCount, new_bytes_counts);
            if (!new_func_count) {
                free(new_names);
                free(new_funcs);
                return -1;
            }
            for (size_t i = max_events; i < new_max; i++) {
                new_func_count[i] = 0;
                new_funcs[i] = NULL;
                new_names[i] = NULL;
            }
            SEL_EventNames = new_names;
            SEL_EventFuncs = new_funcs;
            SEL_EventFuncCount = new_func_count;
            SEL_EventCapacity = new_bytes_names;
        }
        SEL_EventNames[SEL_EventCount] = strdup(name);
        if (!SEL_EventNames[SEL_EventCount]) return -1;
        SEL_EventFuncs[SEL_EventCount] = NULL;
        SEL_EventFuncCount[SEL_EventCount] = 0;
        SEL_EventCount++;
        return 0;
    }
    int SEL_RemoveEvent(const char* name) {
        for (size_t i = 0; i < SEL_EventCount; i++) {
            if (!strcmp(SEL_EventNames[i], name)) {
                free(SEL_EventNames[i]);
                free(SEL_EventFuncs[i]);
                SEL_EventNames[i] = NULL;
                SEL_EventFuncs[i] = NULL;
                for (size_t j = i; j < SEL_EventCount - 1; j++) {
                    SEL_EventNames[j] = SEL_EventNames[j + 1];
                    SEL_EventFuncs[j] = SEL_EventFuncs[j + 1];
                    SEL_EventFuncCount[j] = SEL_EventFuncCount[j + 1];
                }
                SEL_EventCount--;
                SEL_EventNames[SEL_EventCount] = NULL;
                SEL_EventFuncs[SEL_EventCount] = NULL;
                SEL_EventFuncCount[SEL_EventCount] = 0;
                return 0;
            }
        }
        return -1;
    }
    int SEL_ClearEvents(void) {
        for (size_t i = 0; i < SEL_EventCount; i++) {
            free(SEL_EventNames[i]);
            free(SEL_EventFuncs[i]);
            SEL_EventNames[i] = NULL;
            SEL_EventFuncs[i] = NULL;
            SEL_EventFuncCount[i] = 0;
        }
        SEL_EventCount = 0;
        return 0;
    }
    int SEL_Bind(const char* name, void(*func)(va_list)) {
        for (size_t i = 0; i < SEL_EventCount; i++) {
            if (!strcmp(SEL_EventNames[i], name)) {
                size_t new_count = SEL_EventFuncCount[i] + 1;
                void (**new_funcs)(va_list) = (void(**)(va_list))realloc(SEL_EventFuncs[i], new_count * sizeof(void(*)(va_list)));
                if (!new_funcs) return -1;
                SEL_EventFuncs[i] = new_funcs;
                SEL_EventFuncs[i][SEL_EventFuncCount[i]] = func;
                SEL_EventFuncCount[i] = new_count;
                return 0;
            }
        }
        return -1;
    }
    int SEL_Unbind(const char* name, void(*func)(va_list)) {
        for (size_t i = 0; i < SEL_EventCount; i++) {
            if (!strcmp(SEL_EventNames[i], name)) {
                for (size_t j = 0; j < SEL_EventFuncCount[i]; j++) {
                    if (SEL_EventFuncs[i][j] == func) {
                        for (size_t k = j; k < SEL_EventFuncCount[i] - 1; k++) {
                            SEL_EventFuncs[i][k] = SEL_EventFuncs[i][k + 1];
                        }
                        SEL_EventFuncCount[i]--;
                        return 0;
                    }
                }
            }
        }
        return -1;
    }
    int SEL_TriggerV(const char* name, va_list args) {
        for (size_t i = 0; i < SEL_EventCount; i++) {
            if (!strcmp(SEL_EventNames[i], name)) {
                for (size_t j = 0; j < SEL_EventFuncCount[i]; j++) {
                    va_list args_copy;
                    va_copy(args_copy, args);
                    SEL_EventFuncs[i][j](args_copy);
                    va_end(args_copy);
                }
                return 0;
            }
        }
        return -1;
    }
    int SEL_Trigger(const char* name, ...) {
        va_list args;
        va_start(args, name);
        int result = SEL_Trigger(name, args);
        va_end(args);
        return result;
    }
    int SEL_Free(void) {
        for (size_t i = 0; i < SEL_EventCount; i++) {
            free(SEL_EventNames[i]);
            free(SEL_EventFuncs[i]);
        }
        free(SEL_EventNames);
        free(SEL_EventFuncs);
        free(SEL_EventFuncCount);
        SEL_EventNames = NULL;
        SEL_EventFuncs = NULL;
        SEL_EventFuncCount = NULL;
        SEL_EventCount = 0;
        SEL_EventCapacity = 0;
        return 0;
    }
#ifdef __cplusplus
}
#endif