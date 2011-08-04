#include <stdio.h>
#include <string.h>

int main()
{
  char a[4] = "abc";
  char b[] = "1234567890";
  snprintf(b, 4, "%s", a);
  printf("%s", b);
}
