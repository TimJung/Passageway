#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

int firstRise1 = 1;
int firstRise2 = 1;

//function prototypes
void pinHandler1 (void);
void pinHandler2 (void);
struct time {struct timeval tv; struct timezone tz;};
void setTime (struct time *);
void eventAnalyzer(int risenBeam);

//pin numbers. 1 is entry. 2 is exit.
const int BEAM1 = 1;
const int BEAM2 = 4;

//time comparison variables
struct time beam1FallOld;
struct time beam1RiseOld;
struct time beam1FallLatest;
struct time beam1RiseLatest;
struct time beam2FallOld;
struct time beam2RiseOld;
struct time beam2RiseLatest;
struct time beam2FallLatest;
struct time * temp;

//What is the ideal threshold?
const int TIME_OUT = 1000;
const int DELTA_TIME_OUT = 1000;

//Tracks the number of events so they can be sent when alarm is called.
int entryCount;
int exitCount;

int main (void)
{
  //set up for wiringPi
  wiringPiSetup();


  pinMode(BEAM1,INPUT);
  pullUpDnControl(BEAM1,PUD_UP);
  pinMode(BEAM2,INPUT);
  pullUpDnControl(BEAM2, PUD_UP);

  wiringPiISR (BEAM1, INT_EDGE_BOTH, pinHandler1);
  wiringPiISR (BEAM2, INT_EDGE_BOTH, pinHandler2);

  while (1)
  {
	wait(NULL);
  }
}

struct timeval tv;
struct timezone tz;

void pinHandler1 (void){
	if (firstRise1){
		firstRise1 = 0;
		return;
	}
	if (digitalRead(BEAM1)==1){
		//if current read is a 1 then it was a fall
		beam1FallOld = beam1FallLatest;
		setTime(&beam1FallLatest);

		//output time to test
		//printf("Beam 1 fall at: ");
		//printf ("%ld %ld\n", beam1FallLatest.tv.tv_sec, beam1FallLatest.tv.tv_usec);		
	} else {
		//if current read is a 0 then it was a rise
			beam1RiseOld = beam1RiseLatest;
			setTime(&beam1RiseLatest);

			//Beam1Rise. Only analyze if the other beam is currently in the risen state.
			if(beam1RiseLatest.tv.tv_sec > beam2RiseLatest.tv.tv_sec && beam2RiseLatest.tv.tv_sec > beam2FallLatest.tv.tv_sec){
				eventAnalyzer(BEAM1);
				printf("Current number of entries: %d\n", entryCount);
				printf("Current number of exits: %d\n\n", exitCount);
			}

			//output time to test
			// printf("Beam 1 rise at: ");
			// printf ("%ld %ld\n", beam1RiseLatest.tv.tv_sec, beam1RiseLatest.tv.tv_usec);		
	}
}

void pinHandler2 (void){
	
	if (firstRise2){
		firstRise2 = 0;
		return;
	}
	
	if (digitalRead(BEAM2)==1){
		//if current read is a 1 then it was a fall
		beam2FallOld = beam2FallLatest;
		setTime(&beam2FallLatest);

		//output time to test
		//printf("Beam 2 fall at: ");
		//printf ("%ld %ld\n", beam2FallLatest.tv.tv_sec, beam2FallLatest.tv.tv_usec);		
	} else {
		//if current read is a 0 then it was a rise
		beam2RiseOld = beam2RiseLatest;
		setTime(&beam2RiseLatest);
		
		printf("Greater Than: %ld %ld", beam2RiseLatest.tv.tv_usec, beam1RiseLatest.tv.tv_usec);
		printf("    AND Greater Than: %ld %ld\n", beam1RiseLatest.tv.tv_usec, beam1FallLatest.tv.tv_usec);
		//Beam2Rise. Only analyze if the other beam is currently in the risen state.
		if(beam2RiseLatest.tv.tv_usec > beam1RiseLatest.tv.tv_usec && beam1RiseLatest.tv.tv_usec > beam1FallLatest.tv.tv_usec){
			eventAnalyzer(BEAM2);
			printf("Current number of entries: %d\n", entryCount);
			printf("Current number of exits: %d\n\n", exitCount);
		}

		//output time to test
		// printf("Beam 2 rise at: ");
		// printf ("%ld %ld\n", beam2RiseLatest.tv.tv_sec, beam2RiseLatest.tv.tv_usec);		
	}
}

