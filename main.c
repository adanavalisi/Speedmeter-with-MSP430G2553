
#include <msp430.h>
#include "string.h"
#include <stdio.h>
#include "math.h"
#include "stdint.h"
#include "LCD_I2C.H"

int i,j,count,count_2;
int time_1 = 0;
int distance = 50; // meter
int velocity = 0;
char str[100];
int a = 0;
int velocity_count = 0;
unsigned int velocitys[10];
int k = 0;
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timerA(void) {
    count++;
    count_2++;
}

int main(void)
{


  WDTCTL = WDTPW + WDTHOLD;

  //Mode 1, source SMCLK, div by 4
  TA0CTL = MC_1 | ID_2 | TASSEL_2;

  TACCR0 = 250;
  TACCTL0 = CCIE;

  P1DIR|=BIT0|BIT6|BIT1; //Set P1.0 and P1.6 as output pin
    P1OUT&=~BIT0; //Turn off green LED
    P1OUT&=~BIT6; //Turn off green LED
    P1OUT&=~BIT1; //Turn off green LED

    P1DIR&=~BIT5; //Set P1.35as input
    P1REN|=BIT5; //Enable pullup/pulldown resistors for P1.5
    P1OUT|=BIT5; //Set P1.5 to have pull up resistors

    P1IE  |= BIT5;
    P1IES |= BIT5;
    P1IFG &= ~BIT5;


    P2DIR&=~BIT0;
    P2REN|=BIT0;
    P2OUT|=BIT0;

    P2IE  |= BIT0;
    P2IES |= BIT0;
    P2IFG &= ~BIT0;

  lcd_init(0x27);//Initialize the LCD as slave with I2C Slave address as 0x27
  set_backlight();//Turn on the backlight
  velocity = 0;
  __enable_interrupt();

  while(1){
      sprintf(str, "%d", velocity);

      lcd_setcursor(0,2);//Set the position of the Cursor
      lcd_print("velocity of car");//Print the String Sanjeeve R. The String must be enclosed within double brackets.
      lcd_setcursor(1,6);//Set the position of the Cursor
      lcd_print("    ");//Print the String Sanjeeve R. The String must be enclosed within double brackets.
      lcd_setcursor(1,6);//Set the position of the Cursor
      lcd_print(str);//Print the String Sanjeeve R. The String must be enclosed within double brackets.
      lcd_setcursor(1,11);//Set the position of the Cursor
      lcd_print("m/s");//Print the String Sanjeeve R. The String must be enclosed within double brackets.
      if(a == 1){
          a = 0;
          velocity = 0;
          count_2 = 0;
          P1OUT ^= BIT2;
          __delay_cycles(5000000);
          P1OUT ^= BIT2;
      }
      velocity = 0;
      __delay_cycles(2000000);

  }

}
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
      time_1 = count;
      count = 0;

      velocity = (distance / (time_1*0.001));
      if(velocity_count<11)
      {
         velocitys[velocity_count] = velocity ;
         if(velocity_count==10)
         {
             velocity_count = 0 ;

             for(k=0;k<11;k++)
                 velocitys[k]=0;

         }

      }
      velocity_count++;
      __delay_cycles(300000);

      if(velocity >= 100)
          a=1;
      P1IFG &= ~BIT5;

}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    count = 0;
    __delay_cycles(300000);
    P2IFG &= ~BIT0;

}
// Timer A0 interrupt service routine


