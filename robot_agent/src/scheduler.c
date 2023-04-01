/**
 * @file	scheduler.c
 * @author  Eriks Zaharans and Massimiiliano Raciti
 * @date    1 Jul 2013
 *
 * @section DESCRIPTION
 *
 * Cyclic executive scheduler library.
 */

/* -- Includes -- */
/* system libraries */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
/* project libraries */
#include "scheduler.h"
#include "task.h"
#include "timelib.h"

/* -- Defines -- */

/* -- Functions -- */

/**
 * Initialize cyclic executive scheduler
 * @param minor Minor cycle in miliseconds (ms)
 * @return Pointer to scheduler structure
 */
scheduler_t *scheduler_init(void)
{
	// Allocate memory for Scheduler structure
	scheduler_t *ces = (scheduler_t *) malloc(sizeof(scheduler_t));

	return ces;
}

/**
 * Deinitialize cyclic executive scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_destroy(scheduler_t *ces)
{
	// Free memory
	free(ces);
}

/**
 * Start scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_start(scheduler_t *ces)
{
	// Set timers
	timelib_timer_set(&ces->tv_started);
	timelib_timer_set(&ces->tv_cycle);
}

/**
 * Wait (sleep) till end of minor cycle
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_wait_for_timer(scheduler_t *ces)
{
	int sleep_time; // Sleep time in microseconds

	// Calculate time till end of the minor cycle
	sleep_time = (ces->minor * 1000) - (int)(timelib_timer_get(ces->tv_cycle) * 1000);

	// Add minor cycle period to timer
	timelib_timer_add_ms(&ces->tv_cycle, ces->minor);

	// Check for overrun and execute sleep only if there is no
	if(sleep_time > 0)
	{
		// Go to sleep (multipy with 1000 to get miliseconds)
		usleep(sleep_time);
	}
}

/**
 * Execute task
 * @param ces Pointer to scheduler structure
 * @param task_id Task ID
 * @return Void
 */
void scheduler_exec_task(scheduler_t *ces, int task_id)
{
	switch(task_id)
	{
	// Mission
	case s_TASK_MISSION_ID :
		task_mission();
		break;
	// Navigate
	case s_TASK_NAVIGATE_ID :
		task_navigate();
		break;
	// Control
	case s_TASK_CONTROL_ID :
		task_control();
		break;
	// Refine
	case s_TASK_REFINE_ID :
		task_refine();
		break;
	// Report
	case s_TASK_REPORT_ID :
		task_report();
		break;
	// Communicate
	case s_TASK_COMMUNICATE_ID :
		task_communicate();
		break;
	// Collision detection
	case s_TASK_AVOID_ID :
		task_avoid();
		break;
	// Other
	default :
		// Do nothing
		break;
	}
}

/**
 * Run scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_run(scheduler_t *ces)
{
	/* --- Local variables (define variables here) --- */
	struct timeval starttime; //Timer to measure execution time
	
	double current_time=timelib_unix_timestamp()/1000; // current time in sec
	double next_time = ceil(current_time)-current_time;
	int overRun = 0;
	  /* --- Set minor cycle period --- */
	ces->minor = 100;
	int major = 3000;

	/* --- Write your code here --- */
	//double wait_time = next_time + 0.75;
	double wait_time = 0.75 + next_time;//0.750 - 0.539 = 0.211; 0.750 - 0.925 = -0.175
	/*if(wait_time<0){
		wait_time = (1-next_time) + 0.75;//0.075 + 0.75 = 0.825
	}*/
	usleep(wait_time*1e6);//0.211*1000000 = 211*1000; 0.825*1000000 = 825*1000

	scheduler_start (ces); 

while(1){
	int commCount = 0;
 for(int i=1; i<=30;i++){
	
	timelib_timer_set(&starttime);
	/*
	scheduler_exec_task(ces, s_TASK_MISSION_ID);
	double end_time_m = timelib_timer_get(starttime);
	printf("Mission=%f",end_time_m);


	timelib_timer_set(&starttime);
	scheduler_exec_task(ces, s_TASK_NAVIGATE_ID);
	double end_time_n = timelib_timer_get(starttime);
	printf("Navigate=%f",end_time_n);

	timelib_timer_set(&starttime);
	scheduler_exec_task(ces, s_TASK_AVOID_ID);
	double end_time_a = timelib_timer_get(starttime);
	printf("Avoid=%f",end_time_a);


	timelib_timer_set(&starttime);
	scheduler_exec_task(ces, s_TASK_REFINE_ID);
	double end_time_r = timelib_timer_get(starttime);
	printf("Refine=%f",end_time_r);


	timelib_timer_set(&starttime);
	scheduler_exec_task(ces, s_TASK_REPORT_ID);
	double end_time_re = timelib_timer_get(starttime);
	printf("Report=%f",end_time_re);

	timelib_timer_set(&starttime);
	scheduler_exec_task(ces, s_TASK_CONTROL_ID);
	double end_time_co = timelib_timer_get(starttime);
	printf("Control=%f",end_time_co);

	timelib_timer_set(&starttime);
	scheduler_exec_task(ces, s_TASK_COMMUNICATE_ID);
	double end_time_c = timelib_timer_get(starttime);
	printf("Communicate=%f",end_time_c);

	*/
	
	if(i%10 ==1){
		/*if(commCount == 0){
			sleep(1.81);	
			scheduler_exec_task(ces, s_TASK_COMMUNICATE_ID);
			
		}else{
			scheduler_exec_task(ces, s_TASK_COMMUNICATE_ID);
		}
		commCount++;*/

		scheduler_exec_task(ces, s_TASK_COMMUNICATE_ID);
	}
	scheduler_exec_task(ces, s_TASK_REFINE_ID);
	scheduler_exec_task(ces, s_TASK_REPORT_ID);
	scheduler_exec_task(ces, s_TASK_MISSION_ID);
	 
	if(i%5 == 0){
		
		scheduler_exec_task(ces, s_TASK_CONTROL_ID);
		
	}
	if(i%3 == 0 || i%5 == 0){

		scheduler_exec_task(ces, s_TASK_AVOID_ID);
	}
	
	scheduler_exec_task(ces, s_TASK_NAVIGATE_ID); 

	double end_time_i = timelib_timer_get(starttime);
	printf("i=%d",i);
	printf("End_time i=%f",end_time_i);
	printf("\n");
	
	scheduler_wait_for_timer (ces);
 	if(end_time_i > ces->minor){
		overRun++;
	}
		printf("No:of overruns=%d",overRun);
		printf("\n");
		printf("-----------");
		printf("\n");
 }
	

}
}
