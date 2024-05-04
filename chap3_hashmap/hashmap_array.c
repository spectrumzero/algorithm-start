#include "../utils/common.h"
#include <stdlib.h>
#include <string.h>

/*hash表默认大小*/
#define HASHMAP_CAPACITY 100

/*键值对 key-value*/
typedef struct pair {
  int key;
  char *val; // 值作为指针型变量储存。char*，字符指针型。
  // 1，char型变量只占一个字节。一般表示一个字母。
  // 2，char*型变量占8个字节。实际储存的是地址。
  // 3, 声明为指针类型，允许val指向任意长度的字符串。
  // 4，不使用int*，是未来避免类型转换带来的麻烦。
  // 5，不使用string*：因为c语言标准库中并没有c类似c++那样的std::string字符串类型。在c中，字符串通常是以字符数组的形式表示，以空字符‘\0’结尾。
  // 此外，使用char*，可以直接操作字符数组来进行字符串的处理，而不需要考虑std::std型所设计的动态内存分配和释放等复杂性问题。且兼容性更好。
} pair;

/* 键值对的集合*/
// 这个集合的存在，是为了方便遍历键值对
typedef struct mapset {
  void *
      set; // 可能是遍历值，也可能是遍历键，也可能是遍历键值对，所以暂时定义为void，需要时进行类型转换。
  int len; // 记录键值对的长度，方便遍历或者打印。
} mapset;

/*基于数组实现的hash表*/
// 这个hash表，里面所装的东西是一个又一个地址。
typedef struct arrayhash {
  // 初始化一个数组。数组里面放的是pair*变量，即键值对变量。
  // 这个时候我们一般也把这个键值对称作一个“桶”
  pair *buckets[HASHMAP_CAPACITY]; // 初始化一个最大容量
} arrayhash;

/*构造函数*/
arrayhash *newhash() {
  arrayhash *hashmap = (arrayhash *)malloc(sizeof(arrayhash));
  // 修改的源代码：添加以下循环。原来的构造函数并没有初始化hash表中的桶，运行时出现了段错误。
  for (int i = 0; i < HASHMAP_CAPACITY; i++) {
    hashmap->buckets[i] = NULL;
  }
  return hashmap;
}

/*析构函数*/
void delhashmap(arrayhash *hashmap) {
  for (int i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashmap->buckets[i] != NULL) {
      free(hashmap->buckets[i]->val);
      free(hashmap->buckets[i]);
    }
  }
  free(hashmap);
  // 注意，在构造函数和添加操作函数中，hashmap，bucket，bucket->val都是利用malloc动态分配内存的。所以在析构函数中会有三种释放。
}

/*hash函数*/
int hashfunc(int key) {
  int index = key % HASHMAP_CAPACITY;
  return index;
}

/*查询操作*/
// 加const修饰传入的形参，表示传入的数据不可改变。更加符合“查询”的定义
const char *get(const arrayhash *hashmap, const int key) {

  int index = hashfunc(
      key); // 利用hash函数将key的输入空间映射为数组所有索引所构成的合法的输出空间
  const pair *pair =
      hashmap->buckets
          [index]; // 从桶中获取键值对,当然这种说法也只是比较省略的说法。
  if (pair == NULL) {
    return NULL;
  }
  return pair->val; // 最终目的是返回键值对的值。注意这里的val是一个指针型变量
}

/*添加操作*/
// 添加键值对。构造函数仅仅是构造了容量大小的空桶,要往里面放键值对才有实际意义。
void put(arrayhash *hashmap, const int key, const char *val) {
  pair *newpair = (pair *)malloc(sizeof(pair));
  newpair->key = key;
  newpair->val = malloc(strlen(val) + 1);
  // 1,在没有内存记录，即没有已知的对象的情况下，在指针和指针所指向的内存地址(指针只是指向这一地址，但不意味这这个地址里面有了我们想要得到的字符串)之间建立联系的唯一办法，可能函数malloc函数。
  // 2,strlen用于计算字符串的长度，不包括末尾的空字符。
  // 3,+1个字节，是为了给字符串末尾的空字符'\0'留出空间，
  // 以便在将字符串作为c字符串处理的时候，确保字符串的结尾有一个空字符来表示字符串的结束。

  // strcpy(newpair->val, val); // clang：strcpy被弃用。
  // 改用strcpy_s，这个字符复制函数会在复制的时候检测目标缓冲区的大小(第二个参数，即strlen(val)+1)，并且只有在目标缓冲区足够大的时候才会执行复制操作，避免缓冲区溢出。
  strcpy_s(newpair->val, strlen(newpair->val) + 1, val);
  int index = hashfunc(key);
  hashmap->buckets[index] = newpair; // 最后添加到bucket中
}

/*对c语言中的char*和strlen()的进一步补充:
 *
 * 先来理解这行命令：
 *const char *str = "Hello";
  str是一个字符指针型变量。前面有const修饰，表示这个指针所指向的内存的内容是只读的，即“hello”是不可变的（但指针可变,可以被重新赋值）。
  我们已经知道，str这个变量存放的实际是一个地址。那么是什么地址？是“Hello”的H所在的地址，是首字符所在的地址。
 *
 *然后再去理解strlen()
  这个函数的更为完整的声明：
  size_t strlen(const char *str);
  即接受一个常量指针型变量（内容不可改，地址可以改），并返回一个无符号整数类型。
  传入的地址str，正如上面所说的，是字符串的首地址，所以这个函数会从字符穿的起始位置开始，逐个检查每个字符，知道遇到空字符’\0‘,不包括这个空字符地计算字符串的长度。
  而计算的单位是字节。因为一个英文字符一般是1个字节，所以也就是字符数了。
 *
*/

