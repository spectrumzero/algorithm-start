#include "../utils/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 先前我们说，解决hash冲突有两种方式，其中一种是链式寻址，另一种是开放寻址的hash表。这里所写的，就是开放寻址的hash表。

/*键值对*/
typedef struct pair {
  int key;
  char *val;
} pair;

/*开放寻址的hash表的结构体*/
typedef struct hashmapopen {
  int size;
  int capacity;
  double loadthres;
  int extendratio;
  pair **buckets;  // 用作数组的指针/地址
  pair *tombstone; // 墓碑，作为删除标记。方便设置常量桶
} hashmapopen;

/*扩容函数的声明*/
void extend(hashmapopen *hashmap); // 注意没有{}

/*构造函数*/
hashmapopen *newhashmap() {
  hashmapopen *hashmap = (hashmapopen *)malloc(sizeof(hashmapopen));
  hashmap->size = 0;
  hashmap->capacity = 4;
  hashmap->loadthres = 2.0 / 3.0;
  hashmap->extendratio = 2;
  hashmap->buckets = (pair **)malloc(sizeof(pair *) * hashmap->capacity);
  //  修改：添加如下代码，避免段错误
  for (int i = 0; i < hashmap->capacity; i++) {
    hashmap->buckets[i] = NULL;
  }
  // 完成常量桶的初始化
  hashmap->tombstone = (pair *)malloc(sizeof(pair));
  hashmap->tombstone->key = -1;
  hashmap->tombstone->val = "-1";
  return hashmap;
}

/*析构函数*/
void delhashmap(hashmapopen *hashmap) {
  for (int i = 0; i < hashmap->capacity; i++) {
    pair *pair = hashmap->buckets[i];
    if (pair != NULL && pair != hashmap->tombstone) {
      free(pair->val);
      free(pair);
    }
  }
}

/*hash函数*/
int hashfunc(hashmapopen *hashmap, int key) { return key % hashmap->capacity; }

/*计算负载因子*/
double loadfactor(hashmapopen *hashmap) {
  // 转换为浮点数进行计算
  return (double)hashmap->size / (double)hashmap->capacity;
}

/*搜索 key 对应的桶索引*/
// 开放寻址的hash表的精髓或许就在这个搜索key所对应的桶索引里。
// 这个探索函数的功能在于，返回key所对应的桶的索引。注意，链式寻址是横向的，一个桶可能对应多个key；而开放寻址则是经典的纵向的，一个桶对应一个key
// 或者说，一个是通过指针，以一个桶为头节点来遍历单个桶；而另一个则是通过索引，直接遍历hash表。
// 最终返回的桶索引，如果找到了对应的key，那么返回的就是这个key所在的桶的索引。
// 如果遍历了hash表都没有找到key，那么返回的是一个常量桶的索引或者是一个空桶的索引。
int findkbucket(hashmapopen *hashmap, int key) {
  int index = hashfunc(hashmap, key);
  int firsttombstone = -1; // 删除标记
  // 线性探测,当遇到空桶的时候跳出
  while (hashmap->buckets[index] != NULL) {
    // 一、若步长探测时遇到了key，则返回对应的桶索引
    if (hashmap->buckets[index]->key == key) {
      // 若先前已经检测到了一个常量桶，则将所查找的键值对和删除标记调换位置。提高下一次的查找效率。
      if (firsttombstone != -1) {
        // 这个bucket[firsttombstone]先前被标记为合法的null桶，即存放的是键值对-1->-1所在的位置。现在，为了提高今后的查找效率，我们打算将这个桶后移。
        // 我们先让buckets[firsttombstone]重新存放一个新的位置，这个新的地址不再存放-1->-1的键值对的位置，而是存放我们所搜索的键值对的位置
        hashmap->buckets[firsttombstone] = hashmap->buckets[index];
        // 然后，我们再将原来-1->-1键值对的位置，赋值给key所对应的键值对的位置。从而让这个常量的桶距离原初搜索位置更远，提高查找效率
        hashmap->buckets[index] = hashmap->tombstone;
        // 请注意，最后不能返回index了，因为index桶刚刚被赋值为墓碑了。而原来的墓碑桶被拉进，成为了真正的index桶
        // 简单来讲，就是firsttombstone和index调换了位置。返回的buckets[firsttombstone]所放的确实是正确的键值对，而bucket[index]则是墓碑。不要弄错了。
        return firsttombstone;
      }
      // 没有优化、调换位置的话，当然就直接是index了。
      return index;
    }

    // 二、检测机制：在循环中检测到某一bucket[index]实际上是一个常量桶(先前进行过删除键值对的操作，即将被删除的键值对所在的桶设置为墓碑常量桶)。
    if (firsttombstone == -1 && hashmap->buckets[index] == hashmap->tombstone) {
      // 当前桶是一个常量（可以说是合法的null），将这一index赋值给first，保留下来。
      // 众所周知，buckets[-1]不存在。经过上面这个if判断，我们知道了当前index所对应的桶恰好为一个合法的null桶，所以，我们就需要更新墓碑桶的实际索引。
      // 之后，bucket[firsttombstone]就成为了合法那个墓碑桶
      firsttombstone = index;
    }
    // 三、计算桶索引，越过尾部则返回头部
    index = (index + 1) % hashmap->capacity;
  }

  // 如果探测不到key所对应的键值对所在的桶：
  //  若key所对应的桶一开始就是空桶NULL，则直接返回当前空桶的索引。
  //  但如果firsttombstone不等于-1，则返回的桶索引实际上是第一次遇到的常量桶索引，而不是最后空桶的索引。
  return firsttombstone == -1 ? index : firsttombstone;
}

