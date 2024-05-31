#include "./heap.c"

/*Driver Code*/

int main() {
  // 初始化大顶堆
  int nums[] = {9, 8, 6, 6, 7, 5, 2, 1, 4, 3, 6, 2};
  maxheap *heap = newheap(nums, sizeof(nums) / sizeof(int));
  printf("输入数组并建堆后\n");
  printheap(heap->data, heap->size);

  // 获取堆顶元素
  printf("\n堆顶元素为 %d\n", peek(heap));

  // 元素入堆
  push(heap, 7);
  printf("\n元素 7 入堆后\n");
  printheap(heap->data, heap->size);

  // 堆顶元素出堆
  int top = pop(heap);
  printf("\n堆顶元素 %d 出堆后\n", top);
  printheap(heap->data, heap->size);

  // 获取堆的大小
  printf("\n堆元素的数量为 %d\n", size(heap));

  // 判断堆是否为空
  printf("\n堆是否为空 %d\n", isempty(heap));

  // 释放内存
  delheap(heap);
}
