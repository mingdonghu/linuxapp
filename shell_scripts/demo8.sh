#!/bin/bash

echo "输入参数的个数: $#"

if [ $# != 2 ]
then
  echo "输入参数的个数不为2,请重新输入"
else
  echo "第 1 个参数：$1";
  echo "第 2 个参数：$2";
fi