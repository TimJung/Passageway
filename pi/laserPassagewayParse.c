#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <parse.h>
#include "parse_secret.h"

int firstRise1 = 1;
int firstRise2 = 1;

//function prototypes
void pinHandler1 (void);
void pinHandler2 (void);
struct time {struct timeval tv; struct timezone tz;};
void setTime (struct time *);
void eventAnalyzer(int risenBeam);
int isTimeGreater(struct time time1, struct time time2);
long diffTimeMicro (struct time time1, struct time time2);
void resetTime ();


//pin numbers. B1 is entry. B2 is exit.
const int BEAM1 = 1;
const int BEAM2 = 4;

//time structs to be stored in the time
struct timeval tv;
struct timezone tz;

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
const int TIME_OUT = 1000000;
const int DELTA_TIME_OUT = 1000000;

//Tracks the number of events so they can be sent when alarm is called.
int entryCount;
int exitCount;


void alarmHandler(int sig)
{
  //code to send to Parse goes here

  printf("ALARM\n");
  entryCount = 0;
  exitCount = 0;
  alarm(5);
}


/*
 * Main method
 */
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

  //set up alarm
  signal(SIGALRM, alarmHandler);
  alarm(5);

  //Parse
  ParseClient client = parseInitialize(PARSE_APPID, PARSE_KEY);

  parseSendRequest(client, "POST", "/1/classes/TestObject", "{\"foo\":\"bar\"}", NULL);

  while (1)
  {
	//wait(NULL);
	//int test = digitalRead(BEAM1);
	//printf("%d\n", test);
	//delay(100);
  }
}


/*
 * Function: pinHandler1
 * ----------------------
 * Handles the interupt when beam 1's state changes
 */
void pinHandler1 (void){
	if (firstRise1){
		firstRise1 = 0;
		//printf("first rise\n");
		return;
	}
	if (digitalRead(BEAM1)==0){
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
			//printf("%d %d\n", isTimeGreater(beam1RiseLatest, beam2RiseLatest), isTimeGreater(beam2RiseLatest, beam2FallLatest));
			//Beam1Rise. Only analyze if the other beam is currently in the risen state.
			if(isTimeGreater(beam1RiseLatest, beam2RiseLatest) && 
			    isTimeGreater(beam2RiseLatest, beam2FallLatest)){
				eventAnalyzer(BEAM1);
				//printf("Current number of entries: %d\t", entryCount);
				//printf("Current number of exits: %d\n", exitCount);
			}

			//output time to test
			 //printf("Beam 2: ");
			 //printf ("%ld %ld\n", beam2RiseLatest.tv.tv_sec, beam2FallLatest.tv.tv_sec);		
	}
}


/*
 * Function: pinHandler2
 * ----------------------
 * Handles the interupt when beam 1's state changes
 */
void pinHandler2 (void){

	if (firstRise2){
		firstRise2 = 0;
		return;
	}

	if (digitalRead(BEAM2)==0){
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

		//printf("Greater Than: %ld %ld", beam2RiseLatest.tv.tv_usec, beam1RiseLatest.tv.tv_usec);
		//printf("    AND Greater Than: %ld %ld\n", beam1RiseLatest.tv.tv_usec, beam1FallLatest.tv.tv_usec);
		//Beam2Rise. Only analyze if the other beam is currently in the risen state.
		if((isTimeGreater(beam2RiseLatest, beam1RiseLatest) && isTimeGreater(beam1RiseLatest, beam1FallLatest))){
			eventAnalyzer(BEAM2);
			//printf("Current number of entries: %d\t", entryCount);
			//printf("Current number of exits: %d\n", exitCount);
		}

		//output time to test
		//printf("Beam 2 rise at: ");
		//printf ("%ld %ld\n", beam2RiseLatest.tv.tv_sec, beam2RiseLatest.tv.tv_usec);		
	}
}


/*
 * Function: setTime
 * ----------------------
 * Sets the global variables to the current time
 * 
 * ptr: the time struct that is set to the current time
 */
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


/*
 * Function: eventAnalyzer
 * ----------------------
 * Logic that makes the decision on what to do based on rise and fall of each beam.
 * 
 * risenBeam: The beam that has been detected as risen
 */
