/*
 * TWI.h
 *
 *  Created on: Oct 31, 2024
 *      Author: Ahmedmohamed
 */

#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

#define Read 1
#define Write 0
#define slaveAddress 0x3C

void I2C_init_Master(void);
void I2C_Write_Byte(uint8_t data,uint8_t slaveAddr);
uint8_t I2C_Read_Byte(uint8_t data,uint8_t slaveAddr);
void I2C_init_Slave(void);
uint32_t I2C_Slave_Read(void);



#endif /* TWI_H_ */
