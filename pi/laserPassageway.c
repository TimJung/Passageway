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
void setTime (struct tm *);

//pin numbers. 1 is entry. 2 is exit.
const int BEAM1 = 1;
const int BEAM2 = 4;

//time comparison variables
struct tm beam1FallOld;
struct tm beam1RiseOld;
struct tm beam1FallLatest;
struct tm beam1RiseLatest;
struct tm beam2FallOld;
struct tm beam2RiseOld;
struct tm beam2RiseLatest;
struct tm beam2FallLatest;
struct tm * temp;

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
	gettimeofday (&tv, &tz);
	printf ("%ld %ld\n", tv.tv_sec, tv.tv_usec);
	
	if (firstRise1){
		firstRise1 = 0;
		return;
	}
	if (digitalRead(BEAM1)==1){
		//if current read is a 1 then it was a fall
		beam1FallOld = beam1FallLatest;
		setTime(&beam1FallLatest);

		//output time to test
		char buffer[26];
		strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", &beam1FallLatest);
		printf("Beam 1 fall at: ");
		printf("%s\n", buffer);
	} else {
		//if current read is a 0 then it was a rise
			beam1RiseOld = beam1RiseLatest;
			setTime(&beam1RiseLatest);

			//output time to test
			char buffer[26];
			strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", &beam1RiseLatest);
			printf("Beam 1 rise at: ");
			printf("%s\n", buffer);
		
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
		char buffer[26];
		strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", &beam2FallLatest);
		printf("Beam 2 fall at: ");
		printf("%s\n", buffer);
	} else {
		//if current read is a 0 then it was a rise
		beam2RiseOld = beam2RiseLatest;
		setTime(&beam2RiseLatest);

		//output time to test
		char buffer[26];
		strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", &beam2RiseLatest);
		printf("Beam 2 rise at: ");
		printf("%s\n", buffer);
	}
}

void setTime(struct tm * ptr){
	time_t currentTime;
 	currentTime = timegm(ptr);
	temp = gmtime(&currentTime);
	ptr = temp;
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


