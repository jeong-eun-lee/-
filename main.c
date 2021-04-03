#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a[5]={1,2,3,4,5};
    printf("%d, %d\n", a[0], a[1]);
    printf("%d, %d\n", &a[0], &a[1]);
    printf("배열이름: %d\n", a);
    return 0;
}
