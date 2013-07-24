#define HEAP_SIZE 1000000
#define SIGNATURE 0xca11ab1e
#define DEBUG 1
#define debug_print(args ...) if (DEBUG) fprintf(stderr, args)
#include <stdbool.h>

/*****************************************
 * The block structure is a simple header
 * containing pointers to the next and previous
 * blocks of memory in the heap, a size field
 * indicating the allocated size, and a boolean
 * flag indicating whether the memory should
 * be regarded as free or allocated. There
 * is also a signum field which is set
 * to indicate whether a block has been set
 * by myMalloc to prevent attempting to
 * free inappropriate memory blocks.
 * 
 *****************************************/

typedef struct block {
  struct block *next;
  struct block *prev;
  unsigned int size;
  bool free;
  long signum;
} Block;

/************************************************
 * The myMalloc function takes a single unsigned integer
 * argument indicating the size to be allocated
 * and returns a void pointer to the first byte of 
 * the memory allocated by the function that
 * is usable by the caller.
 * 
 * A request for 0 bytes of memory returns a NULL
 * pointer. 
 * 
 * A request that cannot be fulfilled (e.g.,due 
 * to insufficient space) returns a NULL pointer.
 * 
 * The memory pointed to is not initialized in any
 * way.
 * 
 **************************************************/
void *myMalloc(unsigned int size);


/***************************************************
 * The myFree function takes a single void pointer
 * argument and returns nothing. The pointer should
 * point to a region of memory previously allocated 
 * by myMalloc. The memory so allocated will then be
 * considered free and available for reallocation.
 * 
 * If ptr points to a region that has already been 
 * freed, the call has no effect. If ptr is null,
 * the call has no effect. If the function cannot 
 * determine that the region was previously allocated
 * by myMalloc, the call has no effect.
 */
void myFree(void *ptr);

/*****************************************************
 * memoryMap simply walks through the list of blocks,
 * and for each block, prints whether it is free,
 * its address, its size, and the addresses of the previous 
 * and next blocks. It has no other effects.
 */
void memoryMap();

/*****************************************************
 * initialize() is a utility function that sets up
 * the heap by malloc'ing a 1MB (+ header size) block of 
 * memory and setting up a block pointer to the head.
 * 
 * It is not intended to be called by user programs
 * and is only called by myMalloc when it does not
 * detect a "beginheap" pointer to the start of the heap.
 */
void initialize();