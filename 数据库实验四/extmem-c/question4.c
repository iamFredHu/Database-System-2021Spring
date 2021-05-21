/*
Q4:实现基于排序的连接操作算法（Sort-Merge-Join）
对关系S和R计算S.C连接R.A ，并统计连接次数，将连接结果存放在磁盘上。 
（模拟实现 select S.C, S.D, R.A, R.B from S inner join R on S.C = R.A）
*/

#include <string.h>
#include "utils.c"

int sort_merge_join(int R_sort_start, int R_sort_finish, int S_sort_start, int S_sort_finish, int result_start)
{
  printf("----------基于排序的连接操作算法----------\n\n");
  Buffer buf;
  if (!initBuffer(520, 64, &buf))
  {
    perror("Buffer Initialization Failed!\n");
    return -1;
  }

  unsigned char *blk_R;
  unsigned char *blk_S;
  unsigned char *wblk;
  int last_file = 0;
  int blk_R_number = R_sort_start;
  int blk_R_index = 1;
  int finish = 0;
  int first = 1;
  int last_value_S = 0;
  int now_blk_R_number = R_sort_start;
  int result_finish = result_start;
  int save_blk_R_number;
  int save_blk_R_index;
  Tup value_S;
  Tup value_R;

  int wblk_index = 1;
  int j_blk_R_index = 1;

  int count = 0;
  int remember_value_R;
  int find = 0;
  wblk = initNewBuffer(&buf);
  int last_sort_start;
  int last_i;
  int repeat = 0;

  for (; S_sort_start <= S_sort_finish; S_sort_start++)
  {

    if ((blk_S = readBlockFromDisk(S_sort_start, &buf)) == NULL)
    {
      perror("Reading R-Block Failed!\n");
      return -1;
    }

    for (int i = 1; i <= 7; i++)
    {
      if (repeat == 2)
      {
        continue;
      }
      if (S_sort_start == last_sort_start && i == last_i)
      {
        repeat++;
      }
      else
      {
        last_sort_start = S_sort_start;
        last_i = i;
        repeat = 0;
      }

      readTuple(blk_S, i);
      value_S.x = tuple_value.x;
      value_S.y = tuple_value.y;

      if (first == 1)
      {
        find = 0;
        while (now_blk_R_number <= R_sort_finish && find == 0)
        {
          if (last_file != now_blk_R_number)
          {
            if (last_file != 0)
            {
              freeBlockInBuffer(blk_R, &buf);
            }
            if ((blk_R = readBlockFromDisk(now_blk_R_number, &buf)) == NULL)
            {
              perror("Reading R-Block Failed!\n");
              return -1;
            }
            last_file = now_blk_R_number;
          }
          else
          {
          }
          for (; j_blk_R_index <= 7; j_blk_R_index++)
          {
            readTuple(blk_R, j_blk_R_index);
            value_R.x = tuple_value.x;
            value_R.y = tuple_value.y;
            if (value_R.x >= value_S.x)
            {
              find = 1;
              break;
            }
          }

          remember_value_R = value_R.x;
          blk_R_number = now_blk_R_number;
          blk_R_index = j_blk_R_index;
          j_blk_R_index = 1;
          now_blk_R_number++;
         }
        first = 0;
      }
      if (remember_value_R > value_S.x)
      {
        continue;
      }
      else if (remember_value_R == value_S.x)
      {
        now_blk_R_number = blk_R_number;
        j_blk_R_index = blk_R_index;
        find = 0;
        while (now_blk_R_number <= R_sort_finish && find == 0)
        {
          if (last_file != now_blk_R_number)
          {
            freeBlockInBuffer(blk_R, &buf);
            blk_R = readBlockFromDisk(now_blk_R_number, &buf);
            last_file = now_blk_R_number;
          }
          for (; j_blk_R_index <= 7; j_blk_R_index++)
          {
            readTuple(blk_R, j_blk_R_index);
            value_R.x = tuple_value.x;
            value_R.y = tuple_value.y;
            if (value_R.x == value_S.x)
            {
              count++;
              tuple_value.x = value_S.x;
              tuple_value.y = value_S.y;
              writeTuple(wblk, wblk_index);
              wblk_index++;
              if (wblk_index == 8)
              {
                printf("结果写入磁盘%d\n", result_finish);
                wblk_index = 1;
                tuple_value.x = result_finish + 1;
                tuple_value.y = 0;
                writeTuple(wblk, 8);
                writeBlockToDisk(wblk, result_finish, &buf);
                wblk = initNewBuffer(&buf);
                result_finish++;
              }
              tuple_value.x = value_R.x;
              tuple_value.y = value_R.y;
              writeTuple(wblk, wblk_index);
              wblk_index++;
              if (wblk_index == 8)
              {
                printf("注：结果写入磁盘%d\n", result_finish);
                wblk_index = 1;
                tuple_value.x = result_finish + 1;
                tuple_value.y = 0;
                writeTuple(wblk, 8);
                writeBlockToDisk(wblk, result_finish, &buf);
                wblk = initNewBuffer(&buf);
                result_finish++;
              }
            }
            else
            {
              find = 1;
              break;
            }
          }
          now_blk_R_number++;
          j_blk_R_index = 1;
        }
      }
      else
      {
        i--;
        now_blk_R_number = blk_R_number;
        j_blk_R_index = blk_R_index;
        find = 0;
        while (now_blk_R_number <= R_sort_finish && find == 0)
        {
          if (last_file != now_blk_R_number)
          {
            freeBlockInBuffer(blk_R, &buf);
            if ((blk_R = readBlockFromDisk(now_blk_R_number, &buf)) == NULL)
            {
              perror("Reading R-Block Failed!\n");
              return -1;
            }
            last_file = now_blk_R_number;
          }
          else
          {
          }
          for (; j_blk_R_index <= 7; j_blk_R_index++)
          {
            readTuple(blk_R, j_blk_R_index);
            value_R.x = tuple_value.x;
            value_R.y = tuple_value.y;
            if (value_R.x >= value_S.x)
            {
              find = 1;
              break;
            }
          }
          remember_value_R = value_R.x;
          blk_R_number = now_blk_R_number;
          blk_R_index = j_blk_R_index;
          now_blk_R_number++;
          j_blk_R_index = 1;
        }
      }
    }
    freeBlockInBuffer(blk_S, &buf);
  }
  if (wblk_index != 1)
  {
    printf("结果写入磁盘%d\n", result_finish);
    tuple_value.x = result_finish + 1;
    tuple_value.y = 0;
    writeTuple(wblk, 8);
    writeBlockToDisk(wblk, result_finish, &buf);
  }
  printf("总共连接%d次\n", count);
  freeBuffer(&buf);
}

int main()
{
  sort_merge_join(301, 316, 317, 348, 400);
}