#include "page_table.H"
#include "paging_low.H"
#include "console.H"
#include "utils.H"

//initialization of static data members of class PageTable; need to do this to avoid linking errors
PageTable* PageTable::current_page_table = 0;
FramePool* PageTable::kernel_mem_pool = 0;
FramePool* PageTable::process_mem_pool = 0;
unsigned long PageTable::shared_size = 0;
unsigned int PageTable::paging_enabled = 0;

PageTable::PageTable()
{
  //page directory at fixed location (frame 514, 0x202000)
  unsigned long frame_addr = kernel_mem_pool->get_frame_address(kernel_mem_pool->get_frame());
  page_directory = (unsigned long*)frame_addr;

  //set up first page table (covering 0-4MB)
  //this page table will reside in the mapped memory, i.e. process pool
  //paging has not been enabled yet, so all addresses are physical
  frame_addr = process_mem_pool->get_frame_address(process_mem_pool->get_frame());
  unsigned long* page_table = (unsigned long*)frame_addr;
  unsigned long addr = 0; //32-bit
  
  //first page table's page location (frame 515, 0x203000)
  //frame_addr = kernel_mem_pool->get_frame_address(kernel_mem_pool->get_frame());
  //unsigned long* page_table = (unsigned long*)frame_addr;
  //unsigned long addr = 0; //32-bit
  
  //setting up page table for 0-4MB (direct mapping)
  int i, j;
  for(i=0; i<ENTRIES_PER_PAGE; i++)
  {
    page_table[i] = addr | 3; 
    addr += PAGE_SIZE;
  }

  //first PDE points to the just created page table and the last PDE
  //points to the page directory itself to set up recursive mapping
  page_directory[0] = (unsigned long)page_table | 3; 
  page_directory[ENTRIES_PER_PAGE-1] = (unsigned long)page_directory | 3;

  //rest of the page directory entries (1-1022) point to absent page tables
  for(i=1; i<=ENTRIES_PER_PAGE-2; i++)
    page_directory[i] = 0 | 2;
}

void PageTable::load()
{
  current_page_table = this;
  write_cr3((unsigned long)page_directory); //write CR3 with address of page directory
}

void PageTable::enable_paging()
{
  write_cr0(read_cr0() | 0x80000000); //set the paging bit in CR0 to 1;
  paging_enabled = 1;
}

void PageTable::handle_fault(REGS* _r)
{
  if(!paging_enabled)
  {
    Console::puts("Caught a page fault but paging not enabled. Aborting.");
    abort();
  }

  unsigned long fault_addr = read_cr2();
  
  //obtain offsets
  int pd_offset = fault_addr >> 22;
  int pt_offset = (fault_addr >> 12) & 0x03FF;

  //page directory must be accessed via virtual memory
  unsigned long* v_page_directory = (unsigned long*)0xFFFFF000; //top 4KB of virtual address space

  //we don't handle the page fault if the address is beyond 32MB, thus not managed by first 8 page tables; in this case the address itself is invalid
  if(pd_offset > 7)
  {
    Console::puts("\nInvalid memory reference higher than 32MB: ");
    Console::putui(fault_addr);
    abort();
  }

  unsigned long* page_table = 0;
  unsigned long* v_page_table = 0;

  //first check if the PDE is valid
  if((v_page_directory[pd_offset] & 0x01) == 0)
  {
    //this refers to a new page table
    //
    //in this case we need to assign a page to store the new page table first, else it will cause cyclic infinite page faults; the frame should be taken from the process memory pool, and then we take a frame from the process memory pool and assign the page containing the faulted address to this new frame
    
    Console::puts("\nOffsets: ");
    Console::puti(pd_offset);
    Console::puts(" ");
    Console::puti(pt_offset);
    Console::puts("\ncaught a pg fault, missing page table");
    
    unsigned long new_frame = process_mem_pool->get_frame_address(process_mem_pool->get_frame());
    v_page_directory[pd_offset] = new_frame | 3;

    new_frame = process_mem_pool->get_frame_address(process_mem_pool->get_frame());
    v_page_table = (unsigned long*)(0xFFC00000 + pd_offset*PAGE_SIZE);
    
    //page_table = (unsigned long*)(v_page_directory[pd_offset] & 0xFFFFF000); //masking the LSB 12 bits to get the boundary address 
    
    for(int i=0; i<ENTRIES_PER_PAGE; i++)
      v_page_table[i] = 0 | 2; //pages not present
    
    v_page_table[pt_offset] = new_frame | 3; //page containing the faulted address to be marked present
  }
  else
  {
    //here the page table page is already known, already initialized previously; we just need to find a new frame from the process pool for the new page
    Console::puts("\ncaught a pg fault, missing page only");

    unsigned long new_frame = process_mem_pool->get_frame_address(process_mem_pool->get_frame());
    v_page_table = (unsigned long*)(0xFFC00000 + pd_offset*PAGE_SIZE);
    v_page_table[pt_offset] = new_frame | 3; //page containing the faulted address to be marked present
  }
}

void PageTable::init_paging(FramePool *_kernel, FramePool *_process, const unsigned long _shared_size)
{
  kernel_mem_pool = _kernel;
  process_mem_pool = _process;
  shared_size = _shared_size;
}

void PageTable::free_page(unsigned long _page_no)
{
  ;
}

void register_vmpool(VMPool *_pool)
{
  ;
}
