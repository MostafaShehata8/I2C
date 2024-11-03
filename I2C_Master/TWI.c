/*
 * TWI.c
 *
 *  Created on: Oct 31, 2024
 *      Author: Ahmedmohamed
 */


#include "TWI.h"
void I2C_init_Master(void)
{
    //Step 1 Enable the I2C module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Step 2: Wait for the peripheral to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}
    // Step 3: Configure GPIO pins for I2C
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);  // PB2 -> I2CSCL
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);  // PB3 -> I2CSDA

    // Configure the GPIO Pins for I2C communication
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE,GPIO_PIN_3);
    // Step 4: Enable the I2C0 Master block
    I2CMasterEnable(I2C0_BASE);
    // Step 5: Configure the I2C module for I2C communication
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(),false);
}
void I2C_Write_Byte(uint8_t data,uint8_t slaveAddr)
{
   // while (I2CMasterBusBusy(I2C0_BASE)) {}
    //slave address  write or read
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, Write);
    //Send Data
    I2CMasterDataPut(I2C0_BASE, data);
    //start condition then transmit then stop
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    // Wait until the transmit FIFO is not full, then send data
    while (I2CMasterBusy(I2C0_BASE)) {}
    // a block 3la ack
    while(I2CMasterErr(I2C0_BASE)!=I2C_MASTER_ERR_NONE){};
}
uint8_t I2C_Read_Byte(uint8_t data,uint8_t slaveAddr)
{
    uint8_t receivedData = 0;
    // Wait until the transmit FIFO is not full, then send data
    while (I2CMasterBusBusy(I2C0_BASE)) {}
    // Set the slave address and set the master to read mode
    I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddr, Read);

    // send the START bit and followed by the STOP bit
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    // Wait until the master is done receiving
    while (I2CMasterBusy(I2C0_BASE));


    // Check for errors
    if (I2CMasterErr(I2C0_BASE) == I2C_MASTER_ERR_NONE) {
        // No errors
        receivedData = I2CMasterDataGet(I2C0_BASE);
    } else {

        receivedData = 0xFF;
    }

    return receivedData;

}
void I2C_init_Slave(void)
{
    //Step 1 Enable the I2C module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Step 2: Wait for the peripheral to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}
    // Step 3: Configure GPIO pins for I2C
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);  // PB2 -> I2CSCL
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);  // PB3 -> I2CSDA
    // Step 4: Configure the I2C module for I2C communication
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE,GPIO_PIN_3);
    //step 5 :slave init +Enable the I2C0 Slave block
    I2CSlaveInit(I2C0_BASE, slaveAddress);

}
uint32_t I2C_Slave_Read(void)
{
    uint32_t returns_function =I2CSlaveStatus(I2C0_BASE);
    uint32_t recieved_data;
    if(returns_function==I2C_SLAVE_ACT_RREQ || returns_function==I2C_SLAVE_ACT_RREQ_FBR)
    {
        recieved_data=I2CSlaveDataGet(I2C0_BASE);
    }

    return recieved_data;

}




