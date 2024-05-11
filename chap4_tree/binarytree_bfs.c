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

/*层序遍历*/
int *levelorder(treenode *root, int *size) {
  // 辅助队列
  int front, rear;
  int index, *arr;
  treenode *node;
  treenode **queue;

  // 辅助队列的初始化
  queue = (treenode **)malloc(sizeof(treenode *) * MAX_SIZE);
  // 队列区间索引
  front = 0, rear = 0;
  // 加入根节点
  queue[rear++] = root;
  // 初始化一个列表，用于保存遍历序列
  // 辅助数组，完成初始化
  arr = (int *)malloc(sizeof(int) * MAX_SIZE);
  // 数组索引
  index = 0;
  while (front < rear) {
    // 队列出队
    node = queue[front++];
    // 保存节点值
    arr[index++] = node->val;
    if (node->left != NULL) {
      // 左子节点入队
      queue[rear++] = node->left;
    }
    if (node->right != NULL) {
      // 右子节点入队
      queue[rear++] = node->right;
    }
  }
  // 更新数组长度的值
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
