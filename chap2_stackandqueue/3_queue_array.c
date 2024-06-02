#include "../head/queue_array.h"
/*基于环形数组实现的队列*/
typedef struct {
  int *nums; // 用于存储管理数组，数组中的元素即为队列的元素。
  int front;
  // 队首指针，指向队首元素。也就是数组的首元素。为什么这里要特意地声明一个变量作为首元素呢？难道数组的首元素的下标不都是0吗？
  // 这种说法确实不错，但是，这样就使得队列的出队操作所需要的时间复杂度为O(n)。队列是在队首出列的，即通过删除数组的首元素来实现出列操作。
  // 所以有一种更巧妙的办法。基于数组的队列，其入队和出队的操作，都像是数组的一种整体移动。因此，其首索引和尾索引其实是在不断变化。
  // 那么，设置首索引为front，使得队列元素的实际区间为[front,front+quesize-1],这样，通过对front的管理，使得这种移动成为可能。
  // 要删除首元素，只需要将front+1即可。
  int quesize;     // 尾部指针，指向队尾+1。别忘了队尾元素是queusize-1
  int quecapacity; // 队列容量
  int extendratio; // 引进扩容机制，实现动态数组
} arrayqueue;

/*构造函数，在c语言中，结构体成员的初始化是不被允许的，只能通过构造函数来完成属性的初始化*/
arrayqueue *newarrayqueue(int capacity) {
  arrayqueue *queue = (arrayqueue *)malloc(
      sizeof(arrayqueue)); // 熟悉的malloc函数。同样的，将新的队列开在堆区，利用指针进行管理。
  // 初始化数组。
  queue->quecapacity = capacity; // 数组的容量，即队列可容纳的元素容量
  queue->nums = (int *)malloc(
      sizeof(int) * queue->quecapacity); // nums为指针型变量。单个元素的大小*可容纳的元素量
                                         // = 应该开辟的内存大小
  queue->front = queue->quesize =
      0; // 刚开始，队列中没有任何的元素。queuesize设为0。首下标当然是0。一种简便的写法。
  queue->extendratio = 2;
  return queue;
}

/*析构函数*/
void delarrayqueue(arrayqueue *queue) {
  free(queue->nums);
  free(queue); // 同样的，先释放数组容器，再释放队列。
}

/*获取队列的容量*/
int capacity(arrayqueue *queue) { return queue->quecapacity; }

/*获取队列的实际长度*/
int size(arrayqueue *queue) { return queue->quesize; }

/*判断队列是否为空*/
bool empty(arrayqueue *queue) { return queue->quesize == 0; }

/*访问队首元素*/
int peek(arrayqueue *queue) {
  assert(size(queue) != 0); // 不为空才继续下去，需要括号内为真。这里是通过size函数来判断的。
  return queue->nums[queue->front];
}

/*引进扩容机制*/
void extendarrayqueue(arrayqueue *queue) {
  // 新的容量
  int newcap = (queue->quecapacity) * queue->extendratio;
  // 开辟新的数组，存放拷贝过来的元素。
  int *newnums = (int *)malloc(sizeof(int) * newcap);
  // 方便后续对旧数组的释放。
  int *tmp = queue->nums;
  // 所谓的扩容，本质上是用大的容器代替小的容器，所以需要将原数组中的数据拷贝进新的数组。
  // 这是一个循环数组，所以拷贝方式也有所不同。在旧的列表里，我们实际关心的是[front,front+size-1]这一有意义的区间。
  // 所以本质上，我需要拷贝的是[front,front+size-1]，而不是固定的[0,size-1];
  int k = 0;              // 计算要拷贝的次数（元素总量size-1）
  int idx = queue->front; // 我们实际要拷贝的元素的下标
  while (k < queue->quesize) {
    // 从front开始拷贝，然后一直遍历到front+size-1。这里还需要考虑到边界问题，取余即可。
    newnums[idx % queue->quecapacity] = queue->nums[idx % queue->quecapacity];
    k++;   // k最后会加到size，这个时候就退出循环了。
    idx++; // 所以idx实际上加了k次，即size-1次，最后的有效数值为size-1。换言之，当k=size-1时，idx=front+size-1，随后k=size或idx
           // = front+size的时候，就退出了循环。
  }
  // 释放原数组内存
  free(tmp);
  // 更新列表
  queue->nums = newnums;
  queue->quecapacity = newcap;
}