/*删除操作*/
// 删除某一key值所对应的键值对
void removeitem(arrayhash *hashtabel, const int key) {
  int index = hashfunc(key);
  free(hashtabel->buckets[index]->val);
  free(hashtabel->buckets[index]);
  hashtabel->buckets[index] = NULL;
}

/*关于释放内存的补充
 *也是看一个例子吧：
  int * p = malloc(sizeof(int));
  free(p);
  //这一步只是将p指向的内存空间标记为可用状态，以便在后续的内存分配操作可以再次使用这块地址
  //即p依旧指向已经释放的内存地址。而此时再次利用p访问原来的内存地址，会导致未经定义的行为。
  //即，通过释放，那一块内存变得没有主人了，而p之后不能蛮横地宣称这块内存仍然是程序所属的。
 *p = NULL;
  所以，为了避免上述问题，还应该将指针设置为NULL，表示该指针不再指向任何地址。
  这一步需要时在free的后面，道理是类似的，仅仅将指针设为空指针，不能解决内存泄漏的问题，即在程序对堆区的内存的使用结束后，应当主动让这一块内存重新成为可用的，没有归属的。
 *
 *堆区内存应该主动释放，来提高程序运行时的稳定性和性能。程序结束后当然就啥也没有了。
 *
 */

/*获取所有的键值对*/
// 这段代码将hash表中的键值对转换为一个键值对的集合，以方便打印。
void pairset(arrayhash *hashmap, mapset *set) {
  // 定义一个指向pair结构体（而非pair指针)的数组
  pair *entries;
  int index = 0, total = 0;
  /*统计有效键值对数量*/
  for (int i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashmap->buckets[i] != NULL) {
      total++;
    }
  }
  // 为指针所管理的地址分配足够的内存
  entries = malloc(sizeof(pair) * total);
  for (int i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashmap->buckets[i] != NULL) {
      // 为集合的键赋值
      entries[index].key = hashmap->buckets[i]->key;
      // 为集合的值赋值,val为字符指针，分两步：
      // 1，为指针所指向的地址，存放字符串的地址分配足够大小的内存
      entries[index].val = malloc(strlen(hashmap->buckets[i]->val) + 1);
      // 2,将当前桶中的值复制到新分配的内存空间中，完成赋值
      strcpy_s(entries[index].val, strlen(entries[index].val) + 1,
               hashmap->buckets[i]->val);
      index++;
    }
  }
  set->set = entries;
  set->len = total;
}

/*获取所有键*/
void keyset(arrayhash *hashmap, mapset *set) {
  int *keys;
  int i = 0, index = 0;
  int total = 0;
  // 统计有效键值对数量
  for (i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashmap->buckets[i] != NULL) {
      total++;
    }
  }
  keys = malloc(total * sizeof(int));
  for (i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashmap->buckets[i] != NULL) {
      keys[index] = hashmap->buckets[i]->key;
      index++;
    }
  }
  set->set = keys;
  set->len = total;
}

/*获取所有的值*/
void valueset(arrayhash *hashtabel, mapset *set) {
  char **vals;
  int i = 0, index = 0, total = 0;
  /*统计有效键值对数量*/
  for (i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashtabel->buckets[i] != NULL) {
      total++;
    }
  }
  vals = malloc(total * sizeof(char *));
  for (i = 0; i < HASHMAP_CAPACITY; i++) {
    if (hashtabel->buckets[i] != NULL) {
      vals[index] = hashtabel->buckets[i]->val;
      index++;
    }
  }
  set->set = vals;
  set->len = total;
}

/*打印hash表*/
void printhash(arrayhash *hashmap) {
  int i;
  mapset set;
  pairset(hashmap, &set);
  pair *entries = (pair *)set.set;
  for (i = 0; i < set.len; i++) {
    printf("%d -> %s\n", entries[i].key, entries[i].val);
  }

  free(set.set);
}

/*dirver code*/
int main() {
  // 初始化hash表
  arrayhash *hashmap = newhash();
  // 添加操作，在hash表中添加键值对，并打印
  put(hashmap, 24811, "第一");
  put(hashmap, 24812, "第二");
  put(hashmap, 24813, "第三");
  put(hashmap, 24814, "第四");
  put(hashmap, 24815, "第五");
  put(hashmap, 24416, "世界");
  put(hashmap, 24417, "hello");
  put(hashmap, 24418, "World");
  put(hashmap, 24219, "start");
  put(hashmap, 24440, "algorithm");
  put(hashmap, 24702, "问题解决");
  // 桶全部填满了。填满了才能保证获取操作正确。这个明天再补充。源代码的问题主要在key值上
  printf("\n打印完成后，hash表为\nKey -> Value\n");
  printhash(hashmap);

  // 查询操作，向hash表中输入键key，得到值value
  const char *word = get(hashmap, 24410);
  printf("\n输入号码24410,，查询到单词为 %s\n", word);

  // 遍历hash表
  // 遍历键值对：
  printf("\n遍历键值对 key -> value\n");
  printhash(hashmap);

  // 单独遍历键：
  mapset set;
  keyset(hashmap, &set);
  int *keys = (int *)set.set;
  printf("\n单独遍历键 key\n");
  for (int i = 0; i < set.len; i++) {
    printf("%d\n", keys[i]);
  }
  free(set.set);

  // 单独遍历值
  valueset(hashmap, &set);
  char **vals = (char **)set.set;
  printf("\n单独遍历值 value\n");
  for (int i = 0; i < set.len; i++) {
    printf("%s\n", vals[i]);
  }
  free(set.set);

  delhashmap(hashmap);
  return 0;
}
