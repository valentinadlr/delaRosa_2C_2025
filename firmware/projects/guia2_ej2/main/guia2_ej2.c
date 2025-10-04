/*! @mainpage Sistema de medición ultrasónica con FreeRTOS y Timer
 *
 * \section genDesc General Description
 *
 * Se crea un nuevo proyecto en el que se modifica la actividad 1 de la corriente guía, 
 * de manera de utilizar interrupciones para el control de las teclas 
 * y timers para el control de tiempos.  
 *
 * -> Se eliminan los vTaskDelay
 *  
 * @section hardConn Hardware Connection
 * 
 * |   EDU-CIAA-NXP   |  PERIFÉRICO  |
 * |:----------------:|:-------------|
 * | GPIO_3           | ECHO         |
 * | GPIO_2           | TRIGGER      |
 * | +5V              | +5V          |
 * | GND              | GND          |
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
/**
 * @brief Rutina de interrupción para la tecla 1.
 *
 * Alterna el flag `act_med` para iniciar o detener la medición.
 */
void Tecla1(void)
{
    act_med = !act_med;
}
/**
 * @brief Rutina de interrupción para la tecla 2.
 *
 * Alterna el flag `hold` para congelar o liberar el valor mostrado en el display.
 */
void Tecla2(void)
{
    hold = !hold;
}
/**
 * @brief Callback del timer A.
 *
 * Envía notificaciones a las tareas de medición, LEDs y display para que se ejecuten.
 *
 * @param param Parámetro de usuario (no utilizado).
 */
void FuncTimerA(void *param)
{
    vTaskNotifyGiveFromISR(medir_task_handle, pdFALSE);
    vTaskNotifyGiveFromISR(leds_task_handle, pdFALSE);
    vTaskNotifyGiveFromISR(display_task_handle, pdFALSE);
}
/**
 * @brief Tarea encargada de realizar la medición con el sensor ultrasónico.
 *
 * Espera notificaciones del timer y, si `act_med` está activo, actualiza la variable `medida`.
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se utiliza).
 */
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
/**
 * @brief Tarea encargada de controlar los LEDs según la distancia medida.
 *
 * Se ejecuta en cada notificación del timer. Enciende/apaga los LEDs dependiendo del rango de `medida`.
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se utiliza).
 */
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
/**
 * @brief Tarea encargada de manejar la visualización en el display LCD.
 *
 * Muestra la distancia en centímetros si la medición está activa y `hold` es falso.
 * Si `act_med` es falso, apaga el display.
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se utiliza).
 */
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
/**
 * @brief Función principal de la aplicación.
 *
 * Inicializa periféricos (LEDs, switches, HC-SR04, LCD, timer).
 * Configura interrupciones para las teclas.
 * Crea las tareas FreeRTOS:
 * - Medir
 * - LEDs
 * - Display
 * Inicia el timer A que sincroniza la ejecución periódica de dichas tareas.
 */
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
