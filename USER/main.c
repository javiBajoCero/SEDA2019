/**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            Example of simple state machine implemented menu with an HTTP server 
**                          and FTP server with data in a SD card 
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JPM (UAH)
** Created date:            2017-08-17
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/


#include "cabeceras.h"
unsigned int countdown100ms=0;
unsigned int countdown100ms_alarm=0;
unsigned int timeout=0;
 char buffer[30];	// Buffer de recepción
 char *ptr_rx;	// puntero de recepción
 char rx_completa;// Flag de recepción de cadena completa que se activa al recibir CR(0x0D)
 char *ptr_tx;			// puntero de transmisión
 char tx_completa;		// Flag de transmisión de cadena completa
 
 char buffer_2[30];	// Buffer de recepción
 char *ptr_rx_2;	// puntero de recepción
 char rx_completa_2;// Flag de recepción de cadena completa que se activa al recibir CR(0x0D)
 char *ptr_tx_2;			// puntero de transmisión
 char tx_completa_2;		// Flag de transmisión de cadena completa
unsigned int j=0;

unsigned char muestras_adc[15000];
unsigned int index_adc=0;
unsigned char alarma_grabacion=0;
 unsigned char BUSY_RECORDING=0;
 
 float voltios;//single adc
 float battery_lvl;//single adc
 float temperatura;
 int counter=0;

extern   U32 CheckMedia (void);
char* COM_ptr;

unsigned int OLD_TC1=0;
unsigned int OLD_TC2=0;

float vel_izq=0;
float vel_der=0;
char buffer_pantalla[30];
unsigned int rx_buffer_pantalla=0;
/*--------------------------- init ------------------------------------------*/

/*******************************************************************************
* Function Name  : init
* Description    : Initialize every subsystem
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
static void init () { 
  LCD_Initializtion();	
  init_TcpNet (); 
  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05; 
  /* TouchPanel Init  */   
  TP_Init(); 
 // TouchPanel_Calibrate(); /* Uncomment for TouchPanel calibration */
		 config_pwm_1y2();
   config_timer1_CAP10_timer2_CAP20(); 
   config_uart();
	 config_uart_2();
	 init_ADC();
	 I2CConfig();
	 init_ADC_y_timer0();
	init_DAC_y_TIMER3();
	I2CConfig();
}



/*--------------------------- timer_poll ------------------------------------*/

/*******************************************************************************
* Function Name  : timer_poll
* Description    : Call timer_tick() if every 100ms (aprox) 
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
		if(timeout>0){
			timeout--;
		}
		if(countdown100ms_alarm>0){
			countdown100ms_alarm--;
		}
		if(countdown100ms>1){
			countdown100ms--;
		}
		if(!BUSY_RECORDING){
		snap_adc();
		imprime_distancia_motores();
	  imprime_velocidad_motores();
    imprime_temperatura_voltaje();
		OLD_TC1=LPC_TIM1->TC;
		OLD_TC2=LPC_TIM2->TC;
		}
    timer_tick();
  }
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
int main(void)
{ 
	init ();
	
	initScreenStateMachine();
	while (1)	
	{
		 
		screenStateMachine();
      timer_poll ();
      main_TcpNet ();
	}
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
