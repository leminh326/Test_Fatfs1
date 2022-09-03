#include "flash_driver.h"
extern SPI_HandleTypeDef hspi1;

// ********* Config **********//
void W25q64_Write_Enable()
{
	uint8_t CMD_id = 0x06;
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
	W25q64_Wait_Writing_Finish();
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
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,100);
	HAL_SPI_Receive(&hspi1,reg,1,100);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
}
void W25q64_Wait_Writing_Finish()
{
	uint8_t reg = 0;
	W25q64_Read_Status_Register(&reg,1);
	while( (reg & 0b00000001) == 1)
	{
		W25q64_Read_Status_Register(&reg,1);
	}
}

// ********* Erase **********//
void W25q64_Chip_Erase()
{
	uint8_t CMD_id = 0xC7;
	W25q64_Write_Enable();
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&CMD_id,1,100);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);

	W25q64_Wait_Writing_Finish();
}
void W25q64_Sector_Erase(uint32_t sector_number)
{
	uint8_t CMD_id = 0x20;
	sector_number = sector_number*4096;
	uint8_t CMD[4] = {CMD_id,(sector_number & 0xFF0000) >> 16,(sector_number& 0xFF00) >> 8,sector_number & 0xFF};
	W25q64_Write_Enable();

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,100);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);

	W25q64_Wait_Writing_Finish();
}
//void W25q64_Block32_Erase(uint32_t block_number)
//{
//	uint8_t CMD_id = 0x52;
//	uint8_t CMD[4] = {CMD_id,(block_number & 0xFF0000) >> 16,(block_number& 0xFF00) >> 8,block_number & 0xFF};
//	W25q64_Write_Enable();
//
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1,CMD,4,10);
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
//}
//void W25q64_Block64_Erase(uint32_t block_number)
//{
//	uint8_t CMD_id = 0xD8;
//	uint8_t CMD[4] = {CMD_id,(block_number & 0xFF0000) >> 16,(block_number& 0xFF00) >> 8,block_number & 0xFF};
//	W25q64_Write_Enable();
//
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1,CMD,4,10);
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
//}

// ********* Read **********//
HAL_StatusTypeDef W25q64_Read_Byte(uint8_t *pBuffer,uint32_t Bytes_Address)
{
	HAL_StatusTypeDef read_status = HAL_ERROR;
	uint8_t CMD_id = 0x03;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,100);
	read_status = HAL_SPI_Receive(&hspi1,pBuffer,1,100);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
	return read_status;
}
HAL_StatusTypeDef W25q64_Read_Multi_Bytes(uint8_t *pBuffer,uint32_t Bytes_Address,uint16_t num_to_read)
{
	HAL_StatusTypeDef read_status = HAL_ERROR;
	uint8_t CMD_id = 0x03;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	read_status = HAL_SPI_Receive(&hspi1,pBuffer,num_to_read,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
	return read_status;
}
HAL_StatusTypeDef W25q64_Read_Page_Entire(uint8_t *pBuffer,uint32_t Page_number)
{
	HAL_StatusTypeDef read_status = HAL_ERROR;
	uint8_t CMD_id = 0x03;
	uint32_t Bytes_Address = Page_number *256;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	read_status = HAL_SPI_Receive(&hspi1,pBuffer,256,100);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
	return read_status;
}
HAL_StatusTypeDef W25q64_Read_Sector_Entire(uint8_t *pBuffer,uint32_t Sector_number)
{
	HAL_StatusTypeDef read_status = HAL_ERROR;
	uint8_t CMD_id = 0x03;
	uint32_t Bytes_Address = Sector_number *4096;
	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,CMD,4,10);
	read_status = HAL_SPI_Receive(&hspi1,pBuffer,4096,100);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
	return read_status;
}
//
//void W25q64_Read_Page(uint8_t *pBuffer,uint32_t Page_Number)
//{
//	uint8_t CMD_id = 0x03;
//	uint32_t Bytes_Address = Page_Number*256;
//	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};
//
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1,CMD,4,10);
//	HAL_SPI_Receive(&hspi1,pBuffer,256,10);
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
//}
//
//void W25q64_Read_Sector(uint8_t *pBuffer,uint32_t Sector_Number)
//{
//	uint8_t CMD_id = 0x03;
//	uint32_t Bytes_Address = Sector_Number*4096;
//	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};
//
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1,CMD,4,10);
//	HAL_SPI_Receive(&hspi1,pBuffer,4096,10);
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
//}
//void W25q64_Fast_Read_Byte(uint8_t *pBuffer,uint32_t Bytes_Address)
//{
//	uint8_t CMD_id = 0x0B;
//	uint8_t CMD[5] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF,0};
//
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1,CMD,5,10);
//	HAL_SPI_Receive(&hspi1,pBuffer,1,10);
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
//}
// ********* Write **********//
HAL_StatusTypeDef W25q64_Write_Byte(uint8_t write_data,uint32_t Bytes_Address)
{
	uint8_t CMD_id = 0x02;
	uint8_t CMD[5] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF,write_data};
	HAL_StatusTypeDef write_status = HAL_ERROR;

	W25q64_Write_Enable();

	W25q64_Sector_Erase(Bytes_Address/4096);

	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	write_status = HAL_SPI_Transmit(&hspi1,CMD,5,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);

	W25q64_Wait_Writing_Finish();
	return write_status;
}
HAL_StatusTypeDef W25q64_Write_Page_Entire(uint8_t *write_data,uint32_t Page_number)
{
	uint8_t CMD_id = 0x02;
	uint32_t Bytes_Address = Page_number *256;
	uint8_t CMD[260];
	HAL_StatusTypeDef write_status = HAL_ERROR;

	CMD[0] = CMD_id;
	CMD[1] = (Bytes_Address & 0xFF0000) >> 16;
	CMD[2] = (Bytes_Address& 0xFF00) >> 8;
	CMD[3] = Bytes_Address & 0xFF;
	for (int i = 0; i < 256; ++i){ CMD[i+4] = write_data[i]; }

	W25q64_Write_Enable();
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
	write_status = HAL_SPI_Transmit(&hspi1,CMD,260,10);
	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
	W25q64_Wait_Writing_Finish();

	return write_status;
}
HAL_StatusTypeDef W25q64_Write_Sector_Entire(uint8_t *write_data,uint32_t Sector_number)
{
	uint32_t page = Sector_number*16;
	HAL_StatusTypeDef write_status = HAL_ERROR;

	W25q64_Sector_Erase(Sector_number);
	for (int i = 0; i < 16; ++i)
	{
		write_status = W25q64_Write_Page_Entire( write_data + i*256,page);
		page++;
	}

	return write_status;
}
//void W25q64_Write_Sector(uint8_t *write_data,uint32_t Sector_Number)
//{
//	uint8_t CMD_id = 0x02;
//	uint32_t Bytes_Address = Sector_Number*4096;
//	uint8_t CMD[4] = {CMD_id,(Bytes_Address & 0xFF0000) >> 16,(Bytes_Address& 0xFF00) >> 8,Bytes_Address & 0xFF};
//	W25q64_Write_Enable();
//
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1,CMD,4,10);
//	HAL_SPI_Transmit(&hspi1,write_data,4096,10);
//	HAL_GPIO_WritePin(pin_cs_GPIO_Port,pin_cs_Pin,GPIO_PIN_SET);
//}
