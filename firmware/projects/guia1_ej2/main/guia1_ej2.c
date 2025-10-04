/*! @mainpage Blinking switch
 *
 * \section genDesc General Description
 *
 * Este ejemplo hace que:
 * - El LED_1 parpadee mientras se mantenga presionado el SWITCH_1.  
 * - El LED_2 parpadee mientras se mantenga presionado el SWITCH_2.  
 * - El LED_3 parpadee mientras se mantengan presionados simultáneamente el SWITCH_1 y el SWITCH_2.  
 *
 * @section changelog Changelog
 * 
 * 10/09 Creación del documento
 *
 * @author Valentina de la Rosa (valentina.delarosa@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 200
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	uint8_t teclas;
	LedsInit();
	SwitchesInit();
    while(1)    {
    	teclas  = SwitchesRead();
    	switch(teclas){
    		case SWITCH_1:
    			LedToggle(LED_1);
    		break;
    		case SWITCH_2:
    			LedToggle(LED_2);
    		break;
			case SWITCH_1|SWITCH_2:
				LedToggle(LED_3);
			break;
    	}
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}
