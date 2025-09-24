/*! @mainpage Blinking
 *
 * \section genDesc General Description
 *
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 10/09/2025 | Document creation		                         |
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
#include "hc_sr04.h"
#include "gpio_mcu.h"
#include "switch.h"
#include "lcditse0803.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_DELAY_PERIOD 1000
/*==================[internal data definition]===============================*/
TaskHandle_t led1_task_handle = NULL;
TaskHandle_t led2_task_handle = NULL;
TaskHandle_t led3_task_handle = NULL;
bool act_med = false;
bool hold = false;
uint16_t medida = 0;
/*==================[internal functions declaration]=========================*/

static void Teclas(void *pvParameters)
{
    uint8_t teclas;
    while (1)
    {
        teclas = SwitchesRead();
        switch (teclas)
        {
        case SWITCH_1:
            act_med = !act_med;
            break;

        case SWITCH_2:
            hold = !hold;
            break;
        }
        vTaskDelay(CONFIG_DELAY_PERIOD / portTICK_PERIOD_MS);
    }
}

static void Medir(void *pvParameters)
{
    while (1)
    {
        if (act_med)
        {
            medida = HcSr04ReadDistanceInCentimeters();
        }
        vTaskDelay(CONFIG_DELAY_PERIOD / portTICK_PERIOD_MS);
    }
}

static void LEDs(void *pvParameters)
{
    while (1)
    {
        if (act_med)
        {
            if (medida < 10)
                LedsOffAll();
            else if (medida < 20)
            {
                LedOn(LED_1);
                LedOff(LED_2);
                LedOff(LED_3);
            }
            else if (medida < 30)
            {
                LedOn(LED_1);
                LedOn(LED_2);
                LedOff(LED_3);
            }
            else
            {
                LedOn(LED_1);
                LedOn(LED_2);
                LedOn(LED_3);
            }
        }
        else
        {
            LedsOffAll();
        }
        vTaskDelay(CONFIG_DELAY_PERIOD / portTICK_PERIOD_MS);
    }
}

static void Display(void *pvParameters)
{
    while (1)
    {
        if (act_med)
        {
            if (!hold)
                LcdItsE0803Write(medida);
        }
        else
        {
            LcdItsE0803Off();
        }
        vTaskDelay(CONFIG_DELAY_PERIOD / portTICK_PERIOD_MS);
    }
}
/*==================[external functions definition]==========================*/
void app_main(void)
{
    LedsInit();
    SwitchesInit();
    HcSr04Init(GPIO_3, GPIO_2);
    LcdItsE0803Init();
    xTaskCreate(Teclas, "Teclas", 512, NULL, 5, NULL);
    xTaskCreate(Medir, "Medir", 512, NULL, 5, NULL);
    xTaskCreate(LEDs, "LEDs", 512, NULL, 5, NULL);
    xTaskCreate(Display, "Display", 512, NULL, 5, NULL);
}
