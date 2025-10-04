/*! @mainpage Control de LEDs con modos de operación
 *
 * \section genDesc General Description
 *
 * Este programa permite controlar el encendido de los LEDs mediante una estructura que define:  
 * - El número de LED a controlar.  
 * - El modo de operación (ON, OFF, TOGGLE).  
 * - La cantidad de ciclos de parpadeo.  
 * - El período de cada ciclo de encendido/apagado.  
 * Todo a partir del diagrama de flujo proporcionado por la cátedra
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 20/08/2025 | Creación del documento 	                         |
 *
 * @author Valentina de la Rosa (valentina.delarosa@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 100
#define ON 1 // nos conviene definir un estado entero para el mode
#define OFF 2
#define TOGGLE 3
/*==================[internal data definition]===============================*/
struct leds
{
	uint8_t mode;	  // ON, OFF, TOGGLE
	uint8_t n_led;	  // número de LED a controlar
	uint8_t n_ciclos; // cant. de ciclos de encendido/apagado
	uint16_t periodo; // tiempo de cada ciclo

} my_leds;
/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void diagrama_de_flujo(struct leds *puntero)
{
	// if mode = ON : si if n_led = 1 si ..
	if (puntero->mode == ON)
	{
		if (puntero->n_led == 1)
			LedOn(LED_1);
		else if (puntero->n_led == 2)
			LedOn(LED_2);
		else if (puntero->n_led == 3)
			LedOn(LED_3);
	}
	// if mode = OFF : si if n_led = 1 si ..
	else if (puntero->mode == OFF)
	{
		if (puntero->n_led == 1)
			LedOff(LED_1);
		else if (puntero->n_led == 2)
			LedOff(LED_2);
		else if (puntero->n_led == 3)
			LedOff(LED_3);
	}
	// toggle = permite cambiar entre dos opciones on/off
	else if (puntero->mode == TOGGLE)
	{
		for (int i = 0; i < puntero->n_ciclos; i++)
		{
			if (puntero->n_led == 1)
				LedToggle(LED_1);
			else if (puntero->n_led == 2)
				LedToggle(LED_2);
			else if (puntero->n_led == 3)
				LedToggle(LED_3);

			for (int j = 0; j < puntero->periodo / CONFIG_BLINK_PERIOD; j++) // yo le paso un periodo que lo divide por 100
			{
				vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS); // 100/1000
			}
		}
	}
}

void app_main(void)
{
	LedsInit();
	// acá le pasamos los datos que queremos
	my_leds.n_led = 3;
	my_leds.n_ciclos = 10;
	my_leds.periodo = 500;
	my_leds.mode = 3;

	diagrama_de_flujo(&my_leds);

	while (1)
	{
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}
/*==================[end of file]============================================*/
