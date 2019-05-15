
/* Includes ------------------------------------------------------------------*/

#include "cabeceras.h"

/* Definición de los estados */
#define SCREEN_WELCOME      0
#define SCREEN_WELCOME_WAIT 1
#define SCREEN_TEST_MOTORES         2
#define SCREEN_TEST_MOTORES_WAIT    3
#define SCREEN_COMANDO_PANTALLA      4
#define SCREEN_COMANDO_PANTALLA_WAIT  5

/* Variable que almacena el estado */
uint8_t screenState;   
char comandos[25];
 char i2c_buffer[6];
 char distancia_text[12];
unsigned char y;
char dist_izq_old[12];
char dist_der_old[12];
char vel_izq_old[12];
char vel_der_old[12];
char temp_old[12];
char volt_old[12];
char battery_lvl_old[12];

int  velo_A;
int  velo_B;
int  AUXILIAR;
char AUXILIAR_char[3];
float alrma_bateria_humbral=20;//////////////////////////////////////////////
/* Estructura que define una zona de la pantalla */
struct t_screenZone
{
   uint16_t x;         
	uint16_t y;
	uint16_t size_x;
	uint16_t size_y;
	uint8_t  pressed;
};

/* Definicion de las diferentes zonas de la pantalla */
struct t_screenZone zone_7 = {15, 20,  85,  20, 0}; /* distancia motor izq                */
struct t_screenZone zone_8 = {144, 20, 85,  20, 0}; /* distancia motor derech                */

struct t_screenZone zone_11 = {15, 50,  85,  20, 0}; /* vel motor derecha               */
struct t_screenZone zone_12 = {144, 50,  85,  20, 0}; /* vel motor izq                */

struct t_screenZone zone_0 = { 15,  75, 215, 70, 0}; /* Botón "robot espia"   */
struct t_screenZone zone_1 = { 15,  150, 215, 30, 0}; /* Botón "SUCESO"      */
struct t_screenZone zone_2 = { 20,  180, 200,  20, 0}; /* Botón "COMANDOS"    */

struct t_screenZone zone_3 = { 15,  150+10, 30, 30, 0}; /* Botón "VEL_IZQ+"          */
struct t_screenZone zone_4 = { 15, 190+20,  30,  30, 0}; /* Botón  VEL_IZQ-           */
struct t_screenZone zone_5 = {200, 150+10,  30,  30, 0}; /* Botón  VEL_DER +         */
struct t_screenZone zone_13 = {200, 190+20,  30,  30, 0}; /* Botón  VEL_DER  -          */

struct t_screenZone zone_14 = { 15+40,  150+10, 30, 30, 0}; /* Botón "L"          */
struct t_screenZone zone_15=  { 15+40, 190+20,  30,  30, 0}; /* Botón  R           */
struct t_screenZone zone_16=  { 15+80, 150+10,  30,  30, 0}; /* S-           */
struct t_screenZone zone_17 = {15+80, 190+20,  30,  30, 0}; /* Botón  VEL_DER +         */
struct t_screenZone zone_18 = {15+120, 150+10,  30,  30, 0}; /* Botón  VEL_DER  -          */
struct t_screenZone zone_19 = {15+120, 190+20,  60,  30, 0}; /* Botón  VEL_DER  -          */

struct t_screenZone zone_6 = {20, 280,  200,  30, 0}; /* Messages                */

struct t_screenZone zone_9 = {15, 250,  85,  20, 0};  /* volt               */
struct t_screenZone zone_10 = {144, 250,  85,  20, 0}; /* temp                */
struct t_screenZone zone_20 = {15+40, 250,  85,  20, 0}; /* Botón  VEL_DER  -          */

/* Flag que indica si se detecta una pulsación válida */
uint8_t pressedTouchPanel = 0;

uint8_t messageText[25+1] = {"Esto es una prueba 1000"};

void  screenMessageIP(void);
void  screenMessage(void);


extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr


