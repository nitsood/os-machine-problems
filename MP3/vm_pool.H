/*
    File: vm_pool.H

    Author: R. Bettati, Nikhil Gupta
            Department of Computer Science
            Texas A&M University
    Date  : 10/26/2010

    Description: Management of the Virtual Memory Pool


*/

#ifndef _VM_POOL_H_                   // include file only once
#define _VM_POOL_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/
#include "utils.H"
#include "frame_pool.H"
/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* Forward declaration of class PageTable */
class PageTable;

typedef struct REGION_DESCRIPTOR {
  unsigned long start_address;
  unsigned long size;
  unsigned char allocated;       //1 for valid, 0 for invalid
} region_descriptor;



/*--------------------------------------------------------------------------*/
/* V M  P o o l  */
/*--------------------------------------------------------------------------*/

class VMPool { /* Virtual Memory Pool */
private:
   /* -- DEFINE YOUR VIRTUAL MEMORY POOL DATA STRUCTURE(s) HERE. */
   
   //base virtual address of the vm pool
   unsigned long base_address;
   
   //indicates the value of the address at which a new memory region can be 
   //allocated
   unsigned long next_start_address;
   
   //size of the vm pool
   unsigned long size;
   FramePool *frame_pool;
   PageTable *page_table;

   //pointer to the start of the region descriptor list
   region_descriptor *region_desc_list;

   //number of regions currently present in the vm pool, 
   //this includes both allocated and de-allocated regions
   unsigned long num_regions;     

   //maximum number of regions in this vm pool, this number 
   //is determined by the upper limit on the number of region descriptors
   //that can fit in one physical memory frame
   unsigned long max_regions;
 
public:
   VMPool(unsigned long _base_address,
          unsigned long _size,
          FramePool *_frame_pool,
          PageTable *_page_table);
   /* Initializes the data structures needed for the management of this
    * virtual-memory pool.
    * _base_address is the logical start address of the pool.
    * _size is the size of the pool in bytes.
    * _frame_pool points to the frame pool that provides the virtual
    * memory pool with physical memory frames.
    * _page_table points to the page table that maps the logical memory
    * references to physical addresses. */

   unsigned long allocate(unsigned long _size);
   /* Allocates a region of _size bytes of memory from the virtual
    * memory pool. If successful, returns the virtual address of the
    * start of the allocated region of memory. If fails, returns 0. */

   void release(unsigned long _start_address);
   /* Releases a region of previously allocated memory. The region
    * is identified by its start address, which was returned when the
    * region was allocated. */

   BOOLEAN is_legitimate(unsigned long _address);
   /* Returns FALSE if the address is not valid. An address is not valid
    * if it is not part of a region that is currently allocated. */

   /* Method for debugging */
   void regions();
};

#endif
