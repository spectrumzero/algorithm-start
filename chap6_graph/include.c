#include "../utils/common.h"
#define MAX_SIZE 100

/*顶点结构体*/
typedef struct vertex {
  int num; // 顶点的值
} vertex;

/*构造顶点函数*/
vertex *newvertex(int val) {
  vertex *vet = (vertex *)malloc(sizeof(vertex));
  vet->num = val;
  return vet;
}

/*将值数组转换为顶点数组*/
vertex **valstovets(int *nums, int size) {
  vertex **vet = (vertex **)malloc(size * sizeof(vertex *));
  for (int i = 0; i < size; ++i)
    vet[i] = newvertex(nums[i]);
  return vet;
}

/*将顶点数组转换为值数组*/
int *vetstovals(vertex **vet, int size) {
  int *vals = (int *)malloc(size * sizeof(int));
  for (int i = 0; i < size; ++i)
    vals[i] = vet[i]->num;
  return vals;
}

/*链表头节点结构体*/
typedef struct adjlistnode {
  vertex *vet;              // 顶点
  struct adjlistnode *next; // 后继节点
} adjlistnode;

/*基于邻接表实现的无向图类*/
typedef struct graph {
  adjlistnode *heads[MAX_SIZE]; // 由头节点组成的数组
  int size;                     // 头节点的数量
} graph;

/*构造函数*/
graph *newgraph() {
  graph *mygraph = (graph *)malloc(sizeof(graph));
  mygraph->size = 0;
  for (int i = 0; i < MAX_SIZE; i++)
    mygraph->heads[i] = NULL;
  return mygraph;
}

/*析构函数*/
void delgraph(graph *mygraph) {
  for (int i = 0; i < mygraph->size; i++) {
    adjlistnode *cur = mygraph->heads[i];
    while (cur != NULL) {
      adjlistnode *next = cur->next;
      if (cur != mygraph->heads[i])
        // 暂时不要释放头节点
        free(cur);
      cur = next;
    }
    free(mygraph->heads[i]->vet);
    free(mygraph->heads[i]);
  }
  free(mygraph);
}

/*查找顶点所对应的头节点并返回，以方便链表的遍历*/
adjlistnode *findnode(graph *mygraph, vertex *vet) {
  for (int i = 0; i < mygraph->size; i++) {
    if (mygraph->heads[i]->vet == vet)
      return mygraph->heads[i];
  }
  return NULL;
}

/*添加边辅助函数，在某一条链表中添加新的引用关系*/
void addedgehelper(adjlistnode *head, vertex *vet) {
  adjlistnode *node = (adjlistnode *)malloc(sizeof(adjlistnode));
  node->vet = vet;
  node->next = head->next;
  head->next = node;
}

/*添加边，即在顶点1和顶点2这两个顶点引领的链条中添加新的引用关系，1中有2,2中有1*/
void addedge(graph *mygraph, vertex *vet1, vertex *vet2) {
  adjlistnode *head1 = findnode(mygraph, vet1);
  adjlistnode *head2 = findnode(mygraph, vet2);
  assert(head1 != NULL && head2 != NULL && head1 != head2);
  // 添加边
  addedgehelper(head1, vet2);
  addedgehelper(head2, vet1);
}

/*删除边辅助函数*/
void removeedgehelper(adjlistnode *head, vertex *vet) {
  adjlistnode *pre = head;
  adjlistnode *cur = head->next;
  while (cur != NULL && cur->vet != vet) {
    pre = cur;
    cur = cur->next;
  }
  if (cur == NULL) {
    return;
  }
  // 否则
  pre->next = cur->next;
  free(cur);
}

/*删除边，依旧是双向删除*/
void removeedge(graph *mygraph, vertex *vet1, vertex *vet2) {
  adjlistnode *head1 = findnode(mygraph, vet1);
  adjlistnode *head2 = findnode(mygraph, vet2);
  assert(head1 != NULL && head2 != NULL && head2 != head1);
  // 删除
  removeedgehelper(head1, vet2);
  removeedgehelper(head2, vet1);
}

/*添加顶点*/
void addvertex(graph *mygraph, vertex *vet) {
  assert(mygraph != NULL && mygraph->size < MAX_SIZE);
  adjlistnode *head = (adjlistnode *)malloc(sizeof(adjlistnode));
  head->next = NULL;
  head->vet = vet;
  mygraph->heads[mygraph->size++] = head;
}

/*删除顶点*/
void removevertex(graph *mygraph, vertex *vet) {
  adjlistnode *node = findnode(mygraph, vet);
  assert(node != NULL);
  // 删除vet对应的链表
  adjlistnode *cur = node, *pre = NULL;
  while (cur) {
    pre = cur;
    cur = cur->next;
    free(pre);
  }
  // 遍历其它顶点的链表，删除所有包含vet的边
  for (int i = 0; i < mygraph->size; i++) {
    cur = mygraph->heads[i];
    pre = NULL;
    if (cur != node) {
      while (cur) {
        pre = cur;
        cur = cur->next;
        if (cur && cur->vet == vet) {
          pre->next = cur->next;
          free(cur);
          break;
        }
      }
    }
  }
  // 将该顶点之后的顶点向前移动（数组式移动）
  // 先得找到删除的是数组的哪一个索引
  int i = 0;
  for (i = 0; i < mygraph->size; i++) {
    if (mygraph->heads[i] == node)
      break;
  }
  for (int j = i; j < mygraph->size - 1; j++)
    mygraph->heads[j] = mygraph->heads[j + 1];
  mygraph->size--;
  free(vet);
}

/*打印邻接表*/
void printgraph(const graph *mygraph) {
  printf("邻接表 = \n");
  for (int i = 0; i < mygraph->size; i++) {
    adjlistnode *node = mygraph->heads[i];
    printf("%d: [", node->vet->num);
    node = node->next;
    while (node) {
      printf("%d, ", node->vet->num);
      node = node->next;
    }
    printf("]\n");
  }
}
