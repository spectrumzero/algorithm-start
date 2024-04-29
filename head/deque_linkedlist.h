#include "array.h"
#include <assert.h>
#include <stdbool.h>

/*双向链表节点，定义结构体*/
typedef struct doublelistnode {
  int val; // 节点值
  struct doublelistnode *
      next; // 后继节点,在结构体的定义中，struct关键字是不可以省略的。此外，也不要省略"typedef
            // struct
            // doublelistnode"里的doublelistnode，避免后面可能识别不出的情况。
            // 即标签应该写出来，且与别名一致。
  struct doublelistnode *prev; // 前驱节点
} doublelistnode;

/*构造函数，构造的是节点，往往通过传入节点值来进行初始化*/
doublelistnode *newdoublelistnode(int num) {
  doublelistnode *newnode = (doublelistnode *)malloc(sizeof(doublelistnode));
  newnode->val = num;
  newnode->prev = NULL;
  newnode->next = NULL;
  return newnode;
}

/*析构函数,析构节点*/
void deldoublelistnode(doublelistnode *node) { free(node); }
