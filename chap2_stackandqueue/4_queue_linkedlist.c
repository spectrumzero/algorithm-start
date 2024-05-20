#include "../head/queue_linkedlist.h"

/*基于链表实现的队列*/
typedef struct
{
  ListNode *front, *rear; // 头节点、尾节点
  int quesize;            // 链表实际大小
} queuelinkedlist;

/*构造函数*/
queuelinkedlist *newqueuelinkedlist()
{
  // 创建一个实例
  queuelinkedlist *queue = (queuelinkedlist *)malloc(sizeof(queuelinkedlist));
  // 完成实例的初始化
  queue->front = NULL;
  queue->rear = NULL;
  queue->quesize = 0;
  return queue;
}

/*析构函数*/
void delqueuelinkedlist(queuelinkedlist *queue)
{
  // 首先释放节点
  while (queue->front != NULL)
  {
    ListNode *tmp = queue->front; // 通过临时变量tmp来管理释放即将释放的节点内存
    queue->front = queue->front->next;
    free(tmp);
  }
  // 然后释放queue结构体
  free(queue);
}

/*获取队列的长度*/
int getlength(queuelinkedlist *queue) { return queue->quesize; }

/*判断队列是否为空*/
bool empty(queuelinkedlist *queue) { return queue->quesize == 0; }

/*入队，在队尾添加值为num的节点*/
void push(queuelinkedlist *queue, int num)
{
  // 在尾节点添加node
  ListNode *node = newListNode(num);
  // 如果队列为空，则让头、尾节点都指向该节点
  if (queue->front == NULL)
  {
    queue->front = node;
    queue->rear = node;
  }
  // 如果队列不为空，则将该节点添加到尾节点后
  else
  {
    queue->rear->next = node; //首先是让目前的最后节点指向新节点，完成链接。省略了这一步，将会使得oldrear->next依旧为null，发生断链，且无法寻找到newrear。
    queue->rear = node;       //然后是让这个新节点实际成为最后的节点。这个时候，newrear->next = null是合法的。
  }
  queue->quesize++;
}

/*访问队首元素*/
int peek(queuelinkedlist *queue)
{
  assert(
      getlength(queue) &&
      queue->front); // 只有当队列不为空且队首元素有意义的时候，才是合法的访问。
  return queue->front->val;
}

/*出队，删除首节点*/
int pop(queuelinkedlist *queue)
{
  int num = peek(queue); // 要出队的元素
  ListNode *tmp = queue->front;
  queue->front = queue->front->next; // 类似删除链表节点的逻辑，先在逻辑上删除,
  free(tmp);                         // 再在内存里删除
  queue->quesize--;
  return num;
} //与基于数组实现的队列不同，链表数组的出队，确实是删除了节点。而前者更像是忽视节点。

/*打印队列*/
void printqueuelinkedlist(queuelinkedlist *queue)
{
  // 拷贝链表中的数据到数组，利用打印数组来打印队列
  int *arr =
      malloc(sizeof(int) * queue->quesize); // 新开辟一块数组,用于存放拷贝的数据
  int i;                                    // 在这里声明i，还蛮巧妙的。因为下面for循环的node，必须要在整个打印函数的作用域内起作用，而不能仅仅是在循环的语句块中起作用。所以，i也应该跟着在整个函数域内起作用。不然for循环写不了。
  ListNode *node;
  for (i = 0, node = queue->front; i < queue->quesize; i++)
  {
    arr[i] = node->val;
    node = node->next;
  }
  printArray(arr, queue->quesize);
  free(arr); // 释放内存
}

/* Driver Code */
int main()
{
  /* 初始化队列 */
  queuelinkedlist *queue = newqueuelinkedlist();

  /* 元素入队 */
  push(queue, 8);
  push(queue, 0);
  push(queue, 2);
  push(queue, 5);
  push(queue, 9);
  printf("队列 queue = ");
  printqueuelinkedlist(queue);

  /* 访问队首元素 */
  int peekNum = peek(queue);
  printf("队首元素 peek = %d\r\n", peekNum);

  /* 元素出队 */
  peekNum = pop(queue);
  printf("出队元素 pop = %d ，出队后 queue = ", peekNum);
  printqueuelinkedlist(queue);

  /*元素入队*/
  int pushnum = 200;
  push(queue, pushnum);
  printf("入队元素 pushnum = %d ，入队后 queue = ", pushnum);
  printqueuelinkedlist(queue);

  /* 获取队列的长度 */
  int queueSize = getlength(queue);
  printf("队列长度 size = %d\r\n", queueSize);

  /* 判断队列是否为空 */
  bool isEmpty = empty(queue);
  printf("队列是否为空 = %s\r\n", isEmpty ? "true" : "false");

  // 释放内存
  delqueuelinkedlist(queue);

  return 0;
}
