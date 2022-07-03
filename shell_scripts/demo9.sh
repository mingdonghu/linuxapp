#!/bin/bash
# 1. 无输入参数,无返回值

## 函数定义
hello_func()
{
    printf "hello func\n"
}

## 函数调用
hello_func

# 2. 无输入参数,有返回值
## 函数定义
add()
{
  read -p "input a num: " num1
  read -p "input a num: " num2
  sum=`expr ${num1} + ${num2}`  # ``符号在键盘Esc键的下方
  return ${sum}
}

## 函数调用
add
echo "sum=$?" # 函数返回值在调用该函数后通过 $? 来获得

# 3. 有输入参数,有返回值
## 函数定义
add()
{
  echo "第 1 个参数：$1";
  echo "第 2 个参数：$2";
  sum=`expr $1 + $2`
  return ${sum}
}

## 函数调用
add 3 4
echo "sum=$?" # 函数返回值在调用该函数后通过 $? 来获得