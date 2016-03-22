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
void eventAnalyzer();
int isTimeGreater(struct time time1, struct time time2);
long diffTimeMicro (struct time time1, struct time time2);
void resetTime ();
char* concatData(char pid[17], long start, long end, int in, int out);
char* concat(char* s1, char* s2);

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
struct time start;

//What is the ideal threshold?
const int TIME_OUT = 3000000;
const int DELTA_TIME_OUT = 1000000;

//Tracks the number of events so they can be sent when alarm is called.
int entryCount;
int exitCount;
ParseClient client;
char parseObjJSON[200];

/*
 * Function: alarmHandler
 * ----------------------
 * Handles the alarm
 *
 * sig: ???
 */
void alarmHandler(int sig)
{
  FILE *ifp;
  struct time end;
  setTime(&end);
  char pid[17] = "string";

  ifp = fopen("macAdr.txt", "r");
  if (ifp == NULL){
	  fprintf(stderr, "Can't open input file macAdr.txt!\n");
	  exit(1);
  }

  while (fscanf(ifp, "%s", pid) != EOF);

  //put info into JSON formatted string
  if(entryCount > 0 || exitCount > 0){
  sprintf (parseObjJSON,
          "{\"pid\":\"%s\", \"start\":%lu, \"end\":%lu, \"in\":%d, \"out\":%d}",
          pid, start.tv.tv_sec, end.tv.tv_sec, entryCount, exitCount);
  parseSendRequest(client, "POST", "/1/classes/data", parseObjJSON, NULL);
  }
  //printf("%s\n", parseObjJSON);
  entryCount = 0;
  exitCount = 0;
  start = end;
  alarm(60);
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
  setTime(&start);
  alarm(5);

  //Parse
  client = parseInitialize(PARSE_APPID, PARSE_KEY);

  while (1)
  {
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
		return;
	}
	if (digitalRead(BEAM1)==0){
		//if current read is a 1 then it was a fall
		beam1FallOld = beam1FallLatest;
		setTime(&beam1FallLatest);
	} else {
		//if current read is a 0 then it was a rise
			beam1RiseOld = beam1RiseLatest;
			setTime(&beam1RiseLatest);
			//Beam1Rise. Only analyze if the other beam is currently in the risen state.
			if(isTimeGreater(beam1RiseLatest, beam2RiseLatest) &&
			    isTimeGreater(beam2RiseLatest, beam2FallLatest)){
				eventAnalyzer();
			}
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
	} else {
		//if current read is a 0 then it was a rise
		beam2RiseOld = beam2RiseLatest;
		setTime(&beam2RiseLatest);

		//Beam2Rise. Only analyze if the other beam is currently in the risen state.
		if((isTimeGreater(beam2RiseLatest, beam1RiseLatest) && isTimeGreater(beam1RiseLatest, beam1FallLatest))){
			eventAnalyzer();
		}
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
void eventAnalyzer (){

	//A is the current beam being considered. B is the other beam.
	//only continue if time elapsed between 2 beams is less than threshold.
	if((diffTimeMicro(beam1FallLatest, beam1RiseLatest) >= TIME_OUT)||(diffTimeMicro(beam2FallLatest, beam2RiseLatest) >= TIME_OUT)){
		printf("One of the beams was obstructed for too long\n");
		return;
	}

	if(isTimeGreater(beam1FallLatest, beam2FallLatest)){
		//increment Entry/Exit # based on beams
		exitCount+=1;
		printf("EXIT\n");
	} else {
		entryCount +=1;
		printf("ENTRY\n");
	}
	resetTime();
}
	//printf("ALatestFall: %lu\tALatestRise: %lu\tAOldFall: %lu\tAOldRise: %lu\nBLatestFall: %lu\tBLatestFall: %lu\tBLatestRise: %lu\tBOldFall: %lu\tBOldRise:%lu\n",
    //ALatestFall.tv, ALatestRise.tv, AOldFall.tv, AOldRise.tv, BLatestFall.tv, BLatestRise.tv, BOldFall.tv, BOldRise.tv)
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
