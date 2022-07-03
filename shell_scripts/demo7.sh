#!/bin/bash
# 1. 算数运算符
## expr 是一款表达式计算工具
read -p "please input a:" VAR_A
read -p "please input b:" VAR_B

## 加
RES=`expr ${VAR_A} + ${VAR_B}`  # 符号两边的空格不能省
echo "a + b : ${RES}"

## 减
RES=`expr ${VAR_A} - ${VAR_B}`
echo "a - b : ${RES}"

## 乘
RES=`expr ${VAR_A} \* ${VAR_B}`   # 乘号要转义\*
echo "a * b : ${RES}"

## 除
RES=`expr ${VAR_A} / ${VAR_B}`
echo "b / a : ${RES}"

## 取余
RES=`expr ${VAR_A} % ${VAR_B}`
echo "b % a : ${RES}"

## 等于
if [ ${VAR_A} == ${VAR_B} ]    # 注意空格！！
then
  echo "a == b"
fi

## 不等于
if [ ${VAR_A} != ${VAR_B} ]
then
  echo "a != b"
fi

# 2. 关系运算符
# -eq：等于。EQUAL的缩写。
# -ne：不等于。NOT EQUAL的缩写。
# -gt：大于。GREATER THAN的缩写。
# -lt：小于。LESS THAN的缩写
# -ge：大于等于。GREATER THAN OR EQUAL的缩写。
# -le：小于等于。LESS THAN OR EQUAL的缩写。

## 等于
if [ ${VAR_A} -eq ${VAR_B} ]   # 注意空格。
then
  echo "a 等于 b"
fi

## 不等于
if [ ${VAR_A} -ne ${VAR_B} ]  
then
  echo "a 不等于 b"
fi

## 大于
if [ ${VAR_A} -gt ${VAR_B} ]  
then
  echo "a 大于 b"
fi

## 小于
if [ ${VAR_A} -lt ${VAR_B} ]  
then
  echo "a 小于 b"
fi

## 大于等于
if [ ${VAR_A} -ge ${VAR_B} ]  
then
  echo "a 大于等于 b"
fi

## 小于等于
if [ ${VAR_A} -le ${VAR_B} ]  
then
  echo "a 小于等于 b"
fi

# 3. 逻辑运算符
## 逻辑与
if [[ ${VAR_A} -lt 6 && ${VAR_B} -gt 6 ]]  # 注意：1、使用[[]] 2、注意空格
then
  echo "true"
else
  echo "false"
fi

## 逻辑或
if [[ ${VAR_A} -lt 6 || ${VAR_B} -gt 6 ]]
then
  echo "true"
else
  echo "false"
fi

# 4. 字符串运算符
read -p "please input str_a:" VAR_A
read -p "please input str_b:" VAR_B

## 字符串相等判断
if [ ${VAR_A} = ${VAR_B} ]
then
  echo "a 等于 b"
fi

## 字符串不相等判断
if [ ${VAR_A} != ${VAR_B} ]
then
  echo "a 不等于 b"
fi

## 字符串长度是否为0
if [ -z ${VAR_A} ]
then
  echo "字符串 ${a} 长度为 0"
fi

## 字符串长度是否为0
if [ -n "${VAR_A}" ]
then
  echo "字符串 ${VAR_A} 长度不为 0"
fi

## 字符串是否为空
if [ $VAR_A ]
then
  echo "字符串 ${VAR_A} 不为空"
fi