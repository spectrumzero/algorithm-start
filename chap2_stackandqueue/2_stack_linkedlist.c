#include "../head/stack_linkedlist.h"
/*基于链表实现的栈*/
typedef struct {
  ListNode
      *top; // 将头节点作为栈顶。节点类型的结构体，包括节点值，和指向下一节点的地址。不要忘记了哦。
  int size; // 栈的长度
} linkedliststack;

/*构造函数*/
linkedliststack *newlinkedliststack() {
  linkedliststack *s = malloc(sizeof(linkedliststack)); // 将栈开在堆区
  s->top = NULL; // 将节点值设为null，来设置栈的初始状态为没有节点。
  s->size = 0;   // 没有节点，size等于0
  return s;
}

/*析构 */
void dellinkedliststack(linkedliststack *s) {
  while (s->top) {
    ListNode *n = s->top->next;
    free(s->top);
    s->top = n;
  } // 当栈中还有节点的时候，就会进行释放栈中节点的操作（注意，先释放栈里的节点（节点也是开在堆区的），再释放栈本身。
  free(s);
}

/*获取栈的长度*/
int size(linkedliststack *s) { return s->size; }

/*判断栈是否为空*/
bool isempty(linkedliststack *s) { return size(s) == 0; }

/*入栈*/
void push(linkedliststack *s, int num) {
  ListNode *node = (ListNode *)malloc(sizeof(ListNode)); // 你看，新节点是开在堆区的。
  node->next =
      s->top; // 将新节点的下一节点指向头节点，从而保证了这个新节点成为了新的栈顶,实现栈顶插入,符合定义。
  node->val = num; // 更新新加节点的数据
  s->top = node;   // 实际更新栈顶
  s->size++;       // 更新栈大小
}

/*访问栈顶元素*/
int peek(linkedliststack *s) {
  if (s->size == 0) {
    printf("栈为空\n");
    return INT_MAX;
  }
  return s->top->val;
}

/*出栈*/
int pop(linkedliststack *s) {
  int val = peek(s);      // 记录即将被删除的栈顶元素(即头节点)
  ListNode *tmp = s->top; // 记录头节点所在的内存地址
  s->top = s->top->next; // 将头节点指向的下一个节点作为新的头节点，实现了逻辑上的删除
  // 释放内存
  free(tmp); // 逻辑上的删除和内存的实际释放是两个过程
  s->size--;
  return val;
}

/*drivercode*/
int main() {
  /* 初始化栈 */
  linkedliststack *stack = newlinkedliststack();

  /* 元素入栈 */
  push(stack, 1);
  push(stack, 3);
  push(stack, 2);
  push(stack, 5);
  push(stack, 2);

  printf("栈 stack = ");
  printLinkedList(stack->top); // 从头节点(栈顶）开始打印

  /* 访问栈顶元素 */
  int val = peek(stack);
  printf("栈顶元素 top = %d\r\n", val);

  /* 元素出栈 */
  val = pop(stack);
  printf("出栈元素 pop = %d, 出栈后 stack =  ", val);
  printLinkedList(stack->top);

  /* 获取栈的长度 */
  printf("栈的长度 size = %d\n", size(stack));

  /* 判断是否为空 */
  bool empty = isempty(stack);
  printf("栈是否为空 = %s\n", empty ? "true" : "false");

  // 释放内存
  dellinkedliststack(stack);

  return 0;
}
