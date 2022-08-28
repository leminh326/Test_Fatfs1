#ifndef _W25QXXCONFIG_H
#define _W25QXXCONFIG_H

#include "main.h"
extern SPI_HandleTypeDef hspi1;

#define		_W25QXX_SPI								hspi1
#define		_W25QXX_CS_GPIO							GPIOB
#define		_W25QXX_CS_PIN							GPIO_PIN_2
#define		_W25QXX_USE_FREERTOS					0
#define		_W25QXX_DEBUG							0
#endif
