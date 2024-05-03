#include "../utils/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 先前我们说，解决hash冲突有两种方式，其中一种是链式寻址，另一种是开放寻址的hash表。这里所写的，就是开放寻址的hash表。

/*键值对*/
typedef struct pair
{
  int key;
  char *val;
} pair;

/*开放寻址的hash表的结构体*/
typedef struct hashtableopen
{
  int size;
  int capacity;
  double loadthres;
  int extendratio;
  pair **buckets;  // 用作数组的指针/地址
  pair *tombstone; // 墓碑，作为删除标记。
} hashtableopen;

/*扩容函数的声明*/
void extend(hashtableopen *hashtable); // 注意没有{}

/*构造函数*/
hashtableopen *newhashtable()
{
  hashtableopen *hashtable = (hashtableopen *)malloc(sizeof(hashtableopen));
  hashtable->size = 0;
  hashtable->capacity = 4;
  hashtable->loadthres = 2.0 / 3.0;
  hashtable->extendratio = 2;
  hashtable->buckets = (pair **)malloc(sizeof(pair *) * hashtable->capacity);
  //  !!!必须添加如下代码，避免段错误！
  for (int i = 0; i < hashtable->capacity; i++)
  {
    hashtable->buckets[i] = NULL;
  }
  hashtable->tombstone = (pair *)malloc(sizeof(pair));
  hashtable->tombstone->key = -1;
  hashtable->tombstone->val = "-1";
  return hashtable;
}

/*析构函数*/
void delhashtable(hashtableopen *hashtable)
{
  for (int i = 0; i < hashtable->capacity; i++)
  {
    pair *pair = hashtable->buckets[i];
    if (pair != NULL && pair != hashtable->tombstone)
    {
      free(pair->val);
      free(pair);
    }
  }
}

/*hash函数*/
int hashfunc(hashtableopen *hashtable, int key)
{
  return key % hashtable->capacity;
}

/*计算负载因子*/
double loadfactor(hashtableopen *hashtable)
{
  return (double)hashtable->size / (double)hashtable->capacity;
}

/*搜索 key 对应的桶索引*/
int findkbucket(hashtableopen *hashtable, int key)
{
  int index = hashfunc(hashtable, key);
  int firsttombstone = -1; // 删除标记
  // 线性探测,当遇到空桶的时候跳出
  while (hashtable->buckets[index] != NULL)
  {
    // 一、若遇到了key，则返回对应的桶索引
    if (hashtable->buckets[index]->key == key)
    {
      // 若已经知道key所对应的桶是一个常量桶，则将所查找的键值对和删除标记调换位置。提高下一次的查找效率。
      if (firsttombstone != -1)
      {
        hashtable->buckets[firsttombstone] = hashtable->buckets[index];
        hashtable->buckets[index] = hashtable->tombstone;
        // 对以上两步的说明：
        // 1,buckets[firsttombstone]原来是常量桶，这里则将这个常量桶赋值为key所对应的桶。然后，
        // 2,然后再将最后索引到的bucket[index]设置为常量桶，
        return firsttombstone; // 返回key对应的桶实际所在的位置firsttombstone
      }
      return index;
    }

    // 二、检测到key所对应的bucket[index]实际上是一个常量桶。
    if (firsttombstone == -1 &&
        hashtable->buckets[index] == hashtable->tombstone)
    {
      firsttombstone = index;
      // key所对应的桶是一个常量（可以说是合法的null），将这一index赋值给first，保留下来。
    }

    // 三、计算桶索引，越过尾部则返回头部
    index = (index + 1) % hashtable->capacity;
  }

  // 若key所对应的桶一开始就是空桶NULL，则直接返回当前空桶的索引。
  // 若不是一开始就是空桶，那么随着遍历的开展，也还是会判断空桶跳出，返回的同样是空桶的索引。
  // 还有一种情况是，也是最终因为是空桶跳出，但如果firsttombstone不等于-1，则返回的桶实际上是一个常量桶，而不是空桶。
  return firsttombstone == -1 ? index : firsttombstone;
}

/*查询操作，通过键查找值*/
char *get(hashtableopen *hashtable, int key)
{
  // 搜索key对应的桶索引
  int index = findkbucket(hashtable, key);
  // 若找到键值，则返回对应的val
  if (hashtable->buckets[index] != NULL &&
      hashtable->buckets[index] != hashtable->tombstone)
  {
    return hashtable->buckets[index]->val;
  }
  // 若键值对不存在，则返回空字符串
  return "";
}

