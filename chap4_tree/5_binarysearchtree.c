#include "../utils/common.h"

/* 节点结构体*/
typedef struct treenode {
  struct treenode *left;
  struct treenode *right;
  int val;
  int height;
} treenode;

/* 构造节点结构体函数*/
treenode *newtreenode(int val) {
  treenode *newnode = (treenode *)malloc(sizeof(treenode));
  newnode->left = NULL;
  newnode->right = NULL;
  newnode->val = val;
  newnode->height = 0;
  return newnode;
}

/* 二叉树搜索树结构体*/
typedef struct bstree {
  treenode *root;
} bstree;

/*构造树结构体函数*/
bstree *newbstree() {
  // 初始化空树
  bstree *tree = (bstree *)malloc(sizeof(bstree));
  tree->root = NULL;
  return tree;
}

/*释放二叉树各个节点的内存*/
void freememorytree(treenode *root) {
  if (root == NULL) {
    return;
  }
  freememorytree(root->left);
  freememorytree(root->right);
  free(root);
}

/*析构函数*/
void delbstree(bstree *tree) {
  freememorytree(tree->root);
  free(tree);
}

/*获取二叉搜索树的根节点*/
treenode *getroot(bstree *tree) { return tree->root; }

/*查找值为num的节点*/
treenode *search(bstree *tree, int num) {
  treenode *cur = tree->root;
  // 循环查找，越过叶节点后跳出
  while (cur != NULL) {
    if (cur->val < num) {
      // 目标节点在cur的右子树中
      cur = cur->right;
    } else if (cur->val > num) {
      // 目标节点在cur的左子树中
      cur = cur->left;
    } else {
      // 找到目标节点，跳出循环，当前节点就是要找的节点
      break;
    }
  }
  // 返回目标节点
  return cur;
}

/*插入值为num的节点*/
void insert(bstree *tree, int num) {
  // 若树为空，则初始化根节点
  if (tree->root == NULL) {
    tree->root = newtreenode(num);
    return;
  }
  treenode *cur = tree->root, *pre = NULL;
  // 循环查找，越过叶节点后跳出
  while (cur != NULL) {
    if (cur->val == num) {
      // 找到了重复节点，直接返回
      return;
    }
    // 记录cur的位置
    pre = cur;
    if (cur->val < num) {
      // 插入位置在cur的右子树中
      // 继续遍历
      cur = cur->right;
    } else {
      // 插入位置在cur的左子树中
      // 继续遍历
      cur = cur->left;
    }
  }
  // 利用pre所记录的cur的位置来完成插入节点操作
  treenode *node = newtreenode(num);
  if (pre->val < num) {
    pre->right = node;
  } else {
    pre->left = node;
  }
}

/*删除值为num的节点*/
void removenode(bstree *tree, int num) {
  // 若树为空，则直接提前返回
  if (tree->root == NULL) {
    return;
  }
  // 从根部开始访问树
  treenode *cur = tree->root, *pre = NULL;
  // 循环查找，越过叶节点后跳出循环
  while (cur != NULL) {
    if (cur->val == num)
      // 找到待删除的节点，跳出循环
      break;
    pre = cur;
    if (cur->val < num) {
      // 待删除节点在root的右子树中
      cur = cur->right;
    } else {
      // 待删除节点在root的左子树中
      cur = cur->left;
    }
  }
  // 若无待删除节点，则直接返回
  if (cur == NULL)
    return;
  ///// 判断待删除的节点是否存在子节点
  // 情况一：0或1个子节点
  if (cur->left == NULL || cur->right == NULL) {
    // 子节点的数量为0或1(要么是左子，要么是右子)，child = nullptr或该子节点
    treenode *child = cur->left != NULL ? cur->left : cur->right;
    // 确定好了子节点之后，即可删除节点 cur
    if (pre->left == cur) {
      // 改变指向的逻辑
      pre->left = child;
    } else {
      // 改变指向的逻辑
      pre->right = child;
    }
    free(cur);
  }
  // 情况二：有两个子节点
  else {
    // 有两个子节点
    // 获取中序遍历中cur的下一个节点(即右节点)，作为真正的child节点
    treenode *tmp = cur->right;
    while (tmp->left != NULL) {
      tmp = tmp->left;
    }
    int tmpval = tmp->val;
    // 递归删除节点tmp
    removenode(tree, tmp->val);
    // 用tmp覆盖cur
    cur->val = tmpval;
  }
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
/***************************************************/

/*driver code*/
int main() {
  // 初始化二叉搜索树
  int nums[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15};
  bstree *searchtree = newbstree();

  // 循环插入节点，完成树的各个节点的初始化
  for (int i = 0; i < sizeof(nums) / sizeof(int); i++) {
    insert(searchtree, nums[i]);
  }
  printf("初始化的二叉树为\n");
  printtree(getroot(searchtree));

  // 查找节点
  treenode *foundnode = search(searchtree, 7);
  printf("查找到的节点对象的节点值为 %d\n", foundnode->val);

  // 插入节点
  insert(searchtree, 16);
  printf("插入节点 16后，二叉树为\n");
  printtree(getroot(searchtree));

  // 删除节点
  removenode(searchtree, 1);
  printf("删除节点 1后，二叉树为\n");
  printtree(searchtree->root);

  removenode(searchtree, 2);
  printf("删除节点 2后，二叉树为\n");
  printtree(searchtree->root);

  removenode(searchtree, 4);
  printf("删除节点 4后，二叉树为\n");
  printtree(searchtree->root);

  removenode(searchtree, 8);
  printf("删除节点 8后，二叉树为\n");
  printtree(searchtree->root);
  // 释放内存
  delbstree(searchtree);
  return 0;
}
