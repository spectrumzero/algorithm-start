#include "../utils/common.h"

/***
 * 这个以邻接表表示的无向图的逻辑是：外层是一个数组，每个数组的元素是一个顶点指针。
 * 数组的元素是指针型变量的好处在于，一是可以利用操作符访问顶点的值，二是从这个指针型变量出发，可以访问到下一个顶点。即它的邻接节点。
 * 数组表示顶点，而链表是用来表示边关系的。这有点像链式寻址。
 ***/

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
vertex **valstovets(int *nums, int size) {
  vertex **vertices = (vertex **)malloc(size * sizeof(vertex *));

  /* 好像更能get到，为什么要以这种方式去分配内存。
   * 首先，虽然传入的数组在编译的时候就已经确定好了内存大小，但对于我们需要的顶点数组来说，顶点数组的内存大小是只有在运行时才能确定的，
   * 即它得是动态分配内存的。在这里，就是说，它得是被动态地初始化的。因此，不能用静态分配的方式，而需要动态分配。就需要用到malloc函数了
   * 那么为什么要写成vertex **vertices这样的写法呢？经过前面对于int *valsb;和int valsb[] =
   * "fafa";的讨论，我们对于这两种表示的根本不同也有所了解。
   * 这里不能写成vertex *vertices[]
   * =???;的原因很简单，因为这样的写法是一种静态分配内存的写法！你也不知道这里的右值应该怎么去写，且这样写的vertex和上面的vertex，是根本不同的。
   */

  // 通过copy来完成图的初始化。
  for (int i = 0; i < size; ++i) {
    vertices[i] = newvertex(nums[i]);
  }
  return vertices;
}

/*节点结构体，任务在于构建边关系*/
typedef struct adjlistnode {
  vertex *vet;              // 顶点(的指针)
  struct adjlistnode *next; /// 后继节点
} adjlistnode;
// adjlistnode与vet的关系是，adjlistnode可以访问到vet，并联通到下一个节点。这是原本的vertex结构体不能实现的。
// 因此，说这个结构体单纯仅仅是在构建边关系，也不太完整。应该说，它还能实现对顶点的访问。也请注意，顶点与顶点之间的连接，不是通过操作符哦，
// 即顶点和顶点之间所构成的关系不构成链表关系，而是数组索引关系

/*基于邻接表实现的无向图类*/
typedef struct graph {
  adjlistnode *heads[MAX_SIZE]; // 数组，但存放的是指针型变量，是顶点，是头结点。
  int size;                     // 节点数量
} graph;

/*构造函数*/
graph *newgraph() {
  graph *mygraph = (graph *)malloc(sizeof(graph));
  mygraph->size = 0;
  // 不要忘记完整地初始化。这就是构造函数的功能
  for (int i = 0; i < MAX_SIZE; i++) {
    mygraph->heads[i] = NULL;
  }
  return mygraph;
}

/*析构函数*/
// 这种析构逻辑，跟链表一样的。哦不，应该是，所有的析构逻辑应该是类似的。从外到内，深入(递)，从内层到外层，逐层free(归)
// 先用函数把整个图包住
void delgraph(graph *mygraph) {
  // 再用一个大循环包住数组的外壳。
  for (int i = 0; i < mygraph->size; i++) {
    // 保存某一数组索引的头节点，以此为开头，遍历、释放以这个头节点，即顶点开头的整个链表
    adjlistnode *cur = mygraph->heads[i];
    while (cur != NULL) {
      // 保存当前节点指向的下一个节点，以方便后续的遍历
      adjlistnode *next = cur->next;
      if (cur != mygraph->heads[i]) {
        // 请注意，头节点是在最后释放的。这里以此释放的是顶点的邻接节点（不是作为顶点的邻接节点，而是作为邻接节点的邻接节点）
        // 为什么，因为头节点中还要其它的属性，即顶点属性。
        // 那么，和头节点是同样变量类型的邻接节点，为什么不释放vet呢？因为这里会出现重复释放的问题。
        //
        free(cur);
      }
      // 向后遍历
      cur = next;
    }
    // 释放头节点，首先释放其中的顶点属性
    free(mygraph->heads[i]->vet);
    // 然后释放头节点
    free(mygraph->heads[i]);
    // 这一层循环完了后，i++，从而进入以下一个顶点为头结点的链表的遍历和释放
  }
  // 释放最外层的图
  free(mygraph);
}

