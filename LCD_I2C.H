/* LCD_I2C.H
 *
 * Author: Sanjeeve R
 *
 *
 * Description:
 * This project is developed specifically to interface MSP430G2553 Controller/Launchpad with 16x2 LCD using I2C protocol.
 * This project is tested to work on Texas Instruments Code Composer Studio  Version: 10.3.1.00003
 * Connection:
 *        ***********************************
 *        *  LCD | PIN ON CONTROLLER/BOARD  *
 *        ***********************************
 *        *  VCC |  5V                      *
 *        *  GND |  GND                     *
 *        *  SDA |  P1.7                    *
 *        *  SCL |  P1.6                    *
 *        ***********************************
 *
 *
 * */

#ifndef _LCD_I2C_H
#define _LCD_I2C_H

#include<inttypes.h>
#include<msp430g2553.h>
#define CMD 0
#define DATA 1




#define RS 0x01
#define RW 0x02
#define EN 0x04
#define BL 0x08


void lcd_init(uint8_t);
void lcd_write(uint8_t,uint8_t);
void lcd_print(char*);
void lcd_setcursor(uint8_t,uint8_t);
void set_backlight();
#endif
