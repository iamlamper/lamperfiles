#include <stdio.h>

int main()
{
  char buf[5] = "abcdef";
  int n = 0;
  printf("%s", buf);
  fflush(stdout);
  printf("\n----------\n");
  fflush(stdout);
  n = snprintf(buf, 1, "haa");
  printf("%d copyed\n", n);
  printf("%s", buf);
  fflush(stdout);
  return 1;
}
