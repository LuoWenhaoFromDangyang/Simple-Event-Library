#include <stdio.h>
#include <stdarg.h>
#include "./SEL.h"

void foo(va_list args) {
    double a = va_arg(args, double);
    double b = va_arg(args, double);
    va_end(args);
    printf("Received argument: %.2lf %.2lf\n", a, b);
    return;
}

int main() {
    printf("SEL version: %s\n", SEL_C);
    int val;
    if(val = SEL_Init()) {
        printf("Failed to initialize SEL.(code %d)\n", val);
        printf("Please check your RAM usage and try again.\n");
        return 1;
    }
    printf("Successfully initialized SEL.\n");
    if(val = SEL_AddEvent("foo")) {
        printf("Failed to add event 'foo'.(code %d)\n", val);
        printf("Please check your RAM usage and try again.\n");
        return 1;
    }
    if(val = SEL_Bind("foo", foo)) {
        printf("Failed to bind function 'foo'.(code %d)\n", val);
        printf("Please check your RAM usage and try again.\n");
        return 1;
    }
    if(val = SEL_TriggerV("foo", 0.1, 0.2)) {
        printf("Failed to trigger event 'foo'.(code %d)\n", val);
        printf("Please check your RAM usage and try again.\n");
        return 1;
    }
    SEL_Free();
    printf("SEL freed.\n");
    return 0;
}