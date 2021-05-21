#define BLKSIZE 64
#define FINISHED 9999
#include "extmem.c"

typedef struct tuple
{
  int x;
  int y;
} Tup;

Tup tuple_value;

//从内存中读一个元组
int readTuple(unsigned char *blk, int num)
{
  tuple_value.x = 0;
  tuple_value.y = 0;
  unsigned char *new_blk = blk + (num - 1) * 8;
  for (int i = 0; i <= 3; i++)
  {
    if (*new_blk != 0 && *new_blk != ' ')
    {
      tuple_value.x *= 10;
      tuple_value.x += *new_blk - '0';
    }
    new_blk++;
  }
  for (int i = 0; i <= 3; i++)
  {
    if (*new_blk != 0 && *new_blk != ' ')
    {
      tuple_value.y *= 10;
      tuple_value.y += *new_blk - '0';
    }
    new_blk++;
  }
  return tuple_value.x;
}

//向内存中写一个元组
void writeTuple(unsigned char *blk, int num)
{
  int x = tuple_value.x;
  int y = tuple_value.y;
  char char_x[4];
  char char_y[4];
  unsigned char *new_blk = blk + (num - 1) * 8;

  int value_long = 0;

  for (int i = 0; i <= 3; i++)
  {
    if (x != 0)
    {
      char_x[i] = (x % 10) + '0';
      x = x / 10;
      value_long++;
    }
    else
    {
      char_x[i] = 0;
    }
  }
  for (int i = 0; i <= 3; i++)
  {
    if (value_long != 0)
    {
      value_long--;
      *new_blk = char_x[value_long];
    }
    else
    {
      *new_blk = '\0';
    }
    new_blk++;
  }

  value_long = 0;

  for (int i = 0; i <= 3; i++)
  {
    if (y != 0)
    {
      char_y[i] = (y % 10) + '0';
      y = y / 10;
      value_long++;
    }
    else
    {
      char_y[i] = 0;
    }
  }
  for (int i = 0; i <= 3; i++)
  {
    if (value_long != 0)
    {
      value_long--;
      *new_blk = char_y[value_long];
    }
    else
    {
      *new_blk = '\0';
    }
    new_blk++;
  }
}

int selectSortInBuf(int start_blk, int finish_blk, Buffer *buf)
{

  int blk_num = finish_blk - start_blk + 1; //磁盘块数量
  int m = (finish_blk - start_blk) / (buf->numAllBlk - 1) + 1;
  int current_blk = start_blk; 
  int blk_cnt = 0;
  unsigned char *blk;
  unsigned char *blks[buf->numAllBlk - 1];
  int sstart = start_blk;
  Tup t1, t2;

  //按组遍历
  for (int i = 0; i < m; i++)
  {
    blk_cnt = 0;
    for (; blk_cnt < (buf->numAllBlk - 1) && current_blk <= finish_blk; current_blk++, blk_cnt++)
    {
      blks[blk_cnt] = readBlockFromDisk(current_blk, buf);
    }
    //内排序
    for (int j = 0; j < blk_cnt * 7 - 1; j++)
    {
      int tuple_j_index = j / 7;
      int tuple_j_offset = j % 7 + 1;
      for (int k = j + 1; k < blk_cnt * 7; k++)
      {
        int tuple_k_index = k / 7;
        int tuple_k_offset = k % 7 + 1;
        readTuple(blks[tuple_j_index], tuple_j_offset);
        t1 = tuple_value;
        readTuple(blks[tuple_k_index], tuple_k_offset);
        t2 = tuple_value;
        if (t1.x > t2.x)
        {
          tuple_value = t2;
          writeTuple(blks[tuple_j_index], tuple_j_offset);
          tuple_value = t1;
          writeTuple(blks[tuple_k_index], tuple_k_offset);
        }
      }
    }

    for (int i = 0; i < blk_cnt; i++)
    {
      blk = blks[i];
      tuple_value.x = sstart + 1;
      tuple_value.y = 0;
      writeTuple(blk, 8);
      writeBlockToDisk(blk, sstart, buf);
      sstart++;
    }
  }
}

//找到最小值位置
int findMinPos(unsigned char *blk)
{
  int min_position = 1;
  int min_value = FINISHED;
  for (int i = 1; i <= 8; i++)
  {
    readTuple(blk, i);
    if (tuple_value.x < min_value)
    {
      min_value = tuple_value.x;
      min_position = i;
    }
  }
  return min_position;
}

//获得一个新块并清零
unsigned char *initNewBuffer(Buffer *buf)
{
  unsigned char *blkPtr;
  blkPtr = getNewBlockInBuffer(buf);
  memset(blkPtr, 0, (buf->blkSize) * sizeof(unsigned char));
}

void writeBlock1(unsigned char *blk, char str1[], char str2[])
{
  for (int i = 0; i < 4; i++)
  {
    *(blk + i) = str1[i];
  }
  for (int j = 4; j < 8; j++)
  {
    *(blk + j) = str2[j - 4];
  }
}

int writeBlock3(unsigned char *blk, int addr)
{
  char str[5];
  char str_Y[5];
  str[0] = (addr / 100) + 48;
  str[1] = (addr / 10) % 10 + 48;
  str[2] = addr % 10 + 48;
  memset(str_Y, 0, sizeof(str_Y));
  writeBlock1(blk + 8 * 7, str, str_Y);
  return 0;
}

int init_index()
{
  Buffer buf;
  unsigned char *blk;       // 一个指向磁盘块的指针
  unsigned char *blk_write; // 指向新建的用于写入的磁盘块
  initBuffer(520, 64, &buf);
  char str1[5];
  char str2[5];
  int count = 0;
  blk_write = getNewBlockInBuffer(&buf);
  for (int i = 0; i < 32; i++)
  {
    blk = readBlockFromDisk(317 + i, &buf);
    printf("读入数据块%d\n", 317 + i);
    for (int j = 0; j < 4; j++)
    {
      str1[j] = *(blk + j);
    }
    freeBlockInBuffer(blk, &buf);
    str2[0] = (317 + i) / 100 + 48;
    str2[1] = ((317 + i) / 10) % 10 + 48;
    str2[2] = (317 + i) % 10 + 48;
    str2[3] = 0;
    str2[4] = '\0';
    writeBlock1(blk_write + count * 8, str1, str2);
    count++;
    if (count == 7)
    {
      count = 0;
      writeBlock3(blk_write, 301 + i / 8 + 1);
      writeBlockToDisk(blk_write, 301 + i / 8, &buf);
      blk_write = getNewBlockInBuffer(&buf);
      memset(blk_write, 0, buf.blkSize * sizeof(unsigned char));
    }
  }
  writeBlock3(blk_write, 306);
  writeBlockToDisk(blk_write, 305, &buf);
  return 0;
}
