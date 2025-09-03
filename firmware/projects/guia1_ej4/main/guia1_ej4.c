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
 * @author Valentina de la Rosa ()
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{

    for (int i = digits - 1; i >= 0; i--) {
        bcd_number[i] = data % 10;  // Guardar último dígito
        data /= 10;        
}
    return 0; 
}

/*==================[external functions definition]==========================*/
void app_main(void){
    uint32_t numero = 12345;
    uint8_t arreglo[6];
    convertToBcdArray(numero, 6, arreglo);

    printf("Número convertido a BCD: \n");
    for (int i = 0; i < 6; i++) {
        printf("%d ", arreglo[i]);
    }
printf("/n");
}
/*==================[end of file]============================================*/