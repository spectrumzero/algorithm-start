/*****************深度优先遍历******************/
#include "./include.c"

#define MAX_SIZE 100

int isvisited(vertex **res, int size, vertex *vet) {
  for (int i = 0; i < size; i++) {
    if (res[i] == vet)
      return 1;
  }
  return 0;
}

/*深度优先遍历辅助函数*/
void dfs(graph *mygraph, vertex **res, int *ressize, vertex *vet) {
  // 记录访问顶点
  res[(*ressize)++] = vet;
  // 遍历该顶点的所有邻接节点
  adjlistnode *node = findnode(mygraph, vet);
  while (node) {
    if (!isvisited(res, *ressize, node->vet))
      dfs(mygraph, res, ressize, node->vet);
    node = node->next;
  }
}

/*深度优先遍历*/
// 使用邻接表来表示图，以便获取指定顶点的所有邻接顶点
void graphdfs(graph *mygraph, vertex *startvet, vertex **res, int *ressize) {
  dfs(mygraph, res, ressize, startvet);
}

/*driver code*/
int main() {

  int vals[] = {0, 1, 2, 3, 4, 5, 6};
  int size = sizeof(vals) / sizeof(vals[0]);
  vertex **v = valstovets(vals, size);
  vertex *edges[][2] = {{v[0], v[1]}, {v[0], v[3]}, {v[1], v[2]},
                        {v[2], v[5]}, {v[4], v[5]}, {v[5], v[6]}};
  int egdeSize = sizeof(edges) / sizeof(edges[0]);
  graph *mygraph = newgraph();
  // 添加所有顶点和边
  for (int i = 0; i < size; i++) {
    addvertex(mygraph, v[i]);
  }
  for (int i = 0; i < egdeSize; i++) {
    addedge(mygraph, edges[i][0], edges[i][1]);
  }
  printf("\n初始化后，图为\n");
  printgraph(mygraph);

  // 深度优先遍历
  vertex *res[MAX_SIZE];
  int ressize = 0;
  graphdfs(mygraph, v[0], res, &ressize);
  printf("\n深度优先遍历（DFS）顶点序列为\n");

  printArray(vetstovals(res, ressize), ressize);
  // 释放内存
  delgraph(mygraph);
  free(v);
  return 0;
}
