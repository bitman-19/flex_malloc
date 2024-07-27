#ifndef __FLEX_MALLOC_H__
#define __FLEX_MALLOC_H__

#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLEX_DEBUG
#define FLEX_SUPPORT_MUTEX

#define FLEX_4BYTE_BLOCK_NUM        4
#define FLEX_8BYTE_BLOCK_NUM        9
#define FLEX_16BYTE_BLOCK_NUM       7
#define FLEX_32BYTE_BLOCK_NUM       2
#define FLEX_40BYTE_BLOCK_NUM       11  ///< 一般block为2的指数倍，若为特殊块需要特殊处理
#define FLEX_64BYTE_BLOCK_NUM       1
#define FLEX_128BYTE_BLOCK_NUM      0
#define FLEX_256BYTE_BLOCK_NUM      0
#define FLEX_512BYTE_BLOCK_NUM      0
#define FLEX_1024BYTE_BLOCK_NUM     0
#define FLEX_2048BYTE_BLOCK_NUM     0

#define FLEX_HEAP_BLOCK                 \
(                                       \
    0                                   \
    +(FLEX_4BYTE_BLOCK_NUM ? 1 : 0)     \
    +(FLEX_8BYTE_BLOCK_NUM ? 1 : 0)     \
    +(FLEX_16BYTE_BLOCK_NUM ? 1 : 0)    \
    +(FLEX_32BYTE_BLOCK_NUM ? 1 : 0)    \
    +(FLEX_40BYTE_BLOCK_NUM ? 1 : 0)    \
    +(FLEX_64BYTE_BLOCK_NUM ? 1 : 0)    \
    +(FLEX_128BYTE_BLOCK_NUM ? 1 : 0)   \
    +(FLEX_256BYTE_BLOCK_NUM ? 1 : 0)   \
    +(FLEX_512BYTE_BLOCK_NUM ? 1 : 0)   \
    +(FLEX_1024BYTE_BLOCK_NUM ? 1 : 0)  \
    +(FLEX_2048BYTE_BLOCK_NUM ? 1 : 0)  \
)

#define FLEX_BITMAP_NUM                                                                                                                 \
(                                                                                                                                       \
    ((FLEX_4BYTE_BLOCK_NUM>8)?(FLEX_4BYTE_BLOCK_NUM/8 + ((FLEX_4BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_4BYTE_BLOCK_NUM ? 1 : 0))             \
    +((FLEX_8BYTE_BLOCK_NUM>8)?(FLEX_8BYTE_BLOCK_NUM/8 + ((FLEX_8BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_8BYTE_BLOCK_NUM ? 1 : 0))            \
    +((FLEX_16BYTE_BLOCK_NUM>8)?(FLEX_16BYTE_BLOCK_NUM/8 + ((FLEX_16BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_16BYTE_BLOCK_NUM ? 1 : 0))        \
    +((FLEX_32BYTE_BLOCK_NUM>8)?(FLEX_32BYTE_BLOCK_NUM/8 + ((FLEX_32BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_32BYTE_BLOCK_NUM ? 1 : 0))        \
    +((FLEX_40BYTE_BLOCK_NUM>8)?(FLEX_40BYTE_BLOCK_NUM/8 + ((FLEX_40BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_40BYTE_BLOCK_NUM ? 1 : 0))        \
    +((FLEX_64BYTE_BLOCK_NUM>8)?(FLEX_64BYTE_BLOCK_NUM/8 + ((FLEX_64BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_64BYTE_BLOCK_NUM ? 1 : 0))        \
    +((FLEX_128BYTE_BLOCK_NUM>8)?(FLEX_128BYTE_BLOCK_NUM/8 + ((FLEX_128BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_128BYTE_BLOCK_NUM ? 1 : 0))    \
    +((FLEX_256BYTE_BLOCK_NUM>8)?(FLEX_256BYTE_BLOCK_NUM/8 + ((FLEX_256BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_256BYTE_BLOCK_NUM ? 1 : 0))    \
    +((FLEX_512BYTE_BLOCK_NUM>8)?(FLEX_512BYTE_BLOCK_NUM/8 + ((FLEX_512BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_512BYTE_BLOCK_NUM ? 1 : 0))    \
    +((FLEX_1024BYTE_BLOCK_NUM>8)?(FLEX_1024BYTE_BLOCK_NUM/8 + ((FLEX_1024BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_1024BYTE_BLOCK_NUM ? 1 : 0))\
    +((FLEX_2048BYTE_BLOCK_NUM>8)?(FLEX_2048BYTE_BLOCK_NUM/8 + ((FLEX_2048BYTE_BLOCK_NUM%8) ? 1 : 0)):(FLEX_2048BYTE_BLOCK_NUM ? 1 : 0))\
)


int flex_mem_init(void);
void flex_mem_deinit(void);
void *flex_mem_malloc(size_t size);
void flex_mem_free(void* pointer);

#ifdef __cplusplus
}
#endif

#endif //__FLEX_MALLOC_H__
