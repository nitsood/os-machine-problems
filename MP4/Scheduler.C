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
  Console::puts("yielding\n");
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
  Console::puts("Sch terminate called for thread ");
  Console::puti(_thread->ThreadId());
  Console::puts("\n");

  print_q();
  remove(_thread);
}


/* Adds a thread to the end of the ready queue */
void Scheduler::push_thread(Thread* _th)
{
  ready_q[num_threads] = _th;
  num_threads++;
}


/* Removes a thread from the front of the ready queue */
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


/* Removes a thread from the ready queue if it still exists in the queue.
 * It can happen that the thread was killed explicitly while it was waiting 
 * to get CPU. This is when this method will be used. If the thread dies naturally
 * it won't exist in the ready queue anyway. */
void Scheduler::remove(Thread* _th)
{
  //print_q();
  int i=0;
  for(; i<num_threads; i++)
  {
    if(ready_q[i]->ThreadId() == _th->ThreadId())
      break;
  }
 
  //thread not found in queue, nothing else to be done
  if(i == num_threads)
    return;

  ready_q[i] = 0;
  Console::puts("Thread "); Console::puti(i); Console::puts(" removed from ready q\n");
  for(i+=1; i<num_threads; i++)
    ready_q[i-1] = ready_q[i];

  ready_q[num_threads-1] = 0;
  num_threads--;
  print_q();
}


/* Debug method */
void Scheduler::print_q()
{
  Console::puts("ReadyQ ["); Console::puti(num_threads); Console::puts("] ==> ");
  for(int i=0; i<num_threads; i++)
  {
    Console::puti(ready_q[i]->ThreadId());
    Console::puts(" ");
  }
  Console::puts("\n");
}
