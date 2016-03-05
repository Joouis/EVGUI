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

#define THIN_BLOCK(ptr) ((struct thin_block *) ptr - 1)
#define BUFF(blk) ((char *) (blk + 1))

struct thin_block {
    struct thin_block *next;
    size_t sz;
};

static struct thin_block *free_list = NULL;

void thin_init_heap(struct heap_info *info)
{
    struct thin_block *tmp = (struct thin_block *) info->heap;
    tmp->next = NULL;
    tmp->sz = info->size - sizeof(*tmp);
    free_list = tmp;
}

void *thin_malloc(size_t sz)
{
    struct thin_block *last = NULL;
    struct thin_block *tmp = free_list;
    struct thin_block *next = NULL;
    if (sz % 8)
        sz = sz - (sz % 8) + 8;
    while (tmp) {
        if (tmp->sz >= sz) {
            next = tmp->next;
            if (tmp->sz > sz + sizeof(*next)) {
                next = (struct thin_block *) (BUFF(tmp) + sz);
                next->next = tmp->next;
                next->sz = tmp->sz - sz - sizeof(*next);
            }
            if (last) {
                last->next = next;
            } else {
                free_list = next;
            }
            tmp->sz = sz;
            return BUFF(tmp);
        }
        last = tmp;
        tmp = tmp->next;
    };
    return NULL;
}

void thin_coalesce_free_list()
{
    struct thin_block *tmp = free_list;
    while (tmp->next) {
        if ((struct thin_block *) (BUFF(tmp) + tmp->sz) == tmp->next) {
            tmp->sz += tmp->next->sz + sizeof(*tmp->next);
            tmp->next = tmp->next->next;
        } else {
            tmp = tmp->next;
        }
    }
}

int coalesce(struct thin_block *blk)
{
    if ((struct thin_block *) (BUFF(blk) + blk->sz) == blk->next) {
        blk->sz += blk->next->sz + sizeof(*blk->next);
        blk->next = blk->next->next;
        return 1;
    }
    return 0;
}

void thin_free(void *ptr)
{
    struct thin_block *last = NULL;
    struct thin_block *tmp = free_list;
    while (tmp && (void *) tmp < ptr) {
        last = tmp;
        tmp = tmp->next;
    }
    THIN_BLOCK(ptr)->next = tmp;
    if (last) {
        last->next = THIN_BLOCK(ptr);
        if (coalesce(last)) {
            coalesce(last);
        } else {
            coalesce(THIN_BLOCK(ptr));
        }
    } else {
        free_list = THIN_BLOCK(ptr);
        coalesce(THIN_BLOCK(ptr));
    }
    //thin_coalesce_free_list();
}

struct alloc_algo thin_algo = {
    .init = thin_init_heap,
    .malloc = thin_malloc,
    .free = thin_free,
};
