#include "vm_pool.H"
#include "page_table.H"
#include "console.H"
#include "utils.H"

VMPool::VMPool(unsigned long _base_address, unsigned long _size, FramePool* _frame_pool, PageTable* _page_table)
{
  _page_table->register_vmpool(this);
}

unsigned long VMPool::allocate(unsigned long _size)
{}

void VMPool::release(unsigned long _start_address)
{}

BOOLEAN VMPool::is_legitimate(unsigned long _address)
{}
