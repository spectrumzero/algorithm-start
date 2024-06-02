#include "../head/stack_array.h"
#include <limits.h>

#define MAX_SIZE 5000

/*基于数组实现的栈*/
typedef struct {
  int *data; // 存放数组元素
  int size;  // 记录栈实际大小
} arraystack;

/*构造函数*/
arraystack *newarraystack() {
  arraystack *stack = (arraystack *)malloc(sizeof(arraystack));
  // 初始化一个大容量，避免扩容
  stack->data = malloc(sizeof(int) * MAX_SIZE); // 5000大小的数组元素容量
  stack->size = 0;                              // 实际容量初始化为0
  return stack;                                 // 不返回就没有意义了哈哈。
}

/*析构函数 */
void delarraystack(arraystack *stack) {
  free(
      stack
          ->data); // 同理，两块内存都要释放，先释放数组的那一块，再释放结构体（这里的栈）的那一块。
  free(stack);
}

/*获取栈的长度*/
int size(arraystack *stack) { return stack->size; }

/*判断栈是否为空*/
bool isempty(arraystack *stack) {
  return stack->size == 0;
} // 实际上，return
  // stack->size==0,是在做比较，如果lhs=rhs，那么就返回真，bool为1,，表示是空；否则就返回0，表示非空。

/*入栈
 * 栈，先入后出。从栈顶入栈。num是入栈的新元素
 * 还请注意，最新的元素入栈前，栈顶的元素的下标为size-1。
 */
void push(arraystack *stack, int num) {
  if (stack->size == MAX_SIZE) {
    printf("栈已满\n");
    return;
  }
  stack->data[stack->size] = num;
  stack->size++;
}

/*访问栈顶元素*/
int peek(arraystack *stack) {
  if (stack->size == 0) {
    printf("栈为空\n");
    return INT_MAX; // 这个常量在标准库limits.h头文件中定义，表示整型数据类型的最大值。
  }
  return stack->data[stack->size - 1];
}

/*出栈
 * 栈先入后出，所以总是最新进入的元素最快被弹出。而最后一个元素的下标为size-1,那么让size-1-1,最后一个元素更新为原来的倒数第二个元素，此时已经寻不到size-1，就是删除了栈顶的元素
 */
int pop(arraystack *stack) {
  int val = peek(stack); // 保存要删除的栈顶元素
  stack->size--;
  return val;
}

/*drivercode*/
int main() {
  // 初始化栈
  arraystack *stack = newarraystack();

  // 元素入栈
  push(stack, 3);
  push(stack, 2);
  push(stack, 4);
  push(stack, 3);
  push(stack, 9);
  printf("栈 stack = ");
  printArray(stack->data, stack->size);

  // 访问栈顶元素
  int val = peek(stack);
  printf("栈顶元素 top = %d\n", val);

  // 元素出栈
  val = pop(stack);
  printf("出栈元素 pop = %d，出栈后 stack = ", val);
  printArray(stack->data, stack->size);

  // 获取栈的长度
  int size = stack->size;
  printf("栈的长度 size = %d\n", size);

  // 判断栈是否为空
  bool empty = isempty(stack); // 这里用bool型变量empty接收isempty函数的结果，若empty
                               // = 1，表明为真，即空。
  printf("栈是否为空 = %s\n",
         empty ? "true" : "false"); // 三目运算符，如果empty =
                                    // 1，则选择第一个表达式，如果为0，则是第二个表达式。

  // 释放内存
  delarraystack(stack); // 前面定义了释放方式

  return 0;
}
