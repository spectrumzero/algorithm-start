#include "../head/linked_list.h"

/*在链表的节点n0后插入节点p*/
void insert(ListNode *n0, ListNode *p) {
  ListNode *n1 = n0->next;
  p->next = n1;
  n0->next = p; // 这里的n1起到的是类似临时变量的作用
}

/*删除链表的节点n0之后的首个节点*/
void removeitem(ListNode *n0) {
  if (!n0->next)
    return; // 如果条件为真，则说明n0->next =
            // null,说明传入的节点已经是最后一个节点，返回。而这里要执行的命令只有一条，可以省略原有的中括号。
  // n0->p->n1
  ListNode *p = n0->next;
  ListNode *n1 = p->next;
  // 删除p节点，就是让p节点无法被找到，无法被“指”到
  n0->next = n1;

  // 释放内存，防止内存泄漏
  free(p); // p所指向的也是一块开在堆区的内存，要主动释放
}

/*访问链表中索引为index的节点*/
ListNode *access(ListNode *head, int index) {
  // 从头节点开始遍历
  for (int i = 0; i < index; i++) {
    if (head == NULL) // 链表不存在
      return NULL;
    head =
        head->next; // 寻下一个节点。这里的“索引”，可以类似数组一样理解，但链表严格来说并没有index的概念
  }
  return head;
}

/*在链表中查找值为target的首个节点*/
int find(ListNode *head, int target) {
  int index = 0;
  while (head) // 判断链表是否存在
  {
    if (head->val == target)
      return index;
    head = head->next;
    index++;
  }
  return -1;
}

int main() {
  /*初始化链表*/
  // 初始化各个节点
  ListNode *n0 = newListNode(1); // 接受生成新节点的函数所返回的节点地址
  ListNode *n1 = newListNode(3);
  ListNode *n2 = newListNode(5);
  ListNode *n3 = newListNode(2);
  ListNode *n4 = newListNode(4);

  // 构建节点之间的引用
  n0->next = n1;
  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  printf(
      "初始化的链表为\r\n"); //"\r"表示回车的转义字符,回车符通常用于将光标移动到当前行的开头。它一般与"\n"一起使用以便兼容不同系统，最终让光标移动到下一行的开头，方便后续的打印
  printLinkedList(n0);       // 从传入的节点开始打印

  /*插入节点*/
  insert(n3,
         newListNode(100)); // 第二个参数表示这个函数所返回的新节点（这里也就是要插入的节点）的地址
  printf("插入节点后的链表为\r\n");
  printLinkedList(n0);

  /*删除节点*/
  removeitem(n2); // 这里是删除传入的节点之后的首个节点
  printf("删除节点后的链表为\r\n");
  printLinkedList(n0);

  /*访问节点*/
  ListNode *node = access(n0, 3); // 需要从头节点开始遍历
  printf("链表中索引3处的节点值 = %d\r\n", node->val);

  /*查找节点*/
  int index = find(n0, 100); // 同样，也是需要从头节点开始遍历
  printf("链表中值为2的节点的索引 = %d\r\n", index);

  // 释放内存
  freeMemoryLinkedList(n0); // 所有的节点都是开在堆区的，需要主动释放内存，同样，也是需要从头开始释放

  // 除了上面那种初始化链表的方式之外，还有利用数组来初始化链表的方式。
  int array[] = {2, 1, 6, 7, 6, 21, 100};
  ListNode *head = ArrToLinkedList(array, 7);
  printf("利用数组初始化的链表为\r\n");
  printLinkedList(head);
  ListNode *p = newListNode(861); // 新创建一个节点（未就位）
  insert(head, p);                // 在链表中插入新创建的节点（已就位）
  printf("插入后的链表为\r\n");
  printLinkedList(head);
  freeMemoryLinkedList(head); // 释放内存

  return 0;
}