/*******************************************************************************
* Function Name  : squareButton
* Description    : Dibuja un cuadrado en las coordenadas especificadas colocando 
*                  un texto en el centro del recuadro
* Input          : zone: zone struct
*                  text: texto a representar en el cuadro
*                  textColor: color del texto
*                  lineColor: color de la línea
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void squareButton(struct t_screenZone* zone, char * text, uint16_t textColor, uint16_t lineColor)
{
   LCD_DrawLine( zone->x, zone->y, zone->x + zone->size_x, zone->y, lineColor);
   LCD_DrawLine( zone->x, zone->y, zone->x, zone->y + zone->size_y, lineColor);
   LCD_DrawLine( zone->x, zone->y + zone->size_y, zone->x + zone->size_x, zone->y + zone->size_y, lineColor);
   LCD_DrawLine( zone->x + zone->size_x, zone->y, zone->x + zone->size_x, zone->y + zone->size_y, lineColor);
	GUI_Text(zone->x + zone->size_x/2 - (strlen(text)/2)*8, zone->y + zone->size_y/2 - 8,
            (uint8_t*) text, textColor, Black);	
}

/*******************************************************************************
* Function Name  : screenWelcome
* Description    : Visualiza la pantalla de bienveida
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenWelcome(void)
{
		squareButton(&zone_0, "Robot espia", White, Red);
	  squareButton(&zone_1, "Esperando Ordenes", Yellow, Black);
      screenMessageIP();
}

/*******************************************************************************
* Function Name  : screenMain
* Description    : Visualiza la pantalla principal
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenMain(void)
{
   squareButton(&zone_0, "Test motores", White, White);
   squareButton(&zone_3, "+", White, White);
   squareButton(&zone_4, "-", White, White);
	 squareButton(&zone_6, "STOP", White, White);
	 squareButton(&zone_5, "+", White, White);
	 squareButton(&zone_13, "-", White, White);
   //screenMessage();
}

/*******************************************************************************
* Function Name  : screenToggle
* Description    : Visualiza la pantalla secundaria
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenToggle()
{
   squareButton(&zone_0, "Control con pantalla", White, Green);
   squareButton(&zone_14, "F", White, White);
   squareButton(&zone_15, "B", White, White);
	 squareButton(&zone_6, "X... ", White, Green);
	 squareButton(&zone_17, "Y", White, Green);
	 squareButton(&zone_18,"C", White, Red);
	 squareButton(&zone_3, "L", White, White);
	 squareButton(&zone_16,"R", White, White);
	 squareButton(&zone_4,"P", White, Green);

	 squareButton(&zone_19,"00", White, Black);
	 squareButton(&zone_5, "+", White, White);
	 squareButton(&zone_13, "-", White, White);
//   screenMessage();
}

/*******************************************************************************
* Function Name  : screenMessage
* Description    : Visualiza la pantalla de mensajes
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenMessage(void)
{
   squareButton(&zone_6, (char*)messageText, Red, White);
}
/*******************************************************************************
* Function Name  : screenMessageIP
* Description    : Visualiza la pantalla de mensajes
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenMessageIP(void)
{     
   sprintf((char *)messageText,"   IP: %d.%d.%d.%d  ", MY_IP[0], MY_IP[1],
                                                       MY_IP[2], MY_IP[3]);
   squareButton(&zone_6, (char*)messageText, Red, White);
}
/*******************************************************************************
* Function Name  : checkTouchPanel
* Description    : Lee el TouchPanel y almacena las coordenadas si detecta pulsación
* Input          : None
* Output         : Modifica pressedTouchPanel
*                    0 - si no se detecta pulsación
*                    1 - si se detecta pulsación
*                        En este caso se actualizan las coordinadas en la estructura display
* Return         : None
* Attention		  : None
*******************************************************************************/
void checkTouchPanel(void)
{
	Coordinate* coord;
	
	coord = Read_Ads7846();
	
	if (coord > 0) {
	  getDisplayPoint(&display, coord, &matrix );
     pressedTouchPanel = 1;
   }   
   else
     pressedTouchPanel = 0;
}

/*******************************************************************************
* Function Name  : zonePressed
* Description    : Detecta si se ha producido una pulsación en una zona contreta
* Input          : zone: Estructura con la información de la zona
* Output         : Modifica zone->pressed
*                    0 - si no se detecta pulsación en la zona
*                    1 - si se detecta pulsación en la zona
* Return         : 0 - si no se detecta pulsación en la zona
*                  1 - si se detecta pulsación en la zona
* Attention		  : None
*******************************************************************************/
int8_t zonePressed(struct t_screenZone* zone)
{
	if (pressedTouchPanel == 1) {

		if ((display.x > zone->x) && (display.x < zone->x + zone->size_x) && 
			  (display.y > zone->y) && (display.y < zone->y + zone->size_y))
      {
         zone->pressed = 1;
		   return 1;
      }   
	}
   
	zone->pressed = 0;
	return 0;
}

