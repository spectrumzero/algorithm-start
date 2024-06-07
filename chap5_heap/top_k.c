#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 5000

/*大顶堆结构体*/
typedef struct maxheap {
  int size;
  int data[MAX_SIZE];
} maxheap;

int parent(maxheap *heap, int i);
void siftdown(maxheap *heap, int i);

/*堆化*/
maxheap *newheap(int *nums, int size) {
  // 先不管顺序地将元素全部放进堆里
  maxheap *heap = (maxheap *)malloc(sizeof(maxheap)); // 这一步千万别忘
  heap->size = size;
  // memcpy这里的作用在于复制，存有复制元素的heap->data相对于nums是独立的副本。
  // 而heap->data=nums的作用，是让它们两个指向同一块内存，其实没有起到复制的作用。nums和data是相互关联的。
  // 此外，在c语言中，数组类型是不能被赋值的。
  // heap->data = nums;
  memcpy(heap->data, nums, size * sizeof(int));
  // 然后利用堆化来符合定义地建堆，堆化每一个非叶节点下的树
  for (int i = parent(heap, size - 1); i >= 0; i--) {
    siftdown(heap, i);
  }
  return heap;
}
/*趁着堆化，再补充一点数组：
 int nums[size];和
 int*
 nums;还是很不一样的。单独说一个nums，那么的确，它们都表示一个地址。但是对于前者来说，这种地址表示是退化后的结果，而不是其本质含义。
 准确来说，第一种写法，nums本质上是一个数组，是一个内存空间。而第二种写法里的nums，表示的是指向一块内存空间的指针——因此才有malloc大法。
 第一种写法的nums，当它退化为一个地址的时候，也只能是作为右值，而不能是左值。

 至于int *res = (int*)malloc(100*sizeof(int)); res[i] = a;
 这样的写法，其实也可以对应地理解为指针res进化的后果，因为指向了一块具有实际大小的内存块(数组)了嘛。这就是被显式分配内存后，res所具备的自信。

 另， 数组声明时自动分配内存，生命周期与作用域相同。
 */

/*判断堆是否为空*/
int isempty(maxheap *heap) { return heap->size == 0; }

/*访问堆顶元素*/
int peek(maxheap *heap) { return heap->data[0]; }

/*交换元素*/
void swap(maxheap *heap, int i, int j) {
  int tmp = heap->data[i];
  heap->data[i] = heap->data[j];
  heap->data[j] = tmp;
}

/*分别获取当前节点的左子节点、右子节点、父节点的索引(前提：堆是一种完全二叉树，可以很方便地用数组索引来一一对应地表示其中的元素)*/
int left(maxheap *heap, int i) { return 2 * i + 1; }
int right(maxheap *heap, int i) { return 2 * i + 2; }
int parent(maxheap *heap, int i) { return (i - 1) / 2; }

/*从索引为i的元素开始从顶至底堆化（删除）*/
void siftdown(maxheap *heap, int i) {
  while (true) {
    int l = left(heap, i);
    int r = right(heap, i);
    int max = i;
    if (l < heap->size && heap->data[l] > heap->data[max]) {
      max = l;
    }
    if (r < heap->size && heap->data[r] > heap->data[max]) {
      max = r;
    }
    // 若当前节点（父节点）i最大(没有发生过改变)，或索引l和r越界，则无需继续堆化，跳出
    if (max == i) {
      break;
    }
    // 否则，先交换当前的父节点与其子节点的值，让父节点的值始终保持最大
    // 将i的值（小）和max（大）的值交换，实现i大max小，让子节点的值保持更小
    swap(heap, i, max);
    // 然后，再把子节点作为当前节点（父节点），这样向下遍历。就是node->left的感觉
    i = max;
  }
  // 具体到实际的操作中，从顶至底堆化之前，顶节点已经和底节点交换了，底节点的值是属于最后一层的那个水平
}

/*从索引为i的元素开始，从底至顶堆化（添加）*/
void siftup(maxheap *heap, int i) {
  while (true) {
    int p = parent(heap, i);
    if (p < 0 || heap->data[i] <= heap->data[p]) {
      // 当越过根节点或节点无需修复时，结束堆化
      break;
    }
    // 将i（大）的值和p（小）的值交换，实现p大i小，恢复堆的性质
    swap(heap, i, p);
    // 循环向上堆化，就是类似node->pre
    i = p;
  }
  // 同样的，具体到实际的操作中，这是为了应对向堆底的添加操作。
}

