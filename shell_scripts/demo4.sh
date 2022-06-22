#!/bin/bash
# 获取终端输入的数字到变量NUM
read -p "input a num" NUM
echo "NUM = ${NUM}"
# 获取终端输入的数字给到变量STR
read -p "input a string" STR
echo "STR = ${STR}"

# 输出
# # 方式一
echo "1:${STR} is ${NUM}"
# # 方式二
printf "2:${STR} is ${NUM}\n"
