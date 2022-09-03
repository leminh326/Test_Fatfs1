#ifndef FLASH_DRIVER_H_
#define FLASH_DRIVER_H_

#include "main.h"

// Config
void W25q64_Write_Enable();
void W25q64_Read_JEDEC_ID(uint8_t* ID);
void W25q64_Read_Status_Register(uint8_t *reg,uint8_t SelectStatusRegister_1_2_3);
void W25q64_Wait_Writing_Finish();

// Erase
void W25q64_Sector_Erase(uint32_t Bytes_Address);
//void W25q64_Block32_Erase(uint32_t Bytes_Address);
//void W25q64_Block64_Erase(uint32_t Bytes_Address);
void W25q64_Chip_Erase();

// Read
HAL_StatusTypeDef W25q64_Read_Byte(uint8_t *pBuffer,uint32_t Bytes_Address);
HAL_StatusTypeDef W25q64_Read_Sector_Entire(uint8_t *pBuffer,uint32_t Sector_number);
//void W25q64_Fast_Read_Byte(uint8_t *pBuffer,uint32_t Bytes_Address);
//void W25q64_Read_Multi_Bytes(uint8_t *pBuffer,uint32_t Bytes_Address,uint16_t num_to_read);
//void W25q64_Read_Page(uint8_t *pBuffer,uint32_t Page_Number);
//void W25q64_Read_Sector(uint8_t *pBuffer,uint32_t Sector_Number);

// Write
HAL_StatusTypeDef W25q64_Write_Byte(uint8_t write_data,uint32_t Bytes_Address);
HAL_StatusTypeDef W25q64_Write_Sector_Entire(uint8_t *write_data,uint32_t Sector_number);
HAL_StatusTypeDef W25q64_Write_Page_Entire(uint8_t *write_data,uint32_t Page_number);

#endif
