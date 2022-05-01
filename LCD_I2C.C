#include "LCD_I2C.H"

uint8_t LCDOUT=0x00;
void set_backlight(){
    LCDOUT|=BL;
}
void lcd_write(uint8_t value,uint8_t mode){
    if(mode)
        LCDOUT |= RS;
    else
        LCDOUT &= ~RS;
    LCDOUT=((LCDOUT & 0x0F)|(value & 0xF0));
    pulseEN();
    LCDOUT=((LCDOUT & 0x0F)|(value<<4 & 0xF0));
    pulseEN();
}
void lcd_print(char *s){
    while(*s){
        lcd_write(*s,DATA);
        s++;
    }
}

void delay(uint16_t t){
    for(t;t>0;t--){
        __delay_cycles(100);
    }
}
void pulseEN(){
    LCDOUT|=EN;
    send();
    delay(1);
    LCDOUT&=~EN;
    send();
    delay(1);
}
void send(){
    UCB0CTL1&=~UCTXSTP;
    UCB0CTL1 |=  UCTXSTT;
    while(UCB0CTL1&UCTXSTT);
    UCB0CTL1&=~UCTXSTP;
}
void lcd_init(uint8_t addr){
    P1SEL|=BIT6+BIT7;
    P1SEL2|=BIT6+BIT7;

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0|=UCMST+UCMODE_3+UCSYNC;
    UCB0CTL1 = UCSSEL_2 + UCSWRST+UCTR;
    UCB0BR0 = 11;
    UCB0BR1 = 0;

    UCB0I2CSA=addr;


    UCB0CTL1 &= ~UCSWRST;
    IE2 |= UCB0TXIE;
    IFG2&=~IFG2;

    __bis_SR_register(GIE);
    delay(100);
    lcd_write(0x33,CMD);
    delay(50);
    lcd_write(0x32,CMD);
    delay(50);
    lcd_write(0x28,CMD);
    delay(50);
    lcd_write(0x0C, CMD);           // Display ON, Cursor OFF, Blink OFF
       delay(10);
    lcd_write(0x01,CMD);
    delay(40);
    lcd_write(0x06,CMD);
    delay(1);
    lcd_setcursor(0,0);
}
void lcd_setcursor(uint8_t row,uint8_t col){
    uint8_t row_offset[]={0x00,0x40};
    lcd_write(0x80 | (col + row_offset[row]),CMD);
    delay(10);
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){

       UCB0TXBUF=LCDOUT;
       UCB0CTL1&=~UCTXSTT;
       UCB0CTL1|=UCTXSTP;
       IFG2&=~UCB0TXIFG;

}
