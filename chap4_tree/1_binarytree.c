#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*二叉树节点结构体*/
typedef struct treenode {
  int val;                // 节点值
  int height;             // 节点高度
  struct treenode *left;  // 左子节点指针
  struct treenode *right; // 右子节点指针
} treenode;

/*构造函数 */
treenode *newtreenode(int val) {
  treenode *newnode;
  newnode = (treenode *)malloc(sizeof(treenode));
  newnode->val = val;
  newnode->right = NULL;
  newnode->left = NULL;
  return newnode;
}

/*树的分支的结构体*/
typedef struct trunk {
  struct trunk *prev;
  char *str;
} trunk;

/*树的分支的构造函数*/
trunk *newtrunk(trunk *prev, const char *str) {
  trunk *newtrunk = (trunk *)malloc(sizeof(trunk));
  newtrunk->prev = prev;
  newtrunk->str = (char *)malloc(sizeof(char) * 10);
  strcpy_s(newtrunk->str, strlen(newtrunk->str) + 1, str);
  return newtrunk;
}

/*帮助打印树的分支*/
void showtrunks(trunk *trunk) {
  if (trunk == NULL) {
    return;
  }
  showtrunks(trunk->prev);
  printf("%s", trunk->str);
}

/*打印二叉树*/
void printtree(treenode *node, trunk *prev, bool isright) {
  if (node == NULL) {
    return;
  }
  // 先完成对树枝的初始化，以后再修改
  char *prevstr = "    ";
  trunk *trunk = newtrunk(prev, prevstr);
  // 先打印右子树，注意，直到右子树打印完之后，才去打印根节点，最后再递归打印左子树。
  printtree(node->right, trunk, true);
  // 生成当前节点对应的分支样式。根据当前节点的位置和其父节点的情况，我们来确定这个树枝，这个trunk到底长什么样子
  // 如果当前节点没有父节点(即该节点为根节点)，则直接使用”———“作为分支样式
  if (prev == NULL) {
    trunk->str = "———";
  }
  // 如果当前节点是其父节点的右子节点，则使用下述分支样式
  else if (isright) {
    trunk->str = "/———";
    prevstr = "   |";
  }
  // 如果当前节点是其父节点的左子节点，则使用下述分支样式
  else {
    trunk->str = "\\———";
    prev->str = prevstr;
  }

  // 打印生成的分支样式和当前节点的值
  showtrunks(trunk);
  printf("%d\n", node->val);

  // 更新分支样式，确保左子树的内容会在当前节点之后打印
  if (prev != NULL) {
    prev->str = prevstr;
  }
  trunk->str = "   |";
  // 递归打印左子树
  printtree(node->left, trunk, false);
}

/*析构函数，释放二叉树内存*/
void freememorytree(treenode *root) {
  if (root == NULL) {
    return;
  }
  freememorytree(root->left);
  freememorytree(root->right);
  free(root);
}

/* driver code*/
int main() {
  // 初始化二叉树
  treenode *n1 = newtreenode(1);
  treenode *n2 = newtreenode(2);
  treenode *n3 = newtreenode(3);
  treenode *n4 = newtreenode(4);
  treenode *n5 = newtreenode(5);
  // 构建节点之间的引用关系
  n1->left = n2;
  n1->right = n3;
  n2->left = n4;
  n2->right = n5;
  printf("初始化二叉树\n");
  printtree(n1, NULL, false);

  // 插入节点，在n1和n2之间插入新的节点n6
  treenode *n6 = newtreenode(0);
  n1->left = n6;
  n6->left = n2;
  printf("插入节点n6后\n");
  printtree(n1, NULL, false);

  // 删除节点n6
  n1->left = n2;
  free(n6);
  printf("删除节点n6后\n");
  printtree(n1, NULL, false);

  // 析构函数
  freememorytree(n1);

  return 0;
}