/*查询操作，通过键查找值*/
char *get(hashmapopen *hashmap, int key) {
  // 搜索key对应的桶索引。
  int index = findkbucket(hashmap, key);
  // 若找到键值，则返回对应的val
  if (hashmap->buckets[index] != NULL && hashmap->buckets[index] != hashmap->tombstone) {
    return hashmap->buckets[index]->val;
  }
  // 若键值对不存在，则返回空字符串
  return "";
}

/*添加操作*/
void put(hashmapopen *hashmap, int key, const char *val) {
  // 当负载因子超过阈值时，执行扩容
  if (loadfactor(hashmap) > hashmap->loadthres) {
    extend(hashmap);
  }
  // 搜索key对应的桶索引，步长，一对一。
  int index = findkbucket(hashmap, key);
  // 若找到键值对，则覆盖更新val并返回
  if (hashmap->buckets[index] != NULL && hashmap->buckets[index] != hashmap->tombstone) {
    // 首先释放原字符指针的地址对应的内存
    free(hashmap->buckets[index]->val);
    // 然后为这个字符指针赋予新的地址和新的内存空间。
    hashmap->buckets[index]->val = (char *)malloc(sizeof(val) + 1);
    strcpy_s(hashmap->buckets[index]->val, strlen(hashmap->buckets[index]->val) + 1, val);
    hashmap->buckets[index]->val[strlen(val)] = '\0';
    return;
  }
  // 若键值对不存在，则添加该键值对
  pair *newpair = (pair *)malloc(sizeof(pair));
  newpair->key = key;
  newpair->val = (char *)malloc(strlen(val) + 1);
  strcpy_s(newpair->val, strlen(val) + 1, val);
  newpair->val[strlen(val)] = '\0';

  // 最后更新hash
  hashmap->buckets[index] = newpair;
  hashmap->size++;
}

