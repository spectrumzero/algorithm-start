#include "../utils/common.h"

#define MAX_SIZE 5000

/*大顶堆*/
typedef struct {
  int size;           // 实际元素的个数
  int data[MAX_SIZE]; // 大数组
} maxheap;

/*函数声明*/
void siftdown(maxheap *heap, int i);
void siftup(maxheap *heap, int i);
int parent(maxheap *heap, int i);

/*构造函数*/
maxheap *newheap(int nums[], int size) {
  // 所有元素入堆
  maxheap *heap = (maxheap *)malloc(sizeof(maxheap));
  heap->size = size;
  // 一次性拷贝所有元素入堆
  memcpy(heap->data, nums, size * sizeof(int));
  for (int i = parent(heap, size - 1); i >= 0; i--) {
    // 堆化除叶节点以外的其它所有节点，依次对【每个】非叶节点执行从顶至底堆化，进而保证所有以非叶节点为根节点的子树都是符合堆的定义的。进而完成堆的初始化。相对于其它的初始化，堆的初始化要更加麻烦一点，
    // 建一个合法的堆，是今后正确执行添加、删除节点操作的基础。
    // 可以理解，这个堆是从下到上构建的。(对每一个节点，是从上到下完成堆化的)
    // 如果先建立一个空堆，然后依次（而不是像这样一次性地）将元素入堆，然后对该元素进行从底至顶堆化，这样构建的堆是从上到下构建的。构建是会更麻烦一点。
    siftdown(heap, i);
    // 另外，堆的性质完全二叉树性质叶告诉我们，parent(heap,size-1)在非叶节点中的索引值，是最大的。
    // 另外，这是for循环的初始化条件，只在循环开始的时候执行一次。条件部分和迭代部分则在每次循环的时候都会检查和执行。
  }
  return heap;
}

/*析构函数*/
void delheap(maxheap *heap) {
  // 释放内存
  free(heap);
}

/*获取当前节点的左子节点的索引*/
int left(maxheap *heap, int i) { return 2 * i + 1; }

/*获取当前节点的右子节点的索引*/
int right(maxheap *heap, int i) { return 2 * i + 2; }

/*获取当前节点的父节点的索引*/
int parent(maxheap *heap, int i) { return (i - 1) / 2; }

/*利用索引来交换元素*/
void swap(maxheap *heap, int i, int j) {
  int tmp = heap->data[i];
  heap->data[i] = heap->data[j];
  heap->data[j] = tmp;
}

/*获取堆的大小*/
int size(maxheap *heap) { return heap->size; }

/*判断堆是否为空*/
int isempty(maxheap *heap) { return heap->size == 0; }

/*访问堆顶元素*/
int peek(maxheap *heap) { return heap->data[0]; }

/*元素入堆*/
void push(maxheap *heap, int val) {
  if (heap->size == MAX_SIZE) {
    printf("heap is full!");
    return;
  }
  // 添加节点
  heap->data[heap->size] = val;
  heap->size++;

  // 从底至顶堆化
  siftup(heap, heap->size - 1);
}

/*元素出堆*/
int pop(maxheap *heap) {
  if (isempty(heap)) {
    printf("heap is Empty!");
    return INT_MAX;
  }
  // 交换根节点与最右节点（交换首元素与尾元素）
  swap(heap, 0, size(heap) - 1);
  // 记录删除的节点
  int val = heap->data[heap->size - 1];
  // 利用size直接删除
  heap->size--;
  // 从顶至底堆化
  siftdown(heap, 0);

  // 返回堆顶元素
  return val;
}

/*从索引i开始，从顶至底堆化*/
void siftdown(maxheap *heap, int i) {
  while (true) {
    // 判断节点i,l,r中值最大的节点(的索引)，记为MAX
    int l = left(heap, i);
    int r = right(heap, i);
    int max = i;

    if (l < size(heap) && heap->data[l] > heap->data[max]) {
      max = l;
    }
    if (r < size(heap) && heap->data[r] > heap->data[max]) {
      max = r;
    }
    // 若节点i最大火者索引l或r越界，则无需继续堆化，跳出。
    if (max == i) {
      break;
    }
    // 否则交换两节点，即交换两个索引的值，让i保存的值是最大的。
    swap(heap, i, max);
    // 循环向下堆化，i = r或l，直接向子节点方向
    i = max;
  }
}

/*从节点i开始，从底至顶堆化*/
void siftup(maxheap *heap, int i) {
  while (true) {
    // 获取节点i的父节点
    int p = parent(heap, i);
    // 当越过根节点或者节点无需修复时，结束堆化
    if (p < 0 || heap->data[i] <= heap->data[p]) {
      break;
    }
    // 交换两个节点
    swap(heap, i, p);
    // 循环向上堆化，直接向父节点方向
    i = p;
  }
}

/*print a heap*/
void printheap(int *arr, int size) {
  TreeNode *root;
  printf("堆的数组表示：\n");
  printArray(arr, size);
  printf("堆的二叉树表示：\n");
  root = arrayToTree(arr, size);
  printTree(root);
}

/*Driver Code*/
int main() {
  // 初始化大顶堆
  int nums[] = {9, 8, 6, 6, 7, 5, 2, 1, 4, 3, 6, 2, 10, 1, 20, 11};
  maxheap *heap = newheap(nums, sizeof(nums) / sizeof(int));
  printf("输入数组并建堆后\n");
  printheap(heap->data, heap->size);

  // 获取堆顶元素
  printf("\n堆顶元素为 %d\n", peek(heap));

  // 元素入堆
  push(heap, 19);
  printf("\n元素 19 入堆后\n");
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
