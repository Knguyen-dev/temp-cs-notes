# Macros in C

## What are Macros in C?
A macro is defined by the #define directive. A macro is a name given to a piece of code, so when the compiler encounters a macro in a program, it will replace it with the macro's value. In the macro definition, the macros are not terminated by the semicolon (;).

```C
// Syntax of a Macro:
#define macro_name macro_value
#define pi 3.14;

int main () {
  printf("The value of pi is: %f", pi);
}

```

## Why do we use Macros 
Rather than mentioning a piece of code repeatedly, we define the constant value once and use it frequently. We also have function-like macros that we can pass arguments to, which make our program run a little faster.

### Object-like Macros 
A macro is replaced by the value in the object-like macro. Generally a defined value can be a constant numerical value.
```C
#include <stdio.h>
#define pi 3.14

int main() {
  int r = 4;
  float circumference;
  circumference = 2*pi*r;
  printf("Circumference of a circle is: %f", circumference);
  return 0;
}
```

---
### Function-like Macros
```C
#include <stdio.h>
#define add(a, b) (a + b) // macro definition
int main() {
  int a = 10, b = 15, result;
  result = add(a,b);
  printf("Addition of two numbers is: %d", result);
  return 0;
}
```

---
### Conditional Compilation With Macros
Conditional compilation lets us compile specific part sof our code depending on certain conditions. This is primarily controlled using preprocessor directives and macros. Here are the pre-processor directives that conditional compilations use:
- `#ifdef`
- `#ifndef`
- `#if`
- `#else`
- `#elif`
- `#endif`

#### Example of using `#ifdef` and `#ifndef`
The `#ifdef` (if defined) and `#ifndef` (if not defined) directives check if a macro is defined:
```C
#define FEATURE_ENABLED

#ifdef FEATURE_ENABLED
    // Code to include if FEATURE_ENABLED is defined
    printf("Feature is enabled\n");
#endif

#ifndef DEBUG
    // Code to include if DEBUG is not defined
    printf("Debug mode is off\n");
#endif
```
Then if, else, elif, and endif are for complex conditional checks:
```C
#define VERSION 2

#if VERSION >= 2
    // Code for version 2 or higher
    printf("Version 2 or higher\n");
#else
    // Code for versions lower than 2
    printf("Version lower than 2\n");
#endif

#if defined(WINDOWS) && !defined(MACOS)
    // Code specific to Windows but not macOS
    printf("Compiling for Windows\n");
#elif defined(MACOS)
    // Code specific to macOS
    printf("Compiling for macOS\n");
#else
    // Default code
    printf("Compiling for an unknown platform\n");
#endif
```
