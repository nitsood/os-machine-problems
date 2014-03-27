/* 
    Author: Joshua Capehart
			Based on code by:
			R. Bettati
            Department of Computer Science
            Texas A&M University
			
			A thread scheduler.

*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

/*--------------------------------------------------------------------------*/
/* DEFINES 
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* INCLUDES 
 *--------------------------------------------------------------------------*/

#include "machine.H"
#include "thread.H"

/*--------------------------------------------------------------------------*/
/* SCHEDULER
 *--------------------------------------------------------------------------*/

class Scheduler {

  /* number of threads currently in the ready queue */
  unsigned long num_threads;

  /* Thread that is currently in execution, this thread does not exist on the ready queue 
   * while it is running */
  //Thread* running;

  /* ready queue for threads */
  Thread** ready_q;

  /* enque a thread at the end of the ready queue */
  void push_thread(Thread* th);

  /* deque a thread from the front of the ready queue */
  Thread* pop_thread();

  void remove(Thread* th);

public:

  Scheduler();
  /* Setup the scheduler. This sets up the ready queue, for example.
      If the scheduler implements some sort of round-robin scheme, then the 
      end_of_quantum handler is installed here as well. */

  virtual void yield();
  /* Called by the currently running thread in order to give up the CPU. 
      The scheduler selects the next thread from the ready queue to load onto 
      the CPU, and calls the dispatcher function defined in 'threads.h' to
      do the context switch. */

  virtual void resume(Thread * _thread);
  /* Add the given thread to the ready queue of the scheduler. This is called
      for threads that were waiting for an event to happen, or that have 
      to give up the CPU in response to a preemption. */

  virtual void add(Thread * _thread);
  /* Make the given thread runnable by the scheduler. This function is called
  typically after thread creation. Depending on the
      implementation, this may not entail more than simply adding the 
      thread to the ready queue (see scheduler_resume). */

  virtual void terminate(Thread * _thread);
  /* Remove the given thread from the scheduler in preparation for destruction
     of the thread. */

  virtual void print_q();
};

#endif
