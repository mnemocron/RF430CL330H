/**
  ******************************************************************************
  * @file    RF430CL330.h
  * @author  Simon Burkhardt
  * @version V1.0
  * @date    15.04.2016
  * @brief   Basic library to access registers on an RF430CL330 NFC chip
  * @details This library contains definitions with the register addresses
             as well as funtions to read or write the registers of th rf430
  ******************************************************************************
  */

#ifndef STM32F0XX_H
#include "stm32f0xx_hal.h"
#endif

#ifndef STM32F0XX_HAL_I2C_H
#include "stm32f0xx_hal_i2c.h"
#endif

#ifndef RF430CL330_H
#define RF430CL330_H

/**
  * @defgroup RF430 RF430CL330 
  * @{
  * @note datasheet p.13 (section 5.4)
  */

/*
RF430 User Address Map
-----------------------------------------
Address     | Size  | Description       |
-----------------------------------------
0xFFFE      | 2B    | Control Register  |
0xFFFC      | 2B    | Status Register   |
0xFFFA      | 2B    | Interrupt Enable  |
0xFFF8      | 2B    | Interrupt Flags   |
0xFFF6      | 2B    | CRC Result        |
0xFFF4      | 2B    | CRC Length        |
0xFFF2      | 2B    | CRC Start Address |
0xFFF0      | 2B    | Comm WD Ctrl Reg  |
-----------------------------------------
0x0000 -    | 2kB   | NDEF App Memory   |
  0x0BFF    |       |                   |
 */
#define RF430_REG_CONTROL               0xFFFE
#define RF430_REG_STATUS                0xFFFC
#define RF430_REG_INTERRUPTENABLE       0xFFFA
#define RF430_REG_INTERRUPTFLAGS        0xFFF8
#define RF430_REG_CRCRESULT             0xFFF6
#define RF430_REG_CRCLENGTH             0xFFF4
#define RF430_REG_CRCSTARTADDRESS       0xFFF2
#define RF430_REG_COMWDC                0xFFF0
#define RF430_REG_VERSION               0xFFEE

#define RF430_NDEF_START                0x0000
#define RF430_NDEF_END                  0x0BFF

/**
  * @brief default address of RF430 chip
  * @note datasheet p.14 (section 5.5)
  */
#define RF430_ADDRESS                   0x50      // Datasheet p.14 section 5.5

/**
  * @note datasheet p.23 ff. (section 5.7)
  */

//CONTROL_REG bits
#define RF430_BIT_SW_RESET                    0x0001
#define RF430_BIT_RF_ENABLE                   0x0002
#define RF430_BIT_INT_ENABLE                  0x0004
#define RF430_BIT_INTO_HIGH                   0x0008
#define RF430_BIT_INTO_DRIVE                  0x0010
#define RF430_BIT_BIP8_ENABLE                 0x0020
#define RF430_BIT_STANDBY_ENABLE              0x0040
#define RF430_BIT_TEST430_ENABLE              0x0080
// STATUS_REG bits
#define RF430_BIT_READY                       0x0001
#define RF430_BIT_CRC_ACTIVE                  0x0002
#define RF430_BIT_RF_BUSY                     0x0004
// INT_ENABLE_REG bits
#define RF430_BIT_EOR_INT_ENABLE              0x0002
#define RF430_BIT_EOW_INT_ENABLE              0x0004
#define RF430_BIT_CRC_INT_ENABLE              0x0008
#define RF430_BIT_BIP8_ERROR_INT_ENABLE       0x0010
#define RF430_BIT_NDEF_ERROR_INT_ENABLE       0x0020
#define RF430_BIT_GENERIC_ERROR_INT_ENABLE    0x0080
// INT_FLAG_REG bits
#define RF430_BIT_EOR_INT_FLAG                0x0002
#define RF430_BIT_EOW_INT_FLAG                0x0004
#define RF430_BIT_CRC_INT_FLAG                0x0008
#define RF430_BIT_BIP8_ERROR_INT_FLAG         0x0010
#define RF430_BIT_NDEF_ERROR_INT_FLAG         0x0020
#define RF430_BIT_GENERIC_ERROR_INT_FLAG      0x0080

/**
	* @author <a href="www.elecfreaks.com">elecfreaks</a>
	* @note <a href="http://www.elecfreaks.com/wiki/index.php?title=Dynamic_NFC_Tag">source</a>
	* @copyright Copyright &copy; 2014-2024 elecfreaks. Licensed under GNU LGPL v2.1
	*/
