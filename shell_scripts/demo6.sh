#!/bin/bash
# 如何在shell脚本中使用Linux命令

## 方式一: 使用()
echo "方式一: 使用 ()"
for file in $(ls /home)
do
  echo ${file}
done

## 方式二: 使用``,是键盘Esc下方的按键
echo "方式二: 使用\`\`"
for file in `ls /home`
do
  echo ${file}
done
