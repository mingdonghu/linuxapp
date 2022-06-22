#!/bin/bash
# 定义数组:
# # 方式一:
ARRAY=(0 1 2 3)
# # 方式二:
ARRAY=(
4
5
6
7
)
# # 方式三:
ARRAY[0]=8
ARRAY[1]=9
ARRAY[2]=10
ARRAY[3]=11

# 访问数组中的索引为3的元素
echo "ARRAY[3]: ${ARRAY[3]}"

# 访问数组所有元素
echo "all elements: ${ARRAY[@]}"

# 获取数组长度
# # 方式一
echo "1. array len: ${#ARRAY[@]}"
# # 方式二
echo "2. array len: ${#ARRAY[*]}"
