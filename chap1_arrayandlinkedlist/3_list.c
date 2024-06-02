#include "../head/list.h"

/*列表类*/
typedef struct {
  int *arr;        // 数组（存储列表元素）
  int capacity;    // 列表容量（可容纳元素数）
  int size;        // 列表实际大小（实际容纳的元素数）
  int extendratio; // 列表每次扩容的倍数
} mylist;

void extendcapacity(mylist *nums); // 先在这里写一个声明

/*构造一个新的列表*/
mylist *newlist() {
  mylist *nums = (mylist *)malloc(sizeof(
      mylist)); // 在c语言的较新标准中，推荐将malloc返回的通用指针void*显式转化为目标类型的指针
  nums->capacity = 10; // 初始容量设置为10
  nums->arr = (int *)malloc(
      sizeof(int) *
      nums->capacity); // arr接受存放列表元素的堆区地址，本质上是通过数组来管理列表元素
  nums->size = 0;        // 初始化列表时，列表里确实一个元素也没有
  nums->extendratio = 2; // 每次扩容为原来的两倍

  return nums; // 返回列表
}

/*释放内存（析构列表）*/
void dellist(mylist *nums) {
  free(nums->arr); // 释放存放的各个元素的内存，地址指向的是数组
  free(nums); // 释放列表各个要素的内存，例如，这里的nums->arr,也是开在堆区的。
}

/*获取列表实际长度*/
int size(mylist *nums) { return nums->size; }

/*获取列表容量*/
int capacity(mylist *nums) { return nums->capacity; }

/*访问索引index处的列表元素*/
int get(mylist *nums, int index) {
  assert(index >= 0 &&
         index <
             nums->size); // 这里利用了c标准库<assert.h>中的assert函数，来判断index是不是合法索引
  return nums->arr[index];
}

/*更新索引index处的元素*/
void set(mylist *nums, int index, int num) {
  assert(index >= 0 && index < nums->size);
  nums->arr[index] = num;
}

/*在尾部添加元素*/
void add(mylist *nums, int num) {
  if (size(nums) == capacity(nums)) // 利用size函数和capacity函数（的返回值）比较来判断是否需要扩容
  {
    extendcapacity(nums); // 扩容
  }
  nums->arr[size(nums)] = num;
  nums->size++; // 实际包含元素+1

  // 在尾部添加元素效率较高，算法复杂度为O(1)
}

/*插入元素*/
void insert(mylist *nums, int index, int num) {
  assert(index >= 0 && index < size(nums));
  // 元素数量超出容量时，触发扩容机制。添加扩容机制，也有效地解决了之前数组插入时，尾部元素丢失的问题
  if (size(nums) == capacity(nums)) // 利用size函数和capacity函数（的返回值）比较来判断是否需要扩容
  {
    extendcapacity(nums); // 扩容
  }
  for (int i = size(nums); i > index; i--) {
    nums->arr[i] =
        nums->arr[i - 1]; // 顺带一提，在索引2处插入元素，是在索引1的后面插入，即新元素的下标为2
  }
  nums->arr[index] = num;
  nums->size++;
}

/*删除元素*/
int removeitem(mylist *nums, int index) {
  assert(index >= 0 && index < size(nums));
  int num = nums->arr[index];
  for (int i = index; i < size(nums) - 1; i++) {
    nums->arr[i] = nums->arr[i + 1];
  }
  nums->size--;
  return num; // 返回删除的元素的值，便于检测
}

/*列表扩容*/
void extendcapacity(mylist *nums) {
  // 一，分配空间
  int newcapacity = capacity(nums) * nums->extendratio;   // 扩容后的可容纳的元素个数
  int *extend = (int *)malloc(sizeof(int) * newcapacity); // 将扩容后的数组内存地址返回
  int *temp =
      nums->arr; // 方便后续释放不再需要的旧数组内存。nums->arr稍后会将扩容后的数组的新地址存放在内，所以需要先释放旧地址内存，避免内存泄漏

  // 二，拷贝旧数据到新数据
  for (int i = 0; i < size(nums); i++) {
    extend[i] = nums->arr[i];
  }

  // 三，释放旧数据
  free(temp);

  // 四，更新新数据（落实到列表的要素中，扩展列表才是目的）
  nums->arr = extend;
  nums->capacity = newcapacity;
}

/*将列表转换为数组用于打印*/
int *toarray(mylist *nums) { return nums->arr; }

/*drivercode*/
int main() {
  // 初始化列表
  mylist *nums = newlist();

  // 在尾部添加元素
  add(nums, 2);
  add(nums, 9);
  add(nums, 8);
  add(nums, 5);
  add(nums, 3);

  // 打印列表
  printf("列表nums = ");
  printArray(toarray(nums), size(nums)); // 先把列表转换为数组，然后利用数组打印函数来打印列表
  printf("容量 = %d,实际长度 = %d\n", capacity(nums), size(nums));

  // 在中间插入元素
  insert(nums, 3, 6);
  printf("在索引3处插入数字6，得到nums = ");
  printArray(toarray(nums), size(nums));

  // 删除元素
  removeitem(nums, 3);
  printf("删除索引3处的元素，得到nums = ");
  printArray(toarray(nums), size(nums));

  // 访问元素
  int num = get(nums, 2);
  printf("访问索引2处的元素，得到num = %d\n", num);

  // 更新元素
  set(nums, 1, 0);
  printf("将索引1处的元素更新为0，得到nums = ");
  printArray(toarray(nums), size(nums));

  // 测试扩容机制
  for (int i = 0; i < 10; i++) {
    add(nums,
        i); // 在列表尾部添加元素i。测试前，列表的长度为5，在i=5时，add函数会判断此时列表的实际大小已经等于列表容量，会触发扩容机制之后再到尾部添加元素5
  }
  printf("扩容后的列表 nums = ");
  printArray(toarray(nums), size(nums));
  printf("容量 = %d,实际长度 = %d\n", capacity(nums), size(nums));

  // 释放分配内存
  dellist(nums);

  return 0;
}
