#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

//declare your functions
void pinFall1 (void);
void pinRise1 (void);
void pinFall1 (void);
void pinFall2 (void);
void setTime (struct*);

//pin numbers. 1 is entry. 2 is exit.
const int BEAM1 = 1;
const int BEAM2 = 4;

//time comparison variables
struct tm beam1FallOld;
struct tm beam1RiseOld;
struct tm beam1RiseOld;
struct tm beam1FallLatest;
struct tm beam1RiseLatest;
struct tm beam2FallOld;
struct tm beam2RiseOld;
struct tm beam2RiseOld;
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
  
  wiringPiISR (BEAM1, INT_EDGE_FALLING, pinFall1);
  wiringPiISR (BEAM1, INT_EDGE_RISING, pinRise1);
  wiringPiISR (BEAM2, INT_EDGE_FALLING, pinFall2);
  wiringPiISR (BEAM2, INT_EDGE_RISING, pinRise2);
  
  while (1)
  {
	wait();
  }
}

void pinFall1 (void){
	beam1FallOld = beam1FallLatest;
	setTime(&beam1FallLatest);
	
	//output time to test
	char buffer[26];
	strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", beam1FallLatest);
	printf("Beam 1 fall at: ");
	printf("%s\n ", buffer);
}

void pinRise1 (void){
	beam1RiseOld = beam1RiseLatest;
	setTime(&beam1RiseLatest);
	
	//output time to test
	char buffer[26];
	strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", beam1RiseLatest);
	printf("Beam 1 rise at: ");
	printf("%s\n ", buffer);
}

void pinFall2 (void){
	beam2FallOld = beam2FallLatest;
	setTime(&beam2FallLatest);
	
	//output time to test
	char buffer[26];
	strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", beam2FallLatest);
	printf("Beam 2 fall at: ");
	printf("%s\n ", buffer);
}

void pinRise2 (void){
	beam2RiseOld = beam2RiseLatest;
	setTime(&beam2RiseLatest);
	
	//output time to test
	char buffer[26];
	strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", beam2RiseLatest);
	printf("Beam 2 rise at: ");
	printf("%s\n ", buffer);
}

void setTime(struct * ptr){
	time_t currentTime = timegm(&ptr);
	temp = gmtime(currentTime);
	ptr = *temp;
}

//new function

  //Pi Machine Name/ID
  // char hostName[100];
  //gethostname(&hostName, 100);


  	//system time
	//time_t rawTime; 

	//initialize buffer for system time
	//char buffer[26];
	//struct tm* tm_info;
	//time(&rawTime);
	//tm_info = localtime(&rawTime);
	//strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);

	//print or sent to allJoyn router
	//printf("entry: ");
	//printf("%s\n ", buffer);


