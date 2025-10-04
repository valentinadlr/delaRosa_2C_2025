/*! @mainpage Medición de distancia con sensor ultrasónico y visualización
 *
 * \section genDesc General Description
 *
 * Este programa implementa un sistema con el microcontrolador ESP32 que mide distancias 
 * utilizando el sensor ultrasónico HC-SR04 y las muestra en un display LCD ITS-E0803.  
 * Además, el estado de la medición se indica mediante LEDs que se encienden en función del rango de distancia.  
 *
 *  El sistema permite:
 * - Iniciar/detener la medición mediante la tecla 1.  
 * - Congelar (hold) o continuar actualizando el valor mostrado en el display mediante la tecla 2.  
 * - Visualizar la distancia en el display LCD.  
 * - Indicar por LEDs la magnitud de la distancia medida:
 *   - Distancia < 10 cm → todos los LEDs apagados.  
 *   - Distancia 10–20 cm → LED1 encendido.  
 *   - Distancia 20–30 cm → LED1 y LED2 encendidos.  
 *   - Distancia > 30 cm → LED1, LED2 y LED3 encendidos.  
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
/**
 * @brief Lee las teclas y actualiza los flags de control.
 *
 * - Tecla 1: inicia/detiene la medición (`act_med`).  
 * - Tecla 2: congela o libera la actualización del display (`hold`).  
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se usa en esta función).
 */
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
/**
 * @brief Obtiene la distancia con el sensor ultrasónico HC-SR04.
 *
 * Si `act_med = true`, se actualiza la variable global `medida`.  
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se usa en esta función).
 */
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
/**
 * @brief Controla el encendido de los LEDs en función de la distancia medida.
 *
 * - Distancia < 10 cm → LEDs apagados.  
 * - 10–20 cm → LED1 encendido.  
 * - 20–30 cm → LED1 y LED2 encendidos.  
 * - >30 cm → LED1, LED2 y LED3 encendidos.  
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se usa en esta función).
 */
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
/**
 * @brief Controla la visualización en el display LCD.
 *
 * - Si `act_med = true` y `hold = false`, muestra la distancia actual.  
 * - Si `act_med = false`, apaga el display.  
 *
 * @param pvParameters Parámetro de tarea FreeRTOS (no se usa en esta función).
 */
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
/**
 * @brief Función principal de la aplicación.
 *
 * Inicializa los periféricos (LEDs, switches, HC-SR04, display)  
 * y crea las cuatro tareas FreeRTOS: Teclas, Medir, LEDs y Display.  
 */
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
