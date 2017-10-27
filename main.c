// This is released under the GPL Version 2
// Coded by: Jordan Tryon
// Include header in all reproductions
// This is version .9.5 all other versions might be released under a different
// Using Polling to see if it combats vibrations 
// Licence or requriments.
// Date is 2-18-12


#include  <msp430.h>  //Some headers that make it work with whatever version I have of MSPGCC
#include <legacymsp430.h>// Change to whatever complier you use
#define LED0 BIT0 //Just defined the LED to their assigned BIT*
#define LED1 BIT6 //BIT0 is the 1 on the 7 seg display, BIT6 is the C for the left of the Zero
#define LED2 BIT5 //This is the hypen 
#define BUTTON BIT3 // Did the same for the button!

void displaytime(void); // Function to display time!
void disphh(int hh); // Function to display the hour
void dispmm(int mm); // Function to display the Min
void disphyphen(void); // Function to display the hypen between hour and min
void cleardisplay(void);// just displays nothing, basicly just a pause in between
void display1(void); // Turns on the 1 for the 7seg Display
void display0(void); // Turn on the left side of 0 and the right side of 0 (which is 1) on the 7 seg Display

// Timekeeping variables
unsigned int hh = 7, mm = 50, ss = 0; // Set the time here! I useually add a 2 mins
                                      // for flashing and transfer time to the watch
                                      // then I plug in the MSP430 when the time hits


int main(void)
{
	
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
 
  // Code from TI example Timer A setup
  TACCR0 = 32768-1;
  TACTL = TASSEL_1+MC_1;                    // ACLK, upmode
  TACCTL0 |= CCIE;                          // enable TA0CCRO interrupt
  __bis_SR_register(GIE);
  // End TI example Timer A setup

  P1DIR |= LED2; // Set P1.5 to output direction
  P1DIR |= (LED0 + LED1); // Set P1.0 and P1.6 to output direction 



  while(1)
  {  
    if ((P1IN & BIT3))  
    { 
      volatile unsigned long j;
      j = 110000;                              
      do (j--);
      while (j != 0);
      if ((P1IN & BIT3))
        displaytime();
    }
  }
  return 0; 		
}
void displaytime(void)
{

  disphh(hh); // Calls the display hour function with the current hour
  disphyphen(); // Displays a Hypen between hour and min
  dispmm(mm); // Calls the display min function with the current min
}

void cleardisplay(void)
{
  {
    volatile unsigned long j;
    j = 50000;                              
    do (j--);
    while (j != 0);
  }
}

void display1(void) 
{
  P1OUT ^= BIT0;                          // Displays 1 (Turns on)
  {
    volatile unsigned long i;
    i = 50000;                              // Delay
    do (i--);
    while (i != 0);
  }
  P1OUT &= ~BIT0;                          // Clear Display (Turns off)
  cleardisplay();
}

void display0(void)
{
 
  P1OUT ^= BIT0;                          // Turns on the right side of zero (displays 1)
  P1OUT ^= BIT6;                          // Turns on the left side of zero 
  {
    volatile unsigned long i;
    i = 50000;                              // Delay
    do (i--);
    while (i != 0);
  }
  P1OUT &= ~BIT0;                          // Turns off the left side of zero
  P1OUT &= ~BIT6;                          // Turns off the right side of zero
  cleardisplay();

}
void disphyphen(void)
{
  P1OUT ^= LED2;                          // Displays 1 (Turns on)
  {
    volatile unsigned long i;
    i = 90000;                              // Delay
    do (i--);
    while (i != 0);
  }
  P1OUT &= ~LED2;                          // Clear Display (Turns off)
  cleardisplay();
}



void disphh(int hh)  // Displays the hour, also converts Dec to Bin
{
  int timehour = hh;
  int binhour = 8;
  int i = 1;
  

  if (timehour > 12)
  {
    timehour = timehour - 12; // I don't like Military Time, so I just subtract
  }                           // 12 if it is larger than 12 hours
   

  else
  {
    timehour = timehour; //take this out if it work without it                        
  }
  
  for (i = 1; i <= 4; i++)  // Look at the comments for displaymin() for discription on this function.
  {

    if (timehour >= binhour) //If it is a one display a 1
    {
      display1();  
      timehour = timehour - binhour;
    }

    else // Else display a 0
    {
      display0();
    }
   binhour = binhour/2;
 }

}





void dispmm(int mm) // Displays the min, also converts Dec to Bin
{
  int timemin = mm;  //Stores mm to timemin so we don't acidently mess up the timer (I know it is basicly
                    // Impossible but I like to be safe when coding)
  int binmin = 32; // Converts the decimal to Binary and either calls display1 or display0 function.
  int i = 1;      // Basicly if you know how to convert Dec2Bin you can figure this out, I wrote it to 
  for (i = 1; i <= 6; i++) // be easy to understand.
  {

    if (timemin >= binmin) // If it is a 1 displays 1
    {
      display1();
      timemin = timemin - binmin;
    }

    else // Else just display a 0
    {
      display0();
    }
    binmin = binmin/2;  
  }

}



interrupt(TIMERA0_VECTOR) TimerA_procedure(void) // Interrupt for Timer A, Basicly the RTC part
{
  ss++; // Adds one second every second 
  if(ss  == 60)  // If seconds over 60 zero it out and add 1 to Minuets 
  {
   ss = 0;
   mm++;
  }
 
 if(mm  == 60)  // If Minuets over 60 zero it out and add 1 to Hours 
  {
    mm = 0;
    hh++;
  }

  if(hh > 12)  // If Hours over 12 it changes it to 1
  {
    hh = 1;
  }
}


