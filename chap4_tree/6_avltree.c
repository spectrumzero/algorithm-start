#include "../utils/common.h"

/*节点结构体*/
typedef struct treenode {
  struct treenode *left;
  struct treenode *right;
  int val;
  int height; // 为了让树恢复平衡，所以需要计算节点的高度
} treenode;

/*初始化节点指针结构体函数*/
treenode *newtreenode(int val) {
  // 还是再提醒一遍，newnode这里是指针型，是通过操作符访问到结构体中的属性的。
  // newnode本质上存的是一个地址。除了地址就没有了。而如果是treenode
  // node;那么这个node本身就是一个结构体对象，里面有各种属性。
  treenode *newnode = (treenode *)malloc(sizeof(treenode));
  newnode->height = 0;
  newnode->val = val;
  newnode->left = NULL;
  newnode->right = NULL;
  // 返回的严格来说不是节点的结构体，而是一个指向结构体的指针
  return newnode;
}

/*AVL树结构体*/
typedef struct avltree {
  treenode *root;
} avltree;

/*构造树结构体函数*/
avltree *newtree() {
  avltree *newavltree = (avltree *)malloc(sizeof(avltree));
  newavltree->root = NULL;
  return newavltree;
}

/*释放节点内存*/
void freememorytree(treenode *node) {
  if (node == NULL) {
    return;
  }
  freememorytree(node->left);
  freememorytree(node->right);
  free(node);
}

/*析构函数*/
void deltree(avltree *newtree) {
  // 先释放树中各个节点的结构体
  freememorytree(newtree->root);
  // 再释放树的结构体
  free(newtree);
}

/*获取节点高度*/
int getheight(treenode *node) {
  // 节点的高度：从距离该节点最远的叶节点到该节点所经过的边的数量
  // 空节点的高度为-1，叶节点的高度为0。
  if (node != NULL)
    return node->height; // 返回的，要么是叶节点（0），要么是已经计算出了的node
  return -1;             // 空节点
  // 0和-1就像是递归中的一个终止条件，最终一切都是从这里算出的。
}

/*更新以node为根节点的树的高度*/
void updateheight(treenode *node) {
  int lh = getheight(node->left);
  int rh = getheight(node->right);
  // 而树的高度是需要对比的，不能直接在node->height上+1
  //  节点高度等于最高子树高度+1
  node->height = lh > rh ? lh + 1 : rh + 1;
  // 对更新函数的作用的理解，一定要和左旋右旋一起。
}

/*获取平衡因子，保证增删节点之后搜索二叉树依旧为平衡二叉树*/
int balancefactor(treenode *node) {
  // 空节点的平衡因子为0
  if (node == NULL) {
    return 0;
  }
  // 节点平衡因子 = 左子树高度-右子树高度
  return getheight(node->left) - getheight(node->right);
}

/*右旋操作，“/”情况，将node节点围绕左子节点向右旋转*/
treenode *rightrotate(treenode *node) {
  treenode *child, *grandchild;
  child = node->left;
  // 如果grandchild不为null，表示child节点有一个右子节点。child节点当然有一个左子节点，否则需要右旋的情况也就不会发生了。
  grandchild = child->right;
  // 以child为原点，将node向右旋转
  child->right = node;
  // grandchild是不是null，下面这一步都是合理的。将grandchild节点作为node节点的左子节点。
  node->left = grandchild;
  // 更新节点高度
  // 注意，在插入函数中其实更新了两次节点高度，第二次其实在这个右(左)旋函数中
  // node最后成为了child的右子节点，而视情况，它只可能有一个左子节点或干脆没有节点，因此，最后给node更新的情况也就只有两种：
  // 1,node有一个左子节点(这个左子节点必然是一个叶节点)，返回左子节点初始化的height，即0，大于为空的右子节点-1，
  // 由此完成最后的更新，更新lf + 1 = 1;
  // 2, node没有子节点。那么最后的更新是-1+1 = 0，“刚好“表示node是一个叶节点。
  updateheight(node);
  // 同样的，更新原node的子节点——现在是node的父节点的高度。child的度必然是2。而视上述的两种情况，child的平衡因子要么是-1，要么是0。也就是说，child的左子树的高度必然小于等于child的右子树
  // 当child的左右子树相等的时候，height=1，不相等的时候，height应该等于2
  // child的高度最后是由它的左右子节点来决定的，而在右旋中，child的左子节点根本就没有变过，所以，右子节点的状况决定了child的结果，
  // 所以，同样只有两种情况，且这两种情况与上面的两种情况严格对应：
  // 1，node有一个左子节点，child的左子树高度=（上一步计算的1），大于右子节点——一个叶节点的0，所以，最后更新child为1+1=2
  // 2,node没有一个左子节点，child的左右子树都是一个叶节点，结果为0+1 = 1
  updateheight(child);
  // 返回旋转后子树的根节点
  return child;
}