/*查找顶点对应的头节点，从而方便地获取“描述了边关系的顶点”*/
adjlistnode *findnode(graph *mygraph, vertex *vet) {
  // 对整个图进行遍历
  for (int i = 0; i < mygraph->size; i++) {
    if (mygraph->heads[i]->vet == vet) {
      // 找到了，就返回一个功能更全的顶点。
      return mygraph->heads[i];
    }
  }
  return NULL;
}

/*添加边辅助函数*/
void addedgehelper(adjlistnode *head, vertex *vet) {
  adjlistnode *node = (adjlistnode *)malloc(sizeof(adjlistnode));
  // 添加边，是什么意思？本质上就是在一条链表上添加新的节点，以此表示头节点与它之后的节点之间有邻接关系。注意，在一条链表上。
  // 先给新的邻接节点完成初始化，首先是赋值。虽然这里传入的是顶点数据类型，但在这里，它只是头节点的邻接节点。即不是作为顶点的顶点，而是作为邻接节点的顶点
  node->vet = vet;
  // 头插法
  // 完成初始化，并将新节点加入单向链表中，使之成为在头节点之后的第一个节点。
  node->next = head->next;
  head->next = node;
}

/*在顶点vet1和-vet2之间添加边*/
void addedge(graph *mygraph, vertex *vet1, vertex *vet2) {
  // 利用顶点找到对应的有用的头节点，在这个基础上才能进行添加
  adjlistnode *head1 = findnode(mygraph, vet1);
  adjlistnode *head2 = findnode(mygraph, vet2);
  assert(head1 != NULL && head2 != NULL && head1 != head2);
  // 添加边vet1-vet2，这里是双向添加的
  addedgehelper(head1, vet2);
  addedgehelper(head2, vet1);
}

/*删除边辅助函数*/
void removeedgehelper(adjlistnode *head, vertex *vet) {
  // 保存当前的头节点
  adjlistnode *pre = head;
  // 获取当前头节点的后一个节点，即在顶点之后的第一个邻接节点
  adjlistnode *cur = head->next;
  // 从第一个邻接节点开始遍历。从头节点开始遍历没有意义，甚至是违法的，它不可能被删除，要被删除的是它的邻接节点。
  while (cur != NULL && cur->vet != vet) {
    // 只有当邻接节点非null,且不等于
    //  保存这一节点，方便后续修改指向的操作
    pre = cur;
    // 继续向后遍历
    cur = cur->next;
  }
  // 1，当遍历找到了顶点为vet的邻接节点，则跳出循环。此时，我们找到了要删除的临街节点。
  // 2，当遍历完后，没有找到顶点为vet的临街节点，而cur最终也指向了空，则表示没有传入的顶点所对应的邻接节点，直接return
  if (cur == NULL)
    return;
  // 第一种情况发生，则用上一次循环里pre所保存的cur值进行重新指向的操作。直接略过cur，在逻辑上忽略
  pre->next = cur->next;
  // 释放内存，达成实际的释放。
  free(cur);
}

/*删除边vet1-vet2*/
void removeedge(graph *mygraph, vertex *vet1, vertex *vet2) {
  // 同样的，利用顶点找到对应的链表的头节点
  adjlistnode *head1 = findnode(mygraph, vet1);
  adjlistnode *head2 = findnode(mygraph, vet2);
  // 有意义
  assert(head1 != NULL && head2 != NULL);
  // 删除边vet1-vet2
  removeedgehelper(head1, head2->vet);
  removeedgehelper(head2, head1->vet);
}

