/**********************************/
/*Funciones de control del bus I2C*/
/* SDA=P0.0 y SCL=P0.1            */
/**********************************/
#include "cabeceras.h"
#define SDA 5 //pin 0
#define SCL 6 //pin 1

 char BZ,BC;
 
void I2Cdelay(void)//retardo minimo de 4.7 us
{
 unsigned char i;    
 for(i=0;i<100;i++); //Modificar límite para garantizar los tiempos (Bus standar -->F_max=100kHz)
}

//Genera un pulso de reloj (1 ciclo)
void pulso_SCL(void)
{
	 LPC_GPIO0->FIOSET=(1<<SCL);	// Genera pulso de reloj (nivel alto)
	 I2Cdelay();
	 LPC_GPIO0->FIOCLR=(1<<SCL);	// Nivel bajo
	 I2Cdelay();
}

void I2CSendByte(unsigned char byte)
{
 unsigned char i;
 for(i=0;i<8;i++){
	 
	 if (byte &0x80)  LPC_GPIO0->FIOSET=(1<<SDA); // envia cada bit, comenzando por el MSB
	 else  LPC_GPIO0->FIOCLR=(1<<SDA);
	 byte = byte <<1;		// siguiente bit
	 pulso_SCL();
	}

//Leer ACK que envía el Slave (el Master ha de enviar un pulso de reloj)	
// CONFIGURAR PIN SDA COMO ENTRADA;        //espera ACK(config. pin como entrada)
 LPC_GPIO0->FIODIR&=~(1<<SDA);
 pulso_SCL();

// CONFIGURA PIN SDA COMO SALIDA;
 LPC_GPIO0->FIODIR|=(1<<SDA);			// Dejamos SDA de nuevo como salida 
}


//Función que envía START + Byte de dirección del Slave (con bit LSB inicando R/W)	
void I2CSendAddr(unsigned char addr, unsigned char rw)
{
 //CONFIGURAR PINs SDA, SCL COMO SALIDAS;  // Por si se nos olvidada en la conf. general.
 LPC_GPIO0->FIODIR|=(1<<SDA)|(1<<SCL);	
 
 LPC_GPIO0->FIOSET|=(1<<SDA)|(1<<SCL);	   // SDA y SCL a nivel alto para garantizar el
										   // nivel de reposo del bus + tiempo.
 I2Cdelay();
 LPC_GPIO0->FIOCLR|=(1<<SDA);	   // SDA y SCL a nivel alto para garantizar el						   //condicion de START: Bajar SDA y luego SCL
 I2Cdelay();								
 LPC_GPIO0->FIOCLR|=(1<<SCL);
 I2Cdelay();

 I2CSendByte((addr=addr<<1) + rw);  //envia byte de direccion 
 									//addr, direccion (7bits) 
	         						//rw=1, lectura
									//rw=0, escritura

}


// Función para leer un Byte del Slave. El Master envía al final de la lectura
// el bit ACK o NACK (si es último byte leído) que se pasa como argumento de la función.
unsigned char I2CGetByte(unsigned char ACK)	
{
// ACK = 0, para cualquier byte que no sea el ultimo.
// ACK = 1 (NACK), despues de leer el ultimo byte
 unsigned char i, byte;
 //CONFIGURAR PIN SDA COMO ENTRADA;	//configura pin SDA como entrada
 LPC_GPIO0->FIODIR&=~(1<<SDA);
 for(i=0;i<8;i++){					//lee un bit comenzando por el MSB
		
		 LPC_GPIO0->FIOSET=(1<<SCL);//mientras SCL=1
		 I2Cdelay();
		 byte=byte<<1;				
		 if(LPC_GPIO0->FIOPIN&(1<<SDA)) byte++;			//Si leemos "1" sumamos para introducir el "1"
		 LPC_GPIO0->FIOCLR=(1<<SCL);					//Si leemos "0" solo desplazamos (se introduce un "0")
		 I2Cdelay();
		}

 //CONFIGURAR PIN SDA COMO SALIDA;    // Master envía un ACK por cada byte leído.		
 LPC_GPIO0->FIODIR|=(1<<SDA);	
 
 if(ACK)LPC_GPIO0->FIOSET=(1<<SDA);	  // ACK o (NACK) es funcion del último byte leído
 	else LPC_GPIO0->FIOCLR=(1<<SDA);
 
 pulso_SCL();   				      // Pulso de reloj para su envío
 return (byte);
}


void I2CSendStop(void)
{
 LPC_GPIO0->FIOCLR=(1<<SDA);
 I2Cdelay();
 LPC_GPIO0->FIOSET=(1<<SCL);		  // Subir SCL, y después SDA!! para dejar el bus en reposo
 I2Cdelay();
 LPC_GPIO0->FIOSET=(1<<SDA);						
 I2Cdelay();
}
//////////////////////////////
void I2CConfig(void)
{
I2CSendAddr(0x52,0);
I2CSendByte(0xF0);
I2CSendByte(0x55);
I2CSendStop();
	
I2CSendAddr(0x52,0);
I2CSendByte(0xFB);
I2CSendByte(0x00);
I2CSendStop();
	
I2CSendAddr(0x52,0);
I2CSendByte(0x00);
I2CSendStop();

}

void I2CRead(void)
{
I2CSendAddr(0x52,0);
I2CSendByte(0x00);
I2CSendStop();
	
I2CSendAddr(0x52,1);
i2c_buffer[0]=I2CGetByte(0)	;
i2c_buffer[1]=I2CGetByte(0)	;
i2c_buffer[2]=I2CGetByte(0)	;
i2c_buffer[3]=I2CGetByte(0)	;
i2c_buffer[4]=I2CGetByte(0)	;
i2c_buffer[5]=I2CGetByte(1)	;
I2CSendStop();
}

unsigned char I2Conetime(void){
			 I2CRead();
			 BZ=i2c_buffer[5]&(0X01);
	     BC=i2c_buffer[5]&(0X02);
				if(!BC){
					play_alarm();
				}
				if(!BZ){
					set_pwmA((i2c_buffer[1]-0x7C)-(i2c_buffer[0]-0x80));
					set_pwmB((i2c_buffer[1]-0x7C)+(i2c_buffer[0]-0x80));
					return 1;
				}else if(moviendose==0){//solo si no se mueve
					set_pwmA(0);
					set_pwmB(0);
					return 0;
				}
				
}
