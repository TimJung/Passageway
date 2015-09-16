#include <wiringPi.h>
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
		printf("Beam 1 fall at: ");
		printf ("%ld %ld\n", beam1FallLatest.tv.tv_sec, beam1FallLatest.tv.tv_usec);		
	} else {
		//if current read is a 0 then it was a rise
			beam1RiseOld = beam1RiseLatest;
			setTime(&beam1RiseLatest);

			//output time to test
			printf("Beam 1 rise at: ");
			printf ("%ld %ld\n", beam1RiseLatest.tv.tv_sec, beam1RiseLatest.tv.tv_usec);		
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
		printf("Beam 2 fall at: ");
		printf ("%ld %ld\n", beam2FallLatest.tv.tv_sec, beam2FallLatest.tv.tv_usec);		
	} else {
		//if current read is a 0 then it was a rise
		beam2RiseOld = beam2RiseLatest;
		setTime(&beam2RiseLatest);
		
		//output time to test
		printf("Beam 2 rise at: ");
		printf ("%ld %ld\n", beam2RiseLatest.tv.tv_sec, beam2RiseLatest.tv.tv_usec);		
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