/*入队,在对尾（front+size-1）添加num*/
// 理解入队的关键主要是理解在何处赋值。
void push(arrayqueue *queue, int num) {
  if (size(queue) == capacity(queue)) {
    // printf("队列已满\r\n");
    // return;
    printf("队列已满，触发扩容机制\r\n");
    extendarrayqueue(queue);
    printf("扩容后队列的容量为：%d\n", queue->quecapacity);
  }
  // 请注意，前面已经提到，基于数组的队列的入队和出队，是一种整体移动，像贪吃蛇一样。
  // 那么，当队尾front+size-1越过了它的最大值，即quecapacity-1时，此时应该怎么办呢？或者说，此时要添加新值，但添加的时候发现新的边界front+size已经超过合法边界了，怎么办呢？
  // 我们可以通过取余操作实现rear（front+size）越过数组尾部后回到头部,从而实现一种循环效果。
  // 原区间[front,rear-1],新区间[front+1,rear],当新区间rear等于cap，则实际被赋值的是nums[0]。front和rear都更新了，所以还是写成[front,rear-1]。rear是表示要添加的值。是将来的尾部，而不是现在的尾部。
  int rear =
      (queue->front + queue->quesize) %
      queue
          ->quecapacity; // 是取余，而不是取商。另外，rear是新添的元素，所以其表示的其实是原队列的最后一个元素的下一个元素。换言之，每次打印的有效区间其实是[front,rear-1]，即[front,front+size-1]

  // 将num添加至队尾
  queue->nums[rear] = num;
  queue->quesize++;
  // 值得一提的是，一出一入，只是在强调了队列的整体移动，这并没有改变队列的实际大小。换句话说，队列的实际大小是另一个需要考虑的问题。
  // 有必要当队列的实际大小等于可容纳的大小时，在添加新元素时，引进扩容机制，实现动态数组。写在上面。
}

/*出队*/
int pop(arrayqueue *queue) {
  int num = peek(queue);
  // 队首索引向后移动一位，若越过尾部，则返回到数组头部，和上面的入队是一个道理。front的最大值也只能为quecapacity-1。
  // 这样向后移动一位，指的是在逻辑上忽视了原来的front，并要从front+1开始打印。也就是说，num[0]还是那个值。只不过我现在不打印num[0]了，而是打印num[1]，从num[1]开始打印。num[0]我只在有必要（入队元素越过边界的时候）重新赋值。
  // 因此，原来的打印函数是有误的。要重写。
  queue->front = (queue->front + 1) % queue->quecapacity; // front+1的威力，就是整体平移。
  queue->quesize--;
  return num;
}

/* Driver Code */
int main() {
  /* 初始化队列 */
  int capacity = 5; // 初始容量为5
  arrayqueue *queue = newarrayqueue(capacity);

  /* 元素入队 */
  push(queue, 9);
  push(queue, 3);
  push(queue, 2);
  push(queue, 1);
  push(queue, 8);
  printf("队列 queue = ");
  printArray(
      queue->nums, queue->quesize, queue->front,
      queue
          ->quecapacity); // 依旧是通过数组打印函数来打印队列。先传入数组的首地址，再传入数组的大小，从头开始打印。

  /*测试扩容机制*/
  push(queue, 4);
  printf("队列 queue = ");
  printArray(queue->nums, queue->quesize, queue->front, queue->quecapacity);

  /* 访问队首元素 */
  int peekNum = peek(queue);
  printf("队首元素 peek = %d\r\n", peekNum);

  /* 元素出队 */
  peekNum = pop(queue);
  printf("出队元素 pop = %d ，出队后 queue = ", peekNum);
  printArray(queue->nums, queue->quesize, queue->front, queue->quecapacity);

  /*元素入队*/
  int pushnum = 900;
  push(queue, pushnum);
  printf("入队元素 push = %d ，入队后 queue = ", pushnum);
  printArray(queue->nums, queue->quesize, queue->front, queue->quecapacity);

  /* 获取队列的长度 */
  int queueSize = size(queue);
  printf("队列长度 size = %d\r\n", queueSize);

  /* 判断队列是否为空 */
  bool isEmpty = empty(queue);
  printf("队列是否为空 = %s\r\n", isEmpty ? "true" : "false");

  /*打印原来的nums[0],即最初始的front。可以看到还是可以找到。*/
  printf("原来的queue[0]为：%d\n", queue->nums[0]);

  /* 测试环形数组 */
  for (int i = 0; i < 21; i++) {
    push(queue, i);
    pop(queue);
    printf(
        "第 %d 轮入队 + 出队后 queue = ",
        i); // 在第3轮入队的时候，是num[0]被重新赋值为3。num[10]只是在逻辑上合法，在区间上合法，但是语法不合法。
    printArray(queue->nums, queue->quesize, queue->front, queue->quecapacity);
  } // 以front为中心来理解。

  // 打印现在的nums[0]
  printf("现在的queue[0]为：%d\n",
         queue->nums[0]); // 的确如此。入队就是一个赋值、在哪赋值的问题。

  // 释放内存
  delarrayqueue(queue);

  return 0;
}
