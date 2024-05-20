/*二叉树的数组表示*/

#include "../utils/common.h"
#include <limits.h>

/*数组表示下的二叉树结构体*/
typedef struct arraybinarytree {
  int *tree;    // 实际存放值的地方。
  int capacity; // 容量
} arraybinarytree;

/*构造函数*/
arraybinarytree *newabt(int *arr, int arrcapacity) {
  arraybinarytree *abt = (arraybinarytree *)malloc(sizeof(arraybinarytree));
  abt->tree = (int *)malloc(sizeof(int) * arrcapacity);
  // memcpy是c标准库中的一个函数，将源内存区域的内容复制到目标内存区域
  memcpy(abt->tree, arr, sizeof(int) * arrcapacity);
  abt->capacity = arrcapacity;
  return abt;
}

/*析构函数*/
void delabt(arraybinarytree *abt) {
  free(abt->tree);
  free(abt);
}

/*将数组反序列化为二叉树：递归*/
// 这里的INT_MAX是为了应对任意二叉树的情况，而不必要求数组必须要符合完美二叉树的定义
TreeNode *arraytotreedfs(int *arr, int size, int i) {
  if (i < 0 || i >= size || arr[i] == INT_MAX) {
    return NULL;
  }
  TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
  // 先根（父节点）节点
  root->val = arr[i];
  // 再左子树
  root->left = arraytotreedfs(arr, size, 2 * i + 1);
  // 最后右子树
  root->right = arraytotreedfs(arr, size, 2 * i + 2);
  return root; // 返回根节点ROOT
}

/*获取列表容量*/
int getcapacity(arraybinarytree *abt) { return abt->capacity; }

/*获取索引为i的节点的值*/
int getval(arraybinarytree *abt, int i) {
  // 若索引越界，则返回INT_MAX，代表空位
  if (i < 0 || i >= getcapacity(abt))
    return INT_MAX;
  return abt->tree[i];
}

/*获取索引为i节点的左子节点的索引*/
int getleftindex(int i) { return 2 * i + 1; }

/*获取索引为i的节点的右子节点的索引*/
int getrightindex(int i) { return 2 * i + 2; }

/*获取索引为i节点的父节点的索引*/
// 在c语言中，"/"表示整除，向0截断。即，余数被舍弃
int getparentindex(int i) { return (i - 1) / 2; }

/*层序遍历*/
// 因为这里的二叉树本来就是基于数组表示的，所以，这里的层序遍历相对于之前基于链表表示（借助到了辅助用的队列）要简单一点。直接从原(二叉树结构体里的)数组copy到打印用的数组里就好了。
int *levelorder(arraybinarytree *abt, int *returncapacity) {
  // 初始化一个新的数组，确保有足够的容量将二叉树的数组copy过来
  int *res = (int *)malloc(sizeof(int) * getcapacity(abt));
  int index = 0;
  for (int i = 0; i < getcapacity(abt); i++) {
    // 仅仅只在有意义的时候copy
    if (getval(abt, i) != INT_MAX)
      // 后缀自增，先使用index当前的值进行赋值运算，然后再为index进行+1操作。
      res[index++] = getval(abt, i);
  }
  //*returncapacity表示接收到的有意义的值的数量
  *returncapacity = index;
  // 返回遍历的次序结果
  return res;
}

/*深度优先*/
// 对这一参数列表的解释：
// abt，表示传入的数组表示的二叉树的结构体
// i,表示数组元素的索引，传入的时候为0
// order，表示顺序的字符串，自己定义的
// res，用来存放打印二叉树结果的数组
// size，统计数组中的元素多少，方便用来打印数组。传入的时候为0.
//
void dfs(arraybinarytree *abt, int i, char *order, int *res, int *size) {
  // 若为空位，则返回
  if (getval(abt, i) == INT_MAX) {
    return;
  }
  // 前序遍历
  // 提示，strcmp是c标准库中的一个函数，用于比较两个字符串，若相等，则返回0
  if (strcmp(order, "pre") == 0) {
    // 找到位置要放入数组里的值
    res[(*size)++] = getval(abt, i);
  }

  // 注意： 循环外
  // 将传入的索引改为当前节点的左节点的索引
  // 基于链表表示的二叉树在遍历的时候，直接通过left或right操作符就能够逐步往深处探测、递归，改变方向。而基于数组表示的二叉树节点，在给定的一组索引和节点之间，也可以完成一一对应的关系，且不要求被表示的二叉树是完美二叉树。
  // 由此，基于数组表示的二叉树的节点，是以数组索引的方式得到定位的，同样有一个索引到节点的一一映射的关系。
  // 换言之，链表通过操作符，来向左子树/右子树进行递归，而数组，则是通过计算左子树/右子树的索引来精准定义递归的方向的。
  // 只要数组索引和二叉树的节点分布的关系是严格一一对应的，即一个数组索引的配合能且只能表示某一特定具体类型的二叉树，在这个时候，就可以直接通过计算索引（左2*i+1或右2*i+2）来得到递归的逻辑/方向。
  dfs(abt, getleftindex(i), order, res, size);

  // 中序遍历
  if (strcmp(order, "in") == 0) {
    res[(*size)++] = getval(abt, i);
  }

  // 注意：循环外
  // 将传入的索引改为当前节点的右节点的索引
  dfs(abt, getrightindex(i), order, res, size);

  // 后序遍历
  if (strcmp(order, "post") == 0) {
    res[(*size)++] = getval(abt, i);
  }

  // order是传入的一个固定的值，并不会随着递归而发生变化。以前序遍历为例，就只有第一个if生效，会执行if语句块内的内容。
  // 如果将这些if条件语句删去，那么实际上就只有两个递归，一个是左子树，一个是右子树。而这里是以数组的索引的形式替换了原来的链表指针寻找子树的形式。
  // 这样理解的话，那么距离完整的遍历，还差根节点，或者也可以说父节点。而前序遍历和中序遍历和后续遍历，所解决的其实就是打印当前节点的任务，
  // 也就是将当前二叉树的节点添加到负责打印的数组中去。
  // 那么，也就可以理解，为什么这三个if语句在相对于dfs来说的不同的位置，从根本上说，逻辑和前面写到的深度优先遍历的基本逻辑是一致的。根节点两个递归子树函数之前，之间，还是之后，就决定了是前序中序还是后序。
}

