void I2Cdelay(void);
void pulso_SCL(void);
void I2CSendByte(unsigned char byte);
void I2CSendAddr(unsigned char addr, unsigned char rw);
unsigned char I2CGetByte(unsigned char ACK)	;
void I2CSendStop(void);
void I2CConfig(void);
void I2CRead(void);
unsigned char I2Conetime(void);
