#include <stdio.h>

int main()
{
int i = 0;
 int j = 0;

while (i<5)
{
i++;
switch( i)
{
case 2:
continue;
break;
default:
break;
}
printf("%d " ,i);
}
}
