/**
 * Author: Dinesh A. Joshi <dinesh.joshi@yahoo.com>
 * Date  : 31st March 2013
 *
 * A simple toy threading library example
 * --------------------------------------
 * Data structures:
 *   1. tlist     -> list of all threads
 *   2. curthread -> currently running thread 
 *   3. next_free_thread
 *   4. n_running_threads
 *   5. main_ctx  -> context of the main thread
 *                   same as tlist[0]
 *
 * Key functions:
 *   1. thread_create()    - creates a thread
 *   2. thread_execute()   - scaffold's the thread startup routine
 *   3. thread_init()      - initializes the thread DS
 *   4. scheduler()        - schedules the next thread
 *   5. thread_join_wait() - waits for all threads to exit
 *
 *  Working :
 *   - thread_create()  creates a context using makecontext
 *                      sets up thread_execute as the start
 *                      routine. 
 *
 *   - thread_execute() basically wraps the thread's execution
 *                      function. It performs cleanup once
 *                      the thread's execution finishes
 *
 *   - scheduler()      this function swaps between threads.
 *                      The key is tlist[0] is set to the context
 *                      of the main thread. The reason is, 
 *                      otherwise, swapcontext() clobber's the
 *                      first thread and it never enters it's 
 *                      startup routine
 *
 *   - Scheduling is always done via raising the signal at a
 *     pre-defined interval (TIMER_WAIT)
 *
 *  Output:
 *  The output should look similar to:
 *  ------------------------------------------------
 *  Init thread id[0]
 *  Init thread id[1]
 *  Init thread id[2]
 *  Saving main_ctx
 *  M.f1()
 *  11.f2()
 *  22.M.11.22.M.11.22.M.1.2.M.11.22.M.11.22.M.1.22.M.11.2.M...
 *  All done with threads
 *  ------------------------------------------------
 *  The interesting part of the output is what you see after
 *  'Saving main_ctx'. The numbers, characters and symbols can be
 *  interpreted as:
 *
 *  1 = Thread 1 is active
 *  2 = Thread 2 is active
 *  M = Main thread is active
 *  . = context switch
 *  
 * If you don't see them interleaved then you need to adjust the 
 * make_busy() function. Your CPU might be faster so it's executing
 * the busy loop before the signal can be delivered to the process
 * to make a context switch.
 */

#include <stdio.h>
#include <ucontext.h>
#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#define TIMER_WAIT       300000    /* time slice per thread          */
#define MAX_THREADS      10        /* max threads in the system      */
#define THREAD_FN_ITERS  50        /* number of pretend work items   */

typedef enum __thread_status {
    CREATED,
    EXITED
} _thread_status;

typedef struct _thread_t {
    ucontext_t*     ctx;
    _thread_status  status;
} dthread_t;

void scheduler(int sig);

ucontext_t        main_ctx;
sigset_t          block;
struct itimerval  timer_set;
dthread_t         tlist[MAX_THREADS];
int               next_free_thread  = 0;
int               curthread         = 0;
int               n_running_threads = 0;

void block_sig(){
	sigprocmask(SIG_BLOCK, &block, NULL);
}

void unblock_sig(){
	sigprocmask(SIG_UNBLOCK, &block, NULL);	
}


void thread_execute(void *(*start_routine)(void *),void *arg){
	void *retval = start_routine(arg);
    tlist[curthread].status = EXITED;
    n_running_threads--;
    raise(SIGPROF);
}

void _thread_init(int offset, ucontext_t *ctx) {
    tlist[offset].ctx    = ctx;
    tlist[offset].status = CREATED;
    printf("Init thread id[%d]\n", offset);
    fflush(stdout);
}

void thread_create(ucontext_t *tctx, void (*start_routine)(), void *arg) {
	getcontext(tctx);

	tctx->uc_link           = 0;
	tctx->uc_stack.ss_sp    = malloc(16*1024);
	tctx->uc_stack.ss_size  = 16*1024;
	tctx->uc_stack.ss_flags = 0;

	makecontext(tctx, thread_execute, 2, start_routine, 0);
    _thread_init(next_free_thread, tctx);

    next_free_thread++;
    n_running_threads++;
}

void make_busy() {
    for(long i=0; i < 5000; i++) {
        for (long j=0; j < 10000; j++) {
            long k=i*j;
        }
    }
}

void tf1() {
    printf("f1()\n");
    for(int i=0; i < THREAD_FN_ITERS; i++) {
        printf("1");
        fflush(stdout);
    
        /* printf("[t1][%.3d]\n", i); */
        make_busy();
    }
}

void tf2() {
    printf("f2()\n");
    for(int i=0; i < THREAD_FN_ITERS; i++) {
        printf("2");
        fflush(stdout);
        /* printf("[t2][%.3d]\n", i); */
        make_busy();
    }
}


void timer_init(long us_interval){
	sigemptyset(&block);
	sigaddset(&block, SIGPROF);

	timer_set.it_value.tv_sec      = -0;
	timer_set.it_value.tv_usec     = us_interval;
	timer_set.it_interval.tv_sec   = 0;
	timer_set.it_interval.tv_usec  = us_interval;

	signal(SIGPROF, scheduler);
	setitimer(ITIMER_PROF, &timer_set, NULL);
}

/**
 * REALLY HORRIBLE WAY OF DOING "ROUND ROBIN"
 * DONT DO THIS...
 */
int find_next_runnable_thread() {
    for (int i=curthread+1; i < next_free_thread; i++) {
        if (tlist[i].status != EXITED) { 
            if (n_running_threads > 1) {
                if (i != curthread) {
                    return i;
                }
            } else {
                return i;
            }
        }
    }

    for (int i=0; i < next_free_thread; i++) {
        if (tlist[i].status != EXITED) { 
            if (n_running_threads > 1) {
                if (i != curthread) {
                    return i;
                }
            } else {
                return i;
            }
        }
    }
    return -1;
}

void scheduler(int sig){
    static int num_switches=0;

	block_sig();
    timer_init(TIMER_WAIT);
    unblock_sig();

    printf(".");
    fflush(stdout);
    
    num_switches++;

    int next_thread=-1;
	if ((next_thread = find_next_runnable_thread()) != -1) {
        int old_thread = curthread;
        curthread      = next_thread;
        /* printf("SWP[%d,%d]\n", old_thread, next_thread); */
 		swapcontext(tlist[old_thread].ctx, tlist[next_thread].ctx);
	}
	else {
        setcontext(&main_ctx);
	}

    
}

void thread_join_wait() {
    printf("Saving main_ctx\n");
    getcontext(&main_ctx);
    
    while (n_running_threads > 1) {
        /* printf("[MAIN][num_threads=%d]\n", n_running_threads); */
        printf("M");
        fflush(stdout);
        make_busy();
        raise(SIGPROF);
    }
}



int main(void) {
    ucontext_t t1_ctx;
    ucontext_t t2_ctx;
    timer_init(TIMER_WAIT);

    _thread_init(0, &main_ctx);
    next_free_thread++;
    n_running_threads++;
    
    thread_create(&t1_ctx, tf1, NULL);
    thread_create(&t2_ctx, tf2, NULL);

    thread_join_wait();
    printf("\nAll done with threads\n");
    return 0;
}


