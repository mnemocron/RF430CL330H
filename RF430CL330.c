#include "RF430CL330.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"

int RF430_UpdateRegisters(I2C_HandleTypeDef *pTwi, uint8_t devAddress, RF430Registers *reg)
{
	
	if(reg == NULL)
		return 1;                           // ERROR : 1 no input data
	if(devAddress < RF430_ADDRESS)
		return 1;
	
	int result = 0;                         // contains HAL status codes
	static uint8_t regV[2] = {0, 0};        // according to SUM, these variables have to be static
	
	// read 2 bytes (16-bit register) and store them in the control struct
	result = RF430_Read(pTwi, devAddress, RF430_REG_CONTROL, regV, 2);
	if(result) return result;
	reg->regControl = (regV[1]<<8) + regV[0];		// this is the correct order
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_STATUS, regV, 2);
	if(result) return result;
	reg->regStatus= (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_INTERRUPTENABLE, regV, 2);
	if(result) return result;
	reg->regIntEn = (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_INTERRUPTFLAGS, regV, 2);
	if(result) return result;
	reg->regIntFlag = (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_CRCRESULT, regV, 2);
	if(result) return result;
	reg->regCrcRes = (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_CRCLENGTH, regV, 2);
	if(result) return result;
	reg->regCrcLen = (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_CRCSTARTADDRESS, regV, 2);
	if(result) return result;
	reg->regCrcStart = (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_COMWDC, regV, 2);
	if(result) return result;
	reg->regComWdCon = (regV[1]<<8) + regV[0];
	
	result = RF430_Read(pTwi, devAddress, RF430_REG_VERSION, regV, 2);
	if(result) return result;
	reg->regVersion = (regV[1]<<8) + regV[0];
	return result;
	
}

/**
  * @Todo use the proper printf implementation
  */
int RF430_PrintRegisters(UART_HandleTypeDef *serial, RF430Registers *reg)
{
	
	if(reg == NULL)
		return 1;                // ERROR : 1 no input data
	static uint8_t intToAscii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	int result = 0;              // contains sum of all HAL status returns
	
	HAL_UART_Transmit(serial, (uint8_t*)"name\t\t\t\taddress\tvalue\n", 22, 10);
	
	// print the register name followed by the address and the hex value of the register
	// hex conversation : left-shift the four bits to the lowest possible position and mask it with 0xf
	// the array intToAscii[] outputs the correct ascii value
	result += HAL_UART_Transmit(serial, (uint8_t*)"control register\t\t0x", 20, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CONTROL) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CONTROL) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CONTROL) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CONTROL)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regControl) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regControl) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regControl) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regControl)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"status register\t\t0x", 19, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_STATUS) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_STATUS) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_STATUS) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_STATUS)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regStatus) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regStatus) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regStatus) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regStatus)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"interrupt enable\t\t0x", 20, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTENABLE) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTENABLE) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTENABLE) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTENABLE)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntEn) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntEn) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntEn) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntEn)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"interrupt flags\t\t0x", 19, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTFLAGS) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTFLAGS) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTFLAGS) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_INTERRUPTFLAGS)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntFlag) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntFlag) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntFlag) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regIntFlag)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"crc result\t\t\t0x", 15, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCRESULT) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCRESULT) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCRESULT) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCRESULT)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcRes) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcRes) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcRes) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcRes)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"crc length\t\t\t0x", 15, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCLENGTH) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCLENGTH) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCLENGTH) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCLENGTH)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcLen) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcLen) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcLen) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcLen)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"crc start address\t\t0x", 21, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCSTARTADDRESS) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCSTARTADDRESS) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCSTARTADDRESS) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_CRCSTARTADDRESS)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcStart) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcStart) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcStart) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regCrcStart)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"com watchdog\t\t0x", 16, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_COMWDC) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_COMWDC) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_COMWDC) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_COMWDC)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regComWdCon) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regComWdCon) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regComWdCon) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regComWdCon)      ) & 0xf ], 1, 1);
	HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	result += HAL_UART_Transmit(serial, (uint8_t*)"version\t\t\t0x", 12, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_VERSION) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_VERSION) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_VERSION) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((RF430_REG_VERSION)      ) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)"\t0x", 3, 10);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regVersion) >> 12) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regVersion) >>  8) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regVersion) >>  4) & 0xf ], 1, 1);
	result += HAL_UART_Transmit(serial, (uint8_t*)&intToAscii[ ((reg->regVersion)      ) & 0xf ], 1, 1);
	// HAL_UART_Transmit(serial, (uint8_t*)"\n", 1, 1);
	
	return result;
}

int RF430_Write_1(I2C_HandleTypeDef *pTwi, uint8_t devAddress, uint16_t regAddress, uint8_t* pData)
{
	
	if(pData == NULL)                     // check if data is not empty
		return 1;                         // ERROR : 1 no input data
	
	/*
	uint8_t nData[3];		              // temporary output array as in write_64
	nData[0] = ((regAddress>>8)&255);     // split 16bit address into 8bit parts
	nData[1] = (regAddress&255), 0;
	nData[2] = *pData;
	*/

	// 3 bytes of data, 10ms timeout
	if(HAL_I2C_Master_Transmit(pTwi, devAddress, (uint8_t[]) { ((regAddress>>8)&255), (regAddress&255), *pData }, 3, 10) == HAL_ERROR){
	// if(HAL_I2C_Master_Transmit(pTwi, devAddress, nData, 3, 10) == HAL_ERROR){
		return 2;                        // ERROR : 2 transmission error (HAL)
	}
	return 0;
	
}

int RF430_Write_64(I2C_HandleTypeDef *pTwi, uint8_t devAddress, uint16_t regAddress, uint8_t* pData, uint8_t size)
{
	
	if(pData == NULL)                    // check if data is not empty
		return 1;                        // ERROR : 1
	
	uint8_t nData [66];                  // temporary output array // 64 data bytes + 2 register address bytes
	nData[0] = ( ( regAddress >> 8) &255 );     // split 16bit address into 8bit parts
	nData[1] = ( ( regAddress     ) &255 );
	for(int i=0; i<size; i++)
		nData[i+2] = *pData++;                  // prepare data in new array
	
	if(HAL_I2C_Master_Transmit(pTwi, devAddress, nData, (2+size), (64)) == HAL_ERROR){
		return 2;                               // ERROR : 2 transmission error (HAL)
	}
	return 0;
}

int RF430_Read(I2C_HandleTypeDef *pTwi, uint8_t devAddress, uint16_t regAddress, uint8_t *pData, uint16_t size)
{
	
	if((size < 1) || (pData == NULL))           // check if data is not empty
		return 1;      // ERROR : 1 no input data
	// request read from RF430 chip
	if(HAL_I2C_Master_Transmit(pTwi, devAddress, (uint8_t[]) { ((regAddress>>8)&255), (regAddress&255) }, 2, 10) == HAL_ERROR){
		return 2;      // ERROR : 2 transmission error (HAL)
	}
	// read the values
	if(HAL_I2C_Master_Receive(pTwi, devAddress, pData, size, 10) == HAL_ERROR){
		return 3;      // ERROR : 3 receive error (HAL)
	}
	return 0;
	
}
