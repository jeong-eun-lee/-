#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *pa;
    int a=256;
    pa=(char*)&a;
    printf("pa: %d \n", pa);
    printf("*pa: %d \n", *pa);
    return 0;
}
