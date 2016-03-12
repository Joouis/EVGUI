/* Copyright (c) 2015 Peter Enns
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include "heap.h"
#include "sdram.h"

/* Double framebuffer used 320*240*2btye*2 = 307200  */
#define MALLOC_ADDR_START	(SDRAM_BANK_ADDR + 0x50000)
#define HEAP_SIZE (2 * 1024 * 1024)

extern struct alloc_algo thin_algo;

static struct heap_info _info = {
    .heap = (unsigned char *) MALLOC_ADDR_START,
    .size = HEAP_SIZE,
#ifdef USE_TREE_MALLOC
    .algo = &tree_algo,
#else
    .algo = &thin_algo,
#endif
    .initialized = 0,
};

void init_heap()
{
    _info.algo->init(&_info);
    _info.initialized = 1;
}

void *sram_malloc(size_t sz)
{
    if (!_info.initialized) {
        init_heap();
    }
    return _info.algo->malloc(sz);
}

void sram_free(void *ptr)
{
    if (!_info.initialized) {
        init_heap();
    }
    _info.algo->free(ptr);
}
