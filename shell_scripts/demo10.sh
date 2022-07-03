#!/bin/bash
# -e filename 如果 filename存在，则为真
# -d filename 如果 filename为目录，则为真
# -f filename 如果 filename为常规文件，则为真
# -L filename 如果 filename为符号链接，则为真
# -r filename 如果 filename可读，则为真
# -w filename 如果 filename可写，则为真
# -x filename 如果 filename可执行，则为真
# -s filename 如果文件长度不为0，则为真
# -h filename 如果文件是软链接，则为真

FILE_PATH="hello.txt"

if [ -e ${FILE_PATH} ]
then
    echo "文件存在"
else
    echo "文件不存在，创建文件"
    $(touch ${FILE_PATH})
    if [ -r ${FILE_PATH} ]
    then
        echo "文件可读"
    else
        echo "文件不可读"
    fi

    if [ -w ${FILE_PATH} ]
    then
        echo "文件可写"
    else
        echo "文件不可写"
    fi

    if [ -x ${FILE_PATH} ]
    then
        echo "文件可执行"
    else
        echo "文件不可执行"
    fi
fi

rm ${FILE_PATH}
echo "del ${FILE_PATH}"