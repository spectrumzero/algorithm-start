// 广度优先遍历二叉树

#include "../utils/common.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

/*二叉树节点结构体*/
typedef struct treenode {
  struct treenode *left;  // 左子节点指针
  struct treenode *right; // 右子节点指针
  int val;                // 节点值
  int height;             // 节点高度
} treenode;

/*构造节点函数*/
treenode *newtreenode(int val) {
  // 实例化一个节点
  treenode *node;
  node = (treenode *)malloc(sizeof(treenode));
  // 初始化节点
  node->height = 0;
  node->val = val;
  node->left = NULL;
  node->right = NULL;
  return node;
}

/*层序遍历，并以数组的形式返回遍历结果*/
int *levelorder(treenode *root, int *size) {
  // 链表节点
  treenode *node;

  // 辅助队列，队列中放的是二叉树的链表节点。一个又一个。队列的特性是先进先出，与层序遍历的逻辑是比较相符合的。将出队理解为访问一个节点。
  // 层序遍历的逻辑在于，先被访问（出队）的节点的子节点也会先被访问。即，在父节点被访问（出队）了之后，子节点就会入队，随后，子节点就会出队，被访问到。
  /*
      1
     / \
    2   3
   / \   \
  4   5   6
 访问节点 1，将其出队，并将其子节点 2 和 3 入队，队列变为 [2, 3]。
 访问节点 2，将其出队，并将其子节点 4 和 5 入队，队列变为 [3, 4, 5]。
 访问节点 3，将其出队，并将其子节点 6 入队，队列变为 [4, 5, 6]。
 依次访问节点 4、5 和 6，每次出队，直到队列为空。
 最终访问顺序为 1, 2, 3, 4, 5, 6，这就是层序遍历的结果。
 这才是层序遍历的正解。
  */
  treenode **queue;

  // 为辅助队列分配动态内存
  queue = (treenode **)malloc(sizeof(treenode *) * MAX_SIZE);

  // 队列区间的索引，一般来说，在队列里还没有元素的时候，一般把头和尾都设置为0
  int front = 0, rear = 0;

  // 根节点作为队列的第一个节点)，顺便更新尾索引，rear表示最后一个元素的下一个位置
  // 首先是根节点被加入到队列中
  queue[rear++] = root; // 此时，rear =1

  // 创建一个用于存储队列数据的数组，并完成其初始化。
  int *arr = (int *)malloc(sizeof(int) * MAX_SIZE);
  // 数组索引
  int index = 0;
  while (front < rear) {
    // 将队列的当前节点赋值给node，第一次赋值的时候，相当于node = root;并自增
    node = queue[front++];
    // 赋值，将遍历（出队）的值按顺序赋值给数组，并实时更新数组的大小
    arr[index++] = node->val;
    if (node->left != NULL) {
      // 当当前节点还有左节点的时候，就将这个左节点添加到队列的尾部
      // 左节点先入队，也因此先出队，先被遍历。
      queue[rear++] = node->left;
    }
    // 最后是右子节点被拷贝
    if (node->right != NULL) {
      // 当当前节点还有右节点的时候，就将这个右节点添加到队列的尾部
      // 右节点后入队，也因此后出队，后被遍历
      queue[rear++] = node->right;

      // 循环，直到front = rear的时候，就退出循环。
      // 关键是要理解front和rear的差值。rear和root的关系很明显了，而这其实就是front和root的关系。front来遍历，而rear则是定义遍历的边界
      // rear表示队列中最后一个元素的[下一个]位置，而front表示队列中的第一个元素的位置，所以，有效区间是[front,rear-1]，front刚好遍历到rear-1的时候，之后就不再遍历了
    }
  }
  // 更新数组长度的值，并重新分配内存块的大小，减少浪费，且realloc函数会自动释放原来地址所指的内存空间
  *size = index;
  arr = realloc(arr, sizeof(int) * (*size));

  // 释放辅助数组空间
  free(queue);
  return arr;
}

/*将列表反序列化为二叉树：递归*/
treenode *arrayTotreeDFS(int *arr, int size, int i) {
  if (i < 0 || i >= size || arr[i] == INT_MAX) {
    return NULL;
  }
  treenode *root = (treenode *)malloc(sizeof(treenode));
  root->val = arr[i];
  root->left = arrayTotreeDFS(arr, size, 2 * i + 1);
  root->right = arrayTotreeDFS(arr, size, 2 * i + 2);
  return root;
}

/*将列表反序列化为二叉树*/
treenode *arrayTotree(int *arr, int size) {
  return arrayTotreeDFS(arr, size, 0);
}

/*下面要开始二叉树的打印工作(不仅仅是单纯的数字，还有“树枝”（trunk）)，要完成这一工作，要分三步。*/

/* 1,“树枝”的结构体定义及其初始化*/
// 结构体定义
typedef struct trunk {
  struct trunk *pre;
  char *str;
} trunk;
// 初始化
trunk *newtrunk(trunk *pre, char *str) {
  trunk *newtrunk = (trunk *)malloc(sizeof(trunk));
  newtrunk->pre = pre;
  newtrunk->str = (char *)malloc(sizeof(char) * 10);
  strcpy_s(newtrunk->str, strlen(newtrunk->str) + 1, str);
  return newtrunk;
}
// 展现树枝，递归
void showtrunks(trunk *trunk) {
  if (trunk == NULL) {
    return;
  }
  showtrunks(trunk->pre);
  printf("%s", trunk->str);
}

/* 2, 打印助手*/
void printtreehelper(treenode *node, trunk *pre, bool isright) {
  if (node == NULL) {
    return;
  }
  char *prestr = "    "; // 四个空格
  trunk *neotrunk = newtrunk(pre, prestr);
  printtreehelper(node->right, neotrunk, true);
  if (pre == NULL) {
    neotrunk->str = "———";
  } else if (isright) {
    neotrunk->str = "/———";
    prestr = "   |";
  } else {
    neotrunk->str = "\\———";
    pre->str = prestr;
  }
  showtrunks(neotrunk);
  printf("%d\n", node->val);
  if (pre != NULL) {
    pre->str = prestr;
  }
  neotrunk->str = "   |";

  printtreehelper(node->left, neotrunk, false);
}

/* 3，打印二叉树*/
void printtree(treenode *root) { printtreehelper(root, NULL, false); }

/************************至此，二叉树的打印完毕*******************/

/*打印数组，显示节点的层序遍历顺序*/
void printarray(int *arr, int size) {
  if (arr == NULL || size == 0) {
    printf("[]");
    return;
  }
  printf("[");
  for (int i = 0; i < size - 1; i++) {
    printf("%d, ", arr[i]);
  }
  printf("%d]\n", arr[size - 1]);
}

/*递归式释放内存*/
void freememorytree(treenode *root) {
  if (root == NULL) {
    return;
  }
  freememorytree(root->left);
  freememorytree(root->right);
  free(root);
}

/*driver code*/
int main() {
  // 初始化二叉树
  // 这里借助了一个从数组直接生成二叉树的函数
  int nums[] = {1, 2, 5, 4, 8, 6, 7, 9, 0};
  int size = sizeof(nums) / sizeof(int);
  treenode *root = arrayTotree(nums, size);
  printf("初始化二叉树\n");
  printtree(root);

  // 层序遍历
  // 需要传入的数组的长度
  int *arr = levelorder(root, &size);
  printf("层序遍历的节点打印序列 = ");
  printarray(arr, size);

  // 释放内存
  freememorytree(root);
  free(arr);
  return 0;
}
