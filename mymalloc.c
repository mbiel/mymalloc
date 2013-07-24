#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

static Block *beginheap = NULL;

void initialize() {
  beginheap = (Block *)malloc(HEAP_SIZE+sizeof(Block));
  beginheap->next = NULL;
  beginheap->prev = NULL;
  beginheap->size = HEAP_SIZE;
  beginheap->free = true;
  beginheap->signum = SIGNATURE;
}

void memoryMap() {
  Block *p;
  for(p = beginheap; p != NULL; p=p->next) {
    char* lead = p->free == true ? "Free block" : "Used block";
    printf("%s: addr=%p size=%d prev=%p next=%p\n", lead, p, p->size, p->prev, p->next);
  }
  printf("\n");
}

void *myMalloc(unsigned int size)
{
  if (size <= 0) return NULL; // size must be greater than 0

  if(!beginheap) initialize(); // initialize the heap if it does not exist
  
  size = size + sizeof(Block);
  
  debug_print("Trying to allocate block of size %d\n", size);
  
  
  Block *p;
  
  for(p = beginheap; p != NULL; p = p->next)
    if(p->size >= size && p->free == true) { // walk through the heap until we find a big enough free block
      break;
    }
    
    if(p == NULL || p->size < size) { // if we got here and the size is too small, then there is no usable block
      debug_print("Failed to find enough free space!\n\n");
      return NULL;
    }
    
    if(p->size == size) { // all we need to do in this case is mark the block as used
      debug_print("exact match: allocating block of size %d\n", size);
      p->free = false;
      p->signum = SIGNATURE;
    } else { // we need to split the block in two: one used and one free
      Block *newfreeblock = (Block *)((void*)p + size);
      debug_print("allocating block of size %d from address %p to %p\n", size, p, newfreeblock);
      
      // this will be the new free block -- it follows the space we will allocate so set pointers appropriately
      newfreeblock->size = p->size - size;
      newfreeblock->next = p->next;
      newfreeblock->prev = p;
      newfreeblock->signum = SIGNATURE;
      if(newfreeblock->next) {
	newfreeblock->next->prev = newfreeblock;
      }
      newfreeblock->free = true;
      
      //this is the space we are using -- set it to precede the new free block
      p->size = size;
      p->free = false;
      p->next = newfreeblock;
      p->signum = SIGNATURE;
    }
    
    
    debug_print("Returning pointer at %p\n", (void*)(p+1)); // return pointer to usable space (not to header)
    return (void*)(p + 1);
    
}

void myFree(void *ptr) {
  if(ptr == NULL) { // do nothing
    debug_print("Invalid pointer passed to free -- cannot free null pointer\n\n");
    return;
  }
  
  Block *beginfreeblock = (Block*)(ptr - sizeof(Block)); // myFree is passed the user pointer - need to point to header
  if(beginfreeblock->free == true || beginfreeblock->size == 0) { // do nothing
    debug_print("Invalid pointer passed to free -- block already freed.\n\n");
    return;
  }
  if(beginfreeblock->signum != SIGNATURE) { // the pointer points to invalid memory -- do nothing
    debug_print("Invalid pointer passed to free -- signature does not match.\n\n");
    return;
  }
  
  debug_print("Now freeing block: addr=%p size=%d prev=%p next=%p\n\n", beginfreeblock, beginfreeblock->size, beginfreeblock->prev, beginfreeblock->next);
  
  beginfreeblock->free = true;
  
  //if there is a free block following, we should merge the blocks
  
  if(beginfreeblock->next != NULL && beginfreeblock->next->free == true) {
    Block *nextblock = beginfreeblock->next;
    if(nextblock->next != NULL) {
      Block *twoahead = nextblock->next;
      twoahead->prev = beginfreeblock;
    }
    beginfreeblock->size = beginfreeblock->size + nextblock->size;
    beginfreeblock->next = nextblock->next;
    debug_print("Merged following free block\n");
    debug_print("New block is: addr=%p size=%d prev=%p next=%p\n\n", beginfreeblock, beginfreeblock->size, beginfreeblock->prev, beginfreeblock->next);
  }
  
  //if there is a free block preceding, we should also merge the blocks
  if(beginfreeblock->prev != NULL && beginfreeblock->prev->free == true) {
    Block *prevblock = beginfreeblock->prev;
    if(beginfreeblock->next != NULL) {
      Block *nextblock = beginfreeblock->next;
      prevblock->next = nextblock;
      nextblock->prev = prevblock;
    } else {
      prevblock->next = NULL;
    }
    prevblock->size = prevblock->size + beginfreeblock->size;
    prevblock->free = true;
    debug_print("Merged leading free block\n");
    debug_print("New block is: addr=%p size=%d prev=%p next=%p\n\n", prevblock, prevblock->size, prevblock->prev, prevblock->next);
  }
  
}