/*左旋操作，“\”情况，将node节点围绕左子节点向右旋转*/
treenode *leftrotate(treenode *node) {
  treenode *child, *grandchild;
  child = node->right;
  grandchild = child->left;
  // 以child为原点，将node向左旋转
  child->left = node;
  node->right = grandchild;
  // 更新节点高度
  updateheight(node);
  updateheight(child);
  // 返回旋转后子树的根节点
  return child;
}

/*执行旋转操作，使得该子树重新恢复平衡*/
treenode *rotate(treenode *node) {
  // 获取节点node的平衡因子
  int bf = balancefactor(node);
  ////判断树的偏转方向
  // 左偏树
  if (bf > 1) {
    if (balancefactor(node->left) >= 0) {
      // 右旋
      return rightrotate(node);
    } else {
      // 这种情况是在“/”基础上，最下面向右边拐了一下，成为“<”型
      // 先左旋，再右旋
      node->left = leftrotate(node->left);
      return rightrotate(node);
    }
  }

  // 右偏树
  if (bf < -1) {
    if (balancefactor(node->right) <= 0) {
      // 左旋
      return leftrotate(node);
    } else {
      // 在“\”的基础上，最下面向左边拐了一下，成为”>“型
      // 先右旋，再左旋
      node->right = rightrotate(node->right);
      return leftrotate(node);
    }
  }

  // 平衡树，无需旋转，直接返回
  return node;
}

/*递归地在树的节点中插入值为val的节点(辅助函数)*/
treenode *inserthelper(treenode *node, int val) {
  if (node == NULL)
    // 如果这一结构体指针为空，则表示尚未完成指针的初始化和结构体的初始化，表示目前树/子树中没有任何节点
    // 开出一块内存放结构体，并返回其位置给node指针，完成子树的添加工作
    // 初始化一个新节点。
    return newtreenode(val);
  // 1,查找位置并插入节点，要求符合二叉搜索树的定义
  if (val < node->val) {
    // 目标方向是左子树方向，继续向左遍历->添加
    // 这里递归函数的调用再一次调用了梦境，使得对二叉树的更新本质上是从叶节点到根节点的，这也符合探测失衡位置的逻辑，从底部开始测算是否需要右旋/左旋等等
    // 至于为什么要将插入函数的返回值赋值给node->left，是为了保证树的结构是正确的。一方面对于空节点来说，是为了将新的节点正确链接到树上。另一方面，则是由于每一次的插入，其实不仅仅是插入，还
    // 必定伴随着一次旋转，左或右，左右或右左，旋转最后返回的是一个当前子树的根节点，需要让node链接到新的子树，彻底完成更新。
    node->left = inserthelper(node->left, val);
  } else if (val > node->val) {
    // 目标方向是右子树方向，继续向右遍历->添加
    node->right = inserthelper(node->right, val);
  } else {
    // 重复节点不插入，直接返回
    return node;
  }
  // 更新二叉树的高度，更准确地说，是更新以node为根节点的二叉树的高度。
  updateheight(node);

  // 2，执行旋转操作，使得该子树重新恢复平衡
  node = rotate(node);

  // 返回子树的根节点
  return node;
}

/*在树中插入节点*/
void insert(avltree *tree, int val) {
  // 更新根节点
  tree->root = inserthelper(tree->root, val);
}

/*递归查找，递归删除节点（辅助函数）*/
treenode *removehelper(treenode *node, int val) {
  treenode *child, *grandchild;
  if (node == NULL) {
    // 未能找到要删除的节点
    return NULL;
  }
  /////1，查找节点并删除
  if (val < node->val) {
    // 向左遍历并删除，并更新左子树的根节点
    node->left = removehelper(node->left, val);
  } else if (val > node->val) {
    // 向右遍历并删除，并更新右子树的根节点
    node->right = removehelper(node->right, val);
  } else {
    // 刚好找到的要删除的节点
    ////子节点数量为0或1
    if (node->left == NULL || node->right == NULL) {
      // 记录node的左子节点
      child = node->left;
      if (node->right != NULL) {
        // 唯一的子节点是右子节点，重新赋值child
        child = node->right;
      }
      if (child == NULL) {
        // 不论是左子child还是右子child都为null，说明子节点数量 =
        // 0，待删除节点为叶节点，则直接return
        // NULL，这个NULL最后被node->left或node->right所接收，从而达成删除，删除之后就会进入更新节点高度和rotate操作，保证完全二叉树
        return NULL;
      } else {
        // 子节点数量=1，则将返回的节点直接设置为子节点，不再让node—>left或node->right指向待删除的node
        node = child;
      }
    } else {
      // 子节点数量 =2，
      // 则以右子树的最小节点作为下一个节点(其实也可以是左子树的最大节点)
      treenode *tmp = node->right;
      while (tmp->left != NULL) {
        tmp = tmp->left;
      }
      // 找到了右子树的最小值
      int tmpval = tmp->val;
      // 然后从右子树开始遍历，删除这个最小值原本对应的节点，并更新右子树的根节点。确保树的正确链接格式
      node->right = removehelper(node->right, tmp->val);
      // 更新被删除——其实是被替换的节点的值为右子树的最小值
      node->val = tmpval;
    }
  }
  // 更新节点高度
  updateheight(node);

  /////2，执行旋转操作，使得该子树重新恢复平衡
  node = rotate(node);

  // 返回子树的根节点
  return node;
}

