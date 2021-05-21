/*
Q2:实现两阶段多路归并排序算法（TPMMS）
利用内存缓冲区将关系R和S分别排序，并将排序后的结果存放在磁盘上。（不可定义长度大于10的整型或字符型数组）
*/

#include <string.h>
#include "utils.c"

int tpmms(int rstart, int rfinish, int wstart)
{
    Buffer buf;
    unsigned char *blk;
    int sstart, sfinish;
    int temp = rstart;
    int temp_start = rstart;
    int temp_finish = rfinish;
    int index_tuples = 0;
    int index_blks = 0;

    initBuffer(520, 64, &buf);

    //第一趟
    selectSortInBuf(rstart, rfinish, &buf);
    int m = (rfinish - rstart) / (buf.numAllBlk - 1) + 1; //划分为m个子集合
    unsigned char *compare_blk;
    unsigned char *output_blk;
    unsigned char *(sort_blks[buf.numAllBlk + 1]);
    int sort_blks_i[buf.numAllBlk + 1];
    int sposition[buf.numAllBlk + 1];
    int min_value;
    int min_position;
    int p_output;
    compare_blk = initNewBuffer(&buf);
    output_blk = initNewBuffer(&buf);

    tuple_value.x = FINISHED;
    tuple_value.y = FINISHED;
    for (int i = 1; i <= 8; i++)
    {
        writeTuple(compare_blk, i);
    }

    for (int i = 1; i <= m; i++)
    {
        sposition[i] = rstart + (i - 1) * (buf.numAllBlk - 1);
        blk = readBlockFromDisk(sposition[i], &buf);
        sort_blks[i] = blk;
        readTuple(blk, 1);
        writeTuple(compare_blk, i);
        sort_blks_i[i] = 2;
    }
    p_output = 1;

    while (1)
    {
        min_position = findMinPos(compare_blk);
        min_value = readTuple(compare_blk, min_position);
        //都不是finished即找到了最小值
        if (min_value != FINISHED)
        {
            writeTuple(output_blk, p_output);
            p_output++;
            if (p_output == 8)
            {
                printf("输出到文件%d.blk\n", wstart);
                tuple_value.x = wstart + 1;
                tuple_value.y = 0;
                writeTuple(output_blk, 8);
                writeBlockToDisk(output_blk, wstart, &buf);
                output_blk = initNewBuffer(&buf);
                wstart++;
                p_output = 1;
            }
            readTuple(sort_blks[min_position], sort_blks_i[min_position]);
            sort_blks_i[min_position]++;
            //M有下一个元素
            if (tuple_value.x != 0 && (sort_blks_i[min_position] - 1) <= 7)
            {
                writeTuple(compare_blk, min_position);
            }
            else
            {
                //S有下一块
                if ((sposition[min_position] - (rstart - 1)) % (buf.numAllBlk - 1) != 0 && sposition[min_position] != rfinish)
                {
                    freeBlockInBuffer(sort_blks[min_position], &buf);
                    sposition[min_position] = sposition[min_position] + 1;
                    blk = readBlockFromDisk(sposition[min_position], &buf);
                    sort_blks[min_position] = blk;
                    readTuple(blk, 1);
                    writeTuple(compare_blk, min_position);
                    sort_blks_i[min_position] = 2;
                }
                //否则Mi为空，置为特殊值FINISHED
                else
                {
                    tuple_value.x = FINISHED;
                    tuple_value.y = FINISHED;
                    writeTuple(compare_blk, min_position);
                }
            }
        }
        else
        {
            break;
        }
    }
    freeBuffer(&buf);
}

int main()
{
    printf("关系R排序\n\n");
    tpmms(1, 16, 301);
    printf("\n关系S排序\n\n");
    tpmms(17, 48, 317);
}