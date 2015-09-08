#include <wiringPi.h>
#include <stdio.h>
int main (void)
{
  wiringPiSetup () ;
  pinMode (1, INPUT) ;
  for (;;)
  { 
    	printf("output: ");
	printf("%i", digitalRead (1));
	printf("\n");
	delay (500) ;
  }
  return 0 ;
}