/*元素出堆*/
int pop(maxheap *heap) {
  if (isempty(heap)) {
    printf("heap is empty!");
    return INT_MAX;
  }
  // 交换首元素和尾元素
  swap(heap, 0, heap->size - 1);
  // 从堆底删除节点
  int val = heap->data[heap->size - 1];
  heap->size--;
  // 从顶至底堆化
  siftdown(heap, 0);
  // 返回堆顶元素
  return val;
}

/*元素入堆*/
void push(maxheap *heap, int val) {
  // 默认情况下，不应该添加这么多节点
  if (heap->size == MAX_SIZE) {
    printf("heap is Full!");
    return;
  }
  heap->data[heap->size] = val;
  heap->size++;
  // 从底至顶堆化
  siftup(heap, heap->size - 1);
}

/*析构函数*/
void delheap(maxheap *heap) { free(heap); }

/**/
/*******top k问题，给定一个长度为n的无序数组，要求返回数组中最大的k个元素*******/
/**/

/*元素入堆*/
void pushminheap(maxheap *heap, int val) {
  // 对传入的元素取相反数，然后将相反数入小顶堆，绝对值小顶堆。
  push(heap, -val);
}

/*元素出堆*/
int popminheap(maxheap *heap) {
  // 问就是装成是小顶堆。返回的元素是对实际负数取正后的结果。
  // 负进，正出
  return -pop(heap);
}

/*访问堆顶元素*/
int peekminheap(maxheap *heap) {
  // 取出大顶堆的堆顶元素(都是负数)，然后取相反数，构成小顶堆的堆顶。小顶堆，在看作绝对值的时候才是小顶堆
  // 负进正出
  return -peek(heap);
}

/*取出顶堆中所有元素*/
int *getminheap(maxheap *heap) {
  // heap全是负数，且依旧符合大顶堆定义。但我们最后想要的是小顶堆。而原顶堆只有在被看成绝对值的时候，才是小顶堆。
  int *res = (int *)malloc(heap->size * sizeof(int));
  for (int i = 0; i < heap->size; i++) {
    // 对大顶堆中的元素（负数）全部取反，作为小顶堆的元素。放进打印的数组中
    res[i] = -heap->data[i];
  }
  return res;
}

/*基于堆查找数组中最大的k个元素的函数*/
int *topk(int *nums, int size, int k) {
  // 初始化小顶堆（伪）：我们将原来的大顶堆中的所有元素取反，从而用大顶堆来模拟小顶堆
  int *empty = (int *)malloc(0);
  // 初始化一个空堆(你看，这样的建堆，就是从上而下的)
  maxheap *heap = newheap(empty, 0);
  // 将数组的前k个元素入堆(包括第k个)
  for (int i = 0; i < k; i++) {
    pushminheap(heap, nums[i]);
  }
  // 以上，从绝对值的角度来看，其小顶堆的结构已经成立了。但这种成立，只有看成绝对值的时候成立。本质上，其实还是一个大顶堆。
  // 从第k+1个元素开始，保持堆的长度为k
  for (int i = k; i < size; i++) {
    // 若当前元素大于堆顶元素，则将堆顶元素出堆，当前元素入堆。在我们已经实现了小顶堆里，堆顶是最小的，
    // 如果一个元素比最小还小，那么就可以跳过不用考虑了，所以，一般用小顶堆的数据结构来解决topk问题。
    // 但这里用的本质上是大顶堆，所以还多了很多转换的步骤，看起来没有那么直接。
    if (nums[i] > peekminheap(heap)) {
      // 这里的heap已经全部都是负数哦，是从绝对值的角度看的小顶堆。
      // 所以以上的这个条件表达式，其实是两个正数在比较。
      popminheap(heap);           // 正出
      pushminheap(heap, nums[i]); // 负进
    }
  }
  // 获取小顶堆中的所有元素，正正正。
  int *res = getminheap(heap);
  // 释放内存，因为我要的只是存储遍历结果的res。
  delheap(heap);
  return res;
}

/*打印函数*/
void printarray(int *nums, int size) {
  if (nums != NULL && size > 0) {
    printf("[");
  } else {
    printf("数组不存在或数组为空！");
  }
  for (int i = 0; i < size - 1; i++) {
    printf("%d,", nums[i]);
  }
  printf("%d]", nums[size - 1]);
}

/*driver code*/
int main() {
  int nums[] = {9, 7, 6, 13, 2, 1, 10};
  int k = 5;
  int size = sizeof(nums) / sizeof(nums[0]);

  int *res = topk(nums, size, k);
  printf("最大的 %d个元素为：", k);
  printarray(res, k);

  free(res);
  return 0;
}
