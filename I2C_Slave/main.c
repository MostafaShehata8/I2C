
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "TWI.h"
//
#define Read 1
#define Write 0
#define slaveAddress 0x3C

#define Blue_Led GPIO_PIN_3
#define Red_Led  GPIO_PIN_1
#define Green_Led GPIO_PIN_2


void White_light()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on LED


}
void Red_light()
{

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Turn on LED


}
void Green_light()
{

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Turn on LED


}
void Blue_light()
{


    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Turn on LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on LED

}
// Function to configure GPIO for LED
void LEDConfig(void) {
    // Enable the GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configure PF1 (Red LED)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    LEDConfig();
    I2C_init_Slave();
    while(1)
    {

        uint32_t Data= I2C_Slave_Read();
        if (0x00==Data)
        {
            White_light();
            SysCtlDelay(SysCtlClockGet()/30);

        }
        else if(0x01==Data)
        {

            if( GPIOPinRead(GPIO_PORTF_BASE, Blue_Led)>>3  && (GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>1 && (GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>2)//cw PB
                    {
                Red_light();
                    }
            else if( (GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>1 && !((GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>3) && !((GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>2))
            {
                Green_light();
            }
            else if((GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>2 && !((GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>1) && !((GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>3))
            {
                Blue_light();
            }
            else if(GPIOPinRead(GPIO_PORTF_BASE, Blue_Led)>>3 && !((GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>1) && !((GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>2))
            {
                White_light();
            }
        }
        else if(0x02==Data)
        {

            if(   GPIOPinRead(GPIO_PORTF_BASE, Blue_Led)>>3  && (GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>1 && (GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>2)//cw PB
            {
                Blue_light();
            }
            else if(GPIOPinRead(GPIO_PORTF_BASE, Blue_Led)>>3)
            {
                Green_light();
            }
            else if((GPIOPinRead(GPIO_PORTF_BASE, Green_Led))>>2)
            {
                Red_light();
            }
            else if((GPIOPinRead(GPIO_PORTF_BASE, Red_Led))>>1)
            {
                White_light();
            }

        }

    }


}

