# Flexible Arrays in C 
This is a feature introduced in the C99 standard of the C programming language, and it isn't in C++. The idea is that within structs, we can declare an array wthiout a deimsnion and whose size is flexible in nature.

This array should preferably be declared as the last member of the structure, and the structure should contain at least one other member in addition to the flexible array member (FAM).

## How it Works 
```C

// Total size should be 12 bytes? Or no? I don't know how it works
struct student {
    int stud_id;      // 4 bytes
    int name_len;     // 4 bytes
    int struct_size;  // 4 bytes 
    char stud_name[]; // 0 bytes
};
```
Here the `stud_name` is a FAM, so its size isn't fixed. The memory allocation for flexible array members is denoted as:
```C
struct student *s = malloc(sizeof(*s) + sizeof(char[strlen(stud_name)]));
```
The convention is that hte flexible array member doesn't contribute to the size of the structure. Instead the size of the structure is determined by the other data members plus any memory allocated dynamiclaly for the flexible array.




## Credits
- [Flexible Array Members - Geeks for Geeks](https://www.geeksforgeeks.org/flexible-array-members-structure-c/)
- [Flexible Array Members - Wikipedia](https://en.wikipedia.org/wiki/Flexible_array_member)