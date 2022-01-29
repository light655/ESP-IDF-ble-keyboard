#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/**
 * @brief This version is tested on a 4*5 numpad matrix.
 * 
 */

const uint16_t cPins[4] = {16, 17, 5, 18};
const uint16_t rPins[5] = {23, 19, 22, 4, 0};

bool pressed[20], previous_pressed[20];

void matrix_setup() {
    for(int i = 0; i < 20; i++) {
        pressed[i] = false;
        previous_pressed[i] = false;
    }

    for(int i = 0; i < 5; i++) {
        gpio_reset_pin(rPins[i]);
        gpio_set_direction(rPins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(rPins[i], 0);
    }

    for(int i = 0; i < 4; i++) {
        gpio_reset_pin(cPins[i]);
        gpio_set_direction(cPins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(cPins[i], GPIO_PULLDOWN_ENABLE);
    }
}

void matrix_scan() {
    while(true) {
        for(int i = 0; i < 5; i++) {
            gpio_set_level(rPins[i], 1);
            for(int j = 0; j < 4; j++) {
                if(gpio_get_level(cPins[j])) {
                    pressed[i * 4 + j] = true;
                }
                else {
                    pressed[i * 4 + j] = false;
                }
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
            gpio_set_level(rPins[i], 0);
        }

        for(int i = 0; i < 20; i++) {
            if(pressed[i] == true && previous_pressed[i] == false) {
                printf("%i pressed \n", i);
            }
            else if(pressed[i] == false && previous_pressed[i] == true) {
                printf("%i released \n", i);
            }
            previous_pressed[i] = pressed[i];
        }
    }
}

void app_main(void) {
    matrix_setup();

    xTaskCreate(matrix_scan, "scan", 8192, NULL, 1, NULL);

    
}