/*添加顶点*/
void addvertex(graph *mygraph, vertex *vet) {
  // 有意义
  assert(mygraph != NULL && mygraph->size < MAX_SIZE);
  adjlistnode *head = (adjlistnode *)malloc(sizeof(adjlistnode));
  // 初始化
  head->vet = vet;
  head->next = NULL;
  // 添加到数组里。这个新的head，成为了一个新链表的头节点。期待它的邻接节点的加入。
  mygraph->heads[mygraph->size++] = head;
}

/*测试：删除顶点*/
void removevertex(graph *mygraph, vertex *vet) {
  // 先找到相应链表的头节点
  adjlistnode *node = findnode(mygraph, vet);
  assert(node != NULL);
  // 删除顶点vet对应的链表，这一步，直接从头节点开始的。而不像删除边那样。
  adjlistnode *cur = node, *pre = NULL;
  while (cur) {
    pre = cur;
    cur = cur->next;
    free(pre);
  }

  // 遍历其它顶点的链表，删除所有包含vet的边。前面的以vet为顶点的链表已经被释放，即这个链表中的所有变量，都是悬空指针，所以，应当跳过对这一条链表的访问
  for (int i = 0; i < mygraph->size; i++) {
    cur = mygraph->heads[i];
    pre = NULL;
    if (cur != node) {
      // 在符合if条件后，进入对某一合法链表的遍历
      while (cur) {
        pre = cur;
        cur = cur->next;
        if (cur && cur->vet == vet) {
          // 当cur非空，且找到了那个作为邻接节点的顶点时，则执行逻辑上的跳过和内存上的删除操作
          pre->next = cur->next;
          free(cur);
          break;
        }
      }
    }
    // i++，然后进入下一个链表
  }

  // 将该顶点之后的顶点向前移动，以填补空缺
  // 注意，这个i有更广的作用域，作用于整个函数
  int i;
  // 查找被删除的头节点在数组中的索引
  for (i = 0; i < mygraph->size; i++) {
    if (mygraph->heads[i] == node)
      // 循环体中，break语句会立即终止当前的循环，并执行循环之后的代码。也就是说，i++在break后不会被执行。i会被保存下来记录原链表所在索引
      break;
  }
  // 以该索引为边界，将后面的索引向前移动，即数组向前移动覆盖、实现数组层面最后的删除
  for (int j = i; j < mygraph->size - 1; j++) {
    mygraph->heads[j] = mygraph->heads[j + 1];
  }
  // 更新size
  mygraph->size--;
  // 最后释放vet
  free(vet);
}
/****测试结束****/

/*打印邻接表*/
void printgraph(const graph *mygraph) {
  printf("邻接表 = \n");
  // 外部数组层面的大循环
  for (int i = 0; i < mygraph->size; i++) {
    // 记录当前链表的头节点
    adjlistnode *node = mygraph->heads[i];
    // 单独打印头节点，即顶点
    printf("%d: [", node->vet->val);
    // 然后再打印其邻接节点
    node = node->next;
    // 从第一个邻接节点开始循环
    while (node) {
      printf("%d, ", node->vet->val);
      node = node->next;
    }
    // 顶点和邻接节点都打印完毕
    printf("]\n");
    // 这里的++i，和i++的效果是一样的，先相加后返回还是先返回后相加，都是让i+1之后进入下一轮判断，即都早于下一轮判断
  }
}

