#include "../utils/common.h"
#include <time.h>

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
  // 从根部开始访问查找要插入的位置
  treenode *cur = tree->root, *pre = NULL;
  // 循环查找，越过叶节点后跳出
  while (cur != NULL) {
    if (cur->val == num) {
      // 找到了重复节点，直接返回
      return;
    }
    // 记录cur的位置，即cur最后一次的有效值。pre是一个度为0或1的节点。
    pre = cur;
    if (cur->val < num) {
      // 插入位置在cur的右子树中
      // 向右子树继续遍历
      cur = cur->right;
    } else {
      // 插入位置在cur的左子树中
      // 向左子树继续遍历
      cur = cur->left;
    }
  }
  // 利用pre所记录的cur的位置来完成插入节点操作，pre要么是只有一个右子节点，要么只有一个左子节点，要么没有子节点。
  treenode *node = newtreenode(num);
  if (pre->val < num) {
    // pre没有右节点，将这个新节点作为pre的右节点，所以也不存在继续更新的问题
    pre->right = node;
  } else {
    // pre没有左节点，将这个新节点作为pre的左节点，所以也不存在继续更新的问题
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
  ///// 循环查找，越过叶节点后跳出循环
  while (cur != NULL) {
    if (cur->val == num)
      // 找到待删除的节点，跳出循环
      break;
    // 注意，这里的pre节点的用途是记录cur，更确切地说，是用来作为cur->r/l的父节点
    pre = cur;
    if (cur->val < num) {
      // 待删除节点在root的右子树中，继续向右遍历
      cur = cur->right;
    } else {
      // 待删除节点在root的左子树中，继续向左遍历
      cur = cur->left;
    }
  }
  // 如果遍历是因为 cur
  // ==null而不是break跳出，则说明无待删除节点，直接返回，结束删除函数
  if (cur == NULL)
    return;

  ///// 判断找到的待删除的节点是否存在子节点，以执行后续的删除操作
  // 情况一：0或1个子节点
  if (cur->left == NULL || cur->right == NULL) {
    // 子节点的数量为0或1(要么是左子，要么是右子)
    // 将唯一实际存在的子节点赋值给child节点
    // 另外，请注意，如果cur没有子节点，则以下的操作完成的其实也是成立的，但不冲突，甚至是提供一种重复的描述，
    // 即pre->right = null;或pre->left = null; 这样依旧是符合定义的。
    treenode *child = cur->left != NULL ? cur->left : cur->right;
    // 确定好了子节点之后，即可删除节点 cur
    // pre节点是cur的父节点
    if (pre->left == cur) {
      // 若pre的左子节点为cur，则以cur的唯一子节点child作为pre的左子节点，进而取代cur，这样同样符合二叉搜索树的要求
      pre->left = child;
    } else {
      // 反之，则同样将child放在cur所处的位置(pre的右子节点)，这样还是符合秩序的。
      pre->right = child;
    }
    // 改变了cur的指向之后，释放cur的内存。
    cur = NULL;
    free(cur);
  }
  // 情况二：有两个子节点
  else {
    // 有两个子节点，则同样按照搜索二叉树中序遍历的逻辑，有两种处理方式：
    // 1，以右子树中的最小节点来替换要被删除的节点
    // 2，以左子树中的最大节点来替换要被删除的节点
    // 这里采用的是第一种
    // 获取当前节点的右节点
    treenode *tmp = cur->right;
    // 获取右子树中的最小节点
    while (tmp->left != NULL) {
      // 一直向左子树遍历，直到找到待真正的child节点的左子树的最小节点
      tmp = tmp->left;
    }
    // 找到替换用的节点的值
    int tmpval = tmp->val;

    // 删除那个最小值的节点。递归？本质上是“再一次”执行，走一样的流程。
    // 请注意，这个最小值的节点就很好删除了。它在右子树中，且因为是最小值，所以没有左子节点，因此对应情况一，仅仅递归一次。
    // 这个要删除的节点，要么就只有右子节点，要么没有节点，如果它有右子节点，则将这个右子节点作为它的父节点的子节点，tmp被置为Null了。结束递归
    removenode(tree, tmp->val);

    // 将替换用的节点的值赋值给要被删除的节点cur，更新二叉树
    cur->val = tmpval;

    // 其实，让要删除的节点有两个子节点的时候，做的工作更像是一种交换，而不是删除。要做的，其实是将右子树中的最小节点的值赋值给要被“删除”
    // 的节点，然后实际上，真正被删除的，释放了内存的，其实是右子树的最小节点。那个cur原本的值不见了，但它有了新值。
    // 因此，也可以理解为什么cur的新值必须是右子树的最小值或者是左子树的最大值，因为它们是最靠近cur的，即最符合根节点的资格的。
    // 这样只需要两步，就可以高效率地完成树的更新，而不必全盘更新。
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
