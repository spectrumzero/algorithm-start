#include "assert.h"
#include <stdbool.h> //bool型需要用到这个标准库
#include <stdio.h>
#include <stdlib.h>

// 循环打印数组的实现。
// 以数组为基础的列表打印，打印的是[front，front+size]区间的索引，且还要考虑到两端超出边界的情况，所以打印方式也会有所不同。
void printArray(int arr[], int size, int front, int cap)
{
  //空队列
  if (arr == NULL || size == 0)
  {
    printf("[]");
    return;
  }
  printf("[");
  //非空队列
  for (int i = 0; i < size - 1; i++)
  {
    printf("%d, ", arr[front]);
    front = (front + 1) % cap; // 保证了跨越的正确。+1是为了打印下一个，而对容量取余则是为了应对跨越边界的情况。超过了边界，则回头打印下标0索到的数值。一种应该记得的操作。
  }
  printf("%d]\t", arr[front]);                                    // 打印完后，在后面添加一个制表符，对齐排版。
  printf("front = %d | rear-1 = %d \n", front, front + size - 1); // 报告打印的区间[front,rear-1]
}