/*添加操作*/
void put(hashtableopen *hashtable, int key, const char *val)
{
  // 当负载因子超过阈值时，执行扩容
  if (loadfactor(hashtable) > hashtable->loadthres)
  {
    extend(hashtable);
  }
  // 搜索key对应的桶索引
  int index = findkbucket(hashtable, key);
  // 若找到键值对，则覆盖val并返回
  if (hashtable->buckets[index] != NULL &&
      hashtable->buckets[index] != hashtable->tombstone)
  {
    free(hashtable->buckets[index]->val);
    hashtable->buckets[index]->val = (char *)malloc(sizeof(val) + 1);
    strcpy_s(hashtable->buckets[index]->val,
             strlen(hashtable->buckets[index]->val) + 1, val);
    hashtable->buckets[index]->val[strlen(val)] = '\0';
    return;
  }
  // 若键值对不存在，则添加该键值对
  pair *newpair = (pair *)malloc(sizeof(pair));
  newpair->key = key;
  newpair->val = (char *)malloc(strlen(val) + 1);
  // newpair->val = (char *)malloc(sizeof(strlen(val) + 1));
  strcpy_s(newpair->val, strlen(val) + 1, val);
  newpair->val[strlen(val)] = '\0';

  hashtable->buckets[index] = newpair;
  hashtable->size++;
}

/*删除操作*/
void removeitem(hashtableopen *hashtable, int key)
{
  // 搜索key索对应的桶索引
  int index = findkbucket(hashtable, key);
  // 若找到键值对，则用删除标记覆盖它
  if (hashtable->buckets[index] != NULL &&
      hashtable->buckets[index] != hashtable->tombstone)
  {
    pair *delpair = hashtable->buckets[index];
    free(delpair->val);
    free(delpair);
    hashtable->buckets[index] = hashtable->tombstone;
    hashtable->size--;
  }
}

/*扩容hash表，一定要注意，扩容的桶，也要完成每一个桶的初始化！！*/
void extend(hashtableopen *hashtable)
{
  // 暂存原来的hash表的桶
  pair **bucketstmp = hashtable->buckets;
  int oldcap = hashtable->capacity;
  // 初始化扩容后的新hash表，这里根本上其实是初始化桶组。
  hashtable->capacity *= hashtable->extendratio;
  hashtable->buckets = (pair **)malloc(sizeof(pair *) * hashtable->capacity);

  // ！！！！！必须要完成每一个桶的初始化！！！
  for (int i = 0; i < hashtable->capacity; i++)
  {
    hashtable->buckets[i] = NULL;
  }
  //这里需要将size置为零，因为下面将旧桶复制到新桶的时候，会继续计量。
  hashtable->size = 0;
  // 将键值对从原hash搬运至新hashtable
  for (int i = 0; i < oldcap; i++)
  {
    pair *newpair = bucketstmp[i];
    if (newpair != NULL && newpair != hashtable->tombstone)
    {
      put(hashtable, newpair->key, newpair->val);
      free(newpair->val);
      free(newpair);
    }
  }
  free(bucketstmp);
}

/*打印hash表*/
void printhash(hashtableopen *hashtable)
{
  for (int i = 0; i < hashtable->capacity; i++)
  {
    pair *newpair = hashtable->buckets[i];
    if (newpair == NULL)
    {
      printf("NULL\n");
    }
    else if (newpair == hashtable->tombstone)
    {
      printf("TOMBSTONE\n");
    }
    else
    {
      printf("%d -> %s\n", newpair->key, newpair->val);
    }
  }
}

/*driver code*/
int main()
{
  // 初始化hash表
  hashtableopen *hashtable = newhashtable();

  // 添加操作，在hash表中添加键值对
  put(hashtable, 25111, "LAST");
  put(hashtable, 12836, "小哈");
  put(hashtable, 15937, "小啰");
  put(hashtable, 22396, "wuhan");
  put(hashtable, 16750, "小算");
  put(hashtable, 13276, "小法");
  put(hashtable, 10583, "小鸭");
  printf("\n添加完成后,hash表为\nKey —> Value\n");
  printhash(hashtable);

  // 查询操作，向hash表中输入键key，得到值Value
  char *name = get(hashtable, 13276);
  printf("\n输入学号 13276，查询到姓名 %s\n", name);

  // 删除操作，在hash表中删除键值对
  removeitem(hashtable, 15937);
  printf("\n删除 15937后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  removeitem(hashtable, 13276);
  printf("\n删除 13276后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  removeitem(hashtable, 22396);
  printf("\n删除 22395后，哈希表为\nKey -> Value\n");
  printhash(hashtable);

  // 释放哈希表
  delhashtable(hashtable);
  return 0;
}
