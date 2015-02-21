//ccs6 written by David Whitfield
//The program decodes an IR signal with the TSOP32438

#include "msp430.h"

#define buzzer BIT1 //red LED
#define rec1 BIT0 //green LED
#define rec2 BIT2 //relay1 for light 1
#define debug BIT6 //relay1 for light 1

int counttimes =100;

int resetcount =990;
int loopcount=0;
int count =0;
int one=0;
int two =0;
int three =0;
int four =0;
int five =0;
int six =0;
int seven =0;
int eight =0;
int ii=0;
int i=0;
int RXinRange =0;

void main(void){

  WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer

  P1DIR |= (buzzer + debug);     //output direction                       // Set all pins but RXD to output
  P1OUT =0;
  P1IE |= rec1; // P1.3 interrupt enabled
  P1IES &= ~rec1; // P1.3 interrupt enabled
  P1IFG &= ~rec1; // P1.3 IFG cleared
  P1IE &= ~rec2; // P1.3 interrupt enabled
  P1IES |= rec2; // P1.3 interrupt enabled
  P1IFG &= ~rec2; // P1.3 IFG cleared

  TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
  TA0CCR0 =  25;                            //count up to 20



    __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts



  while(1)
  {
    _delay_cycles(1000);

    resetcount++;
    if(resetcount > 1000)
    {
      loopcount = 0;

__delay_cycles(100);
if(RXinRange >= 40)
{
	P1OUT |= buzzer;
}
else
{
	P1OUT &= ~buzzer;
}
RXinRange =0;
P1IE |= (rec1 + rec2);
    }

    if(resetcount >2000)
    {
    	resetcount =2000;
    }

    if(count >1000)
    {
      count =999;
    }

  }
}




#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  TA0CTL |= TACLR;
  P1IE &= ~(rec1 + rec2);
 P1OUT ^= debug;
  if(loopcount >2 && loopcount < (counttimes +2))
  {

	  if(count>0 && count < 10)
	 	  {
	 		  RXinRange++;
	 	  }
  }

  if(loopcount < (counttimes +2))
  {
	  P1IE |= (rec1 + rec2);
  }

  resetcount=0;
  loopcount++;
  count=0;
 P1IFG &= ~(rec1 + rec2); // P1.3 IFG cleared

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  count++;
}
