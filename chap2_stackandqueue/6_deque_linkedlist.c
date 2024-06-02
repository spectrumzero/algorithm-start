#include "../head/deque_linkedlist.h"
/*在双向链表节点的基础上，实现双向队列*/
typedef struct {
  doublelistnode *front, *rear; // 头节点front，尾节点rear。好了，这就是链表了，链表就是节点的链接。
  int quesize; // 双向队列的实际长度
} dequelinkedlist;

/*构造队列*/
dequelinkedlist *newdequelinkedlist() {
  dequelinkedlist *deque = (dequelinkedlist *)malloc(sizeof(dequelinkedlist));
  deque->front = NULL;
  deque->rear = NULL;
  deque->quesize = 0;
  return deque;
}

/*析构函数*/
void deldequelinkedlist(dequelinkedlist *deque) {
  // 释放所有节点
  for (int i = 0; i < deque->quesize && deque->front != NULL; i++) {
    doublelistnode *tmp = deque->front;
    deque->front = deque->front->next;
    free(tmp);
  }
  // 释放deque结构体
  free(deque);
}

/*获取队列的长度*/
int size(dequelinkedlist *deque) { return deque->quesize; }

/* 判断队列是否为空 */
bool empty(dequelinkedlist *deque) { return (size(deque) == 0); }

/*入队*/
void push(dequelinkedlist *deque, int num, bool isfront) {
  // 首先是创建入队的节点
  doublelistnode *node = newdoublelistnode(num);
  // 若链表为空，则命令front和rear都指向node，表示第一个节点
  if (empty(deque)) {
    deque->front = deque->rear = node; // front和deque就不再是null了。
  }
  // 队首元素入队操作
  else if (isfront) {
    // 将node添加至链表头部
    deque->front->prev = node; // 将front->prev(NULL)赋值为node。方向一。
    node->next = deque->front; // 只需要在后面建立链接，不需要管前面，使其为NULL。方向二。
    deque->front = node; // 最后别忘了更新头节点
  }
  // 队尾入队操作
  else {
    // 将node添加至链表队尾
    deque->rear->next = node; // 将rear->next(NULL)赋值为node;方向一。
    // 这是双向节点，不是单向节点。所以光上面的单向链接是不够的。
    node->prev = deque->rear; // 只需要在前面建立链接，不需要管后面。任其为NULL。方向二。
    deque->rear = node;
  }
  deque->quesize++; // 更新队列长度
}

/*队首入队*/
void pushfirst(dequelinkedlist *deque, int num) {
  push(deque, num, true); // 还可以这样的。
}

/*队尾入队*/
void pushlast(dequelinkedlist *deque, int num) { push(deque, num, false); }

/*访问队首元素*/
int peekfirst(dequelinkedlist *deque) {
  assert(size(deque) && deque->front);
  return deque->front->val;
}

/*访问队尾元素*/
int peeklast(dequelinkedlist *deque) {
  assert(size(deque) && deque->rear);
  return deque->rear->val;
}

/*出队*/
int pop(dequelinkedlist *deque, bool isfront) {
  if (empty(deque))
    return -1; // 简要写法
  int val;     // 用以暂存,且应该作用于整个函数域
  // 队首出队
  if (isfront) {
    val = peekfirst(deque);
    doublelistnode *fnext = deque->front->next;
    // 判断原队首指向的下一个节点是否为null，若是，表示front是原队列的唯一一个/最后一个节点，跳过判断，直接将front赋值为null即可，表示删除。
    // 除此之外，空指针null->???会导致未定义的行为。即它不应该指向任何东西。
    if (fnext) {
      fnext->prev = NULL; // 表示fnext节点为首节点，往前不指向任何东西。断绝和front的关联。方向一。
      deque->front->next =
          NULL; // front->frev本身是NULL，所以不去管，这里则彻底让原front在逻辑上断链了。方向二。
      deldoublelistnode(deque->front); // 内存上清理
    }
    deque->front =
        fnext; // 如果只有一个节点，那么在出队之后，front被置为NULL了。体现出节点初始化的重要性。
  }
  // 队尾出队操作
  else {
    val = peeklast(deque);
    doublelistnode *rpre = deque->rear->prev; // 命名还蛮讲究的
    if (rpre) {
      rpre->next = NULL; // 表示rpre节点为尾节点。方向一。
      deque->rear->prev =
          NULL; // rear->next本身是NULL，所以不去管，这里让原rear在逻辑上彻底断链。方向二。
      deldoublelistnode(deque->rear);
    }
    deque->rear = rpre;
  }
  deque->quesize--;
  return val;
}

/*队首出列*/
int popfirst(dequelinkedlist *deque) { return pop(deque, true); }

/*队尾出列*/
int poplast(dequelinkedlist *deque) { return pop(deque, false); }

/*打印队列*/
void printdequelinkedlist(dequelinkedlist *deque) {
  int *arr = malloc(sizeof(int) * deque->quesize);
  int i;
  doublelistnode *node;
  for (i = 0, node = deque->front; i < deque->quesize; i++) {
    arr[i] = node->val;
    node = node->next;
  }
  printArray(arr, deque->quesize);
  free(arr);
}

/* Driver Code */
int main() {
  /* 初始化双向队列 */
  dequelinkedlist *deque = newdequelinkedlist(); // 都这样实例化的啊。
  pushlast(deque, 6);
  pushlast(deque, 2);
  pushlast(deque, 7);
  printf("双向队列 deque = ");
  printdequelinkedlist(deque);

  /* 访问元素 */
  int peekfirstNum = peekfirst(deque);
  printf("队首元素 peekfirst = %d\r\n", peekfirstNum);
  int peeklastNum = peeklast(deque);
  printf("队尾元素 peeklast = %d\r\n", peeklastNum);

  /* 元素入队 */
  pushlast(deque, 4);
  printf("元素 4 队尾入队后 deque =");
  printdequelinkedlist(deque);
  pushfirst(deque, 1);
  printf("元素 1 队首入队后 deque =");
  printdequelinkedlist(deque);

  /* 元素出队 */
  int poplastNum = poplast(deque);
  printf("队尾出队元素 poplast = %d ，队尾出队后 deque = ", poplastNum);
  printdequelinkedlist(deque);
  int popfirstNum = popfirst(deque);
  printf("队首出队元素 popfirst = %d ，队首出队后 deque = ", popfirstNum);
  printdequelinkedlist(deque);

  /* 获取队列的长度 */
  int dequeSize = size(deque);
  printf("双向队列长度 size = %d\r\n", dequeSize);

  /* 判断队列是否为空 */
  bool isEmpty = empty(deque);
  printf("双向队列是否为空 = %s\r\n", isEmpty ? "true" : "false");

  // 释放内存
  deldequelinkedlist(deque);

  // 测试
  dequelinkedlist *deque2 = newdequelinkedlist();
  printf("新队列的大小为：%d  ", deque2->quesize);
  pushfirst(deque2, 300);
  printf("新队列的大小为：%d\n", deque2->quesize);
  printf("新队列的头节点的值为：%d  ", deque2->front->val);
  printf("新队列的尾节点的值为：%d", deque2->rear->val);

  return 0;
}
