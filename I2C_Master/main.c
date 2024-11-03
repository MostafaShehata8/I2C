/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "inc/hw_gpio.h"
#include "TWI.h"

#define First_Switch    GPIO_PIN_0
#define Second_Switch   GPIO_PIN_4

#define Read 1
#define Write 0
#define slaveAddress 0x3C

void LEDConfig(void) {
    // Enable the GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configure PF1 (Red LED)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3 );

    // Step 3: Unlock Port F pin 0 (only required for PF0, as it is locked by default)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   // Unlock Port F
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;       // Commit register for PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;               // Re-lock Port F

    // Step 4: Configure pins 0 and 4 as input with pull-up resistors
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);


}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4| SYSCTL_USE_PLL |SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);

    I2C_init_Master();
    LEDConfig();
    while (1)
    {

        //        I2C_Write_Byte(0x01, slaveAddress);
        //        SysCtlDelay(SysCtlClockGet()/30);
        //        I2C_Write_Byte(0x00, slaveAddress);
        uint8_t switch1= GPIOPinRead(GPIO_PORTF_BASE, First_Switch) ;

        uint8_t switch2= GPIOPinRead(GPIO_PORTF_BASE, Second_Switch)>>4;

        if(!switch1 || !switch2)
        {
            SysCtlDelay(SysCtlClockGet()/3000);
            if(!switch1 && !switch2)
            {
                // Red_light();
                I2C_Write_Byte(0x00,slaveAddress);
                SysCtlDelay(SysCtlClockGet()/30);
            }
            else if(!switch1 && switch2)
            {
                //Blue_light();
                I2C_Write_Byte(0x01,slaveAddress);
                SysCtlDelay(SysCtlClockGet()/30);
            }
            else if(!switch2 && switch1)
            {
                //Green_light();
                I2C_Write_Byte(0x02,slaveAddress);
                SysCtlDelay(SysCtlClockGet()/30);
            }
        }
    }


}

