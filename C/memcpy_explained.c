/*
The Copy Memory Block function:

void *memcpy(void *dest_str, const void *src_str, size_t n);
- dest_str: A pointer ot the destination array where the content is going to be sent
- src_str:  The parameter is used to define the source of the data to be copied.
- n: Number of bytes to be copied
*/


int example_1() {
  const char src[50] = "Apple";
  char dest[50];
  strcpy(dest, "Hello");

  // Output: Hello
  printf("%s\n", dest);

  // Write up to 6 bytes
  // Note: Remember strlen doesn't count \0, so it's 5 characters. So we add + 1
  // to make sure we count the null terminator character.
  memcpy(dest, src, strlen(src) + 1);

  // Output: Apple with null terminator character 
  printf("%s\n", dest);
}

int example_2() {
  char str1 = "Tutorials";
  char str2 = "Point";

  // Copy the content of first_str to sec_str
}