#include <wiringPi.h>
int main (void)
{
  wiringPiSetup () ;
  pinMode (1, INPUT) ;
  for (;;)
  {
    if(digitalRead (1))
    { 
    	Console.WriteLine("output: " + digitalRead (1));
    }
	delay (500) ;
  }
  return 0 ;
}