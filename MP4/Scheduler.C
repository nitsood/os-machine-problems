#include "Scheduler.H"

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

Thread* pop_thread();
{
}
