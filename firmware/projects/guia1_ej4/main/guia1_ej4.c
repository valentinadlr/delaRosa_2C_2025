/*! @mainpage Conversión de Entero a BCD
 *
 * @section genDesc General Description
 *
 * Con ayuda del programa se implementa una función que convierte un número entero de 32 bits
 * a su representación en formato BCD.   
 * El usuario debe indicar dígitos de salida y un puntero
 * 
 * @section hardConn Hardware Connection
 *
 * En este caso no se requiere
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 10/09/2025 | Document creation		                         |
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
/**
 * @brief Convierte un número entero de 32 bits a formato BCD.
 *
 * @param[in] data Número entero a convertir.
 * @param[in] digits Cantidad de dígitos de salida.
 * @param[out] bcd_number Puntero al arreglo donde se almacenará el resultado en BCD.
 *
 * @return 0 si la conversión fue exitosa.
 *
 * @note Los dígitos se guardan en el arreglo en orden, de izquierda a derecha.
 */
int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{

    for (int i = digits - 1; i >= 0; i--) {
        bcd_number[i] = data % 10;  // Guardar último dígito
        data /= 10;        
}
    return 0; 
}

/*==================[external functions definition]==========================*/
/**
 * @brief Función principal de la aplicación.
 *
 * - Declara un número entero a convertir.  
 * - Llama a @ref convertToBcdArray para obtener su representación en BCD.  
 * - Imprime el resultado en la consola.  
 */
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