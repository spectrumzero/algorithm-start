/*********************广度优先遍历******************/

#include "./include.c"

/*节点队列结构体*/
typedef struct queue {
  vertex *vet[MAX_SIZE];
  int front, rear, size;
} queue;

/*构造函数*/
queue *newqueue() {
  queue *q = (queue *)malloc(sizeof(queue));
  q->front = q->rear = q->size = 0;
  for (int i = 0; i < MAX_SIZE; i++)
    q->vet[i] = NULL;
  return q;
}

/*判断队列是否为空*/
int isempty(queue *q) { return q->size == 0; }

/*入队*/
void enqueue(queue *q, vertex *vet) {
  q->vet[q->rear] = vet;
  q->rear = (q->rear + 1) % MAX_SIZE;
  q->size++;
}

/*出队*/
vertex *dequeue(queue *q) {
  vertex *vet = q->vet[q->front];
  q->front = (q->front + 1) % MAX_SIZE;
  q->size--;
  return vet;
}

/*检查顶点是否已经被访问*/
int isvisited(vertex **visited, int size, vertex *vet) {
  // 遍历查找节点，使用o(n)时间
  for (int i = 0; i < size; i++) {
    if (visited[i] == vet)
      return 1;
  }
  return 0;
}

/*广度优先*/
// 使用邻接表表示图，以便获取指定顶点的所有邻接顶点
void graphbfs(graph *mygraph, vertex *startvet, vertex **res, int *ressize, vertex **visited,
              int *visitedsize) {
  queue *q = newqueue();
  enqueue(q, startvet);
  visited[(*visitedsize)++] = startvet;
  // 以顶点vet为起点，循环访问至访问玩所有顶点
  while (!isempty(q)) {
    // 队首顶点出队
    vertex *vet = dequeue(q);
    // 记录访问顶点
    res[(*ressize)++] = vet;
    // 遍历该顶点的所有邻接顶点
    adjlistnode *node = findnode(mygraph, vet);
    while (node != NULL) {
      // 跳过已经被访问过的顶点
      if (!isvisited(visited, *visitedsize, node->vet)) {
        // 只入队来访问的顶点
        enqueue(q, node->vet);
        // 标记该顶点已被访问
        visited[(*visitedsize)++] = node->vet;
      }
      node = node->next;
    }
  }
  // 释放内存
  free(q);
}

/*drive code*/
int main() {
  // 初始化图
  int vals[] = {9, 8, 7, 6, 7, 4, 3, 2, 1, 0};
  int size = sizeof(vals) / sizeof(vals[0]);
  vertex **v = valstovets(vals, size);
  vertex *edges[][2] = {{v[0], v[1]}, {v[0], v[3]}, {v[1], v[2]}, {v[1], v[4]},
                        {v[2], v[5]}, {v[3], v[4]}, {v[3], v[6]}, {v[4], v[5]},
                        {v[4], v[7]}, {v[5], v[8]}, {v[6], v[7]}, {v[7], v[8]}};
  int edgesize = sizeof(edges) / sizeof(edges[0]);
  graph *mygraph = newgraph();
  // 添加所有顶点和边
  for (int i = 0; i < size; i++) {
    addvertex(mygraph, v[i]);
  }
  for (int i = 0; i < edgesize; i++) {
    addedge(mygraph, edges[i][0], edges[i][1]);
  }
  printf("初始化后，图为\n");
  printgraph(mygraph);

  // 广度优先遍历
  // 顶点遍历序列
  vertex *res[MAX_SIZE];
  int ressize = 0;
  // 用于记录已被访问过的顶点
  vertex *visited[MAX_SIZE];
  int visitedsize = 0;

  graphbfs(mygraph, v[0], res, &ressize, visited, &visitedsize);
  printf("\n广度优先遍历(BFS)顶点序列为\n");
  printArray(vetstovals(res, ressize), ressize);

  // 释放内存
  delgraph(mygraph);
  free(v);
  return 0;
}