/*删除节点*/
void removeitem(avltree *tree, int val) { treenode *root = removehelper(tree->root, val); }

/*查找节点*/
treenode *search(avltree *tree, int val) {
  treenode *cur = tree->root;
  // 循环查找，越过叶子节点后跳出
  while (cur != NULL) {
    if (cur->val < val) {
      // 目标节点在cur的右子树中
      cur = cur->right;
    } else if (cur->val > val) {
      // 目标节点在cur的左子树中
      cur = cur->left;
    } else {
      // 找到目标节点，跳出循环
      break;
    }
  }
  return cur;
}

/*打印函数*/
void printtree(treenode *root);

/*测试样例*/
void testinsert(avltree *tree, int val) {
  insert(tree, val);
  printf("\n插入节点 %d 后，avl树为 \n", val);
  printtree(tree->root);
}

void testremove(avltree *tree, int val) {
  removeitem(tree, val);
  printf("\n删除节点 %d 后，avl树为 \n", val);
  printtree(tree->root);
}

/*Driver Code*/
int main() {
  // 初始化空avl树
  avltree *tree = (avltree *)newtree();
  // 插入节点，请关注插入节点后，avl树是如何保持平衡的。
  testinsert(tree, 3);
  testinsert(tree, 2);
  testinsert(tree, 8);
  testinsert(tree, 1);
  testinsert(tree, 4);
  testinsert(tree, 5);
  testinsert(tree, 9);
  testinsert(tree, 7);
  testinsert(tree, 6);
  testinsert(tree, 10);
  testinsert(tree, 11);

  // 插入重复的节点
  testinsert(tree, 8);

  // 删除节点，请关注删除节点后，avl树是如何保持平衡的
  testremove(tree,
             8); // 删除度为0的节点
  testremove(tree,
             5); // 删除度为1的节点
  testremove(tree,
             4); // 删除度为2的节点

  // 查询节点
  treenode *node = search(tree, 7);
  printf("\n查询到的节点对象节点值 = %d \n", node->val);

  // 析构函数
  deltree(tree);

  return 0;
}

/*打印函数*/
typedef struct trunk {
  struct trunk *pre; // 上一个分支（往根节点方向）的结构体指针
  char *str;         // 当前分支的样式
} trunk;

trunk *newtrunk(trunk *pre, char *str) {
  trunk *neotrunk = (trunk *)malloc(sizeof(trunk));
  neotrunk->pre = pre;
  neotrunk->str = (char *)malloc(sizeof(char) * 10);
  strcpy_s(neotrunk->str, strlen(neotrunk->str) + 1, str);
  return neotrunk;
}

void showtrunks(trunk *neotrunk) {
  if (neotrunk == NULL)
    return;
  showtrunks(neotrunk->pre);
  // 从最高处打印到最低处，从根处打印到叶处，递归与栈的逻辑是一致的。
  printf("%s", neotrunk->str);
}

void printtreehelper(treenode *node, trunk *pre, bool isright) {
  if (node == NULL)
    return;
  char *prev_str = "    ";
  trunk *neotrunk = newtrunk(pre, prev_str);

  // 递归式打印，先打印右子树
  printtreehelper(node->right, neotrunk, true);
  if (pre == NULL) {
    neotrunk->str = "———";
  } else if (isright) {
    // 不是根节点，如果是右子树，则打印右子树的分支
    neotrunk->str = "/———";
    prev_str = "   |";
  } else {
    // 左子树，则打印左子树的分支，添加一个\作为转义符号
    neotrunk->str = "\\———";
    pre->str = prev_str;
  }
  // 让分支显现出来
  showtrunks(neotrunk);
  // 在分支上打印数字
  printf("%d\n", node->val);

  // 打印完右子树后，重置分支样式
  if (pre != NULL) {
    pre->str = prev_str;
  }
  neotrunk->str = "   |";

  // 再打印左子树
  printtreehelper(node->left, neotrunk, false);
}

void printtree(treenode *root) { printtreehelper(root, NULL, false); }
