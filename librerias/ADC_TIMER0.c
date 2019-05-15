#include "cabeceras.h"

void init_ADC_y_timer0(void)
{	
	LPC_SC->PCONP|= (1<<12);					// POwer ON
	LPC_PINCON->PINSEL1|= (1<<14);  	// ADC input= P0.23 (AD0.0)
	LPC_PINCON->PINMODE1|= (2<<14); 	// Deshabilita pullup/pulldown
	LPC_SC->PCLKSEL0|= (0x00<<8); 		// CCLK/4 (Fpclk después del reset) (100 Mhz/4 = 25Mhz)
	LPC_ADC->ADCR= (0x01<<0)|		  	  // Canal 0
								 (0x01<<8)|		  	  // CLKDIV=1   (Fclk_ADC=25Mhz /(1+1)= 12.5Mhz)
								 (0x01<<21)|			 	// PDN=1 POWERDOWN = NO
								 (4<<24);				    // Inicio de conversión con el Match 1 del Timer 0
	LPC_ADC->ADINTEN=(1<<0);					// Hab. interrupción fin de conversión Canal 0 (Ojo no se ha utilizado |=, porque despues del reset queda habilitada ADGDR)
//LPC_ADC->ADINTEN=(1<<8);					// idem Reg. Global
	NVIC_EnableIRQ(ADC_IRQn);					// ? 
	NVIC_SetPriority(ADC_IRQn,1);			// ?    

	
		  LPC_SC->PCONP|=(1<<1);						// ? 
	  LPC_PINCON->PINSEL3|= 0x0C000000; // ?
    LPC_TIM0->PR = 0x00;							// ?   
    LPC_TIM0->MCR = 0x10;   					// ?   
    LPC_TIM0->MR1 = (Fpclk/F_sample/2)-1; // Se han de producir DOS Match para iniciar la conversión!!!!   
    LPC_TIM0->EMR = 0x00C2;   				// ? 
    LPC_TIM0->TCR = 0x02;     				// stop and reset timer
}	

void ADC_IRQHandler(void)
{
	
	if(index_adc>=N_muestras){
		index_adc=0;
		BUSY_RECORDING=0;
		LPC_TIM0->TCR = 0x02;							// Timer STOP y RESET	
	}else{
	muestras_adc[index_adc]= ((LPC_ADC->ADDR0>>8)&0xFF);	// se borra automat. el flag DONE al leer ADCGDR
	index_adc++;
	}
}



// Timer 1 interrumpe periódicamente a F_sample = 8000 Hz !!!!
// La muestra correpondiente del array,  se saca al DAC en cada interrupción
//void TIMER3_IRQHandler(unsigned int adc_dac)
//{
//	if(adc_dac==1){
//	static uint16_t indice_muestra;
//	LPC_TIM3->IR|= (1<<0); 													// Borrar flag
//	LPC_DAC->DACR= muestras[indice_muestra++]<<8; // 8 bits!!!!
//	if(indice_muestra==N_muestras-1){
//																	indice_muestra=0;
//																	LPC_TIM3->TCR=0x02;	//Stop Timer and reset, DAC= 0V.
//																	LPC_DAC->DACR=0;	 // 0 V
//														}
//	}else if(adc_dac==0){
//		
//	}
//}	


void record(void){
	BUSY_RECORDING=1;
	LPC_TIM0->TCR=0x01;		//Start Timer .	
}

