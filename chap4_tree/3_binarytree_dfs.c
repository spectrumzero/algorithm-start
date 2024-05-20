/*深度优先遍历二叉树*/

#include "../utils/common.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 100

// 辅助数组，用于存储遍历序列后的结果
int arr[MAX_SIZE]; // 全局变量

// 二叉树结构体
typedef struct treenode {
  int height;
  int val;
  struct treenode *right;
  struct treenode *left;
} treenode;

/*将列表/数组反序列化为二叉树：递归*/
treenode *arrayTotreeDFS(int *arr, int size, int i) {
  if (i < 0 || i >= size || arr[i] == INT_MAX) {
    return NULL;
  }
  treenode *root = (treenode *)malloc(sizeof(treenode));

  // 注意，其实这里的root->val
  // =arr[i];可以放在不同的位置，只是影响到构造的顺序（前序/中序/后续），而不影响最后结果的完成
  root->val = arr[i];
  // 所以，递归是构造二叉树的最佳方法吧
  // 首先构造左子树
  root->left = arrayTotreeDFS(arr, size, 2 * i + 1);
  // 然后构造右子树
  root->right = arrayTotreeDFS(arr, size, 2 * i + 2);

  return root;
}

/*将列表反序列化为二叉树*/
treenode *arraytotree(int *arr, int size) {
  return arrayTotreeDFS(arr, size, 0);
}

/*前序遍历*/
void preorder(treenode *root, int *size) {
  if (root == NULL) {
    return;
  }
  // 访问优先级：根节点->左子树->右子树
  arr[(*size)++] = root->val;
  // 左子树
  preorder(root->left, size);
  // 右子树
  preorder(root->right, size);
}

/*中序遍历*/
void inorder(treenode *root, int *size) {
  if (root == NULL) {
    return;
  }
  // 访问优先级：左子树-》根节点-》右子树
  inorder(root->left, size);
  arr[(*size)++] = root->val; // 解size引用，并实时更新
  inorder(root->right, size);
}

/*后序遍历*/
void postorder(treenode *root, int *size) {
  if (root == NULL) {
    return;
  }
  postorder(root->left, size);
  postorder(root->right, size);
  arr[(*size)++] = root->val;
}

/***********打印二叉树*********/
typedef struct trunk {
  struct trunk *pre;
  char *str;
} trunk;

trunk *newtrunk(trunk *pre, char *str) {
  trunk *neotrunk = (trunk *)malloc(sizeof(trunk));
  neotrunk->pre = pre;
  neotrunk->str = (char *)malloc(sizeof(char) * 10);
  strcpy_s(neotrunk->str, strlen(neotrunk->str) + 1, str);
  return neotrunk;
}

void showtrunks(trunk *trunk) {
  if (trunk == NULL) {
    return;
  }
  showtrunks(trunk->pre);
  printf("%s", trunk->str);
}

void printtreehelper(treenode *node, trunk *pre, bool isright) {
  if (node == NULL) {
    return;
  }
  char *prestr = "    ";
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

void printtree(treenode *root) { printtreehelper(root, NULL, false); }
/*****************二叉树打印函数实现结束*************/

/*打印遍历后的数组结果*/
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

/*析构函数*/
void freememorytree(treenode *root) {
  if (root == NULL) {
    return;
  }
  freememorytree(root->left);
  freememorytree(root->right);
  free(root);
  return;
}

/*drivercode*/
int main() {
  // 初始化二叉树
  //  这里借助了一个从数组直接生成二叉树的函数
  int nums[] = {1, 9, 0, 7, 4, 2, 8, 5, 6};
  int size = sizeof(nums) / sizeof(int); // 数组的长度
  treenode *root = arraytotree(nums, size);
  printf("初始化二叉树\n");
  printtree(root);

  // 前序遍历
  // 初始化辅助数组
  // size这里的用途是为了“凑数”，凑参数，但是，当它被以地址的形式传入到遍历的函数之中后，它实际上记录了打印数组的数组的元素数量。
  size = 0;
  preorder(root, &size);
  printf("前序遍历的节点打印序列 = ");
  printarray(arr, size);

  // 中序遍历
  size = 0;
  inorder(root, &size);
  printf("中序遍历的节点打印序列 = ");
  printarray(arr, size);

  // 后序遍历
  size = 0;
  postorder(root, &size);
  printf("后序遍历的节点打印序列 = ");
  printarray(arr, size);

  // 释放内存
  freememorytree(root);

  return 0;
}
