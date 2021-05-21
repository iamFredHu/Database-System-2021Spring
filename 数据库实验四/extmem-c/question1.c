/*
Q1:实现基于线性搜索的关系选择算法
基于ExtMem程序库，使用C语言实现线性搜索算法，
选出S.C=50的元组，记录IO读写次数，并将选择结果存放在磁盘上。
（模拟实现 select S.C, S.D from S where S.C = 50）
*/

#include <string.h>
#include "utils.c"

int linear_search()
{
    Buffer buf;
    unsigned char *blk; // 指向磁盘块的指针
    unsigned char *blk_write; // 指向新建的用于写入的磁盘块

    int sc = 50;
    char s[3];

    printf("------------------------------\n");
    printf("基于线性搜索的选择算法 S.C=50:\n");
    printf("------------------------------\n");

    initBuffer(520, 64, &buf);

    //从Buffer中获取一个新的Block
    blk_write = getNewBlockInBuffer(&buf);

    int x = -1;
    int y = -1;
    int i = 0;
    int j = 0;

    int start_addr = 17; //文件17.blk至48.blk 为关系S的元组数据 所以从17开始
    int final_addr;
    int addr_counter = 1;
    int write_times = 0;
    int find_times = 0;
    char write_str[5];
    char str1[5];
    char str2[5];
    
    //读取数据块
    for (j = 0; j < 32; j++)
    {
        blk = readBlockFromDisk(start_addr, &buf);
        printf("读入数据块%d\n", start_addr);
        for (i = 0; i < 7; i++)
        {
            for (int k = 0; k < 4; k++)
            {
                str1[k] = *(blk + i*8 + k);
            }
            x = atoi(str1);// 元组中第一个值
            for (int k = 0; k < 4; k++)
            {
                str2[k] = *(blk + i*8 + 4 +k);
            }
            y = atoi(str2);// 元组中第二个值
            if (x == sc)
            {
                find_times ++;
                printf("(X=%d,Y=%d)\n",x, y);
                writeBlock1(blk_write+8*write_times, str1, str2);
                write_times ++;
                if (write_times == 7) //将blk_write写入磁盘
                {
                    str1[0] = '1';
                    str1[1] = '0';
                    str1[2] = '0'+addr_counter;
                    addr_counter++;
                    memset(str2, 0, sizeof(str2));
                    writeBlock1(blk_write+8*write_times, str1, str2);
                    write_times = 0;
                    writeBlockToDisk(blk_write, 100, &buf);
                    freeBlockInBuffer(blk_write, &buf);
                    blk_write = getNewBlockInBuffer(&buf);
                    memset(blk_write, 0, buf.blkSize*sizeof(unsigned char));
                }
            }
        }
        for (int k = 0; k < 4; k++)
        {
            write_str[k] = *(blk + i*8 +k);
        }
        start_addr = atoi(write_str);
        freeBlockInBuffer(blk, &buf);
    }
    
    str1[0] = '1';
    str1[1] = '0';
    str1[2] = '0' + addr_counter-1;
    final_addr = atoi(str1);
    writeBlockToDisk(blk_write, final_addr, &buf);

    printf("\n满足选择条件的元组一共%d个\n", find_times);
    printf("\nIO读写一共 %lu次\n", buf.numIO);// 输出IO的次数
    return 0;
}

int main()
{
    linear_search();
}
