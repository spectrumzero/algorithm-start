#include <stdio.h>
#include <stdlib.h>

/*定义链表节点结构体*/
typedef struct ListNode
{
    int val;               //该变量储存这一节点的值
    struct ListNode *next; //该变量指向下一个节点所在的地址

} ListNode; // typedef关键字为结构体起了一个别名，这样可以使我们以后在声明变量时，可以直接使用ListNode,而不必每次都写struct ListNode

/*初始化一个新节点*/
ListNode *newListNode(int val)
{
    ListNode *node;
    node = (ListNode *)malloc(sizeof(ListNode));
    node->val = val;
    node->next = NULL; //在链表中，每个节点都有指向下一个节点的指针。如果这里不去主动设置next指针为空，那么这个值将是不确定的，可能会指向一个随机的地址，进而导致链表可能出现问题。一般期望新创建的指针为链表的最后一个节点。
    return node;       //直接返回新节点的地址
}

/*打印链表*/
void printLinkedList(ListNode *node)
{
    if (node == NULL)
    {
        return; //上述条件表示这个链表不存在
    }
    while (node->next != NULL)
    {
        printf("%d ->", node->val);
        node = node->next; //一直打印到最后一个节点（最后一个节点不打印）
    }
    printf("%d\n", node->val); //打印最后一个节点
}

/*释放链表内存*/
void freeMemoryLinkedList(ListNode *head)
{
    ListNode *pre;
    while (head != NULL)
    {
        pre = head;
        head = head->next;
        free(pre);
    }
    return;
}

/*利用数组来初始化链表*/
ListNode *ArrToLinkedList(const int *arr, size_t size) //这里的const修饰arr指针，表示arr指针指向的内存地址不可修改，也就是原数组不可以修改。但是被转换后的链表是可以改的。此外，这里的size_t表示无符号整数类型，通常表示数组中的索引、循环计数器等等。
{
    if (size <= 0)
        return NULL;
    ListNode *node = newListNode(arr[0]);
    ListNode *head = node;
    for (int i = 1; i < size; i++)
    {
        node->next = newListNode(arr[i]);
        node = node->next; //下一个节点。最后，node->next没有被重新赋值，所以其指向的是一个null值，这也符合我们的原意，表示这是最后一个节点。
    }
    return head;
}
