#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/ledc.h>
#include <driver/gpio.h>
#include "main.h"

// Function Prototypes
void taskSetup();
void updateTask(void*);

//my functions
void configureLEDC();
void gpioSettings();


// This function sets up the PWM timer register, led register, button 0 and 1 registers
void taskSetup() 
{
  configureLEDC();

    // Set PWM duty cycle for channel 0
    duty_cycle0 = 0;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL0, duty_cycle0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL0);

    // Set PWM duty cycle for channel 1
    duty_cycle1 = 0;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL1, duty_cycle1);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL1);

    //gpio settings
    gpioSettings();

}



// this causes channel one to have output to figure 1 on email
// even when i isolate the pwm channel to only deal with channel 1 it does not do much
// to act like channel zero
void updateTask(void *blank)
{
    while(1) {
           // pwm channel 0
        if(gpio_get_level(BUTTON_FORWARD_GPIO) == 0) {//if foward button is pressed
            duty_changed = 1;
            duty_cycle0 += 15;
            if(duty_cycle0 > LEDC_FULL_SPEED) {
                duty_cycle0 -= 15;
            }

            // BUTTON DEBOUNCE
            vTaskDelay(DEBOUNCE_TICKS);
        }
         //pwm channel 1
        else if(gpio_get_level(BUTTON_BACKWARD_GPIO) == 0) {//backward button is pressed
            duty_changed = 1;
            duty_cycle1 += 15;
            if(duty_cycle1 > LEDC_FULL_SPEED) {
                duty_cycle1 -= 15;
            }

            // BUTTON DEBOUNCE
            vTaskDelay(DEBOUNCE_TICKS);
        }
        //only channel 1 decreses because of the constant high state nothing happens to channel 1
        else if(gpio_get_level(BUTTON_DECREASE_GPIO) == 0) {//if decrease button is pressed
            duty_changed = 1;
            duty_cycle0 -= 15; // Decrement duty_cycle1 for the  channel0
            duty_cycle1 -= 15; // Decrement duty_cycle1 for the channel1
            if(duty_cycle0 < 0 && duty_cycle1 < 0) {
                duty_cycle0 += 15;
                duty_cycle1 -= 15;
            }

            // BUTTON DEBOUNCE
            vTaskDelay(DEBOUNCE_TICKS);
        }

        // Only read from duty_cycle1 when it's been changed
        if(duty_changed == 1) {
            duty_changed = 0;
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL0, duty_cycle0);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL0);
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL1, duty_cycle1);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL1);
        }

        vTaskDelay(1);
    }
}

// The main function is what actually executes in the program after everything gets compiled. Therefore, it must call your taskSetup() function 
// and then proceed to create the task which will continuously execute.
void app_main() 
{
  taskSetup();
  xTaskCreate(&updateTask, "updateTask", 4096, NULL, 5, NULL);
}
