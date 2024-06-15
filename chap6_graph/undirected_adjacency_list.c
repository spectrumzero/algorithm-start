#include "../utils/common.h"

/*节点最大数量为100*/
#define MAX_SIZE 100

/*顶点结构体*/
typedef struct vertex {
  int val;
} vertex;

/*构造顶点函数*/
vertex *newvertex(int val) {
  vertex *vet = (vertex *)malloc(sizeof(vertex));
  vet->val = val;
  return vet;
}

/*将值数组转换为顶点数组*/
vertex **valstovets(int *vals, int size) {
  vertex **vertices = (vertex **)malloc(size * sizeof(vertex *));
  for (int i = 0; i < size; ++i) {
    vertices[i] = newvertex(vals[i]);
  }
  return vertices;
}

/*节点结构体*/
typedef struct adjlistnode {
  vertex *vet;              // 顶点
  struct adjlistnode *next; /// 后继节点
} adjlistnode;

/*基于邻接表实现的无向图类*/
typedef struct graph {
  adjlistnode *heads[MAX_SIZE]; // 数组，但存放的是链表(链表的头节点)
  int size;                     // 节点数量
} graph;

/*构造函数*/
graph *newgraph() {
  graph *mygraph = (graph *)malloc(sizeof(graph));
  if (!mygraph) {
    return NULL;
  }
  mygraph->size = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    mygraph->heads[i] = NULL;
  }
  return mygraph;
}

/*析构函数*/
void delgraph(graph *mygraph) {
  for (int i = 0; i < mygraph->size; i++) {
    adjlistnode *cur = mygraph->heads[i];
    while (cur != NULL) {
      adjlistnode *next = cur->next;
      if (cur != mygraph->heads[i]) {
        free(cur);
      }
      cur = next;
    }
    free(mygraph->heads[i]->vet);
    free(mygraph->heads[i]);
  }
  free(mygraph);
}

/*查找顶点对应的节点*/
adjlistnode *findnode(graph *mygraph, vertex *vet) {
  for (int i = 0; i < mygraph->size; i++) {
    if (mygraph->heads[i]->vet == vet) {
      return mygraph->heads[i];
    }
  }
  return NULL;
}

/*添加边辅助函数*/
void addedgehelper(adjlistnode *head, vertex *vet) {
  adjlistnode *node = (adjlistnode *)malloc(sizeof(adjlistnode));
  node->vet = vet;
  // 头插法
  node->next = head->next;
  head->next = node;
}

/*添加边*/
void addedge(graph *mygraph, vertex *vet1, vertex *vet2) {
  adjlistnode *head1 = findnode(mygraph, vet1);
  adjlistnode *head2 = findnode(mygraph, vet2);
  assert(head1 != NULL && head2 != NULL && head1 != head2);
  // 添加边vet1-vet2
  addedgehelper(head1, vet2);
  addedgehelper(head2, vet1);
}

/*删除边辅助函数*/
void removeedgehelper(adjlistnode *head, vertex *vet) {
  adjlistnode *pre = head;
  adjlistnode *cur = head->next;
  // 在链表中搜索vet对应的节点
  while (cur != NULL && cur->vet != vet) {
    pre = cur;
    cur = cur->next;
  }
  if (cur == NULL)
    return;
  // 将vet对应节点从链表中删除，改变指向
  pre->next = cur->next;
  // 释放内存
  free(cur);
}

/*删除边*/
void removeedge(graph *mygraph, vertex *vet1, vertex *vet2) {
  adjlistnode *head1 = findnode(mygraph, vet1);
  adjlistnode *head2 = findnode(mygraph, vet2);
  assert(head1 != NULL && head2 != NULL);
  // 删除边vet1-vet2
  removeedgehelper(head1, head2->vet);
  removeedgehelper(head2, head1->vet);
}

/*添加顶点*/
void addvertex(graph *mygraph, vertex *vet) {
  assert(mygraph != NULL && mygraph->size < MAX_SIZE);
  adjlistnode *head = (adjlistnode *)malloc(sizeof(adjlistnode));
  head->vet = vet;
  head->next = NULL;
  // 在邻接表中添加一个新链表
  mygraph->heads[mygraph->size++] = head;
}

/*测试：删除顶点*/
void removevertex(graph *mygraph, vertex *vet) {
  adjlistnode *node = findnode(mygraph, vet);
  assert(node != NULL);
  // 在邻接表中删除顶点vet对应的链表
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
  // 将该顶点之后的顶点向前移动，以填补空缺
  int i;
  for (i = 0; i < mygraph->size; i++) {
    if (mygraph->heads[i] == node)
      break;
  }
  for (int j = i; j < mygraph->size - 1; j++) {
    mygraph->heads[j] = mygraph->heads[j + 1];
  }
  mygraph->size--;
  free(vet);
}
/****测试结束****/

/*打印邻接表*/
void printgraph(const graph *mygraph) {
  printf("邻接表 = \n");
  for (int i = 0; i < mygraph->size; ++i) {
    adjlistnode *node = mygraph->heads[i];
    printf("%d: [", node->vet->val);
    node = node->next;
    while (node) {
      printf("%d, ", node->vet->val);
      node = node->next;
    }
    printf("]\n");
  }
}

/*Driver Code*/
int main() {
  // 图的初始化
  int vals[] = {1, 3, 2, 5, 4};
  int size = sizeof(vals) / sizeof(vals[0]);
  vertex **v = valstovets(vals, size);
  vertex *edges[][2] = {{v[0], v[1]}, {v[0], v[3]}, {v[1], v[2]},
                        {v[2], v[3]}, {v[2], v[4]}, {v[3], v[4]}};
  int edgesize = sizeof(edges) / sizeof(edges[0]);
  graph *mygraph = newgraph();
  // 添加所有顶点和边
  for (int i = 0; i < size; i++) {
    addvertex(mygraph, v[i]);
  }
  for (int i = 0; i < edgesize; i++) {
    addedge(mygraph, edges[i][0], edges[i][1]);
  }
  printf("\n初始化后，图为\n");
  printgraph(mygraph);

  // 添加边
  // 顶点1,2，即v[0]，v[2]
  addedge(mygraph, v[0], v[2]);
  printf("\n添加边1-2后，图为\n");
  printgraph(mygraph);

  // 删除边
  // 顶点1,3，即v[0]，v[1]
  removeedge(mygraph, v[0], v[1]);
  printf("\n删除边1-3后，图为\n");
  printgraph(mygraph);

  // 添加顶点
  vertex *v5 = newvertex(6);
  addvertex(mygraph, v5);
  printf("\n添加顶点 6 后，图为\n");
  printgraph(mygraph);

  // 添加边6-3,6-2
  addedge(mygraph, v5, v[2]);
  addedge(mygraph, v5, v[1]);
  printf("\n添加边6-3和6-2后，图为\n");
  printgraph(mygraph);

  // 删除边2-4，4-5
  removeedge(mygraph, v[2], v[4]);
  removeedge(mygraph, v[4], v[3]);
  printf("\n删除边2-4和4-5后，图为\n");
  printgraph(mygraph);

  // 删除顶点
  // printf("\n删除顶点3后，图为\n");
  // removevertex(mygraph, v[1]);
  // printgraph(mygraph);

  // 释放内存
  delgraph(mygraph);
  free(v);
  return 0;
}
