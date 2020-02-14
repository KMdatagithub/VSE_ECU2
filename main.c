/*
 * VSE_ECU2.c
 *
 * Created: 2/11/2020 7:46:13 PM
 * Author : Khaled Magdy
 */ 

#include <stdio.h>
#include <stdint.h>
#include "softwareDelay.h"
#include "interrupt.h"
#include "timers.h"
#include "gpio.h"
#include "pushButton.h"
#include "sevenSeg.h"
#include "UART.h"
#include "SPI.h"

#define Start 1
#define Stop  2

static volatile char     g_buffer[50];
static volatile uint8_t  g_TMR1_C     = 0;
static volatile uint16_t g_distance   = 0;
static volatile uint16_t g_time       = 0;
static volatile uint8_t  g_timeBase   = 1;
static volatile uint8_t  g_speed      = 0;
static volatile uint8_t  sys_state    = Stop;

int main(void)
{
	//--------[ UART Init ]--------
	UART_cfg uart1_cfg;
	uart1_cfg.baudrate =	   38400;
	uart1_cfg.dataBits =	   UART_Bits_8;
	uart1_cfg.mode =		   UART_TX;
	uart1_cfg.parity =		   No_Parity;
	uart1_cfg.sampleEdge =     RisingEdg;
	uart1_cfg.stopBits =	   StopBits_1;
	uart1_cfg.uartSync =	   UART_Async;
	uart1_cfg.uartInterrupts = UART_No_Interrupts;
	UART_Init(&uart1_cfg);
	
	//--------[ SPI Init ]--------
	SPI_Init(SPI_Slave, SPI_CLK_F_64, SPI_Mode0, LSB_First, SPI_Interrupts_Enabled);
	
	//--------[ Timer1 Init ]--------
	timer1Init(T1_NORMAL_MODE, T1_OC1_DIS, T1_PRESCALER_256, 49910, 0, 0, 0, T1_INTERRUPT_NORMAL);
    
	//--------[ Buttons Init ]--------
	pushButtonInit(BTN_0);
	pushButtonInit(BTN_1);
	
    while(1) 
    {
		// Start Button
		if(pushButtonGetStatus(BTN_0) == Pressed)
		{
			if(sys_state == Stop)
			{
			  sys_state = Start;
		      timer1Init(T1_NORMAL_MODE, T1_OC1_DIS, T1_PRESCALER_256, 49910, 0, 0, 0, T1_INTERRUPT_NORMAL);
			  timer1Start();	
			}
			softwareDelayMs(500);
		}
		// Stop Button
		if(pushButtonGetStatus(BTN_1) == Pressed)
		{
			if(sys_state == Start)
			{
			  sys_state = Stop;
			  timer1Stop();
			  sprintf(g_buffer, "Total Trip Measurements:  Time= %d Seconds, Distance= %d Meters\r\n", g_time, g_distance);
			  UART_Write_String(g_buffer);
			  g_TMR1_C =0;
			  g_distance = 0;
			  g_speed = 0;
			}
			softwareDelayMs(500);
		}
		
    }
}

ISR(TIMER1_OVF_vect)
{
	gpioPinToggle(GPIOC, BIT5);
	timer1Set(50000);
	g_TMR1_C++;
	if(g_TMR1_C == 4)
	{
		g_time += g_timeBase;
		g_TMR1_C = 0;
	    g_distance += (g_speed * g_timeBase);
		sprintf(g_buffer, "Measurements:  Speed= %d, Time= %d, Distance= %d\r\n", g_speed, g_time, g_distance);
		UART_Write_String(g_buffer);
	}
}

ISR(SPI_STC_vect)
{
	g_speed = SPDR-48;
}