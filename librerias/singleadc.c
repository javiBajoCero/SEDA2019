#include "cabeceras.h"

unsigned int k;
//void ADC_IRQHandler(void)
//{
//	// se borra automat. el flag DONE al leer ADCGDR
// //voltios= ((LPC_ADC->ADGDR>>4)&0xFFF)*3.3/4095;
//}

void init_ADC(void)
{	
	LPC_SC->PCONP|= (1<<12);					// POwer ON
	LPC_PINCON->PINSEL1|= (1<<16);  	// ADC input= P0.24 (AD0.1)
	LPC_PINCON->PINSEL1|= (1<<18);  	// ADC input= P0.25 (AD0.2)
	LPC_PINCON->PINMODE1|= (2<<16);
	LPC_PINCON->PINMODE1|= (2<<18); 	// Deshabilita pullup/pulldown
	
	LPC_SC->PCLKSEL0|= (0x00<<8); 		// CCLK/4 (Fpclk después del reset) (100 Mhz/4 = 25Mhz)
//	LPC_ADC->ADCR= //(1<<0)|		  	  // Canal 0,1,2
//								 (0x01<<8)|		  	  // CLKDIV=1   (Fclk_ADC=25Mhz /(1+1)= 12.5Mhz)
//								 (0x01<<21)|			 	// PDN=1 POWERDOWN = NO
//								 //(4<<24);				    // Inicio de conversión con el Match 1 del Timer 0
//										(0<<24);//NO START
	
//	LPC_ADC->ADINTEN=(1<<0);					// Hab. interrupción fin de conversión Canal 0 (Ojo no se ha utilizado |=, porque despues del reset queda habilitada ADGDR)
//LPC_ADC->ADINTEN=(1<<8);					// idem Reg. Global
//	NVIC_EnableIRQ(ADC_IRQn);					// ? 
//	NVIC_SetPriority(ADC_IRQn,1);			// ?        
}	

void snap_adc(void){
	//PREPARA ADC PARA SNAPING
		LPC_ADC->ADCR= (0x01<<8)|		  	  // CLKDIV=1   (Fclk_ADC=25Mhz /(1+1)= 12.5Mhz)
								   (0x01<<21)|			 	// PDN=1 POWERDOWN = NO
										(0<<24);//NO START

	//SNAP CANAL 1
	LPC_ADC->ADCR&= ~(1<<2);		  	  // Canal 2
	LPC_ADC->ADCR|= (1<<1);	  	  // Canal 1
	LPC_ADC->ADCR|=(0X01<<24);//ADC NOW!
	for(k=0;k<100;k++);
	voltios= 2*((LPC_ADC->ADDR1>>4)&0xFFF)*3.3/4095;
	battery_lvl=100*((voltios-3.2));
	//SNAP CANAL 2
	LPC_ADC->ADCR&= ~(1<<1);		  	  // Canal 2
	LPC_ADC->ADCR|= (1<<2);		  	  // Canal 2
	LPC_ADC->ADCR|=(0X01<<24);//ADC NOW!
	for(k=0;k<100;k++);
	temperatura= 100*((LPC_ADC->ADDR2>>4)&0xFFF)*3.3/4095;
		//LPC_ADC->ADCR|=(0X01<<24);//ADC NOW!
	
	//DEJAR ADC LISTO OTRA VEZ PARA AUDIO
		LPC_ADC->ADCR= (0x01<<0)|		  	  // Canal 0
								 (0x01<<8)|		  	  // CLKDIV=1   (Fclk_ADC=25Mhz /(1+1)= 12.5Mhz)
								 (0x01<<21)|			 	// PDN=1 POWERDOWN = NO
								 (4<<24);				    // Inicio de conversión con el Match 1 del Timer 0

	
	
}
