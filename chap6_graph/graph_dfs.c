/*****************深度优先遍历******************/
/* 深度优先遍历，就是要一直走到头。
例子1：
初始化后，图为
邻接表 =
9: [6, 8, ]
8: [5, 7, 9, ]
7: [4, 8, ]
6: [3, 5, 9, ]
5: [2, 4, 6, 8, ]
4: [1, 5, 7, ]
3: [2, 6, ]
2: [1, 3, 5, ]
1: [2, 4, ]

深度优先遍历（DFS）顶点序列为
[9, 6, 3, 2, 1, 4, 5, 8, 7]

解释：
找到9的第一个没有被访问到的临街节点为6，然后调用递归，将6传入；
找到6的第一个没有被访问到的邻接节点为3，然后调用递归，将3传入;
同理：
3->2;2->1;1->4;4->5;5->8;8->7;
遍历到7的时候，所有的元素我们都找到了。所以后续就不可能进一步调用递归。等到while循环结束(node=null)即可。

例子2：
初始化后，图为
邻接表 =
0: [3, 1, ]
1: [2, 0, ]
2: [5, 1, ]
3: [0, ]
4: [5, ]
5: [6, 4, 2, ]
6: [5, ]

深度优先遍历（DFS）顶点序列为
[0, 3, 1, 2, 5, 6, 4]

找到0的第一个没有被访问到的邻接节点3，调用递归，将3传入
3没有符合要求的邻接节点，等待以3为头节点的链表遍历完，没有递归产生，node=null，返回函数，结束上层的递归
回到节点0，node->next直接到1，调用递归，将1传入
1->2,2->5,5->6,6->4
同理，访问到4之后已经不可能调用到递归，等待各层的循环结束。依次返回函数。
 *
 */
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
  // 找到该顶点所对应的链表头节点
  adjlistnode *node = findnode(mygraph, vet);
  // 找到之后遍历边链表
  while (node) {
    if (!isvisited(res, *ressize, node->vet))
      // 若是已经访问过的节点，则不需要递归操作
      // 而递归的终止条件是，isvisited
      dfs(mygraph, res, ressize, node->vet);
    // 第一次循环时，dfs不会调用，因为需要node节点被更新为当前节点的邻接节点。到了后期，则可能要到n次才会调用。
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