/*前序遍历*/
int *preorder(arraybinarytree *abt, int *returnsize) {
  // 构建一个数组，方便打印遍历的结果。更好的表达是，将依次遍历的结果放进这个数组中，方便更加直观地打印出来
  int *res = (int *)malloc(sizeof(int) * getcapacity(abt));
  // 这里size的作用是统计遍历到的有效值的数量，并最后返回给returnsize，方便打印。。
  int size = 0;
  // 而之所以是地址传入，是避免拷贝构造函数的调用。我需要的是同一个统计量
  dfs(abt, 0, "pre", res, &size);
  // 遍历完成后，接下来要做的事情就是打印了
  *returnsize = size;
  return res;
}

/*中序遍历*/
int *inorder(arraybinarytree *abt, int *returnsize) {
  int *res = (int *)malloc(sizeof(int) * getcapacity(abt));
  int size = 0;
  dfs(abt, 0, "in", res, &size);
  // 遍历完成后，接下来要做的事情就是打印了
  *returnsize = size;
  return res;
}

/*后续遍历*/
int *postorder(arraybinarytree *abt, int *returnsize) {
  int *res = (int *)malloc(sizeof(int) * getcapacity(abt));
  int index = 0;
  dfs(abt, 0, "post", res, &index);
  // 遍历完成后，接下来要做的事情就是打印了
  *returnsize = index;
  return res;
}

/*drivercode*/
int main() {
  // 初始化一个数组
  // 使用INT_MAX代表空位null，显式显示所有的none
  // 注意：以下数组为静态数组
  int arr[] = {1, 2,       3,       4,  INT_MAX, 6,       7, 8,
               9, INT_MAX, INT_MAX, 12, 28,      INT_MAX, 15};
  // 对于静态数组，可以用sizeof的方式去计算数组中元素的实际大小
  int arrcapacity = sizeof(arr) / sizeof(arr[0]);
  // 验证数组索引表示和二叉树的一一对应的关系，唯一地表示一个二叉树
  TreeNode *root = arraytotreedfs(arr, arrcapacity, 0);
  printf("\n初始化二叉树\n");
  printf("二叉树的数组表示\n");
  printArray(arr, arrcapacity);
  printf("二叉树的链表表示\n");
  printTree(root);

  // 通过上面的数组来初始化基于数组表示的二叉树的结构体。
  arraybinarytree *abt = newabt(arr, arrcapacity);

  // 访问节点(计算某一已知索引的相关索引（父、子）)，注意，最大索引为arrcapacity-1，超过这一索引则返回一个表示none的常量
  int i = 5;
  int l = getleftindex(i), r = getrightindex(i), p = getparentindex(i);
  printf("\n当前节点的索引为 %d，值为 %d\n", i, getval(abt, i));
  printf("其左节点的索引为 %d，值为 %d\n", l,
         l < arrcapacity ? getval(abt, l) : INT_MAX);
  printf("其右节点的索引为 %d，值为 %d\n", r,
         r < arrcapacity ? getval(abt, r) : INT_MAX);
  printf("其父节点的索引为 %d，值为 %d\n", p,
         p < arrcapacity ? getval(abt, p) : INT_MAX);

  // 遍历树
  int returnSize; // 计算列表的实际大小，方便打印
  int *res;

  // 层序遍历
  res = levelorder(abt, &returnSize);
  printf("\n层序遍历为： ");
  // 依次打印有意义的值
  printArray(res, returnSize);
  free(res);

  // 前序遍历
  res = preorder(abt, &returnSize);
  printf("前序遍历为： ");
  printArray(res, returnSize);
  free(res);

  // 中序遍历
  res = inorder(abt, &returnSize);
  printf("中序遍历为： ");
  printArray(res, returnSize);
  free(res);

  // 后序遍历
  res = postorder(abt, &returnSize);
  printf("后序遍历为： ");
  printArray(res, returnSize);
  free(res);

  // 释放内存
  delabt(abt);
  return 0;
}