/*删除操作*/
void removeitem(hashmapopen *hashmap, int key) {
  // 搜索key索对应的桶索引
  int index = findkbucket(hashmap, key);
  // 若找到键值对，则用删除标记覆盖它
  if (hashmap->buckets[index] != NULL && hashmap->buckets[index] != hashmap->tombstone) {
    // 暂存当前桶所存放的键值对的地址
    pair *delpair = hashmap->buckets[index];
    // 分别释放字符指针和键值对，完成删除
    free(delpair->val);
    free(delpair);
    // 后续：free只是让原指针指向的内存变为可以使用的，但原指针所存放的地址还是不变。一般来讲，我们会将原指针赋值为空指针，以避免未经定义的行为。
    // 但是，由于这是一个开放寻址的hash表，所以不能将某一个桶置为空指针，否则就无法访问到这个桶索引之后的桶（因为是线性步长访问的）
    // 因此，需要将这个桶，这个指针设置为一个常量桶，一个常量指针。这个常量指针是在狗仔hash表的时候就被初始化了。
    hashmap->buckets[index] = hashmap->tombstone;
    hashmap->size--;
  }
}

/*扩容hash表，一定要注意，扩容的桶，也要完成每一个桶的初始化！！*/
void extend(hashmapopen *hashmap) {
  // 暂存原来的hash表的桶，方便释放。扩容啊，覆盖啊，都差不多的逻辑。要造一个新的容器，然后丢掉旧容器。
  pair **bucketstmp = hashmap->buckets;
  int oldcap = hashmap->capacity;
  // 初始化扩容后的新hash表，这里根本上其实是初始化桶组。
  // 注意，只是扩大桶，即更新hash表里面的桶，而没有必要制造一个新的hash表。要做的是更新扩大旧有的hash表的容量。
  hashmap->capacity *= hashmap->extendratio;
  hashmap->buckets = (pair **)malloc(sizeof(pair *) * hashmap->capacity);
  // ！！！！！必须要完成每一个桶的初始化！！！
  for (int i = 0; i < hashmap->capacity; i++) {
    hashmap->buckets[i] = NULL;
  }
  // 这里需要将size置为零，因为下面将旧桶复制到新桶的时候，会继续计量。
  hashmap->size = 0;
  // 将有效的键值对从原hash搬运至新hashmap
  for (int i = 0; i < oldcap; i++) {
    pair *newpair = bucketstmp[i];
    if (newpair != NULL) {
      // 直接添加，直接更新
      put(hashmap, newpair->key, newpair->val);
      // 拷贝成功后即释放原有的桶
      free(newpair->val);
      free(newpair);
    }
  }
  free(bucketstmp);
}

/*打印hash表*/
void printhash(hashmapopen *hashmap) {
  for (int i = 0; i < hashmap->capacity; i++) {
    pair *newpair = hashmap->buckets[i];
    if (newpair == NULL) {
      printf("NULL\n");
    } else if (newpair == hashmap->tombstone) {
      printf("TOMBSTONE\n");
    } else {
      printf("%d -> %s\n", newpair->key, newpair->val);
    }
  }
}

/*driver code*/
int main() {
  // 初始化hash表
  hashmapopen *hashmap = newhashmap();

  // 添加操作，在hash表中添加键值对
  put(hashmap, 25111, "LAST");
  put(hashmap, 12836, "小哈");
  put(hashmap, 15937, "小啰");
  printf("\n添加完成后,hash表为\nKey —> Value\n");
  printhash(hashmap);

  // 查询操作，向hash表中输入键key，得到值Value
  char *name = get(hashmap, 25111);
  printf("\n输入学号 25111，查询到姓名 %s\n", name);

  // 删除操作，在hash表中删除键值对
  removeitem(hashmap, 15937);
  printf("\n删除 15937后，哈希表为\nKey -> Value\n");
  printhash(hashmap);

  put(hashmap, 13276, "小法");
  put(hashmap, 10583, "小鸭");
  printf("\n添加完成后,扩容后的hash表为\nKey —> Value\n");
  printhash(hashmap);

  removeitem(hashmap, 13276);
  printf("\n删除 13276后，哈希表为\nKey -> Value\n");
  printhash(hashmap);

  // 释放哈希表
  delhashmap(hashmap);
  return 0;
}
