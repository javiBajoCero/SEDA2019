#include "cabeceras.h"


/*
 * UART0 interrupt handler
 */
void UART2_IRQHandler(void) {
	
    switch(LPC_UART2->IIR&0x0E) {
	
	case 0x04:								 /* RBR, Receiver Buffer Ready */
	*ptr_rx_2=LPC_UART2->RBR; 	   			 /* lee el dato recibido y lo almacena */
	    if (*ptr_rx_2++ ==13) 				// Caracter return --> Cadena completa
	    					{
							*ptr_rx_2=0;		/* Añadimos el caracter null para tratar los datos recibidos como una cadena*/ 
							rx_completa_2 = 1;/* rx completa */
							ptr_rx_2=buffer_2;	/* puntero al inicio del buffer para nueva recepción */
	    					}	
		break;
	
    
   case 0x02:								/* THRE, Transmit Holding Register empty */
		if (*ptr_tx_2!=0) LPC_UART2->THR=*ptr_tx_2++;	/* carga un nuevo dato para ser transmitido */
		else tx_completa_2=1;
		break;

    }
}

// Función para enviar una cadena de texto
// El argumento de entrada es la dirección de la cadena, o
// directamente la cadena de texto entre comillas
void tx_cadena_UART2(char *cadena)
{
ptr_tx_2=cadena;
tx_completa_2=0;
LPC_UART2->THR=*ptr_tx_2++;	 // IMPORTANTE: Introducir un carácter al comienzo para iniciar TX o
}							 // activar flag interrupción por registro transmisor vacio


int uart2_set_baudrate(unsigned int baudrate) {     
int errorStatus = -1; //< Failure 
 
// UART clock (FCCO / PCLK_UART2)      
//unsigned int uClk = SystemFrequency / 4;     
unsigned int uClk =SystemCoreClock/4;     
unsigned int calcBaudrate = 0;    
unsigned int temp = 0; 
unsigned int mulFracDiv, dividerAddFracDiv;     
unsigned int divider = 0;     
unsigned int mulFracDivOptimal = 1;     
unsigned int dividerAddOptimal = 0;     
unsigned int dividerOptimal = 0; 
unsigned int relativeError = 0;     
unsigned int relativeOptimalError = 100000; 
 
uClk = uClk >> 4; /* div by 16 */ 
 
    /*      * The formula is :      
	* BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * DLL)      *      
	* The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:      
	* 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15      */     
	for (mulFracDiv = 1; mulFracDiv <= 15; mulFracDiv++) {         
		for (dividerAddFracDiv = 0; dividerAddFracDiv <= 15; dividerAddFracDiv++) {             
			temp = (mulFracDiv * uClk) / (mulFracDiv + dividerAddFracDiv); 
 
      divider = temp / baudrate;             
			if ((temp % baudrate) > (baudrate / 2)) 
			divider++; 
 
      if (divider > 2 && divider < 65536) {                 
			calcBaudrate = temp / divider; 
 
      if (calcBaudrate <= baudrate) {                     
			relativeError = baudrate - calcBaudrate;                 
			} 
			else {                     
			relativeError = calcBaudrate - baudrate;                 
			} 
 
      if (relativeError < relativeOptimalError) {                     
			mulFracDivOptimal = mulFracDiv;                     
			dividerAddOptimal = dividerAddFracDiv;                     
			dividerOptimal = divider;                     
			relativeOptimalError = relativeError;                     
			if (relativeError == 0)                         
			break;                 
			}             
		}         
} 
 
        if (relativeError == 0)             
		break;     
} 
 
    if (relativeOptimalError < ((baudrate * 3) / 100)) { 
 
      LPC_UART2->LCR |= (1<<7);  // importante poner a 1         
			LPC_UART2->DLM = (unsigned char) ((dividerOptimal >> 8) & 0xFF);         
			LPC_UART2->DLL = (unsigned char) dividerOptimal;         
			LPC_UART2->LCR &= ~(1<<7); // importante poner a 0 
 
      LPC_UART2->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F); 
 
      errorStatus = 0; //< Success     } 
 
     } 
		return errorStatus;
	}

 					   					  
void uart2_init(int baudrate) {          
	LPC_SC->PCONP|=1<<24;// POWER UP uart2
    LPC_PINCON->PINSEL0 |= (1 << 20) | (1 << 22);    // Change P0.10 and P0.11 mode to TXD2 and RXD2
	
	LPC_UART2->LCR = 3; //longitud de palabra de 8 bit, un bitstop y sin paridad, con un uno en el bit 0 y el 1, en el bit 2 ponemos un 0 para 1 bitstop, y en el bit 3 tambien un 0 para quitar la paridad 
	
	uart2_set_baudrate(baudrate);                 // Set the baud rate          
			
	LPC_UART2->IER=1; 		//Habilitamos interrupcion con un uno en el bit 0        
	NVIC_EnableIRQ(UART2_IRQn);                        // Enable the UART interrupt (for Cortex-CM3 NVIC)
} 

void config_uart_2(void){
	ptr_rx_2=buffer_2;	                // inicializa el puntero de recepción al comienzo del buffer
uart2_init(9600);							 // configura la UART0 a 9600 baudios, 8 bits, 1 bit stop
					
tx_cadena_UART2("Esperando comandos! \n\r");
}


void clear_buffer_2(void){
	unsigned int o;
	for(o=0;o<=30;o++){
		buffer_2[o]=0x00;
	}
}

