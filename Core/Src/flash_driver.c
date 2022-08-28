#include "flash_driver.h"

extern SPI_HandleTypeDef hspi1;

void W25q64_Write_Enable()
{
	uint8_t CMD_id = 0x06;
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Read_JEDEC_ID(uint8_t* ID)
{
	uint8_t CMD_id = 0x9F;
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,10);
	HAL_SPI_Receive(&hspi1,ID,3,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Read_Status_Register(uint8_t *reg,uint8_t SelectStatusRegister_1_2_3)
{
	uint8_t CMD_id = 0x00;
	if	    (SelectStatusRegister_1_2_3==1){CMD_id = 0x05;}
	else if (SelectStatusRegister_1_2_3==2){CMD_id = 0x35;}
	else								   {CMD_id = 0x15;}

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,10);
	HAL_SPI_Receive(&hspi1,reg,1,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Sector_Erase(uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x20;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};
	W25q64_Write_Enable();

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Block32_Erase(uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x52;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};
	W25q64_Write_Enable();

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Block64_Erase(uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0xD8;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};
	W25q64_Write_Enable();

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Chip_Erase(uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0xC7;
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Read_Byte(uint8_t *pBuffer,uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x03;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	HAL_SPI_Receive(&hspi1,pBuffer,1,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Fast_Read_Byte(uint8_t *pBuffer,uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x0B;
	uint8_t CMD[5] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF,0};

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,5,10);
	HAL_SPI_Receive(&hspi1,pBuffer,1,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Write_Byte(uint8_t write_data,uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x02;
	uint8_t CMD[5] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF,write_data};
	W25q64_Write_Enable();

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,5,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Write_Page(uint8_t write_data,uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x02;
	uint8_t CMD[5] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF,write_data};
	W25q64_Write_Enable();

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,5,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
