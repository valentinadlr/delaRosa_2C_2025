/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Valentina de la Rosa(valentina.delarosa@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct
{
    gpio_t pin;  
    io_t dir;    
} gpioConf_t;
/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void setBCD(uint8_t bcd, gpioConf_t *gpioVector)
{
    for (int i = 0; i < 4; i++)
    {
        uint8_t bit = (bcd >> i) & 0x01; //deja el último bit para ponerlo en alto o en bajo
        if (bit)
        {
            GPIOOn(gpioVector[i].pin);   
        }
        else
        {
            GPIOOff(gpioVector[i].pin);  
    	}
	}
}

void DisplayBCD(uint32_t valor, int digitos, gpioConf_t *gpiosBCD, gpioConf_t *gpiosSEL) {
    uint32_t divisor = 1;  // arranca en 1 fuera del bucle

    for (int d = 0; d < digitos; d++) {
        uint8_t digito = (valor / divisor) % 10; // separa el dígito correspondiente
        divisor *= 10;  // para el siguiente dígito

        setBCD(digito, gpiosBCD);

        GPIOOn(gpiosSEL[d].pin);

        for (volatile int i = 0; i < 5000; i++); // delay pobre pero funciona

        GPIOOff(gpiosSEL[d].pin);
    }
}

int app_main(void)
{
    gpioConf_t gpios[4] = {
        {GPIO_20, GPIO_OUTPUT}, 
        {GPIO_21, GPIO_OUTPUT},
        {GPIO_22, GPIO_OUTPUT},
        {GPIO_23, GPIO_OUTPUT}
    };

	gpioConf_t gpiosSEL[3] = {
        {GPIO_9, GPIO_OUTPUT}, 
        {GPIO_18, GPIO_OUTPUT},
        {GPIO_19,  GPIO_OUTPUT}
    };


    for (int i = 0; i < 4; i++)
	{
        GPIOInit(gpios[i].pin, gpios[i].dir);
	}

	for (int i=0; i<3; i++) 
	{ 
		GPIOInit(gpiosSEL[i].pin, gpiosSEL[i].dir);
	}

    uint32_t numero = 123;

    DisplayBCD(numero, 3, gpios, gpiosSEL);

	return 0 ;
}
/*==================[end of file]============================================*/