/*******************************************************************************
* Function Name  : zoneNewPressed
* Description    : Detecta si se ha producido el flanco de una nueva pulsación en 
*                  una zona contreta
* Input          : zone: Estructura con la información de la zona
* Output         : Modifica zone->pressed
*                    0 - si no se detecta pulsación en la zona
*                    1 - si se detecta pulsación en la zona
* Return         : 0 - si no se detecta nueva pulsación en la zona
*                  1 - si se detecta una nueva pulsación en la zona
* Attention		  : None
*******************************************************************************/
int8_t zoneNewPressed(struct t_screenZone* zone)
{
	if (pressedTouchPanel == 1) {

		if ((display.x > zone->x) && (display.x < zone->x + zone->size_x) && 
			  (display.y > zone->y) && (display.y < zone->y + zone->size_y))
      {
         if (zone->pressed == 0)
         {   
            zone->pressed = 1;
            return 1;
         }
		   return 0;
      }
	}

   zone->pressed = 0;
	return 0;
}


/*******************************************************************************
* Function Name  : fillRect
* Description    : Rellena de un color determinado el interior de una zona 
* Input          : zone: Estructura con la información de la zona
*                  color: color de relleno
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void fillRect(struct t_screenZone* zone, uint16_t color)
{
	uint16_t i;
	
	for (i = zone->y+1; i < zone->y + zone->size_y-1; i ++) {
		LCD_DrawLine(zone->x + 1, i, zone->x + zone->size_x -1, i, color);
	}
}


/*******************************************************************************
* Function Name  : updateLEDs
* Description    : Actualiza en función de las variables led1 y led2 la visualización 
*                  de los LEDs de la Mini-DK2 y el color de los cuadros en pantalla
*                  relacionados. 
* Input          : zone: Estructura con la información de la zona
*                  color: color de relleno
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/

/*******************************************************************************
* Function Name  : initScreenStateMachine
* Description    : Inicializa la máquina de estados al primer estado. 
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void 	initScreenStateMachine(void)
{
	screenState = SCREEN_WELCOME;
}





	void onetimeWEBruedas(void){
 unsigned int val=0;
		
		if(comandos[j]==0x0D){//terminado
								for(j=0;j<25;j++){//clear buffer
						    comandos[j]=0x00;
				        }
			          j=0;
			return;
		}
		
	if(comandos[0] && !moviendose){					 	// HAY UNA CADENA NUEVA Y EL ROBOT NO SE MUEVE

						switch ( comandos[j] )
						{
								case 'F'://Fxx: Avanza xx centímetros en línea recta (Forward)
										j++;
										val =10*(comandos[j]- '0');
									  j++;
										val =val+ (comandos[j]- '0');
										moveLinecm(val);
										//while(moviendose!=0);	
										j++;
										break;
								case 'B'://Bxx: Retrocede xx centímetros en línea recta (Backward)
										j++;
								    val =10*(comandos[j]- '0');
									  j++;
										val =val+ (comandos[j]- '0');
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

//char dist_izq_old[12];
//char dist_der_old[12];
//char vel_izq_old[12];
//char vel_der_old[12];
//char temp_old[12];
//char volt_old[12];
void imprime_distancia_motores(void){

		snprintf(distancia_text, sizeof(distancia_text), "%.2f", (double)(LPC_TIM1->TC)/0x19);
	  strcat(distancia_text, "cm");
	 
	  //fillRect(&zone_7, Black);
	  squareButton(&zone_7, dist_izq_old, Black, Black);
		squareButton(&zone_7, distancia_text, Red, Black);
	  strcpy(dist_izq_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array


		snprintf(distancia_text, sizeof(distancia_text), "%.2f", (double)(LPC_TIM2->TC)/0x19);
	  strcat(distancia_text, "cm");
    //fillRect(&zone_8, Black);
    squareButton(&zone_8, dist_der_old, Black, Black);
		squareButton(&zone_8, distancia_text, Red, Black);
    strcpy(dist_der_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array

}


void imprime_temperatura_voltaje(void){
	  snprintf(distancia_text, sizeof(distancia_text), "%.2f", voltios);
	  strcat(distancia_text, "V");
	 // fillRect(&zone_9, Black);
	  squareButton(&zone_9, volt_old, Black, Black);
		squareButton(&zone_9, distancia_text, Red, Black);
	  strcpy(volt_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array

		snprintf(distancia_text, sizeof(distancia_text), "%.0f", battery_lvl);
	  strcat(distancia_text, "%");
	 // fillRect(&zone_9, Black);
	  squareButton(&zone_20, battery_lvl_old, Black, Black);
					if(battery_lvl>80){//colores//////////////////////////////////////
						squareButton(&zone_20, distancia_text, Green, Black);
					}else if(battery_lvl>40){
						squareButton(&zone_20, distancia_text, Yellow, Black);
					}else{
						squareButton(&zone_20, distancia_text, Red, Black);
					}
	  strcpy(battery_lvl_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array
		
			if(battery_lvl<alrma_bateria_humbral && !countdown100ms_alarm){
				countdown100ms_alarm=50;
				play_alarm();
			}


		snprintf(distancia_text, sizeof(distancia_text), "%.1f", temperatura);
	  strcat(distancia_text, "C");
   // fillRect(&zone_10, Black);
	  squareButton(&zone_10, temp_old, Black, Black);
		squareButton(&zone_10, distancia_text, Red, Black);
    strcpy(temp_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array
}

void imprime_velocidad_motores(void){
    vel_izq=(abs(LPC_TIM1->TC-OLD_TC1)*10)/0x19;
		snprintf(distancia_text, sizeof(distancia_text), "%.0f", vel_izq);
	  strcat(distancia_text, "cm/s");
	  //fillRect(&zone_11, Black);
		squareButton(&zone_11, vel_izq_old, Black, Black);
		squareButton(&zone_11, distancia_text, Yellow, Black);
	    strcpy(vel_izq_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array


    vel_der=(abs(LPC_TIM2->TC-OLD_TC2)*10)/0x19;
		snprintf(distancia_text, sizeof(distancia_text), "%.0f", vel_der);
	  strcat(distancia_text, "cm/s");
    //fillRect(&zone_12, Black);
		squareButton(&zone_12, vel_der_old, Black, Black);
		squareButton(&zone_12, distancia_text, Yellow, Black);
    strcpy(vel_der_old,distancia_text);
	  for(y=0;y<12;y++) {distancia_text[y]=0x00;}//erase array
}

/*******************************************************************************
* Function Name  : screenStateMachine
* Description    : Máquina de estados de la aplicación. 
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenStateMachine(void)
{ 
	/* Lee posible pulsación del TouchPanel  */
   checkTouchPanel();
	switch (screenState)
	{
      case SCREEN_WELCOME:
      LCD_Clear(Black);
			screenWelcome();
			screenState = SCREEN_WELCOME_WAIT;
			break;
			 
		case SCREEN_WELCOME_WAIT://///////////////////////
         if (zonePressed(&zone_0)) {
					 timeout=100;
			   screenState = SCREEN_TEST_MOTORES;
			}else{
				   if(I2Conetime() && !BUSY_RECORDING && !moviendose){
						 fillRect(&zone_1, Black);
						 squareButton(&zone_1, "I2C", Yellow, Black);//i2c
					 }else if(comandos[0] && !BUSY_RECORDING){
						 fillRect(&zone_1, Black);
						 squareButton(&zone_1, "WEB", Yellow, Black);//
						 fillRect(&zone_2, Black);
						// squareButton(&zone_2, comandos, Yellow, Black);//
						 onetimeWEBruedas();//web
					 }else if(rx_completa_2 && !BUSY_RECORDING){
						fillRect(&zone_1, Black);
						 squareButton(&zone_1, "BLUETOOTH", Yellow, Black);//
						 	 onetimeBLUETOOTHruedas();//bluetooth
						 fillRect(&zone_2, Black);
						// squareButton(&zone_2, buffer_2, Yellow, Black);//
					 }else if(rx_completa && !BUSY_RECORDING){
						 fillRect(&zone_1, Black);
						 squareButton(&zone_1, "PC", Yellow, Black);//
						 	 onetimeUARTruedas();//pc
						 fillRect(&zone_2, Black);
						// squareButton(&zone_2, buffer, Yellow, Black);//
					 }
				 }					 
			break;
			 
		case SCREEN_TEST_MOTORES:
			LCD_Clear(Black);
			screenMain();		
			screenState = SCREEN_TEST_MOTORES_WAIT;
			break;
			 
		case SCREEN_TEST_MOTORES_WAIT:
						if (zoneNewPressed(&zone_0)) {
				screenState = SCREEN_COMANDO_PANTALLA ;
			}else if(!timeout){
				screenState = SCREEN_WELCOME ;//timeout!
			}else if (zonePressed(&zone_3)) {
				timeout=100;
				   velo_B++;
			}else if (zonePressed(&zone_4)) {
				timeout=100;
				   velo_B--;
			} else if (zonePressed(&zone_5)) {
				timeout=100;
				  velo_A++;
			} else if (zonePressed(&zone_13)) {
				timeout=100;
				  velo_A--;
			} else if (zonePressed(&zone_6)) {
				timeout=100;
				  velo_A=0;
				  velo_B=0;

			}
			if(velo_A>100){velo_A=100;}
			if(velo_A<-100){velo_A=-100;}
			if(velo_B>100){velo_B=100;}
			if(velo_B<-100){velo_B=-100;}
			set_pwmA(  velo_A);//
			set_pwmB(  velo_B);//

		   break;
					
		case SCREEN_COMANDO_PANTALLA :
			LCD_Clear(Black);
			screenToggle();			
			screenState = SCREEN_COMANDO_PANTALLA_WAIT;
		  buffer_pantalla[0]='X';
		  rx_buffer_pantalla=0;
			j=0;
			break;
			 
		case SCREEN_COMANDO_PANTALLA_WAIT:
			if(!BUSY_RECORDING){
			squareButton(&zone_19, AUXILIAR_char, White, Green);
		  fillRect(&zone_6, Black);
		  squareButton(&zone_6, buffer_pantalla, White, Black);
			}
			if (zoneNewPressed(&zone_0) || !timeout) {
			   screenState = SCREEN_WELCOME;
			}else  if (zoneNewPressed(&zone_5)) {//+
				timeout=100;
					AUXILIAR++;
				if(AUXILIAR>99){AUXILIAR=99;}
				if(AUXILIAR<0){AUXILIAR=0;}
				 snprintf(AUXILIAR_char, sizeof(AUXILIAR_char), "%02d", AUXILIAR);
			}else if (zoneNewPressed(&zone_13)) {//-
				timeout=100;
          AUXILIAR--;
				if(AUXILIAR>99){AUXILIAR=99;}
				if(AUXILIAR<0){AUXILIAR=0;}
				 snprintf(AUXILIAR_char, sizeof(AUXILIAR_char), "%02d", AUXILIAR);
			}else if (zoneNewPressed(&zone_14)&& !BUSY_RECORDING) {//f
				timeout=100;
				buffer_pantalla[j+1]='F';
				strcat(buffer_pantalla,AUXILIAR_char);
				j+=3;
			}else if (zoneNewPressed(&zone_15)&& !BUSY_RECORDING) {//b
				timeout=100;
				buffer_pantalla[j+1]='B';
				strcat(buffer_pantalla,AUXILIAR_char);
				j+=3;
			}else if (zoneNewPressed(&zone_16)&& !BUSY_RECORDING) {//R
				timeout=100;
				buffer_pantalla[j+1]='R';
				j++;
			}else if (zoneNewPressed(&zone_3)&& !BUSY_RECORDING) {//L
				timeout=100;
				buffer_pantalla[j+1]='L';
				j++;
			}else if (zoneNewPressed(&zone_4)) {//P
				timeout=100;
				buffer_pantalla[j+1]='P';
				strcat(buffer_pantalla,AUXILIAR_char);
				j+=3;
			}else if (zoneNewPressed(&zone_18)&& !BUSY_RECORDING) {//C
				timeout=100;
				buffer_pantalla[j+1]='C';
				j++;
			}else if (zoneNewPressed(&zone_17)&& !BUSY_RECORDING) {//Y
				timeout=100;
				buffer_pantalla[j+1]='Y';
				j++;
			}else if (zoneNewPressed(&zone_6)&& !BUSY_RECORDING) {//SEND!
				timeout=100;
				if(j==0){
					buffer_pantalla[0]='X';
				}else{
				buffer_pantalla[j+1]=0x0D;
        rx_buffer_pantalla=1;
				j=0;
				}
				
			}
      onetimePANTALLAruedas();
			break;

		default:
			break;
	}


}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
