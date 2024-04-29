#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//打印
void printArray(int *array, int size)
{
    if (array == NULL || size == 0) //数组为null表示数组根本不存在，是一个空指针；而size==0则表示，数组存在，但是里面没有元素。
    {
        printf("[]");
        return;
    }
    printf("[");
    for (int i = 0; i < size - 1; i++)
    {
        printf("%d,", array[i]);
    }
    printf("%d]\n", array[size - 1]);
}