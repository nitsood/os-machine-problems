#include "Scheduler.H"

Scheduler::Scheduler()
{
}

virtual void Scheduler::yield()
{
}

virtual void Scheduler::resume(Thread *_thread)
{
}


virtual void Scheduler::add(Thread *_thread)
{
}

virtual void Scheduler::terminate(Thread *_thread)
{
}
