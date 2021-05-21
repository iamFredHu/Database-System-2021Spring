/*
Q5:实现基于排序或散列的两趟扫描算法实现并（S R） 、交（S R） 、差（S - R）其中一种集合操作算法。
将结果存放在磁盘上，并统计并、交、差操作后的元组个数。
*/

#include <string.h>
#include "utils.c"

int two_scan_minus(int R_sort_start, int R_sort_finish, int S_sort_start, int S_sort_finish, int result_start)
{
  printf("----------基于排序的两趟扫描算法（差）----------\n\n");
  Buffer buf;
  initBuffer(520, 64, &buf);

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
  int remeber_hash[100][1000];
  memset(remeber_hash, 0, sizeof(remeber_hash));
  int last_sort_start;
  int last_i;
  int repeat = 0;
  int now_S_finish = 0;

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
        repeat = 0;
        readTuple(blk_S, i);
        value_S.x = tuple_value.x;
        value_S.y = tuple_value.y;

        count++;
        printf("(%d, %d)\n", value_S.x, value_S.y);
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
        continue;
      }
      if (S_sort_start == last_sort_start && i == last_i)
      {
        readTuple(blk_S, i);
        value_S.x = tuple_value.x;
        value_S.y = tuple_value.y;
        repeat++;
      }
      else
      {
        readTuple(blk_S, i);
        value_S.x = tuple_value.x;
        value_S.y = tuple_value.y;
        last_sort_start = S_sort_start;
        last_i = i;
        repeat = 0;
        remeber_hash[value_S.x][value_S.y] = 1;
        now_S_finish = 0;
      }

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
            blk_R = readBlockFromDisk(now_blk_R_number, &buf);
            last_file = now_blk_R_number;
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
        now_S_finish = 1;
      }
      else if (remember_value_R == value_S.x)
      {
        now_S_finish = 1;
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
            if (value_R.x == value_S.x && value_R.y == value_S.y && remeber_hash[value_S.x][value_S.y] == 1)
            {
              remeber_hash[value_S.x][value_S.y] = 0;
            }
            else if (value_R.x > value_S.x)
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
            blk_R = readBlockFromDisk(now_blk_R_number, &buf);
            last_file = now_blk_R_number;
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
      if (remeber_hash[value_S.x][value_S.y] == 1 && now_S_finish == 1)
      {
        count++;
        printf("(%d, %d)\n", value_S.x, value_S.y);
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
  printf("S和R的差集(S-R)有%d个\n", count);
  freeBuffer(&buf);
}

int main()
{
  two_scan_minus(301, 316, 317, 348, 500);
}
