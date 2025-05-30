
### Pass by Reference in C, and how it changes in C++
```C++

// Passing by pointer: Here the memory location (address) of the variables is passed to the parameters.
// You can do address-of to get the memory address of the pointer itself, or just de-reference to mess
// with the values of the variables that the pointers are pointing to. So this is called "Call by reference".
int swap(int *x, int *y){
   int z;
   z = *x;   /* save the value at address x */
   *x = *y;  /* put y into x */
   *y = z;   /* put z into y */ 
   return 0;
}

// Passing by reference (C++ exclusive): This allows a ufnction to modify a variable without having to 
// create a copy of it. Don't think of this as "I'm adding 5 to a memory address", think of it as 
// "I'm modifying the actual variable, no copying or pointer syntax, just directly accessing it via an alias"
void square(int &n) {
    n *= n;
}


int a = 5;
square(a);  // a is now 25

// Of course you could write the same thing in C, and underneath the same thing is happening
void square_c(int *n) {
  int value = *n;
  *n = value * value;
}


// Refernces in C++ review
int a = 10;
int &ref = a; // ref is an alias to a
ref = 15;     // a is now 15
```

| Feature              | Reference (`&`)                   | Pointer (`*`)                  |
| -------------------- | --------------------------------- | ------------------------------ |
| Syntax               | `void f(int &x)`                  | `void f(int *x)`               |
| Must be initialized? | ✅ Must be initialized             | ❌ Can be null or uninitialized |
| Use inside function  | Acts like a normal variable (`x`) | Needs dereference (`*x`)       |
| Null-safe?           | ✅ Cannot be null                  | ❌ Can be null (need to check)  |
| Change original?     | ✅ Yes                             | ✅ Yes (after dereferencing)    |
| Cleaner syntax       | ✅ Yes                             | ❌ No (more verbose)            |


```C++
static bool read_str(const uint8_t *&cur, const uint8_t *end, size_t n, string &out) {
  if (cur + n > end) {
    return false;
  }
  out.assign(cur, cur+n);
  cur += n;
  return true;
}

static bool read_str_c(const uint8_t **cur, const uint8_t *end, size_t n, char *out) {
  if (*cur + n > end) {
    return false;
  }
  memcpy(out, *cur, n);
  *cur += n;
  return true;
}

```


### Understanding ```const uint8_t *&cur```



`cur` is a reference to a pointer to a const uint8_t:
- `uint8_t *`: The pointer itself is a pointer to a `uint8_t`.
- `&`: This is pass by reference, we're passing the pointer itself by reference.
- `const`: You can't modify `*cur` but you can reassign `cur`. This allows us to advance the pointer of the byte array to the next byte but not modify the `*cur`, which is the actual uint8_t itself

You're passing in a pointer to a read-only byte array. Then we're modifying the pointer itself via reference so the caller sees the new pointer value:

`T *` – pointer to a value of type T
`T &` – reference to a value (modifies the original)
`T *&` – reference to a pointer (modifies the original pointer)
`const T *` – pointer to const data (data can't change)
`T *const` – const pointer (pointer can't change)
`const T *&` – reference to pointer to const data