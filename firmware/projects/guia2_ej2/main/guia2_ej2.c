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
#include "timer_mcu.h"
/*==================[macros and definitions]=================================*/
/*==================[internal data definition]===============================*/
TaskHandle_t medir_task_handle = NULL;
TaskHandle_t leds_task_handle = NULL;
TaskHandle_t display_task_handle = NULL;
bool act_med = false;
bool hold = false;
uint16_t medida = 0;
int periodoUS = 1000000;
/*==================[internal functions declaration]=========================*/

void Tecla1(void)
{
    act_med = !act_med;
}

void Tecla2(void)
{
    hold = !hold;
}

void FuncTimerA(void *param)
{
    vTaskNotifyGiveFromISR(medir_task_handle, pdFALSE);
    vTaskNotifyGiveFromISR(leds_task_handle, pdFALSE);
    vTaskNotifyGiveFromISR(display_task_handle, pdFALSE);
}

static void Medir(void *pvParameters)
{
    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (act_med)
        {
            medida = HcSr04ReadDistanceInCentimeters();
        }
    }
}

static void LEDs(void *pvParameters)
{
    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
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
    }
}

static void Display(void *pvParameters)
{
    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (act_med)
        {
            if (!hold)
                LcdItsE0803Write(medida);
        }
        else
        {
            LcdItsE0803Off();
        }
    }
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
    LedsInit();
    SwitchesInit();
    HcSr04Init(GPIO_3, GPIO_2);
    LcdItsE0803Init();
    SwitchActivInt(SWITCH_1, Tecla1, NULL);
    SwitchActivInt(SWITCH_2, Tecla2, NULL);
    xTaskCreate(Medir, "Medir", 512, NULL, 5, &medir_task_handle);
    xTaskCreate(LEDs, "LEDs", 512, NULL, 5, &leds_task_handle);
    xTaskCreate(Display, "Display", 512, NULL, 5, &display_task_handle);
    timer_config_t timer = {.timer = TIMER_A, .period = periodoUS, .func_p = FuncTimerA, .param_p = NULL};

    TimerInit(&timer);

    TimerStart(TIMER_A);
}
