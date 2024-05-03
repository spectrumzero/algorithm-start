#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 通常情况下，hash函数的输入空间远远大于输出空间，所以，hash冲突是不可避免的。即，必然会有多个整数key映射到同一桶索引。
// 解决hash冲突的第一种方式，是改良hash表的结构，改良方法主要包括链式地址和开放寻址。
// 这里主要是链式地址的实现。让桶和桶之间依旧是数组排列，可以直接通过计算偏移量寻址，
// 但在每个桶内部所存放的，就不是直接的键值对，而是可以扩展长度的链表节点。这样，每个桶就可以存放不止一个key。
// 最终对键值对的访问，是通过链表节点间接实现的。

// 假设val字符的最大值为 100
#define MAX_SIZE 100

/*键值对结构体*/
typedef struct pair {
  int key;
  char val[MAX_SIZE]; // 数组表示字符，注意不同于先前所写的那个简单hash

} pair;

/*链表节点结构体*/
typedef struct node {
  pair *pair;
  struct node *nextnode;
} node;

/*链式地址哈希表*/
typedef struct hashtablechaining {
  int size; // hash表实际数量，这里表示的是节点数量，准确说，键值对的数量。
  int capacity;     // hash表的容量，这里表示的是桶的数量
  double loadthres; // 触发扩容的负载因子阈值
  int extendratio;  // 扩容倍数
  node **buckets;
  // int a;表示一个整型变量
  // int * a; 表示一个存放了某一int型变量所在内存地址的变量,a是一个整形指针变量
  // int ** a;
  // 表示一个存放了某一整型指针变量所在的内存地址的变量，a是一个整型指针指针变量
  // 以此类推：
  // bucket是一个node型指针指针变量，存放的是node*这一node指针变量所在的地址。
  // 而node*这个指针，最终指向的是node这一结构体变量。
  // node结构体里面，又有两个结构体指针，其中pair*变量存放的是指向pair的地址，即最后我们想要的键值对。
  //
  // 整个访问键值对的流程：
  // 就像int *a通过地址直接访问某一int型变量b，bucket也是直接通过地址访问node*，
  // 然后node*也是直接通过地址访问node这一结构体下结构体指针变量pair，最后pair直接通过指针访问key，或者value
  //
  // 另外，bucket和bucket[i]是有区别的。
  // bucket正如上面所说的，是一个node**型，它直接这样出现表示的是指向node*的地址；
  // 但是bucket[i]，首先意味着规定bucket所指向的地址是一块连续分布的内存，可以通过计算偏移量来访问这块地址里的一个又一个元素，即node*变量
  // bucket[i]是node*，而bucket是node**。
  // 由此，最后对键值对的访问，就变成了bucket[i]->pair->key，bucket[i]->pair->value

} hashtablechaining;

/*构造函数*/
hashtablechaining *newhashtable() {
  hashtablechaining *hashtable =
      (hashtablechaining *)malloc(sizeof(hashtablechaining));
  hashtable->size = 0;
  hashtable->capacity = 4;
  hashtable->loadthres = 2.0 / 3.0;
  hashtable->extendratio = 2;
  hashtable->buckets = (node **)malloc(hashtable->capacity * sizeof(node *));
  for (int i = 0; i < hashtable->capacity; i++) {
    hashtable->buckets[i] = NULL; // 全部初始化为空桶
  }
  return hashtable;
}

/*析构函数*/
void delhashtable(hashtablechaining *hashtable) {
  for (int i = 0; i < hashtable->capacity; i++) {
    node *curbucket = hashtable->buckets[i];
    while (curbucket) {
      node *tmp = curbucket;
      curbucket = curbucket->nextnode;
      free(tmp->pair);
      free(tmp);
    }
  }
  free(hashtable->buckets);
  free(hashtable);
}

/*hash函数*/
int hashfunc(hashtablechaining *hashtable, int key) {
  return key % hashtable->capacity;
}

/*计算并获取当前的负载因子*/
double loadfactor(hashtablechaining *hashtable) {
  return (double)hashtable->size / (double)hashtable->capacity;
}

/*查询操作*/
char *get(hashtablechaining *hashtable, int key) {
  int index = hashfunc(hashtable, key);
  // 遍历桶，若找到key，则返回对应的val
  node *curbucket = hashtable->buckets[index];
  //
  while (curbucket) {
    if (curbucket->pair->key == key) {
      return curbucket->pair->val;
    }
    curbucket = curbucket->nextnode;
  }
  return ""; // 若未找到key，则返回空字符串
}

/*添加操作*/
// 因为添加操作实际上需要用到下面的扩容hash函数，所以这里只能写一个声明，而原型则应该写在扩容函数的后面。
void put(hashtablechaining *hashtable, int key, const char *val);