#define RF430_DEFAULT_DATA                                                              \
{                                                                                       \
/*NDEF Tag Application Name*/                                                           \
0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01,                                               \
                                                                                        \
/*Capability Container ID*/                                                             \
0xE1, 0x03,                                                                             \
0x00, 0x0F, /* CCLEN */                                                                 \
0x20,       /* Mapping version 2.0 */                                                   \
0x00, 0x3B, /* MLe (49 bytes); Maximum R-APDU data size */                              \
0x00, 0x34, /* MLc (52 bytes); Maximum C-APDU data size */                              \
0x04,       /* Tag, File Control TLV (4 = NDEF file) */                                 \
0x06,       /* Length, File Control TLV (6 = 6 bytes of data for this tag) */           \
0xE1, 0x04, /* File Identifier */                                                       \
0x0B, 0xDF, /* Max NDEF size (3037 bytes of useable memory) */                          \
0x00,       /* NDEF file read access condition, read access without any security */     \
0x00,       /* NDEF file write access condition; write access without any security */   \
                                                                                        \
/* NDEF File ID */                                                                      \
0xE1, 0x04,                                                                             \
                                                                                        \
/* NDEF File for Hello World  (48 bytes total length) */                                \
0x00, 0x14, /* NLEN; NDEF length (3 byte long message) */                               \
0xD1, 0x01, 0x10,                                                                       \
0x54, /* T = text */                                                                    \
0x02,                                                                                   \
0x65, 0x6E, /* 'e', 'n', */                                                             \
                                                                                        \
/* 'Hello, world!' NDEF data; Empty NDEF message, length should match NLEN*/            \
0x48, 0x65, 0x6C, 0x6C, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21            \
}

/**
	*	@brief control struct containing 16-bit variables of the rf430cl330 registers
	*/
typedef struct {
	uint16_t regControl;		// 0xfffe
	uint16_t regStatus;			// 0xfffc
	uint16_t regIntEn;			// 0xfffa
	uint16_t regIntFlag;		// 0xfff8
	uint16_t regCrcRes;			// 0xfff6
	uint16_t regCrcLen;			// 0xfff4
	uint16_t regCrcStart;		// 0xfff2
	uint16_t regComWdCon;		// 0xfff0
	uint16_t regVersion;		// 0xffee
} RF430Registers;

/**
  * @brief updates the values in the control struct to the values on the rf430cl330
  * @details reads the registers of the rf430cl330 chip on the i2c address provided,
							and stores the values in the provided control struct
	* @param pTwi I2C_HandleTypeDef* pointer to I2C instance
  * @param reg RF430Registers* control struct to save the values
  * @param devAddress I2C address of the RF430 device
  * @return int 0 if update was successful, error code if an error occured
  */
int RF430_UpdateRegisters(I2C_HandleTypeDef *pTwi, uint8_t devAddress, RF430Registers *reg);

/**
  * @brief prints out the register from the control struct
  * @details sends the registers and their values over the uart port
	* @param serial UART_HandleTypeDef* pointer to I2C instance
  * @param reg RF430Registers* control struct to read values from
  * @return int 0 if update was successful, error code if an error occured
  */
int RF430_PrintRegisters(UART_HandleTypeDef *serial, RF430Registers *reg);

/**
  * @brief writes one byte of data to a register of the RF430 memory
  * @details writes one bytes of data to the specified register address
  * @param pTwi I2C_HandleTypeDef* pointer to I2C instance
  * @param devAddress I2C address of the RF430 device
  * @param regAddress 16 bit address of the register to write to
  * @param pData pointer to the data byte
  * @return int 0 if write was successful, error code if an error occured
  */
int RF430_Write_1(I2C_HandleTypeDef *pTwi, uint8_t devAddress, uint16_t regAddress, uint8_t* pData);

/**
  * @brief writes up to 64 bytes of data to the registers of the RF430 memory
  * @param pTwi I2C_HandleTypeDef* pointer to I2C instance
  * @param devAddress I2C address of the RF430 device
  * @param regAddress 16 bit address of the register to write to
  * @param pData pointer to the data byte
  * @param size in bytes of the data
  * @return int 0 if write was successful, error code if an error occured
  */
int RF430_Write_64(I2C_HandleTypeDef *pTwi, uint8_t devAddress, uint16_t regAddress, uint8_t* pData, uint8_t size);

/**
  * @brief reads a specified number of bytes from the RF430 memory
  * @param pTwi I2C_HandleTypeDef* pointer to I2C instance
  * @param devAddress I2C address of the RF430 device
  * @param regAddress 16 bit start address of the register(s) to read from
  * @param pData pointer to the array where the read values shall be written to
  * @param size number of bytes to read 
  * @return int 0 if read was successful, error code if an error occured
  */
int RF430_Read(I2C_HandleTypeDef *pTwi, uint8_t devAddress, uint16_t regAddress, uint8_t *pData, uint16_t size);

/**
  * @}
  */

#endif    /* RF430CL330_H */
