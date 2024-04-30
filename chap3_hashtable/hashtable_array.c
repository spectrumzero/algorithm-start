#include "../utils/common.h"
#include <stdlib.h>

/*hash表默认大小*/
#define HASHTABLE_CAPACITY 100

/*键值对 key-value*/
typedef struct pair {
  int key;
  char *val;
} pair;

/* 键值对的集合*/
typedef struct tableset {
  void *set;
  int len;
} tableset;

/*基于数组实现的hash表*/
typedef struct arrayhash {
  pair *buckets[HASHTABLE_CAPACITY]; // 初始化一个数组。
} arrayhash;

/*构造函数*/
arrayhash *newhash() {
  arrayhash *hashtable = (arrayhash *)malloc(sizeof(arrayhash));
  return hashtable;
}

/*析构函数*/
void delhashtable(arrayhash *hashtable) {
  for (int i = 0; i < HASHTABLE_CAPACITY; i++) {
    if (hashtable->buckets[i] != NULL) {
      free(hashtable->buckets[i]->val);
      free(hashtable->buckets[i]);
    }
  }
  free(hashtable);
}

/*hash函数*/
int hashfunc(int key) {
  int index = key % HASHTABLE_CAPACITY;
  return index;
}

/*查询操作*/
