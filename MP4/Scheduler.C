#include "Scheduler.H"
#include "thread.H"
#include "utils.H"
#include "console.H"

Scheduler::Scheduler()
{
  ready_q = (Thread**)0x200000; //Ready queue starts at 2MB
  num_threads = 0;
}

void Scheduler::yield()
{
  //pick the next thread from the ready queue
  Thread* next_th = pop_thread();
  //Console::puts("Popped thread: ");
  //Console::puti(next_th->ThreadId());
  //Console::puts("\n");

  //switch to the next thread
  Thread::dispatch_to(next_th);
}

void Scheduler::resume(Thread* _thread)
{
  push_thread(_thread);
  //print_q();
}

void Scheduler::add(Thread* _thread)
{
  push_thread(_thread);
}

void Scheduler::terminate(Thread *_thread)
{
}

void Scheduler::push_thread(Thread* th)
{
  ready_q[num_threads] = th;
  num_threads++;
}

Thread* Scheduler::pop_thread()
{
  Thread* last;
  if(num_threads > 0)
  {
    last = ready_q[0];
    //memset(ready_q[0], 0, sizeof(Thread*));
    for(int i=1; i<num_threads; i++)
    {
      //ready_q[i-1] = (Thread*) memcpy(ready_q[i-1], ready_q[i], sizeof(Thread*));
      ready_q[i-1] = ready_q[i];
    }
    //memset(ready_q[num_threads-1], 0, sizeof(Thread*));
    ready_q[num_threads-1] = 0;
    num_threads--;    
  }
  else
    last = NULL;
  
  return last;
}

void Scheduler::print_q()
{
  Console::puts("ReadyQ ==> ");
  for(int i=0; i<num_threads; i++)
  {
    Console::puti(ready_q[i]->ThreadId());
    Console::puts(" ");
  }
  Console::puts("\n");
}
