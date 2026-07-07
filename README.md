# Simple Event Library (SEL)
---
The Simple Event Library (SEL) is a lightweight event handling library for Python >= 3.6 and C/C++. It provides a simple and efficient way to manage events and event handlers.

Current version: 1.0.41 (see `res/common/ver.txt`)

## Installation

To install SEL, you can:
```bash
git clone https://github.com/LuoWenhaoFromDangYang/Simple-Event-Library.git
cd Simple-Event-Library
pip install .
```
Or:
```bash
pip install SimpleELpy==1.0.41
```

## Usage

### Python
Use SEL on Python like this:
```python
import SimplEL as SEL

def foo(*args):
    a = args[0]
    b = args[1]
    print(f"Received argument: {a:.2f} {b:.2f}")
    return

def main():
    print(f"SEL version: {SEL.SEL_PY}")
    SEL.Events.add_event("foo")
    SEL.Events.bind("foo", foo)
    SEL.Events.trigger("foo", 0.1, 0.2)
    return 0

if __name__ == "__main__":
    main()
```
### C/C++
Or use SEL on C/C++ like this:
```c
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
```

### C++
And use SEL on C++ like this:
```cpp
#include <iostream>
#include <cstdarg>
#include "./SEL.h"

void foo(va_list args) {
    double a = va_arg(args, double);
    double b = va_arg(args, double);
    va_end(args);
    std::cout << "Received argument: " << a << " " << b << std::endl;
    return;
}

int main() {
    std::cout << "SEL version: " << SEL_C << std::endl;
    SEL::Events events;
    int val;
    if(val = events.Init()) {
        std::cout << "Failed to initialize SEL.(code " << val << ")\n";
        std::cout << "Please check your RAM usage and try again.\n";
        return 1;
    }
    std::cout << "Successfully initialized SEL.\n";
    if(val = events.AddEvent("foo")) {
        std::cout << "Failed to add event 'foo'.(code " << val << ")\n";
        std::cout << "Please check your RAM usage and try again.\n";
        return 1;
    }
    if(val = events.Bind("foo", foo)) {
        std::cout << "Failed to bind function 'foo'.(code " << val << ")\n";
        std::cout << "Please check your RAM usage and try again.\n";
        return 1;
    }
    if(val = events.Trigger("foo", 0.1, 0.2)) {
        std::cout << "Failed to trigger event 'foo'.(code " << val << ")\n";
        std::cout << "Please check your RAM usage and try again.\n";
        return 1;
    }
    events.Free();
    std::cout << "SEL freed.\n";
    return 0;
}
```

## Initialization

To initialize SEL, you can use functions like these:
 - In Python, SEL initializes automatically when imported.
 - In it's C/C++, you need to call `SEL_Init()` manually.
 - In it's C++, you need to create a `SEL::Events` object and call `events.Init()` manually.
These function returns an integer value indicating the success or failure of the initialization. If the initialization is successful, the function returns 0. If the initialization fails, the function returns a non-zero value.

## Compiling C/C++ programs
Compile with:
```bash
gcc -o my_program my_program.c ./res/c/main.c
```
Or:
```bash
g++ -o my_program my_program.cpp ./res/c/main.c
```

## License
MIT License

## Contributing
Feel free to open issues or pull requests on GitHub.
