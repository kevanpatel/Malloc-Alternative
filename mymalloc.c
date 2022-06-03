#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"


static char memory[MEMSIZE];



int mem_init(){
    if(MEMSIZE < 2*sizeof(struct Header)){
        return -1;
    }
    struct Header in_head, in_free, epilogue;
    in_head.status = ALLOCATED;
    in_head.size = sizeof(struct Header);
    in_head.prev = NULL;
    in_head.next = (struct Header *)(memory+sizeof(struct Header));
    
    in_free.status = FREE;
    in_free.size = MEMSIZE - 2*sizeof(struct Header);
    in_free.prev = (struct Header *)memory;
    in_free.next = (struct Header *) (memory + MEMSIZE - sizeof(struct Header));

    epilogue.status = ALLOCATED;
    epilogue.size = sizeof(struct Header);
    epilogue.prev = (struct Header *)(memory+sizeof(struct Header));
    epilogue.next = NULL;
    memcpy(memory, &in_head, sizeof(struct Header));
    memcpy(memory+sizeof(struct Header), &in_free, sizeof(struct Header));
    memcpy(memory+MEMSIZE-sizeof(struct Header), &epilogue, sizeof(struct Header));
    return 0;
}

void *mymalloc(size_t size, char *file, int line){
    struct Header test;
    struct Header *current;
    memcpy(&test, memory, sizeof(struct Header));
    if(test.status == 0){
        if(mem_init() == -1){
            printf("Not enough heap memory\n");
            return NULL;
        }
    }

    if(size % 8 != 0){
        size = 8 * ((size / 8) + 1);
    }

    memcpy(&test, memory, sizeof(struct Header));
    current = test.next;
    while(1){
        if(current == NULL){
            printf("No free space left\n%s Line: %d\n", file, line);
            return NULL;
        }
        if(current->status == ALLOCATED){
            current = current->next;
            continue;
        }
        if(current->status == FREE){
            if((current->size - sizeof(struct Header)) < size){
                current = current->next;
                continue;
            }
            if((size + 2*sizeof(struct Header)) >= current->size){
                current->status = ALLOCATED;
                return (void *)current + sizeof(struct Header);
            }
            
            struct Header *next = (struct Header *)((void *)current + size + sizeof(struct Header));
            next->status = FREE;
            next->size = current->size - size - sizeof(struct Header);
            next->next = current->next;
            
            if(next->next != NULL){
                next->next->prev = next;
            }
            
            next->prev = current;
            current->next = next;
            current->status = ALLOCATED;
            current->size = size + sizeof(struct Header);

            return (void *)current + sizeof(struct Header);
        }
    }
}

void myfree(void *ptr, char *file, int line){
    if(ptr == NULL){
        return;
    }
    int in_array = (ptr >= (void *)memory) && (ptr < (void *)(memory + MEMSIZE));
    if(!in_array){
        printf("Error: Pointer not obtained from malloc\n%s Line: %d\n", file, line);
        return;
    }
    struct Header *block = (struct Header *)(ptr - sizeof(struct Header));
    // If user manipulates memory this check may fail
    // Can guarantee not at start of chunk by looping through entire heap and making sure
    if(block->status != FREE && block->status != ALLOCATED){
        printf("Error: Pointer does not point to start of chunk\n%s Line: %d\n", file, line);
        return;
    }
    if(block->status == FREE){
        printf("Error: Double free\n%s Line: %d\n", file, line);
        return;
    }
    if((void *)block == memory || (void *)block == memory+MEMSIZE-sizeof(struct Header)){
        printf("Error: Pointer does not point to start of a valid chunk\n%s Line: %d\n", file, line);
        return;
    }
    block->status = FREE;
    struct Header *prev, *next;
    prev = block->prev;
    next = block->next;
    if(prev->status == ALLOCATED && next->status == ALLOCATED){
        return;
    }
    else if(prev->status == FREE && next->status == ALLOCATED){
        prev->size = prev->size + block->size;
        prev->next = next;
        next->prev = prev;
    }
    else if(prev->status == ALLOCATED && next->status == FREE){
        block->size = block->size + next->size;
        block->next = next->next;
        next->next->prev = block;
    }
    else if(prev->status == FREE && next->status == FREE){
        prev->size = prev->size + block->size + next->size;
        prev->next = next->next;
        next->next->prev = prev;
    }
    return;
}
