#include <LPC17xx.H>
#include <Math.h>
#include "ADC_TIMER0.h"
#include "DAC_TIMER3.h"

#define F_cpu 100e6			// Defecto Keil (xtal=12Mhz)
#define F_pclk F_cpu/4 	// Defecto despues del reset
#define F_sample 8000
#define N_muestras 15000 // igual a la longitud del array generado en Matlab


extern unsigned char muestras_adc[15000];
extern unsigned int index_adc;
extern unsigned char alarma_grabacion;