void setTime(struct time * ptr){
	struct time t1 = *ptr;
	struct timeval tv1 = t1.tv;
	struct timezone tz1 = t1.tz;
	gettimeofday (&tv1, &tz1);
//	gettimeofday (ptr.tv, ptr.tz);
//	printf ("%ld %ld\n", tv.tv_sec, tv.tv_usec);
//	printf ("%ld %ld\n", tv1.tv_sec, tv1.tv_usec);
	t1.tv = tv1;
	t1.tz = tz1;
	*ptr = t1;
}

void eventAnalyzer (int risenBeam){
  
  long ALatestFall;
  long ALatestRise;
  long AOldFall;
  long AOldRise;
  long BLatestFall;
  long BLatestRise;
  long BOldFall;
  long BOldRise;
  char * type;
  
	if (risenBeam == BEAM1){
		ALatestFall = beam1FallLatest.tv.tv_sec;
		ALatestRise	= beam1RiseLatest.tv.tv_sec;
		AOldFall = beam1FallOld.tv.tv_sec;
		AOldRise = beam1RiseOld.tv.tv_sec;
		
		BLatestFall = beam2FallLatest.tv.tv_sec;
		BLatestRise = beam2RiseLatest.tv.tv_sec;
		BOldFall = beam2FallOld.tv.tv_sec;
		BOldRise = beam2RiseOld.tv.tv_sec;
		
		type = "ENTRY";
	} else {
		//set the variables oppositely
		ALatestFall = beam2FallLatest.tv.tv_sec;
		ALatestRise	= beam2RiseLatest.tv.tv_sec;
		AOldFall = beam2FallOld.tv.tv_sec;
		AOldRise = beam2RiseOld.tv.tv_sec;
		
		BLatestFall = beam1FallLatest.tv.tv_sec;
		BLatestRise = beam1RiseLatest.tv.tv_sec;
		BOldFall = beam1FallOld.tv.tv_sec;
		BOldRise = beam1RiseOld.tv.tv_sec;

		type = "EXIT";
	}
	
	
	//A is the current beam being considered. B is the other beam.
	//case 0
		//only continue if time elapsed between 2 beams is less than threshold.
		if((ALatestFall - BLatestFall >= TIME_OUT) && (ALatestFall - BLatestFall <= TIME_OUT*-1)){
			return;
		}
	//case 1
		// if (BOldRise > ALatestFall){
		// 	if(((BOldFall - ALatestFall) - (BLatestRise - ALatestRise) < DELTA_TIME_OUT) && 
		// 		((BOldFall - ALatestFall) - (BLatestRise - ALatestRise) > DELTA_TIME_OUT*-1)){
		// 		//ENTRY and EXIT
		// 		entryNumber +=1;
		// 		exitNumber +=1;
		// 		return;
		// 	}			
		// }
	//case 2
		if (AOldRise > BLatestFall){
			if((AOldFall - BLatestFall) - (ALatestRise - BLatestRise) < DELTA_TIME_OUT &&
			 ((AOldFall - BLatestFall) - (ALatestRise - BLatestRise) > DELTA_TIME_OUT*-1)){
				//ENTRY and EXIT
				entryCount +=1;
				exitCount +=1;
				return;
			}			
		}
	//case 3
		if(ALatestFall < BLatestFall){
			//increment Entry/Exit # based on type
			if (strcmp(type, "ENTRY")){
				entryCount +=1;
			} else {
				exitCount +=1;
			}
			return;
		}
		printf("No event occurred. Why are we here???");
}

//Pi Machine Name/ID
// char hostName[100];
//gethostname(&hostName, 100);

//System Time
//time_t rawTime; 

//initialize buffer for system time
//char buffer[26];
//struct tm* tm_info;
//time(&rawTime);
//tm_info = localtime(&rawTime);
//strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);


