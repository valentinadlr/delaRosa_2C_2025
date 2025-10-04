/*! @mainpage Visualización de número en Display BCD
 *
 * @section genDesc General Description
 *
 * Este programa implementa la conversión de un número entero a formato BCD 
 * y lo muestra a través de un conjunto de pines GPIO del microcontrolador ESP32, 
 * que controlan tanto las líneas de datos como la selección de cada dígito del display.  
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * |  D0 (bit0)     |  GPIO_20      |
 * |  D1 (bit1)     |  GPIO_21      |
 * |  D2 (bit2)     |  GPIO_22      |
 * |  D3 (bit3)     |  GPIO_23      |
 * |  SEL_1         |  GPIO_9       |
 * |  SEL_2         |  GPIO_18      |
 * |  SEL_3         |  GPIO_19      |
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 10/09/2025 | Document creation		                         |
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
/**
 * @brief Estructura de configuración de un pin GPIO.
 *
 * Contiene el número de pin y la dirección de operación (entrada/salida).
 */
typedef struct
{
    gpio_t pin;  
    io_t dir;    
} gpioConf_t;
/*==================[internal functions declaration]=========================*/
/**
 * @brief Configura las salidas GPIO para representar un dígito en BCD.
 *
 * @param[in] bcd Valor del dígito en formato decimal (0–9).
 * @param[in] gpioVector Vector de 4 pines GPIO que representan los bits del BCD.
 */
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
/**
 * @brief Convierte un número entero a un arreglo de dígitos decimales (para BCD).
 *
 * @param[in] data Número entero a convertir.
 * @param[in] digits Cantidad de dígitos de salida.
 * @param[out] bcd_number Arreglo donde se almacenan los dígitos.
 * @return 0 si la conversión fue exitosa.
 */
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{

    for (int i = digits - 1; i >= 0; i--) {
        bcd_number[i] = data % 10;  // Guardar último dígito
        data /= 10;        
}
    return 0; 
}
/**
 * @brief Muestra un número en un display multiplexado en formato BCD.
 *
 * @param[in] valor Número a mostrar.
 * @param[in] digitos Cantidad de dígitos a visualizar.
 * @param[in] gpiosBCD Vector de pines para los bits de datos (BCD).
 * @param[in] gpiosSEL Vector de pines de selección para cada dígito del display.
 */
void DisplayBCD(uint32_t valor, int digitos, gpioConf_t *gpiosBCD, gpioConf_t *gpiosSEL) {
    uint8_t arreglo[digitos]; 

    convertToBcdArray(valor, digitos, arreglo);

    for (int d = 0; d < digitos; d++) {
        uint8_t digito = arreglo[d];  

        setBCD(digito, gpiosBCD);     

        GPIOOn(gpiosSEL[d].pin);     

        GPIOOff(gpiosSEL[d].pin);     
    }
}
/*==================[external functions definition]==========================*/
/**
 * @brief Función principal de la aplicación.
 *
 * - Inicializa los pines GPIO asociados al BCD y a la selección de dígitos.  
 * - Convierte el número entero definido en `numero` a formato BCD.  
 * - Lo muestra en el display multiplexado.  
 *
 * @return 0 si la ejecución es exitosa.
 */
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