#include "cabeceras.h"

char i;
float ciclo;
char finMovimiento=0;
unsigned char moviendose;
unsigned int ii;
						
	void config_timer1_CAP10_timer2_CAP20(){
   
    LPC_SC->PCONP|=(1<<22)|(1<<2);  				// Power ON: Timer 2 y timer 1
    LPC_PINCON->PINSEL3|= (3<<4);     //PAG 110 PIN1.18 MODO CAP1.0
																						//				 PIN1.27 (22) MODO CAP0.1  EL 1.26(CAP0.0)(20) ES USADO POR LA PANTALLA ???????????????? O NO creo que es 1.27 el que usa la pantalla
    LPC_PINCON->PINSEL0|= (3<<8);     //PAG 110 PIN0.4 MODO CAP2.0
		
		LPC_TIM1->MR0=0xFFFFFFFF;//LOAD THE MATCH REGISTER TO BIG VALUE
		LPC_TIM1->MCR = 0x00000003;  //INTERRUPT IN TC==mr0	AND RESET tc CUANDO MATCH
    LPC_TIM1->CTCR = 0x01;  //COUNTER RISING EDGE CAP1.0		
    LPC_TIM1->CCR = 0x0002;  //falling EDGE NO INTERRUPT WHEN CAPTURE		
		LPC_TIM1->TCR = 0x0001;  	//
		
		LPC_TIM2->MR0=0xFFFFFFFF;//LOAD THE MATCH REGISTER TO BIG VALUE
    LPC_TIM2->MCR = 0x00000003;//INTERRUPTS IN TC== mr0 AND RESET tc CUANDO MATCH
		LPC_TIM2->CTCR = 0x01;		//COUNTER RISING EDGE CAP2.0 pag 496		
    LPC_TIM2->CCR = 0x0002;  	//falling EDGE NO INTERRUPT WHEN CAPTURE		
		LPC_TIM2->TCR = 0x0001;  	//
   NVIC_SetPriorityGrouping(2);				//  prigourp
	 NVIC_SetPriority(TIMER2_IRQn,2);   //  programa prioridad de interrupcion
	 NVIC_SetPriority(TIMER1_IRQn,2);   //   	 
   NVIC_EnableIRQ(TIMER2_IRQn);       //   habilita interruptiones
	 NVIC_EnableIRQ(TIMER1_IRQn);       //  
   
	}
	
	void config_pwm_1y2(void)
  {
	  LPC_GPIO1->FIODIR|=(1<<30);	//pines 1.30 
	  LPC_GPIO1->FIODIR|=(1<<31);	//y 1.31 como salida	
				LPC_GPIO1->FIOCLR|=(1<<30);	//ESTADO INICIAL LOW
			  LPC_GPIO1->FIOCLR|=(1<<31);	
		
		//LPC_PINCON->PINSEL4|=(1<<0); // P2.0 salida PWM (PWM1.1)
		LPC_PINCON->PINSEL3|=(2<<16); // P1.24 salida PWM (PWM1.5)
		//LPC_PINCON->PINSEL3|=(2<<8); // P1.20 salida PWM (PWM1.2)
		LPC_PINCON->PINSEL3|=(2<<14); // P1.23 salida PWM (PWM1.4)
		LPC_SC->PCONP|=(1<<6);//power ON, PAG.65
		LPC_PWM1->MR0=Fpclk*Tpwm-1;//frecuency pwm
		//LPC_PWM1->PCR|=(1<<10); //configurado el ENA2 (1.2) 
		LPC_PWM1->PCR|=(1<<12); //configurado el ENA4 (1.4) 
		//LPC_PWM1->PCR|=(1<<9);  //y ENA1(1.1) pag 531
		LPC_PWM1->PCR|=(1<<13);  //y ENA1(1.1) pag 531
		LPC_PWM1->MCR|=(1<<1);//reset timer on match0
		LPC_PWM1->TCR|=(1<<0)|(1<<3);//start pwm timer, pwm timer mode enable
		
				//LPC_PWM1->MR1=0; // estado inicial de pwm 0
		    LPC_PWM1->MR5=0; // estado inicial de pwm 0
				//LPC_PWM1->MR2=0; // TH
		    LPC_PWM1->MR4=0; // TH
				LPC_PWM1->LER|=(1<<5);
				LPC_PWM1->LER|=(1<<4);
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void set_pwmA(int ciclo)
	{
		if(ciclo>=0){
			ciclo=100-ciclo;	
			LPC_GPIO1->FIOSET|=(1<<31);	
		}else{
		ciclo=-ciclo;
			LPC_GPIO1->FIOCLR|=(1<<31);			
		}
		LPC_PWM1->MR5=((Fpclk*Tpwm*ciclo)/100)-1; // TH
		LPC_PWM1->LER|=(1<<5);
		/*the PWM Latch Enable Registers are used to control the update of the PWM Match
registers when they are used for PWM generation. When software writes to the location of
a PWM Match register while the Timer is in PWM mode, the value is captured, but not
used immediately.*/
}
		void set_pwmB(int ciclo)
	{
		if(ciclo>=0){
			ciclo=100-ciclo;	
			LPC_GPIO1->FIOSET|=(1<<30);	
		}else{
		ciclo=-ciclo;
		//ciclo=100-ciclo;	
			LPC_GPIO1->FIOCLR|=(1<<30);			
    //ciclo=abs(ciclo);
		}
//		LPC_PWM1->MR2=((Fpclk*Tpwm*ciclo)/100)-1; // TH
//		LPC_PWM1->LER|=(1<<2);
		LPC_PWM1->MR4=((Fpclk*Tpwm*ciclo)/100)-1; // TH
		LPC_PWM1->LER|=(1<<4);
	}

	void moveLinecm(int centimeters){
		int sentido;
		uint16_t encoder_count;
		moviendose=0x02;


					if(centimeters>=0){//MANAGE SIGN
					sentido=1;
				}else{
					sentido=-1;
					centimeters=-centimeters;
				}
			 LPC_TIM1->TCR=0x2;//RESET tc basura
			 LPC_TIM2->TCR=0x2;
		 encoder_count=centimeters*(240/9.5);// centimeters*( cuentas cada vuelta/circunferencia de la rueda)
			 LPC_TIM1->MR0=encoder_count;//LOAD THE MATCH REGISTER
			 LPC_TIM2->MR0=encoder_count;
			 LPC_TIM1->TCR=0x1;//enable tc
			 LPC_TIM2->TCR=0x1;
		set_pwmA(70*sentido);	 
		set_pwmB(70*sentido);
	}
	
	
		void turnDegrees(int degrees){
		int sentido;
		uint16_t encoder_count;
		moviendose=0x02;

		if(degrees>=0){//MANAGE SIGN
					sentido=1;
				}else{
					sentido=-1;
					degrees=-degrees;
				}
		 encoder_count=degrees*0.18*(240/9.5);// grados* (numero de centimetros en cada grado)*(conversion a centimetros)
			 LPC_TIM1->MR0=encoder_count;//LOAD THE MATCH REGISTER
			 LPC_TIM2->MR0=encoder_count;
			 LPC_TIM1->TCR=0x1;//enable tc
			 LPC_TIM2->TCR=0x1;
		set_pwmA(-70*sentido);	 
		set_pwmB(70*sentido);
	}
	
	void onetimeUARTruedas(void){
 unsigned int val=0;
		
		if(buffer[j]==0x0D){//terminado
								rx_completa=0; 				// Borrar flag para otra recepción
								j=0;
								clear_buffer();
								tx_cadena_UART0("ok\n\r"); 
			return;
		}
		
	if(rx_completa && !moviendose){					 	// HAY UNA CADENA NUEVA Y EL ROBOT NO SE MUEVE

						switch ( buffer[j] )
						{
								case 'F'://Fxx: Avanza xx centímetros en línea recta (Forward)
										j++;
										val =10*(buffer[j]- '0');
									  j++;
										val =val+ (buffer[j]- '0');
										moveLinecm(val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'B'://Bxx: Retrocede xx centímetros en línea recta (Backward)
										j++;
								    val =10*(buffer[j]- '0');
									  j++;
										val =val+ (buffer[j]- '0');
										moveLinecm(-val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'R'://R: Giro de 90º a la derecha (Right)
										turnDegrees(90);
										//while(moviendose!=0);	
										j++;
										break;
								case 'L'://L: Giro de 90º a la izquierda (Left)
									  turnDegrees(-90);
										//while(moviendose!=0);	
								    j++;
										break;
								case 'P'://Pxx: Se detiene durante xx segundos (Pause).
									if(countdown100ms==0){
										val =10*(buffer[j+1]- '0');
										val =val+ (buffer[j+2]- '0');
										countdown100ms=(val*10)+1;
									  }else if(countdown100ms==1){
											countdown100ms=0;
											j+=3;
										}

										break;
								case 'C'://C: Inicia la grabación de un mensaje (ReCord)
									record();
								  j++;
										break;
								case 'Y'://Y: Reproduce el mensaje grabado si no esta ocupado grabando (PlaY)
									  if(!BUSY_RECORDING){
										play_recorded();
										j++;
											}

										break;
								case 'S'://S: Detiene la grabación de un mensaje (Stop) (opcional)
									
										break;
							  case 'X'://SALTA PRIMER FLAG
										j++;
										break;
								default:
									//return;
								   break;
									//	break;
							}
						}

}

	void onetimeBLUETOOTHruedas(void){
 unsigned int val=0;
		
		if(buffer_2[j]==0x0D){//terminado
								rx_completa_2=0; 				// Borrar flag para otra recepción
								j=0;
								clear_buffer_2();
								tx_cadena_UART2("ok\n\r"); 
			return;
		}
		
	if(rx_completa_2 && !moviendose){					 	// HAY UNA CADENA NUEVA Y EL ROBOT NO SE MUEVE

						switch ( buffer_2[j] )
						{
								case 'F'://Fxx: Avanza xx centímetros en línea recta (Forward)
										j++;
										val =10*(buffer_2[j]- '0');
									  j++;
										val =val+ (buffer_2[j]- '0');
										moveLinecm(val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'B'://Bxx: Retrocede xx centímetros en línea recta (Backward)
										j++;
								    val =10*(buffer_2[j]- '0');
									  j++;
										val =val+ (buffer_2[j]- '0');
										moveLinecm(-val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'R'://R: Giro de 90º a la derecha (Right)
										turnDegrees(90);
										//while(moviendose!=0);	
										j++;
										break;
								case 'L'://L: Giro de 90º a la izquierda (Left)
									  turnDegrees(-90);
										//while(moviendose!=0);	
								    j++;
										break;
								case 'P'://Pxx: Se detiene durante xx segundos (Pause).
									if(countdown100ms==0){
										val =10*(buffer_2[j+1]- '0');
										val =val+ (buffer_2[j+2]- '0');
										countdown100ms=(val*10)+1;
									  }else if(countdown100ms==1){
											countdown100ms=0;
											j+=3;
										}

										break;
								case 'C'://C: Inicia la grabación de un mensaje (ReCord)
									record();
								  j++;
										break;
								case 'Y'://Y: Reproduce el mensaje grabado si no esta ocupado grabando (PlaY)
									  if(!BUSY_RECORDING){
										play_recorded();
										j++;
											}

										break;
								case 'S'://S: Detiene la grabación de un mensaje (Stop) (opcional)
									
										break;
							  case 'X'://SALTA PRIMER FLAG
										j++;
										break;
								default:
									//return;
								   break;
									//	break;
							}
						}

}


	void onetimePANTALLAruedas(void){
 unsigned int val=0;
		
		if(buffer_pantalla[j]==0x0D){//terminado
								rx_buffer_pantalla=0; 				// Borrar flag para otra recepción
								j=0;
									for(ii=0;ii<=30;ii++){
										buffer_pantalla[ii]=0x00;
									}
			return;
		}
		
	if(rx_buffer_pantalla && !moviendose){					 	// HAY UNA CADENA NUEVA Y EL ROBOT NO SE MUEVE

						switch ( buffer_pantalla[j] )
						{
								case 'F'://Fxx: Avanza xx centímetros en línea recta (Forward)
										j++;
										val =10*(buffer_pantalla[j]- '0');
									  j++;
										val =val+ (buffer_pantalla[j]- '0');
										moveLinecm(val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'B'://Bxx: Retrocede xx centímetros en línea recta (Backward)
										j++;
								    val =10*(buffer_pantalla[j]- '0');
									  j++;
										val =val+ (buffer_pantalla[j]- '0');
										moveLinecm(-val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'R'://R: Giro de 90º a la derecha (Right)
										turnDegrees(90);
										//while(moviendose!=0);	
										j++;
										break;
								case 'L'://L: Giro de 90º a la izquierda (Left)
									  turnDegrees(-90);
										//while(moviendose!=0);	
								    j++;
										break;
								case 'P'://Pxx: Se detiene durante xx segundos (Pause).
										if(countdown100ms==0){
										val =10*(buffer_pantalla[j+1]- '0');
										val =val+ (buffer_pantalla[j+2]- '0');
										countdown100ms=(val*10)+1;
									  }else if(countdown100ms==1){
											countdown100ms=0;
											j+=3;
										}

										break;
								case 'C'://C: Inicia la grabación de un mensaje (ReCord)
									record();
								  j++;
										break;
								case 'Y'://Y: Reproduce el mensaje grabado (PlaY)
									  if(!BUSY_RECORDING){
										play_recorded();
										j++;
											}
										break;
								case 'S'://S: Detiene la grabación de un mensaje (Stop) (opcional)
									
										break;
							  case 'X'://SALTA PRIMER FLAG
										j++;
										break;
								default:
									//return;
								   break;
									//	break;
							}
						}

}



	////////////////////////////////////////////////////irq
	void TIMER2_IRQHandler(void) {//PAG 493
	LPC_TIM2->TCR=0x2;//RESET AND DISABLE TC
  set_pwmA(0);//STOP MOTORS
	moviendose--;
	LPC_TIM2->MR0=0xFFFF;//reiniciar tc y cargar valor absurdo en mr
  LPC_TIM2->TCR=0x1;
	LPC_TIM2->IR|=(1<<0);//CLEAR INTERRUPT
}

	void TIMER1_IRQHandler(void) {//PAG 493
	LPC_TIM1->TCR=0x2;//
  set_pwmB(0);//STOP MOTORS
	moviendose--;
	LPC_TIM1->MR0=0xFFFF;//LOAD THE MATCH REGISTER VALOR GRANDE PARA QUE NUNCA LLEGUE
  LPC_TIM1->TCR=0x1;//enable tc
	LPC_TIM1->IR|=(1<<0);//CLEAR INTERRUPT
}