void eventAnalyzer (int risenBeam){
  
	struct time ALatestFall;
	struct time ALatestRise;
	struct time AOldFall;
	struct time AOldRise;
	struct time BLatestFall;
	struct time BLatestRise;
	struct time BOldFall;
	struct time BOldRise;
	char type[6];

	if (risenBeam == BEAM1){
		ALatestFall = beam1FallLatest;
		ALatestRise = beam1RiseLatest;
		AOldFall = beam1FallOld;
		AOldRise = beam1RiseOld;

		BLatestFall = beam2FallLatest;
		BLatestRise = beam2RiseLatest;
		BOldFall = beam2FallOld;
		BOldRise = beam2RiseOld;

		strcpy(type, "ENTRY");
	} else {
		//set the variables oppositely
		ALatestFall = beam2FallLatest;
		ALatestRise	= beam2RiseLatest;
		AOldFall = beam2FallOld;
		AOldRise = beam2RiseOld;

		BLatestFall = beam1FallLatest;
		BLatestRise = beam1RiseLatest;
		BOldFall = beam1FallOld;
		BOldRise = beam1RiseOld;

		strcpy(type, "EXIT");
	}

	//A is the current beam being considered. B is the other beam.
	//case 0
	//only continue if time elapsed between 2 beams is less than threshold.
	if((diffTimeMicro(ALatestFall, BLatestFall) >= TIME_OUT)){
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
	if (isTimeGreater(AOldRise, BLatestFall)){
		if(diffTimeMicro(AOldFall, BLatestFall) -
		   diffTimeMicro(ALatestRise, BLatestRise) < DELTA_TIME_OUT){
			//ENTRY and EXIT
			entryCount +=1;
			exitCount +=1;
			resetTime();
			return;
		}
	}
	//case 3
	if(isTimeGreater(ALatestFall, BLatestFall)){
		//increment Entry/Exit # based on type
		if (strcmp(type, "ENTRY")){
			entryCount +=1;
		} else {
			exitCount +=1;
		}
		resetTime();
		return;
	}
	printf("No event occurred. Why are we here???");
}


/*
 * Function: isTimeGreater
 * ----------------------
 * Sets the global variables to the current time
 *
 * time1:
 * time2:
 *
 * returns: a 1 if time1 is greater, or a 0 if time2 is greater
 */
int isTimeGreater (struct time time1, struct time time2){
	if (time1.tv.tv_sec == time2.tv.tv_sec){
		if(time1.tv.tv_usec > time2.tv.tv_usec){
			return 1;
		}
	//if the seconds are equal then compare microseconds.
	} else if (time1.tv.tv_sec > time2.tv.tv_sec){
		return 1;
	}
	return 0;
}


/*
 * Function: diffTimeMicro
 * ----------------------
 * utility function to intelligently get the difference in
 * microseconds between two time structs
 *
 * time1: first time struct
 * time2: second time struct
 *
 * returns: the difference in microseconds between the two times
 */
long diffTimeMicro (struct time time1, struct time time2){
	//if seconds are equal just compare microseconds
	if (time1.tv.tv_sec == time2.tv.tv_sec){
	  return abs(time1.tv.tv_usec - time2.tv.tv_usec);

	//if time1 has greater # of seconds then add the multiple to time2 and subtract from time1
	} else if (time1.tv.tv_sec < time2.tv.tv_sec) {
	  int count = time2.tv.tv_sec - time1.tv.tv_sec;
	  return (count*1000000 + time2.tv.tv_usec) - time1.tv.tv_usec;

	//if time2 has greater # of seconds then add the multiple to time1 and subtract from time2
	} else {
	  int count = time1.tv.tv_sec - time2.tv.tv_sec;
	  return (count*1000000 + time1.tv.tv_usec) - time2.tv.tv_usec;
	}
}


/*
 * Function: resetTime
 * ----------------------
 * Prepares global variables for future time comparison
 */
void resetTime(){
  beam1FallOld = beam1FallLatest;
  beam1RiseOld = beam1RiseLatest;
  beam2FallOld = beam2FallLatest;
  beam2RiseOld = beam2RiseLatest;

  struct time reset;
  beam1FallLatest = reset;
  beam1RiseLatest = reset;
  beam2FallLatest = reset;
  beam2RiseLatest = reset;
}

