# Conceptual Overview 

```C
// Changes what myPtr points to, it now holds the memory address of myVar
myPtr = &myVar 

// Changes the value stored at the memory address that myPtr is pointing to. We're
// not changing where myPtr points, it still points at that variable. However we 
// did just change teh 
*myPtr = value 

// This makes myPtr point to the same address as otherPtr. So now both myPtr and otherPtr
// point to the same location in memory
myPtr = otherPtr

// Copies the value from otherPtr's pointee to myPtr's pointee.
*myPtr = *otherPtr;
```