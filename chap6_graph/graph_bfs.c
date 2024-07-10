/*********************广度优先遍历******************/

#include "./include.c"

/*节点队列结构体*/
typedef struct queue {
  // 存放顶点指针的数组
  vertex *vet[MAX_SIZE];
  // 基于数组实现的队列，数组负责存放队列元素。front，rear负责记录出队和入队的位置。
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

/*顶点入队*/
void enqueue(queue *q, vertex *vet) {
  // 队尾入队，队首出队，始终记得，队列的实际区间为[front,rear-1]
  q->vet[q->rear] = vet;
  // 更新区间
  q->rear = (q->rear + 1) % MAX_SIZE;
  // 更新队列元素的大小
  q->size++;
}

/*出队*/
vertex *dequeue(queue *q) {
  // 队尾入队，队首出队
  vertex *vet = q->vet[q->front];
  // 更新区间
  q->front = (q->front + 1) % MAX_SIZE;
  // 更新队列元素的大小
  q->size--;
  return vet;
}

/*检查传入的顶点vet是否已经被访问(被记录在大小为size的visited数组中)，避免重复访问*/
int isvisited(vertex **visited, int size, vertex *vet) {
  // 遍历查找节点，使用o(n)时间
  // 遍历保存了先前访问过顶点的visited数组，检查该顶点是否有被访问过。
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
  // 创建一个新的队列
  queue *q = newqueue();
  // 起手就将起始顶点入队
  enqueue(q, startvet);
  // 将起始顶点添加到visited数组，避免重复访问，并增加已访问顶点的计数。
  visited[(*visitedsize)++] = startvet;
  // 以顶点vet为起点，广度遍历其它顶点。遍历的结果在res数组中。
  while (!isempty(q)) {
    // 队首顶点出队
    vertex *vet = dequeue(q);
    // 然后将这一出队的顶点添加到result数组中，并更新result的计数
    res[(*ressize)++] = vet;
    // 利用findnode函数，找到出队顶点在图中的头节点，从而找到这个头结点有哪些邻接节点
    adjlistnode *node = findnode(mygraph, vet);
    // 找到有效的node节点后，则遍历接下来的整个链表
    while (node != NULL) {
      // 遍历边链表
      if (!isvisited(visited, *visitedsize, node->vet)) {
        // 判断该顶点是否有被访问过，只入队未访问的顶点，若有未访问的节点，则下次循环再出队；若没有，则队列size=0，下次的循环会直接跳出。
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
  int vals[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
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
  // 初始化一个保留遍历结果的数组
  vertex *res[MAX_SIZE];
  int ressize = 0;
  // 初始化一个记录已访问节点的数组
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
