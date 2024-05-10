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
  char *prevstr = "    ";
  trunk *trunk = newtrunk(prev, prevstr);
  printtree(node->right, trunk, true);
  if (prev == NULL) {
    trunk->str = "———";
  } else if (isright) {
    trunk->str = "/———";
    prevstr = "   |";
  } else {
    trunk->str = "\\———";
    prev->str = prevstr;
  }
  showtrunks(trunk);
  printf("%d\n", node->val);

  if (prev != NULL) {
    prev->str = prevstr;
  }
  trunk->str = "   |";

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
