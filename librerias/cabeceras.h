/////////////////////////////////////////////////////////////////////////////////////////////////////defines
#define F_cpu 100e6			// Defecto Keil (xtal=12Mhz)
#define Fpclk 25e6	// Fcpu/4 (defecto después del reset)
#define Tpwm 1e-3	// Perido de la señal PWM (1ms) (1khz)
#ifndef UART_H_
#define UART_H_
// Accepted Error baud rate value (in percent unit)
#define UART_ACCEPTED_BAUDRATE_ERROR    3
#define CHAR_8_BIT                      (3 << 0)
#define STOP_1_BIT                      (0 << 2)
#define PARITY_NONE                     (0 << 3)
#define DLAB_ENABLE                     (1 << 7)
#define FIFO_ENABLE                     (1 << 0)
#define RBR_IRQ_ENABLE                  (1 << 0)
#define THRE_IRQ_ENABLE                 (1 << 1)
#define UART_LSR_THRE   				        (1 << 5)
#define RDA_INTERRUPT                   (2 << 1)
#define CTI_INTERRUPT                   (6 << 1)
#endif /* UART_H_ */
#define F_sample 8000
#define N_muestras 15000 // igual a la longitud del array generado en Matlab
//////////////////////////////////////////////////////////////////////////////////////////////////////includes
//seccion de includes para el main
#include <LPC17xx.H>                   /* LPC17xx definitions               */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "ruedas.h"
#include "singleadc.h"
#include "i2c_lpc17xx.h"
#include "uart2_blue.h"
#include "GLCD.h"
#include "TouchPanel.h"
#include "Net_Config.h"
#include "RTL.h"
#include "menu.h"
#include "ADC_TIMER0.h"
#include "DAC_TIMER3.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////variables globales (están en main.c)
//seccion de variables globales
extern unsigned char moviendose;
extern unsigned int countdown100ms;
extern unsigned int countdown100ms_alarm;
extern unsigned int timeout;
//uart
extern char buffer[30];	// Buffer de recepción
extern char buffer_2[30];	// Buffer de recepción
extern char *ptr_rx;	// puntero de recepción
extern char *ptr_rx_2;			// puntero de transmisión
extern char rx_completa;// Flag de recepción de cadena completa que se activa al recibir CR(0x0D)
extern char rx_completa_2;// Flag de recepción de cadena completa que se activa al recibir CR(0x0D)
extern char *ptr_tx;			// puntero de transmisión
extern char *ptr_tx_2;			// puntero de transmisión
extern char tx_completa;		// Flag de transmisión de cadena completa
extern char tx_completa_2;		// Flag de transmisión de cadena completa
extern char finMovimiento;
extern unsigned int size,j;
extern  unsigned int val;
 
extern float voltios;//adc single
extern float battery_lvl;//adc single
extern float voltios_bateria_humbral;
extern float temperatura;//adc single
extern char i2c_buffer[6],BZ,BC;	// Buffer de recepción

extern unsigned char muestras_adc[15000];
extern unsigned int index_adc;
extern unsigned char alarma_grabacion;
extern unsigned char BUSY_RECORDING;

extern unsigned int OLD_TC1;
extern unsigned int OLD_TC2;

extern float vel_izq;
extern float vel_der;

extern int velo_A;
extern int velo_B;
extern char buffer_pantalla[30];	// Buffer de recepción pantalla
extern unsigned int rx_buffer_pantalla;





