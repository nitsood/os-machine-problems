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
  
  num_regions = 0;
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
  unsigned long start_address = 0;
  unsigned long required_pages = _size;

  if(num_regions >= max_regions)
  {
    //Unable to assign regions as desc_list is full;
    return 0;
  }

  //calculate the start address
  if(num_regions == 0)
    start_address = base_address;
  else
    start_address = region_desc_list[num_regions-1].start_address + region_desc_list[num_regions-1].size;
 
  //calculate how many virtual memory pages will be required
  if((_size % PageTable::PAGE_SIZE) == 0)
    required_pages = _size/PageTable::PAGE_SIZE;
  else
    required_pages = ((int)_size/PageTable::PAGE_SIZE) + 1; //ceil

  Console::puts("\nrequired size: ");
  Console::puti(_size);
  Console::puts(" and num pages: ");
  Console::puti(required_pages);
  Console::puts("\n");

  region_desc_list[num_regions].start_address = start_address;
  region_desc_list[num_regions].size = required_pages * PageTable::PAGE_SIZE;
  region_desc_list[num_regions].allocated = 1;
  num_regions++;
  
  return start_address;
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
    Console::puts("\nGoing to free the page: ");
    Console::putui(page_address);
    page_table->free_page(page_address);
    page_address += PageTable::PAGE_SIZE;
  }
  
  //mark the region as 'unallocated' in the region descriptor list using a flag in the descriptor
  u_region->allocated = 0;
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
    Console::puts("\n\nRegion: ");
    Console::puti(i+1);
    Console::puts("\n");
    Console::putui(region_desc_list[i].start_address);
    Console::puts(" ");
    Console::putui(region_desc_list[i].size);
    Console::puts(" ");
    Console::puti((int)region_desc_list[i].allocated);
  }
}