/*扩容hash表*/
void extend(hashtablechaining *hashtable) {
  // 暂存原来的hashtable
  int oldcap = hashtable->capacity;
  node **oldbuckets = hashtable->buckets;
  // 初始化扩容后的新hashtable
  hashtable->capacity *= hashtable->extendratio;
  hashtable->buckets = (node **)malloc(sizeof(node *) * hashtable->capacity);
  for (int i = 0; i < hashtable->capacity; i++) {
    hashtable->buckets[i] = NULL;
  }
  hashtable->size = 0;
  // 将键值对从原来的hash表搬运至新的hash表
  for (int i = 0; i < oldcap; i++) {
    node *curbucket = oldbuckets[i];
    while (curbucket) {
      put(hashtable, curbucket->pair->key, curbucket->pair->val);
      node *temp = curbucket;
      curbucket = curbucket->nextnode;
      // 释放内存
      free(temp->pair);
      free(temp);
    }
  }
  free(oldbuckets);
}

/*添加键值对操作*/
void put(hashtablechaining *hashtable, int key, const char *val) {
  // 首先判断，当负载因子超过阈值的时候，则执行扩容
  if (loadfactor(hashtable) > hashtable->loadthres) {
    extend(hashtable);
  }
  int index = hashfunc(hashtable, key);
  // 遍历这个桶(注意不是遍历hash表)。注意，一个又一个桶形成了数组，但单个桶里面放的是一个又一个链表节点（最终通过链表节点的值来获取键值对的值），这样可以避免hash冲突的情况。
  // 但下面这个while判断，并不是解决hash冲突，而是更新某一节点的值（根本来讲，它所指向的键值对的值）
  node *curbucket = hashtable->buckets[index];
  while (curbucket) {
    if (curbucket->pair->key == key) {
      strcpy_s(curbucket->pair->val, strlen(curbucket->pair->val) + 1, val);
      // strcpy(curbucket->pair->val, val); //
      // 若遇到指定的key，则更新对应的val为传入的val
      return;
    }
    curbucket = curbucket->nextnode;
  }
  // 上述循环完成了对单个桶的遍历，确保所传入的key在这个桶里面是独一无二的，但是这不意味着桶里面没有其它的链表节点。
  // 若无该key，则将键值对直接添加至链表头部
  // 1,初始化一个新的键值对
  pair *newpair = (pair *)malloc(sizeof(pair));
  newpair->key = key;                                    // 先拷贝键
  strcpy_s(newpair->val, strlen(newpair->val) + 1, val); // 再拷贝值
  // 2,初始化一个新的链表节点
  node *newnode = (node *)malloc(sizeof(node));
  newnode->pair = newpair;
  // 请注意，以下这两步，实际上让bucket[index]这个node*变量，这个链表节点，始终是链表的头节点。
  newnode->nextnode =
      hashtable->buckets
          [index]; // buckets[index]本质上存放的是node的地址，nextnode同样的存放的是node的地址。
  hashtable->buckets[index] = newnode;
  hashtable->size++;
}

/*删除键值对操作*/
void removeitem(hashtablechaining *hashtable, int key) {

  int index = hashfunc(hashtable, key);
  node *curbucket = hashtable->buckets[index];
  node *prebucket = NULL;
  while (curbucket) {
    if (curbucket->pair->key == key) {
      // 从中删除键值对
      if (prebucket) {
        prebucket->nextnode = curbucket->nextnode;
      } else {
        hashtable->buckets[index] = curbucket->nextnode;
      }
      // 释放内存
      free(curbucket->pair);
      free(curbucket);
      hashtable->size--;
      return;
    }
    prebucket = curbucket;
    curbucket = curbucket->nextnode;
  }
}

/*打印hash表*/
void printhash(hashtablechaining *hashtable) {
  for (int i = 0; i < hashtable->capacity; i++) {
    node *curbucket = hashtable->buckets[i];
    printf("[");
    while (curbucket) {
      printf("%d -> %s, ", curbucket->pair->key, curbucket->pair->val);
      curbucket = curbucket->nextnode;
    }
    printf("]\n");
  }
}

/*driver code*/
int main() {
  // 初始化hash表
  hashtablechaining *hashtable = newhashtable();

  // 在hash表几次添加键值对，同时测试扩容机制
  put(hashtable, 12836, "小哈");
  put(hashtable, 15937, "小啰");
  put(hashtable, 16750, "小算");
  printf("\n添加完成后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  // 测试扩容机制
  put(hashtable, 13276, "小法");
  printf("\n扩容，第四次添加完成后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  // 继续添加
  put(hashtable, 10583, "小呀");
  printf("\n最终添加完成后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  // 查询操作：向hash表中输入键key，得到值Value
  char *namevalue = get(hashtable, 15937);
  printf("\n输入学号 15937，查询到姓名 %s\n", namevalue);

  // 删除操作，在hash中删除键值对
  removeitem(hashtable, 12836);
  removeitem(hashtable, 15937);
  printf("\n删除号码 12836 和 15937 后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  // 释放hash表占用的空间
  delhashtable(hashtable);
}