/*Driver Code*/
int main() {
  // 图的初始化
  int vals[] = {1, 3, 2, 5, 4};
  int size = sizeof(vals) / sizeof(vals[0]);
  // 注意，sizeof是一个运算符，用于计算某个类型或者对象在内存中占用的字节数。vals是一个数组名，而vals[0]表示数组中的一个元素。
  // 上面这个式子，实际上是5*4/4 = 5
  // 关于sizeof和strlen，这二者有一个补充说明：
  // 有char* valsa = "faffaf";和char valsb[] =
  // "faffaf";前者定义了一个字符型指针，后者定义了一个字符数组。
  // 对于前者而言，sizeof(valsa)的结果，是返回8，即一个指针型变量的固定大小；
  // 对于后者而言，sizeof(valsb)返回的是6*1 +1=
  // 7。为什么？因为这是一个特殊的数组，字符数组，还有一个"/0"结尾的字符，以此表示这是一个字符数组。而sizeof会完整地记录这个变量的大小。
  // sizeof适用的情况更多，不仅仅包括字符数组。但strlen是专门计算字符串——即字符数组的长度的，但不包括后面的终止符。
  // 所以，对于char *valsa = "faffaf"而言，strlen返回的是1*6;对于char valsb[] =
  // "faffaf"而言，strlen(valsb)所返回的结果是一样的。因为都是传入的首地址。
  // 此外，汉字等于三个英文字符
  //
  // 在topk问题里面，讲到过数组的退化。char valsb[] = "faffaf";这个valsb本质上代表的就跟int i =
  // 0;的i一样，就是一块内存空间的名字，我们可以看到没有退化的结果就是，当它作为参数，被
  // 传入sizeof里面的时候，返回的是6*1+1，或者如上面的vals而言，是5*4 =
  // 20bytes。这就是所谓的没有退化的数组。但当它被作为参数传入不同于sizeof的函数时，例如topk里面写的堆化函数，这个时候就发生退化，而只是表示一个地址而已。
  //
  // 总的来说，sizeof的适用性更广泛，而strlen主要用于计算字符数组的大小的情况。它们都在编译的时候，就能确定变量的内存大小。

  vertex **v = valstovets(vals, size);
  // 注意，v[]是一个vertex指针型变量。而edges[][2]，本质上跟v[]是一样的，都是一个vertex*类型的元素或单元.
  // 再如，vertex *v[MAX_SIZE];和vertex *edges[][2] =
  // {...};其实是一样的，v和edges，都是一个数组。只不过，前者是一维的，而后者是二维的。
  // int nums[][2] = {1};这是不合法的定义。应该是int nums[][2] = {{1,2},{2,2},{...}};
  // 在下面这个数组中，数组一共有6个元素，数组元素的计算方式，还是只看首个[]，而不是[]*[]，这在下面我有提到。
  // 每个元素又包含两个指针型变量（放有两个地址），这两个小单元也必须要是vertex*型的。
  //
  // vals数组的数量一共有五个，其索引从0到4。而在被转换为顶点数组之后，v数组的索引同样是从0到4
  vertex *edges[][2] = {
      {v[0], v[1]}, {v[0], v[3]}, {v[1], v[2]}, {v[2], v[3]}, {v[2], v[4]}, {v[3], v[4]},
  };
  int edgesize = sizeof(edges) / sizeof(edges[0]);
  graph *mygraph = newgraph();

  // 添加所有顶点
  for (int i = 0; i < size; i++) {
    addvertex(mygraph, v[i]);
  }

  // 添加所有边
  for (int i = 0; i < edgesize; i++) {
    // 这里就体现出二维数组的用处。二维数组是这样写的：int nums[][2] = {{1,2},{3,1},{4,2}};
    // 怎么理解呢？就是这个二维数组，一共有3个元素。就像一维数组的索引那样，二维数组的第一个元素的索引为0，之后是1,2
    // 这里的i，就是指的0,1,2，表示数组中的元素索引，这就是第一个“[]”的内涵。
    // 而第二个[]里面的取值范围，是由数组中每一个元素中所拥有的单元数量决定的，也是从0开始计数，所以，对于二维数组edge[][2]，其第二个[]中数字的取值范围也就是0和1了
    // 二维数组的好处在于，可以在定义这个数组的同时，就把图的边关系比较直观地表示出来。当然，这是依靠下面这一步完成最后的实现的。
    addedge(mygraph, edges[i][0], edges[i][1]);
  }

  // 完成初始化
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
  printf("\n删除顶点 4 和顶点 1 后，图为\n");
  removevertex(mygraph, v[4]);
  removevertex(mygraph, v[0]);
  printgraph(mygraph);

  // 释放内存
  delgraph(mygraph);
  free(v);
  return 0;
}
