#include "../head/queue_array.h"

/* 基于环形数组实现的双向队列 */
typedef struct {
  int *nums;       // 用于存储队列元素的数组
  int front;       // 队首索引，指向队首元素
  int quesize;     // 实际大小
  int queCapacity; // 队列容量
} dequearray;

/*构造函数*/
dequearray *newdequearray(int capacity) {
  dequearray *deque = (dequearray *)malloc(sizeof(dequearray));
  deque->queCapacity = capacity;
  deque->nums = (int *)malloc(sizeof(int) * deque->queCapacity);
  deque->front = deque->quesize = 0;
  return deque;
}

/* 析构函数 */
void deldequearray(dequearray *deque) {
  free(deque->nums);
  free(deque);
}

/* 获取双向队列的容量 */
int capacity(dequearray *deque) { return deque->queCapacity; }

/* 获取双向队列的长度 */
int size(dequearray *deque) { return deque->quesize; }

/* 判断双向队列是否为空 */
bool empty(dequearray *deque) { return deque->quesize == 0; }

/*计算环形数组索引，即实际要被赋值的位置*/
int dequeindex(dequearray *deque, int i) {
  // 通过取余操作实现数组收尾相连。当i越过数组尾部时，回到头部，当i越过数组头部后，回到尾部。
  // 下面这个表达式的目的，是为了确定队首插入位置front-1和队尾插入位置front+size的实际位置。
  // i+capacity(deque)为负数的情况，可能会发生吗？
  // 在不扩容的情况下,队列的最大容量为cap，front先占据了一个要素，所以剩余(cap-1)个位置，所以插入位置的最小值为front-(cap-1)=front-cap+1。
  // 因此，即便在这种情况下，i+cap也是大于1的。
  // 扩容之后，cap也会发生变化，上述的这种情况也是成立的。索引不可能为负数这样的非法索引。
  return ((i + capacity(deque)) % capacity(deque));
}

/*队首入队*/
void pushfirst(dequearray *deque, int num) {
  if (deque->quesize == capacity(deque)) {
    printf("双向队列已满\r\n");
    return;
  }
  // 队首指针向左移动一位，通过取余操作实现front越过数组头部回到尾部。
  // 计算实际要被赋值的位置。
  deque->front = dequeindex(deque, deque->front - 1);
  // 将num添加到队首
  deque->nums[deque->front] = num;
  deque->quesize++;
}

/*队尾入队*/
void pushlast(dequearray *deque, int num) {
  if (deque->quesize == capacity(deque)) {
    printf("双向队列已满\r\n");
    return;
  }
  // 计算实际要被赋值的位置
  int rear = dequeindex(deque, deque->front + deque->quesize);
  // 将num添加至队尾
  deque->nums[rear] = num;
  deque->quesize++;
}

/* 访问队首元素 */
int peekfirst(dequearray *deque) {
  // 访问异常：双向队列为空
  assert(empty(deque) == 0);
  return deque->nums[deque->front];
}

/* 访问队尾元素 */
int peeklast(dequearray *deque) {
  // 访问异常：双向队列为空
  assert(empty(deque) == 0);
  // 计算实际的队尾元素所在位置，
  int last = dequeindex(deque, deque->front + deque->quesize - 1);
  return deque->nums[last];
}

/*队首出队*/
int popfirst(dequearray *deque) {
  int num = peekfirst(deque); // 记录出队元素
  // 队首索引向后移动一位。
  // 为什么不能直接++？因为这是一个循环数组，我们始终都需要考虑到实际的位置，要让front有实际的意义。
  deque->front = dequeindex(deque, deque->front + 1);
  deque->quesize--;
  return num;
}

/*队尾出队*/
int poplast(dequearray *deque) {
  int num = peeklast(deque);
  deque->quesize--;
  return num;
}

/* Driver Code */
int main() {
  /* 初始化队列 */
  int capacity = 10;
  dequearray *deque = newdequearray(capacity);
  pushlast(deque, 3);
  pushlast(deque, 2);
  pushlast(deque, 5);
  printf("双向队列 deque = ");
  printArray(deque->nums, deque->quesize, deque->front, deque->queCapacity);

  /* 访问元素 */
  int peekfirstNum = peekfirst(deque);
  printf("队首元素 peekfirst = %d\r\n", peekfirstNum);
  int peeklastNum = peeklast(deque);
  printf("队尾元素 peeklast = %d\r\n", peeklastNum);

  /* 元素入队 */
  pushlast(deque, 4);
  printf("元素 4 队尾入队后 deque = ");
  printArray(deque->nums, deque->quesize, deque->front, deque->queCapacity);
  pushfirst(deque, 1);
  pushfirst(deque, 3);
  pushfirst(deque, 9);
  pushfirst(deque, 9);
  printf("元素 1,3,9,9 队首依次入队后 deque = ");
  printArray(deque->nums, deque->quesize, deque->front, deque->queCapacity);

  /* 元素出队 */
  int poplastNum = poplast(deque);
  printf("队尾出队元素 = %d ，队尾出队后 deque= ", poplastNum);
  printArray(deque->nums, deque->quesize, deque->front, deque->queCapacity);
  int popfirstNum = popfirst(deque);
  printf("队首出队元素 = %d ，队首出队后 deque= ", popfirstNum);
  printArray(deque->nums, deque->quesize, deque->front, deque->queCapacity);

  /* 获取队列的长度 */
  int dequesize = size(deque);
  printf("双向队列长度 size = %d\r\n", dequesize);

  /* 判断队列是否为空 */
  bool isEmpty = empty(deque);
  printf("队列是否为空 = %s\r\n", isEmpty ? "true" : "false");

  // 释放内存
  deldequearray(deque);

  return 0;
}
