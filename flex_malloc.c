#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "flex_malloc.h"

#ifdef FLEX_SUPPORT_MUTEX
#include <pthread.h>
#endif

#ifdef FLEX_DEBUG
#define FLEX_DBG(fmt, args...) printf(fmt, ##args)
#else
#define FLEX_DBG(fmt, args...)
#endif
#define FLEX_ERR(fmt, args...)   printf(fmt, ##args)

typedef struct {
    unsigned char bitmap[FLEX_BITMAP_NUM];      ///< bitmap = 总内存块数/8
    int offset[FLEX_HEAP_BLOCK];                ///< heap_block 较首地址的偏移
    int size[FLEX_HEAP_BLOCK];                  ///< heap_block 每一块内存的大小
    int total_num[FLEX_HEAP_BLOCK];             ///< heap_block 总的内存块
    int valid_num[FLEX_HEAP_BLOCK];             ///< heap_block 可用的内存块, 去除已申请
    char* heap[FLEX_HEAP_BLOCK + 1];             ///< 总的内存池
#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_t lock;
#endif
} flex_mem_t;

static flex_mem_t g_flex_mem;
static int g_init_flag = 0;
static int g_supp_max_str_len = 0;

int flex_mem_init(void)
{
    memset(&g_flex_mem, 0, sizeof(flex_mem_t));
    /* 分配内存 */
    long int pool_size = FLEX_4BYTE_BLOCK_NUM * 4 + FLEX_8BYTE_BLOCK_NUM * 8
        + FLEX_16BYTE_BLOCK_NUM * 16 + FLEX_32BYTE_BLOCK_NUM * 32 + FLEX_40BYTE_BLOCK_NUM * 40
        + FLEX_64BYTE_BLOCK_NUM * 64 + FLEX_128BYTE_BLOCK_NUM * 128
        + FLEX_256BYTE_BLOCK_NUM * 256 + FLEX_512BYTE_BLOCK_NUM * 512
        + FLEX_1024BYTE_BLOCK_NUM * 1024 + FLEX_2048BYTE_BLOCK_NUM * 2048;

    if (!(g_flex_mem.heap[0] = malloc(pool_size)))
    {
        return -1;
    }

    /* 初始化区块 */
    int i, j;
    unsigned char* bitmap;
    int index = 0;
    int offset = 0;;
    int block_num = 0;
    long int block_size = 0;
    bitmap = g_flex_mem.bitmap;
    for (i = 1; i <= FLEX_HEAP_BLOCK; i++) 
    {
        block_size = 0;
        switch(index)
        {
            case 0:
                if (FLEX_2048BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_2048BYTE_BLOCK_NUM * 2048;
                    block_num = FLEX_2048BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 2048)
                    {
                        g_supp_max_str_len = 2048;
                    }
                    index = 1;
                    break;
                }

            case 1:
                if (FLEX_1024BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_1024BYTE_BLOCK_NUM * 1024;
                    block_num = FLEX_1024BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 1024)
                    {
                        g_supp_max_str_len = 1024;
                    }
                    index = 2;
                    break;
                }

            case 2:
                if (FLEX_512BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_512BYTE_BLOCK_NUM * 512;
                    block_num = FLEX_512BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 512)
                    {
                        g_supp_max_str_len = 512;
                    }
                    index = 3;
                    break;
                }

            case 3:
                if (FLEX_256BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_256BYTE_BLOCK_NUM * 256;
                    block_num = FLEX_256BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 256)
                    {
                        g_supp_max_str_len = 256;
                    }
                    index = 4;
                    break;
                }

            case 4:
                if (FLEX_128BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_128BYTE_BLOCK_NUM * 128;
                    block_num = FLEX_128BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 128)
                    {
                        g_supp_max_str_len = 128;
                    }
                    index = 5;
                    break;
                }

            case 5:
                if (FLEX_64BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_64BYTE_BLOCK_NUM * 64;
                    block_num = FLEX_64BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 64)
                    {
                        g_supp_max_str_len = 64;
                    }
                    index = 6;
                    break;
                }

            case 6:
                if (FLEX_40BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_40BYTE_BLOCK_NUM * 40;
                    block_num = FLEX_40BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 40)
                    {
                        g_supp_max_str_len = 40;
                    }
                    index = 7;
                    break;
                }

            case 7:
                if (FLEX_32BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_32BYTE_BLOCK_NUM * 32;
                    block_num = FLEX_32BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 32)
                    {
                        g_supp_max_str_len = 32;
                    }
                    index = 8;
                    break;
                }

            case 8:
                if (FLEX_16BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_16BYTE_BLOCK_NUM * 16;
                    block_num = FLEX_16BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 16)
                    {
                        g_supp_max_str_len = 16;
                    }
                    index = 9;
                    break;
                }

            case 9:
                if (FLEX_8BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_8BYTE_BLOCK_NUM * 8;
                    block_num = FLEX_8BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 8)
                    {
                        g_supp_max_str_len = 8;
                    }
                    index = 10;
                    break;
                }

            case 10:
                if (FLEX_4BYTE_BLOCK_NUM)
                {
                    block_size = FLEX_4BYTE_BLOCK_NUM * 4;
                    block_num = FLEX_4BYTE_BLOCK_NUM;
                    if (g_supp_max_str_len < 4)
                    {
                        g_supp_max_str_len = 4;
                    }
                    index = 0;
                    break;
                }
        }

        int bit_offset = (block_num>8) ? (block_num/8 + ((block_num%8) ? 1 : 0)) : 1;
        /* 重置当前区块 bitmap */
        for (j = 0; j < bit_offset; j++)
        {
            bitmap[j] |= 0xFF;
        }

        /* 下一个区块 bitmap 偏移 */
        bitmap += bit_offset;

        /* 当前区块 bitmap 偏移 */
        g_flex_mem.offset[i - 1] = offset;
        FLEX_DBG("offset %d\n", offset);

        offset += bit_offset;

        g_flex_mem.size[i - 1] = block_size/block_num;
        g_flex_mem.total_num[i - 1] = block_num;
        g_flex_mem.valid_num[i - 1] = block_num;
        FLEX_DBG("size %d\n", g_flex_mem.size[i - 1]);

        /* 下一个区块地址 */
        g_flex_mem.heap[i] = g_flex_mem.heap[i - 1] + block_size;
    }

