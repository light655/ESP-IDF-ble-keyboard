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
const uint16_t rPins[5] = {23, 19, 22, 0, 4};

bool pressed[20], previous_pressed[20];

void matrix_setup() {
    for(int i = 0; i < 20; i++) {
        pressed[i] = false;
        previous_pressed[i] = false;
    }

    for(int i = 0; i < 5; i++) {
        gpio_reset_pin(rPins[i]);
        gpio_set_direction(rPins[i], GPIO_MODE_OUTPUT);
    }

    for(int i = 0; i < 4; i++) {
        gpio_reset_pin(cPins[i]);
        gpio_set_direction(cPins[i], GPIO_MODE_INPUT);
    }
}

void matrix_scan() {
    for(int i = 0; i < 5; i++) {
        gpio_set_level(rPins[i], 1);
        for(int j = 0; j < 4; j++) {
            previous_pressed[i * 4 + j] = pressed[i * 4 + j];
            if(gpio_get_level(rPins[j])) {
                pressed[i * 4 + j] = true;
            }
            else {
                pressed[i * 4 + j] = false;
            }
        }
    }
}

void app_main(void) {

}