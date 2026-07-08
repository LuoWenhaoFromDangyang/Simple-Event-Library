# Simple Event Library (SEL)
---
The Simple Event Library (SEL) is a lightweight event handling library for Python >= 3.6 and C/C++. It provides a simple and efficient way to manage events and event handlers.

Current version: 1.0.413 (see `res/common/ver.txt`)

## Installation

To install SEL, you can:
```bash
git clone https://github.com/LuoWenhaoFromDangYang/Simple-Event-Library.git
cd Simple-Event-Library
pip install .
```
Or:
```bash
pip install SimpleELpy==1.0.413
```

## Usage

### Python
Use SEL on Python like this:
```python
import SEL

def foo(*args):
    a = args[0]
    b = args[1]
    print(f"Received argument: {a:.2f} {b:.2f}")
    return a + b

def main():
    print(f"SEL version: {SEL.SEL_PY}")
    SEL.Events.add_event("foo")
    SEL.Events.bind("foo", foo)
    retval = SEL.Events.trigger("foo", 0.1, 0.2)[0]
    print(f"Result: {retval}")
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

## APIs

### Python
 - `SEL.Events.add_event(event_name: str) -> None`: Adds an event with the given name.
 - `SEL.Events.remove_event(event_name: str) -> None`: Removes an event with the given name.
 - `SEL.Events.get_events() -> dict[str, list[Callable]]`: Returns a dict of all events and their corresponding handlers.
 - `SEL.Events.bind(event_name: str, handler: Callable) -> None`: Binds a handler to an event.
 - `SEL.Events.unbind(event_name: str, handler: Callable) -> None`: Unbinds a handler from an event.
 - `SEL.Events.trigger(event_name: str, *args, **kwargs) -> list[Any]`: Triggers an event with the given arguments.

### C/C++
 - `int SEL_Init()`: Initializes SEL. Returns 0 on success, non-zero on failure.
 - `int SEL_AddEvent(const char* event_name)`: Adds an event with the given name. Returns 0 on success, non-zero on failure.
 - `int SEL_RemoveEvent(const char* event_name)`: Removes an event with the given name. Returns 0 on success, non-zero on failure.
 - `int SEL_ClearEvents()`: Clears all events. Returns 0 on success, non-zero on failure.
 - `int SEL_Bind(const char* event_name, void (*handler)(va_list))`: Binds a handler to an event. Returns 0 on success, non-zero on failure.
 - `int SEL_Unbind(const char* event_name, void (*handler)(va_list))`: Unbinds a handler from an event. Returns 0 on success, non-zero on failure.
 - `int SEL_TriggerV(const char* event_name, va_list args)`: Triggers an event with the given arguments. Returns 0 on success, non-zero on failure.
 - `int SEL_Trigger(const char* event_name, ...)`: Triggers an event with the given arguments. Returns 0 on success, non-zero on failure.
- `int SEL_Free()`: Frees SEL. Always returns 0.

### C++
- `int SEL::Events::Init()`: Same as `SEL_Init()`.
- `int SEL::Events::AddEvent(const char* event_name)`: Same as `SEL_AddEvent()`.
- `int SEL::Events::RemoveEvent(const char* event_name)`: Same as `SEL_RemoveEvent()`.
- `int SEL::Events::ClearEvents()`: Same as `SEL_ClearEvents()`.
- `int SEL::Events::Bind(const char* event_name, void (*handler)(va_list))`: Same as `SEL_Bind()`.
- `int SEL::Events::Unbind(const char* event_name, void (*handler)(va_list))`: Same as `SEL_Unbind()`.
- `int SEL::Events::TriggerV(const char* event_name, va_list args)`: Same as `SEL_TriggerV()`.
- `int SEL::Events::Trigger(const char* event_name, ...)`: Same as `SEL_Trigger()`.
- `int SEL::Events::Free()`: Same as `SEL_Free()`.

## Compiling C/C++ programs
Compile with:
```bash
gcc -o your_out your_code.c ./res/c/main.c
```
Or:
```bash
g++ -o your_out your_code.cpp .res/c/main.c
```

## License
MIT License

## Contributing
Feel free to open issues or pull requests on GitHub.
