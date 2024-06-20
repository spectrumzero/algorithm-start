#include "../utils/common.h"

/*假设顶点最大值为 100*/
#define MAX_SIZE 100

/*基于领接矩阵实现的无向图结构体*/
typedef struct graph {
  int vertices[MAX_SIZE];            // 存放顶点的一维数组
  int adjmatrix[MAX_SIZE][MAX_SIZE]; // 一个二维数组，表示边
  int size;
} graph;

/*构造函数*/
graph *newgraph() {
  graph *mygraph = (graph *)malloc(sizeof(graph));
  mygraph->size = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    for (int j = 0; j < MAX_SIZE; j++) {
      // 完成二维数组的初始化，表示边关系，为0表示不存在边关系
      mygraph->adjmatrix[i][j] = 0;
    }
  }
  return mygraph;
}

/*析构函数*/
void delgraph(graph *mygraph) { free(mygraph); }

/*添加顶点*/
void addvertices(graph *mygraph, int val) {
  if (mygraph->size == MAX_SIZE) {
    // 标准错误流，stdio库
    fprintf(stderr, "图的数量已达最大值\n");
    return;
  }
  // 不到达最大值，则允许添加。
  // 添加第n个顶点
  int n = mygraph->size;
  mygraph->vertices[n] = val;
  // 并将第n行和列归零
  for (int i = 0; i <= n; i++) {
    mygraph->adjmatrix[n][i] = mygraph->adjmatrix[i][n] = 0;
  }
  // 更新图的大小，按照顶点个数来计算的
  mygraph->size++;
}

/*删除顶点*/
void removevertices(graph *mygraph, int index) {
  if (index < 0 || index >= mygraph->size) {
    fprintf(stderr, "顶点索引越界\n");
    return;
  }
  // 在顶点列表中移除索引为Index的顶点
  for (int i = index; i < mygraph->size - 1; i++)
    // 删除方式和数组的原则是一样的，就是以那个要被删除的点为边界，把数组整体前移，把那个边界给挤出去。
    mygraph->vertices[i] = mygraph->vertices[i + 1];
  // 顶点列表中的顶点删除完了，但边列表中以另一种形式存在的顶点还没有被删除
  // 在领接矩阵中删除索引为index的行，i表示行，j表示列
  for (int i = index; i < mygraph->size - 1; i++) {
    for (int j = 0; j < mygraph->size; j++) {
      mygraph->adjmatrix[i][j] = mygraph->adjmatrix[i + 1][j];
    }
  }
  // 在领接矩阵中删除索引为index的列，i表示行，j表示列
  // 注意，下面之所以是i<mygraph->size-1,而不是i<mygraph->size，是因为当i =
  // mygraph->size-1，即遍历到最后一行的情况，对我们来说是没有意义的。
  for (int i = 0; i < mygraph->size - 1; i++) {
    for (int j = index; j < mygraph->size - 1; j++) {
      mygraph->adjmatrix[i][j] = mygraph->adjmatrix[i][j + 1];
    }
  }
  mygraph->size--;
}

/*添加边，新建索引为i的顶点与索引为j的顶点之间的引用关系*/
// i和j表示的是vertices的索引index，一个是行中的顶点，一个是列中的顶点。mygraph->adjmatrix[i][j]表示的是顶点之间的关系。
// 注意，顶点不能与自身相连，因此邻接矩阵主对角线元素没有意义
void addedge(graph *mygraph, int i, int j) {
  if (i < 0 || j < 0 || i >= mygraph->size || j > mygraph->size || i == j) {
    fprintf(stderr, "边索引越界或相等\n");
    return;
  }
  // 1表示有引用关系，这里是无向图，所以需要同时赋值为1
  mygraph->adjmatrix[i][j] = 1;
  mygraph->adjmatrix[j][i] = 1;
}

/*删除边*/
void removeedge(graph *mygraph, int i, int j) {
  if (i < 0 || j < 0 || i > mygraph->size - 1 || j > mygraph->size - 1 || i == j) {
    fprintf(stderr, "边索引越界或相等");
    return;
  }
  mygraph->adjmatrix[i][j] = 0;
  mygraph->adjmatrix[j][i] = 0;
}

/*打印邻接矩阵*/
void printgraph(graph *mygraph) {
  printf("顶点列表 = ");
  printArray(mygraph->vertices, mygraph->size);
  printf("邻接矩阵 = \n");
  for (int i = 0; i < mygraph->size; i++)
    printArray(mygraph->adjmatrix[i], mygraph->size);
}

/*driver code*/
int main() {
  // 初始化无相图
  graph *mygraph = newgraph();
  int vertices[] = {1, 3, 2, 5, 4};
  // 利用常见的数组赋值法去添加顶点
  for (int i = 0; i < 5; i++)
    addvertices(mygraph, vertices[i]);

  // 添加完了顶点后，就可以依据索引(注意不是直接用值)来构造顶点与顶点之间的引用关系，添加边
  // 初始化一个n行2列的数组，表示有n个子数组，其中每个数组中有2个元素，这里其实也可以写成edges[6][2]。我们可以看到，这里一共有2*6=12个元素，每个元素都有唯一一个数组索引表示
  int edges[][2] = {{0, 1}, {0, 3}, {1, 2}, {2, 3}, {2, 4}, {3, 4}};
  for (int i = 0; i < 6; i++)
    // 请注意，所谓的添加边，只是在既有的值/顶点之间添加一个关系，而不是新建一个顶点出来。因此，这里二维数组的作用，仅仅是提供一个索引而已，二维数组的索引所指向的值，最后还是作为索引被传入add函数的。所以，要保证的其实仅仅是二维数组中的元素是小于graph的size的
    addedge(mygraph, edges[i][0], edges[i][1]);
  printf("\n初始化后，图为\n");
  printgraph(mygraph);

  // 添加边
  // 顶点1,2的索引分别为0,2
  addedge(mygraph, 0, 2);
  printf("\n添加边1-2后，图为\n");
  printgraph(mygraph);

  // 删除边
  // 顶点1,3的索引分别为0,1
  removeedge(mygraph, 0, 1);
  printf("\n删除边1-3后，图为\n");
  printgraph(mygraph);

  // 添加顶点
  addvertices(mygraph, 6);
  printf("\n添加顶点6后，图为\n");
  printgraph(mygraph);

  // 删除顶点
  // 顶点3的索引为1
  removevertices(mygraph, 1);
  printf("\n删除顶点3后，图为\n");
  printgraph(mygraph);
  // 顶点1的索引为0
  removevertices(mygraph, 0);
  printf("\n删除顶点1后，图为\n");
  printgraph(mygraph);

  // 释放内存，数组的生命和其作用域一致
  delgraph(mygraph);

  return 0;
}
