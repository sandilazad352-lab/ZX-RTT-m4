#!/bin/bash

num=0
_bool=n
read -p "make sure rename y/n: " _bool
#在修改IFS前，保留原IFS
IFS_OLD=$IFS
#https://blog.csdn.net/qq_45698148/article/details/120064768
#设置换行为内部域分隔符
IFS=$(echo -en "\n\b")
ASSET_PATH="."
# ASSET_PATH="bg_anim/com0x"

if [ "$_bool" != "y" ]; then  
    echo "quit"
    exit;
else
    echo "start rename"
fi

for f in $(ls $ASSET_PATH/*.jpg);
do
    new_name=$(echo $num | awk '{printf("%d.jpg\n",$0)}')
    mv "$f" "$ASSET_PATH/$new_name"
    # echo "$f - $ASSET_PATH/$new_name"
    num=$((${num} + 1))
done

IFS=$IFS_OLD