#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_init(&g_flex_mem.lock, NULL);
#endif  
    g_init_flag = 1;
    return 0;
}

void flex_mem_deinit(void)
{
#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_destroy(&g_flex_mem.lock);
#endif
    if(g_flex_mem.heap[0])
    {
        free(g_flex_mem.heap[0]);
    }
}
#define EZ_BYTE_ALIGN_32(size)  (((size) + (32) - 1)&(~((32) - 1)))

void *flex_mem_malloc(size_t size)
{
    static long long int cnt = 0;
    size_t ulen = (size_t)size - 1;
    int i, j, btlen, bufflen;
    char* heap;
    unsigned char* bitmap;

    if (!g_init_flag)
    {
        goto Exit;
    }

    if (size <= 0 || size > g_supp_max_str_len)
    {
        goto Exit;
    }

    if (1 >= ulen)
        ulen = 1;

    i = 0;
    while (ulen)
    {
        ulen = ulen >> 1;
        i++;
    }

    if (i == 1) i = 2;
    /* bufflen 一般按2的次方取值, 特殊块可以取特殊值, 但必须是8的倍数 */
    if (FLEX_40BYTE_BLOCK_NUM && (32 < size && size <= 40))
    {
        bufflen = 40;
    }
    else
    {
        bufflen = 1 << i;
    }

#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_lock(&g_flex_mem.lock);
#endif
    int curr = 0;
    for (j = FLEX_HEAP_BLOCK - 1; j >= 0; j--)
    {
        /* 从小到大匹配内存块, 小内存使用完了向上分配大内存 */
        if ((g_flex_mem.size[j] == bufflen && 0 < g_flex_mem.valid_num[j]) ||
            (g_flex_mem.size[j] > bufflen && 0 < g_flex_mem.valid_num[j]))
        {
            if (g_flex_mem.size[j] > bufflen)
            {
                FLEX_DBG("[info] %s %d block is no more, malloc %d block.\n", __func__, bufflen, g_flex_mem.size[j]);
            }
            bufflen = g_flex_mem.size[j];
            curr = j;
            break;
        }
    }

    if (0 > j)
    {
#ifdef FLEX_SUPPORT_MUTEX
        pthread_mutex_unlock(&g_flex_mem.lock);
#endif
        FLEX_ERR("[error] %s no memory, size %d.\n", __func__, bufflen);
        goto Exit;
    }

    heap = g_flex_mem.heap[curr];
    bitmap = g_flex_mem.bitmap + g_flex_mem.offset[curr];
    btlen = g_flex_mem.total_num[curr];
    /* 找到对应heap_blcok中未分配的地址 */
    for (i = 0; i < btlen; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (*bitmap & (0x80 >> j))
            {
                *bitmap &= (~(0x80 >> j));

                g_flex_mem.valid_num[curr]--;

#ifdef FLEX_SUPPORT_MUTEX
                pthread_mutex_unlock(&g_flex_mem.lock);
#endif

                FLEX_DBG("[info] %s malloc size %p-%d, surplus num %d.\n", __func__, heap, bufflen, g_flex_mem.valid_num[curr]);
                return heap;
            }
            heap += bufflen;
        }
        bitmap++;
    }

    FLEX_ERR("[error] %s Assertion.\n", __func__);

#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_unlock(&g_flex_mem.lock);
#endif

Exit:
    cnt++;
    if (0 == (cnt%30))
    {
        FLEX_ERR("[error] %s malloc cnt %lld.\n", __func__, cnt);
    }
    return malloc(EZ_BYTE_ALIGN_32(size));
}

void flex_mem_free(void* pointer)
{
    char* heap;
    unsigned char* bitmap;
    int i;
    int bufflen;
    int stroff;
    
    if (NULL == pointer) return;

    if (!g_init_flag)
    {
        goto Exit;
    }
    /* 判断地址有效性 */
    if (g_flex_mem.heap[0] > (char*)pointer || (char*)pointer >= g_flex_mem.heap[FLEX_HEAP_BLOCK])
    {
        goto Exit;
    }
    /* 寻找当前指针所在的区块位置 */
    for (i = 1; i <= FLEX_HEAP_BLOCK; i++) {
        if ((char*)pointer < g_flex_mem.heap[i])
            break;
    }
    i--;
    /* 获取当前指针分配的内存大小 */
    bufflen = g_flex_mem.size[i];

#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_lock(&g_flex_mem.lock);
#endif
    heap = g_flex_mem.heap[i];
    bitmap = g_flex_mem.bitmap + g_flex_mem.offset[i];

    /* 重置bitmap中对应的bit位 */
    stroff = 0;
    while ((char*)pointer != heap) {
        heap += bufflen;
        stroff ++;
    }

    bitmap = bitmap + (stroff>>3);
    *bitmap |= (0x80 >> (stroff&0x07));

    g_flex_mem.valid_num[i]++;

    FLEX_DBG("[info] %s free size %p-%d, surplus num %d.\n", __func__, heap, bufflen, g_flex_mem.valid_num[i]);

#ifdef FLEX_SUPPORT_MUTEX
    pthread_mutex_unlock(&g_flex_mem.lock);
#endif

    return;

Exit:
    free(pointer);
}
