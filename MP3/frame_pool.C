#include "frame_pool.H"
#include "console.H"
#include "utils.H"

FramePool::FramePool(unsigned long _base_frame_no, unsigned long _nframes, unsigned long _info_frame_no)
{
  base_frame_no = _base_frame_no;
  nframes = _nframes;
  info_frame_no = _info_frame_no;

  //assign base physical address according to which pool this is
  if(_base_frame_no == 512) //kernel mem pool
  {
    //kernel mem pool
    base_address = 0x200000; //2MB
    frame_bitmap = (unsigned char*)base_address;
    
    //reserve the first frame for the bitmap itself; and 
    //set the rest to be 'usable'
    frame_bitmap[0] = (unsigned char)0x01;
    for(int i=1; i<nframes/8; i++)
      frame_bitmap[i] = 0; 
  }
  else if(_base_frame_no == 1024) //process mem pool
  {
    base_address = 0x400000; //4MB

    //to calculate the address of the process pool bitmap, 
    //we are given the info_frame_no within the directly mapped region
    unsigned long x = 0x200000;
    frame_bitmap = (unsigned char*)(x+((_info_frame_no-512)<<12));
    
    //the bitmap frame was already marked used when the info frame was requested in the kernel; all the frames in process pool as of now are usable 
    for(int i=0; i<nframes/8; i++)
      frame_bitmap[i] = 0; 
  }    
}

unsigned long FramePool::get_frame()
{
  int byte_ctr = 0;
  char found = 'f';
  for(; byte_ctr<nframes; byte_ctr++)
  {
    //find the first byte that has a bit set to 0
    if(frame_bitmap[byte_ctr] != 0xFF)
    {
      found = 't';
      break; //found
    }
  }
 
  if(found == 'f') return 0;

  //now we find the first bit that is zero in the given byte
  //assuming for now such a byte is always found
  unsigned char bits8 = frame_bitmap[byte_ctr];
  int zero_bit = 0;
  while(bits8 & 0x01)
  {
    zero_bit++;
    bits8 = bits8 >> 1;
  }

  //set the frame to 'used' before returning its number
  frame_bitmap[byte_ctr] |= (0x01 << zero_bit);
  //Console::puts("\n--after reserving frame ");
  //Console::print_bin(frame_bitmap[byte_ctr]);
  return base_frame_no+(8*byte_ctr)+zero_bit;
}

unsigned long FramePool::get_frame_address(unsigned long _frame_no)
{
  return base_address+((_frame_no-base_frame_no)<<12);
}

void FramePool::mark_inaccessible(unsigned long _start_frame, unsigned long _nframes)
{
  int byte_ctr = (_start_frame-base_frame_no)/8;
  int last_byte = byte_ctr+(_nframes/8);
  for(; byte_ctr<last_byte; byte_ctr++)
    frame_bitmap[byte_ctr] = 0xFF; //setting 8 bits at once
}

void FramePool::release_frame(unsigned long _frame_no)
{
  //from _frame_no obtain the byte and the bit number to set
  //Console::puts("\nReleasing frame number: ");
  //Console::puti(_frame_no);
  int byte_ctr = (_frame_no-base_frame_no)/8;
  int bit_ctr = _frame_no%8;
  frame_bitmap[byte_ctr] ^= (0x01<<bit_ctr);
  //Console::puts("\n--after releasing frame ");
  //Console::print_bin(frame_bitmap[byte_ctr]);
}

unsigned char* FramePool::get_bitmap_address()
{
  return frame_bitmap;
}
