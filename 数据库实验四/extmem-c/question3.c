/*
Q3:实现基于索引的关系选择算法
利用（2）中的排序结果为关系S建立索引文件，利用索引文件选出S.C=50的元组，并将选择结果存放在磁盘上。
记录IO读写次数，与（1）中的结果对比。（模拟实现 select S.C, S.D from S where S.C = 50）
*/

#include <string.h>
#include "utils.c"

int index_search()
{
    Buffer buf;
    unsigned char *blk;
    unsigned char *blk_write;
    int sc = 50;
    char s[3];
    /* 获取用户请求搜索的S.C值 */
    printf("------------------------------\n");
    printf("基于索引的关系选择算法 S.C=50:\n");
    printf("------------------------------\n");

    /* 定义一个buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    /* 定义相关存储数据块blk中数据的变量 */
    int x = -1;
    int y = -1;
    int z = -1;
    int min_block;
    int max_block;
    char str1[5];
    char str2[5];
    char str3[5];
    int count = 0;
    int block_index = 0;
    int t_number = 0;
    int flag = 0;

    init_index();
    for (int i = 0; i < 5; i++)
    {
        blk = readBlockFromDisk(301+i, &buf);
        printf("读入索引块：%d\n", 301+i);
        for (int j = 0; j < 7; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                str1[k] = *(blk + j*8 + k);
            }
            x =atoi(str1);
            if (x < sc)
            {
                for (int k = 0; k < 4; k++)
                {
                    str3[k] = *(blk + j*8 + 4 + k);
                }
                z = atoi(str3);
                continue;
            }
            else if (x > sc)
            {
                for (int k = 0; k < 4; k++)
                {
                    str2[k] = *(blk + j*8 + 4 + k);
                }
                y = atoi(str2);
                flag = 1;
                break;
            }
        }
        freeBlockInBuffer(blk, &buf);
        if (flag == 1)
        {
            flag = 0;
            break;
        }
    }
    min_block = z;
    max_block = y;
    blk_write = getNewBlockInBuffer(&buf);
    for (int i = min_block; i <= max_block; i++)
    {
        blk = readBlockFromDisk(i, &buf);
        printf("读入数据块%d\n", i);
        for (int j = 0; j < 7; j++)
        {
            for (int k = 0; k < 4; k ++)
            {
                str1[k] = *(blk + j*8 +k);
            }
            x = atoi(str1);
            for (int k = 0; k < 4; k++)
            {
                str2[k] = *(blk + j*8 + 4 + k);
            }
            y = atoi(str2);
            freeBlockInBuffer(blk, &buf);
            if (x == sc)
            {
                t_number ++;
                printf("找到元组(%d, %d)\n",x, y);
                writeBlock1(blk_write+8*count, str1, str2);
                count ++;
                if (count == 7)
                {
                    count = 0;
                    block_index ++;
                    writeBlock3(blk_write, 310+block_index);
                    writeBlockToDisk(blk_write, 309+block_index, &buf);
                    blk_write = getNewBlockInBuffer(&buf);
                    memset(blk_write, 0, buf.blkSize*sizeof(unsigned char));
                }
            }
        }
    }
    block_index ++;
    writeBlock3(blk_write, 310+block_index);
    writeBlockToDisk(blk_write, 309+block_index, &buf);
    printf("一共找到%d个符合要求的元组\n", t_number);
    printf("IO次数为 %lu\n", buf.numIO);
    printf("\n");
    return 0;
}

int main()
{
    index_search();
}
