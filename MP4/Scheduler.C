#include "Scheduler.H"
#include "thread.H"
#include "utils.H"

Scheduler::Scheduler()
{
  num_threads = 0;
}

virtual void Scheduler::yield()
{
}

virtual void Scheduler::resume(Thread* _thread)
{
  push_thread(_thread);
}

virtual void Scheduler::add(Thread* _thread)
{
  push_thread(_thread);
}

virtual void Scheduler::terminate(Thread *_thread)
{
}

void Scheduler::push_thread(Thread* th)
{
  ready_q[num_threads] = th;
  num_threads++;
}

Thread* pop_thread()
{
  Thread* last;
  if (num_threads>0)
  {
    last = ready_q[0];
    for(int i=1;i<num_threads;i++)
    {
     memset(ready_q[i-1], 0, sizeof(Thread*));
     memcpy(ready_q[i-1], ready_q[i], sizeof(Thread*));
    }
    memset(ready_q[num_threads-1], 0, sizeof(Thread*));
    num_threads;    
  }
  else
  {
    last = NULL;
  }
  return last;
}
