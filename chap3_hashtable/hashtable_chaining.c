#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设val最大值为 100
#define MAX_SIZE 100

/*键值对结构体*/
typedef struct pair
{
  int key;
  char val[MAX_SIZE]; // 数组表示字符，注意不同于先前所写的那个简单hash

} pair;

/*链表节点结构体*/
typedef struct node
{
  pair *pair;
  struct node *nextnode;
} node;

/*链式地址哈希表*/
typedef struct hashtablechaining
{
  int size;         // hash表实际数量
  int capacity;     // hash表的容量
  double loadthres; // 触发扩容的负载因子阈值
  int extendratio;  // 扩容倍数
  node **buckets;
  // 桶数组。buckets[i]表示一个指针/地址，而这个地址指向的变量，依旧是一个指针，即node*。
  // 而node*这个指针，最终指向的是node这一结构体变量。
  // node结构体里面，又有两个结构体指针，其中pair*变量存放的是指向pair的地址，即最后我们想要的键值对。
} hashtablechaining;

/*构造函数*/
hashtablechaining *newhashtable()
{
  hashtablechaining *hashtable =
      (hashtablechaining *)malloc(sizeof(hashtablechaining));
  hashtable->size = 0;
  hashtable->capacity = 4;
  hashtable->loadthres = 2.0 / 3.0;
  hashtable->extendratio = 2;
  hashtable->buckets = (node **)malloc(hashtable->capacity * sizeof(node *));
  for (int i = 0; i < hashtable->capacity; i++)
  {
    hashtable->buckets[i] = NULL; // 全部初始化为空桶
  }
  return hashtable;
}

/*析构函数*/
void delhashtable(hashtablechaining *hashtable)
{
  for (int i = 0; i < hashtable->capacity; i++)
  {
    node *curbucket = hashtable->buckets[i];
    while (curbucket)
    {
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
int hashfunc(hashtablechaining *hashtable, int key)
{
  return key % hashtable->capacity;
}

/*计算并获取当前的负载因子*/
double loadfactor(hashtablechaining *hashtable)
{
  return (double)hashtable->size / (double)hashtable->capacity;
}

/*查询操作*/
char *get(hashtablechaining *hashtable, int key)
{
  int index = hashfunc(hashtable, key);
  // 遍历桶，若找到key，则返回对应的val
  node *curbucket = hashtable->buckets[index];
  while (curbucket)
  {
    if (curbucket->pair->key == key)
    {
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
void extend(hashtablechaining *hashtable)
{
  // 暂存原来的hashtable
  int oldcap = hashtable->capacity;
  node **oldbuckets = hashtable->buckets;
  // 初始化扩容后的新hashtable
  hashtable->capacity *= hashtable->extendratio;
  hashtable->buckets = (node **)malloc(sizeof(node *) * hashtable->capacity);
  for (int i = 0; i < hashtable->capacity; i++)
  {
    hashtable->buckets[i] = NULL;
  }
  hashtable->size = 0;
  // 将键值对从原来的hash表搬运至新的hash表
  for (int i = 0; i < oldcap; i++)
  {
    node *curbucket = oldbuckets[i];
    while (curbucket)
    {
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
void put(hashtablechaining *hashtable, int key, const char *val)
{
  // 首先判断，当负载因子超过阈值的时候，则执行扩容
  if (loadfactor(hashtable) > hashtable->loadthres)
  {
    extend(hashtable);
  }
  int index = hashfunc(hashtable, key);
  // 遍历桶。若遇到指定的key，则更新对应的val并返回,这是第一种情况
  node *curbucket = hashtable->buckets[index];
  while (curbucket)
  {
    if (curbucket->pair->key == key)
    {
      strcpy_s(curbucket->pair->val, strlen(curbucket->pair->val) + 1, val);
      // strcpy(curbucket->pair->val, val); // 若遇到指定的key，则更新对应的val为传入的val
      return;
    }
    curbucket = curbucket->nextnode;
  }
  // 若无该key，则将键值对直接添加至链表头部
  pair *newpair = (pair *)malloc(sizeof(pair));
  newpair->key = key;                                    // 先拷贝键
  strcpy_s(newpair->val, strlen(newpair->val) + 1, val); // 再拷贝值
  // strcpy(newpair->val, val); // 再拷贝值
  node *newnode = (node *)malloc(sizeof(node));
  newnode->pair = newpair;
  newnode->nextnode = hashtable->buckets[index];
  hashtable->buckets[index] = newnode;
  hashtable->size++;
}

/*删除键值对操作*/
void removeitem(hashtablechaining *hashtable, int key)
{

  int index = hashfunc(hashtable, key);
  node *curbucket = hashtable->buckets[index];
  node *prebucket = NULL;
  while (curbucket)
  {
    if (curbucket->pair->key == key)
    {
      // 从中删除键值对
      if (prebucket)
      {
        prebucket->nextnode = curbucket->nextnode;
      }
      else
      {
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
void printhash(hashtablechaining *hashtable)
{
  for (int i = 0; i < hashtable->capacity; i++)
  {
    node *curbucket = hashtable->buckets[i];
    printf("[");
    while (curbucket)
    {
      printf("%d -> %s, ", curbucket->pair->key, curbucket->pair->val);
      curbucket = curbucket->nextnode;
    }
    printf("]\n");
  }
}

/*driver code*/
int main()
{
  // 初始化hash表
  hashtablechaining *hashtable = newhashtable();

  // 在hash表几次添加键值对，同时测试扩容机制
  put(hashtable, 12836, "小哈");
  put(hashtable, 15937, "小啰");
  put(hashtable, 16750, "小算");
  printf("\n添加完成后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  //测试扩容机制
  put(hashtable, 13276, "小法");
  printf("\n扩容，第四次添加完成后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  //继续添加
  put(hashtable, 10583, "小呀");
  printf("\n最终添加完成后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  //查询操作：向hash表中输入键key，得到值Value
  char *namevalue = get(hashtable, 15937);
  printf("\n输入学号 15937，查询到姓名 %s\n", namevalue);

  //删除操作，在hash中删除键值对
  removeitem(hashtable, 12836);
  removeitem(hashtable, 15937);
  printf("\n删除号码 12836 和 15937 后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  // 释放hash表占用的空间
  delhashtable(hashtable);
}
