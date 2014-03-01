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
}

unsigned long VMPool::allocate(unsigned long _size)
{
  unsigned long descriptor_size = sizeof(region_descriptor);
  unsigned long max_regions = PageTable::PAGE_SIZE/descriptor_size;
  unsigned long start_address = 0;
  if(num_regions >= max_regions)
  {
    //Unable to assign regions as desc_list is full;
    return 0;
  }

  if(num_regions == 0)
    start_address = base_address;
  else
    start_address = region_desc_list[num_regions-1].base_address + region_desc_list[num_regions-1].size;
  
  region_desc_list[num_regions].base_address = start_address;
  region_desc_list[num_regions].size = _size;
  region_desc_list[num_regions].allocated = 1;
  num_regions++;
  
  return start_address;
}

void VMPool::release(unsigned long _start_address)
{}

BOOLEAN VMPool::is_legitimate(unsigned long _address)
{
  for(int i=0;i<num_regions;i++)
  {
    if (region_desc_list[i].base_address<=_address && _address<region_desc_list[i].base_address+region_desc_list[i].size)
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
    Console::putui(region_desc_list[i].base_address);
    Console::puts("\n");
    Console::putui(region_desc_list[i].size);
  }
}
