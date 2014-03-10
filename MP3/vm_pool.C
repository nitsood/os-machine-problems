#include "vm_pool.H"
#include "page_table.H"
#include "console.H"
#include "utils.H"


VMPool::VMPool(unsigned long _base_address, unsigned long _size, FramePool* _frame_pool, PageTable* _page_table)
{
  base_address = _base_address;
  size = _size;
  frame_pool = _frame_pool;
  page_table = _page_table;
  
  //initializing the region descriptor list 
  num_regions = 0;
  next_start_address = base_address;
  unsigned long head = frame_pool->get_frame_address(frame_pool->get_frame());
  region_desc_list = (region_descriptor*)head;
  _page_table->register_vmpool(this);
  unsigned long descriptor_size = sizeof(region_descriptor);
  max_regions = PageTable::PAGE_SIZE/descriptor_size;
}


unsigned long VMPool::allocate(unsigned long _size)
{
  if(_size == 0)
  {
    Console::puts("\nRequested size allocation is 0, nothing to be done.");
    return 0;
  }
  
  unsigned long required_pages = _size;

  if(num_regions >= max_regions)
  {
    Console::puts("\nUnable to assign regions as descriptor list is full");
    return 0;
  }

  //calculate how many virtual memory pages will be required
  if((_size % PageTable::PAGE_SIZE) == 0)
    required_pages = _size/PageTable::PAGE_SIZE;
  else
    required_pages = ((int)_size/PageTable::PAGE_SIZE) + 1; //ceil

  //traverse the region descriptor list
  for(int i=0; i<num_regions; i++)
  {
    //1. check if any previously de-allocated region can fit the requirement
    if((region_desc_list[i].size >= _size) && !region_desc_list[i].allocated)
    {
      unsigned long fragmented_size = region_desc_list[i].size/PageTable::PAGE_SIZE - required_pages;
      region_desc_list[i].size = required_pages * PageTable::PAGE_SIZE;
      region_desc_list[i].allocated = 1;

      //2. add a new unallocated region for the fragmented block at the end of the descriptor list
      if(fragmented_size > 0)
      {
        region_desc_list[num_regions].start_address = region_desc_list[i].start_address + (required_pages*PageTable::PAGE_SIZE);
        region_desc_list[num_regions].size = fragmented_size * PageTable::PAGE_SIZE;
        region_desc_list[num_regions].allocated = 0; 
        num_regions++;
      }

      return region_desc_list[i].start_address;
    }
  }

  //if no unallocated region could be re-used, create a new region at the end of the descriptor list
  unsigned long addr = next_start_address;
  region_desc_list[num_regions].start_address = next_start_address;
  region_desc_list[num_regions].size = required_pages * PageTable::PAGE_SIZE;
  region_desc_list[num_regions].allocated = 1;
  num_regions++;

  //update next_start_address
  next_start_address += required_pages * PageTable::PAGE_SIZE;
  
  //return address of region allocated
  return addr;
}


void VMPool::release(unsigned long _start_address)
{
  //find the descriptor of the region to be released
  int i = 0;
  for(; i<num_regions; i++)
  {
    if(region_desc_list[i].start_address == _start_address)
      break;
  }

  if(i >= num_regions)
  {
    Console::puts("\nTrying to release a memory region that was not allocated previously");
    abort();
  }
 
  region_descriptor* u_region = &region_desc_list[i];
  unsigned long page_address = _start_address;
  for(i=0; i<(u_region->size/PageTable::PAGE_SIZE); i++)
  {
    //free each page in the region one by one
    page_table->free_page(page_address);
    page_address += PageTable::PAGE_SIZE;
  }
  
  //mark the region as 'unallocated' in the region descriptor list using a flag in the descriptor
  u_region->allocated = 0;

  //after deallocation, we need to reload the page table, so that the TLB is flushed
  page_table->load();
}


BOOLEAN VMPool::is_legitimate(unsigned long _address)
{
  for(int i=0; i<num_regions; i++)
  {
    if((region_desc_list[i].start_address <= _address) && 
       (_address < (region_desc_list[i].start_address+region_desc_list[i].size)) &&
       region_desc_list[i].allocated == 1
      )
      return true;
  }
  
  return false;
}

void VMPool::regions()
{
  Console::puts("\nNumber of regions: ");
  Console::puti(num_regions);
  for(int i=0; i<num_regions; i++)
  {
    Console::puts("\nRegion: ");
    Console::puti(i+1);
    Console::puts("\n");
    Console::putui(region_desc_list[i].start_address);
    Console::puts(" ");
    Console::putui(region_desc_list[i].size);
    Console::puts(" ");
    Console::puti((int)region_desc_list[i].allocated);
  }
}
