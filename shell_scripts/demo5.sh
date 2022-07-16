#!/bin/bash

# 1. if判断
VAR_A=1
VAR_B=2

if [ ${VAR_A} == ${VAR_B} ]
then
  printf "VAR_A == VAR_B\n"
elif [ ${VAR_A} -lt ${VAR_B} ]
then
  printf "VAR_A < VAR_B\n"
else
  printf "VAR_A > VAR_B\n"
fi

# 2. for循环
## 循环输出数字
for i in 1 2 3 4 5 6 7 8 9 0
do
  echo "i = ${i}"
done

## 循环输出字符串
for str in alpha beta gamma deta epsilon zeta eta iota kappa lambda mu nu xi omicron pi rho sigma tau upsilon phi
do
  echo "str = ${str}"
done

# 3. case
read -p "input a num(1~3):" NUM 
case ${NUM} in
  1) printf "NUM = 1\n"
  ;;
  2) printf "NUM = 2\n"
  ;;
  3) printf "NUM = 3\n"
  ;;
  *) printf "error\n"
  ;;
esac

# 4. while
read -p "input loop number:" LOOP_NUM
while(( $LOOP_NUM>0 ))
do
  echo $LOOP_NUM
  let "LOOP_NUM--"
done

# 5. select 循环
:<<!
select 菜单对象 in 列表项内容
do
  相关命令
done
!

select DAY in Mon Tue Wed Thu Fri Sat Sun
do
  case $DAY in
    Mon) echo "Today is Monday" 
      ;;
    Tue) echo "Today is Tuesday" 
      ;;
    Wed) echo "Today is Wednesday" 
      ;;
    Thu) echo "Today is Thursday" 
      ;;
    Fri) echo "Today is Friday" 
      ;;
    Sat|Sun) echo "You can have a rest today" 
      ;;
    *) echo "Unknown input, exit now" && break 
      ;;
  esac
done