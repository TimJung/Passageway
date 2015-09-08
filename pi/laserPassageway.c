
#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main (void)
{
  //input is used for measuring what the last input was from the wire
  int input = 1;
  //get the system time
  time_t rawTime;
  // char hostName[100];
  // gethostname(&hostName, 100);

  //set up for wiringPi
  wiringPiSetup();
  pinMode(1,INPUT);
  pullUpDnControl(1,PUD_UP);

  for (;;)
  {
	//initialize buffer for system time
	char buffer[26];
	struct tm* tm_info;

	int current = digitalRead(1);
	if(input == 1 && current == 0 ){
		//do time magic
		time(&rawTime);
		tm_info = localtime(&rawTime);
		strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
	
		//print or sent to allJoyn router
		printf("hit: ");
		printf("%s : %s ", buffer, hostName);
	}
	input = current; 
	delay(250);
  }
  return 0;
}
