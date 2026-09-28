#ifndef __MyI2C_H
#define __MyI2C_H
void MyI2C_Init(void);
void SCL_W(uint8_t Bitvalue);
void SDA_W(uint8_t Bitvalue);
uint8_t  SDA_R(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_Receive(void);
void MyI2C_SendACK(uint8_t ACK);
uint8_t MyI2C_ReceiveACK(void);

#endif
