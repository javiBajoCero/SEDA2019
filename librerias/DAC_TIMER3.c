
#include "cabeceras.h"
//#include "audio_muestras_samsung.h"//se tiene que incluir aqui
#include "audio_muestras_viva.h"//se tiene que incluir aqui


void TIMER3_IRQHandler(void)
{
	static uint16_t indice_muestra;
	LPC_TIM3->IR|= (1<<0); 													// Borrar flag
	if(alarma_grabacion==0){
			LPC_DAC->DACR= muestras[indice_muestra++]<<8; // 8 bits!!!!
	}else if(alarma_grabacion==1){
			LPC_DAC->DACR= muestras_adc[indice_muestra++]<<8; // 8 bits!!!!
	}
	if(indice_muestra==N_muestras-1){
																	indice_muestra=0;
																	LPC_TIM3->TCR=0x02;	//Stop Timer and reset, DAC= 0V.
																	LPC_DAC->DACR=0;	 // 0 V
														}
}	

void init_DAC_y_TIMER3(void)
{
	LPC_PINCON->PINSEL1|= (2<<20); 	 	// DAC output = P0.26 (AOUT)
	LPC_PINCON->PINMODE1|= (2<<20); 	// Deshabilita pullup/pulldown
	LPC_DAC->DACCTRL=0;								//  
		  //LPC_SC->PCONP|=(1<<2);						// 	TIMER 1
		LPC_SC->PCONP|=(1<<23);						// 	TIMER 3
    LPC_TIM3->PR = 0x00;     	 				// 
    LPC_TIM3->MCR = 0x03;							// Reset TC on Match, and Interrupt!  
    LPC_TIM3->MR0 = (Fpclk/F_sample)-1;  // El timer 1 interrumpe cada 125 us 
    LPC_TIM3->TCR = 0x02;							// Timer STOP y RESET
    NVIC_EnableIRQ(TIMER3_IRQn);			//
	  NVIC_SetPriority(TIMER3_IRQn,0);  // Prioridad Máxima  
	  //LPC_TIM3->TCR=0x01;		//Start Timer .	
}


void play_recorded(void){
		alarma_grabacion=1;
	LPC_TIM3->TCR=0x01;		//Start Timer .	

}

void play_alarm(void){
		alarma_grabacion=0;
	LPC_TIM3->TCR=0x01;		//Start Timer .	

}
