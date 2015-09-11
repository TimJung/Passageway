#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//pin numbers
const int BEAM1 = 1;
const int BEAM2 = ?;

//logical state of beams
const int BROKEN = 0;
const int COMPLETE = 1;

int main (void)
{
  //previous values from the last run through
  int prevBeam1 = COMPLETE;
  int prevBeam2 = COMPLETE;

  //set up for wiringPi
  wiringPiSetup();
  pinMode(BEAM1,INPUT);
  pullUpDnControl(BEAM1,PUD_UP);
  pinMode(BEAM2,INPUT);
  pullUpDnControl(BEAM2, PUD_UP);

  for (;;)
  {
	int currBeam1 = digitalRead(BEAM1);
	int currBeam2 = digitalRead(BEAM2);

	//first beam
	if (prevBeam1 == COMPLETE && currBeam1 == BROKEN){
		//hit on the first beam
		//get system time and set a variable
		//compare to last time of 2nd beam
		//make an intelligent decision
	}

	//second beam
	if (prevBeam2 == COMPLETE && currBeam2 == BROKEN){
		//hit on second beam
		//get system time and set a varibale
		//compare to last time of 1st beam
		//make an intelligent decision
	}
	
	prevBeam1 = currBeam1;
	prevBeam2 = currBeam2;
	//delay(100);
  }
  return 0;
}

//new function

  //Pi Machine Name/ID
  // char hostName[100];
  //gethostname(&hostName, 100);


  	//system time
	time_t rawTime; 

	//initialize buffer for system time
	char buffer[26];
	struct tm* tm_info;
	time(&rawTime);
	tm_info = localtime(&rawTime);
	strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);

	//print or sent to allJoyn router
	printf("entry: ");
	printf("%s\n ", buffer);



