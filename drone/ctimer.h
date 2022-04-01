#ifndef CTIMER_H_
#define CTIMER_H_

#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>

//=============================================================================
class CTimer
{
private:
	int chid; 			//channel id
	int coid; 			//connection back to channel
	char msgbuf[100]; 	//Message msg;
	struct sigevent event; //event to deliver
	struct itimerspec itime; //timer's data struct
	timer_t timer_id; 	//timer's ID for the timer
	uint64_t cps; 		// number of cycles per second
	uint64_t tick_cycles, tock_cycles;
	
public:
	CTimer(int sec, int msec);
	~CTimer();
	int errdisp; 		// indicates that the error messages needed to be displayed or not
	int debugdisp; 		// indicates that the debug messages needed to be displayed or not
	void settimer(long seconds, long nanoseconds);
	void wait();
	void tick();
	double tock();
};

#endif /*CTIMER_H_*/
