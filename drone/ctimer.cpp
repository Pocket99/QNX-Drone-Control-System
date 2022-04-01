// standard needed header files.
#include "ctimer.h"

// macro definition for error output
#define errcout if(errdisp) cout

// macro definition for debug output
#define debugcout if(debugdisp) cout

using namespace std;

CTimer::CTimer(int sec, int msec)
{
	int nanosec;
	
	errdisp = 1; // to display the errors set it to 1; otherwise set it to 0
	debugdisp = 0; // to display the debug prompts set it to 1; otherwise set it to 0
	nanosec = 1000000*msec;
	
	// create a channel
	chid = ChannelCreate(0);
	coid = ConnectAttach(0 ,0, chid, 0, 0); 	//Attach timer to our channel
	if (coid == -1)
	{
		errcout << "Timer: ConnectAttach ERROR :(" << endl;
	//exit(EXIT_FAILURE);
	}
	//initialization macro of the sigevent:"event" structure
	
	//SIGEV_PULSE event;
	SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, 1, 0);
	debugcout << "TIMER pulse initiated" << endl;
	//create timer
	//if (timer_create(CLOCK_REALTIME, &event, &timer_id) == -1)
	if (timer_create(CLOCK_REALTIME, &event, &timer_id) == -1)
		debugcout << " timer_create ERROR"<< endl;
		
	// setting the timer
	settimer(sec, nanosec); 
	
	// find out how many cycles per second
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
}

CTimer::~CTimer()
{
	ChannelDestroy(chid);
	debugcout << "TIMER Destructor called" << endl;
}

/* Sets the timer with appropriate values. */
void CTimer::settimer(long seconds, long nanoseconds)
{
	//Will receive pulse initially (the itime.it_value) and every
	//(the itime.it_interval)seconds thereafter
	itime.it_value.tv_sec = seconds;
	itime.it_value.tv_nsec = nanoseconds;
	itime.it_interval.tv_sec = seconds;
	itime.it_interval.tv_nsec = nanoseconds;
	timer_settime(timer_id, 0, &itime, NULL); //0 for relative time
}

void CTimer::wait()
{ 
	// Wait for timer tick
	int rcvid;
	rcvid = MsgReceive(chid, &msgbuf, sizeof(msgbuf), NULL);
}

void CTimer::tick()
{
	// snap the time
	tick_cycles=ClockCycles( );
}

double CTimer::tock()
{
	// snap the time
	tock_cycles=ClockCycles( );
	return (double)((int)(((double)(tock_cycles-tick_cycles)/cps)*100000))/100;
}
