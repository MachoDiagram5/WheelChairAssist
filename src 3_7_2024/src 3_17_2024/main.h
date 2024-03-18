// Definitions for main.cpp
#define LEDC_TIMER0               LEDC_TIMER_0//channel 0&1 is for the first hbridge
#define LEDC_TIMER1               LEDC_TIMER_1
#define LEDC_TIMER2               LEDC_TIMER_2//channel 1&2 is for second hbridge
#define LEDC_TIMER3               LEDC_TIMER_3
#define LEDC_MODE                LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO0           (23) // Define the output GPIO for channel 0
#define LEDC_OUTPUT_IO1           (25) // Define the output GPIO for channel 1
#define LEDC_OUTPUT_IO2           (26) // Define the output GPIO for channel 2
#define LEDC_OUTPUT_IO3           (27) // Define the output GPIO for channel 3
#define LEDC_CHANNEL0            LEDC_CHANNEL_0
#define LEDC_CHANNEL1            LEDC_CHANNEL_1
#define LEDC_CHANNEL2            LEDC_CHANNEL_2
#define LEDC_CHANNEL3            LEDC_CHANNEL_3
#define LEDC_DUTY_RES            LEDC_TIMER_8_BIT // Set duty resolution to 8 bits
#define LEDC_FREQUENCY           (15000) // Frequency in Hertz. Set frequency at 15 kHz

// Definitions for pwmUpdateTask.cpp
#define BUTTON_FORWARD_GPIO      GPIO_NUM_22 // GPIO PIN for forward button
#define BUTTON_DECREASE_GPIO     GPIO_NUM_21 // GPIO PIN for decrease button
#define BUTTON_BACKWARD_GPIO     GPIO_NUM_18 // GPIO PIN for backward button

#define DEBOUNCE_TICKS           (20) // Debounce time in milliseconds
#define LEDC_FULL_SPEED          (255) // Maximum duty cycle

#define BUTTON_FORWARD_SEL       (BIT(22)) // Configures the pin mask for the forward button GPIO pin
#define BUTTON_DECREASE_SEL      (BIT(21)) // Configures the pin mask for the decrease button GPIO pin
#define BUTTON_BACKWARD_SEL      (BIT(18)) // Configures the pin mask for the backward button GPIO pin

// PWM settings - Must be global so they can be accessed by other functions
ledc_timer_config_t timer_conf;
ledc_channel_config_t ledc_conf0; // Configuration for channel 0
ledc_channel_config_t ledc_conf1; // Configuration for channel 1
ledc_channel_config_t ledc_conf2; // Configuration for channel 2
ledc_channel_config_t ledc_conf3; // Configuration for channel 3
int32_t duty_cycle0; // Duty cycle for channel 0
int32_t duty_cycle1; // Duty cycle for channel 1
int32_t duty_cycle2; // Duty cycle for channel 2
int32_t duty_cycle3; // Duty cycle for channel 3
int duty_changed; // Flag to indicate duty cycle change

gpio_config_t button_forward_config;
gpio_config_t button_decrease_config;
gpio_config_t button_backward_config;

void gpioSettings() {
    // Button GPIO settings
  button_forward_config.pin_bit_mask = BUTTON_FORWARD_SEL;           
  button_forward_config.mode = GPIO_MODE_INPUT;
  button_forward_config.pull_up_en = GPIO_PULLUP_ENABLE;
  button_forward_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  button_forward_config.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&button_forward_config);
//decrease
  button_decrease_config.pin_bit_mask = BUTTON_DECREASE_SEL;           
  button_decrease_config.mode = GPIO_MODE_INPUT;
  button_decrease_config.pull_up_en = GPIO_PULLUP_ENABLE;
  button_decrease_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  button_decrease_config.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&button_decrease_config);
//backward
  button_backward_config.pin_bit_mask = BUTTON_BACKWARD_SEL;           
  button_backward_config.mode = GPIO_MODE_INPUT;
  button_backward_config.pull_up_en = GPIO_PULLUP_ENABLE;
  button_backward_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  button_backward_config.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&button_backward_config);
}
void configureLEDC() {
    // PWM timer settings for Timer 0
    ledc_timer_config_t timer_conf0 = {
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER0, // Use Timer 0 for channel 0
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf0);

    // PWM channel settings for channel 0
    ledc_channel_config_t ledc_conf0 = {
        .gpio_num = LEDC_OUTPUT_IO0,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_conf0);

    // PWM timer settings for Timer 1
    ledc_timer_config_t timer_conf1 = {
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER1, // Use Timer 1 for channel 1
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf1);

    // PWM channel settings for channel 1
    ledc_channel_config_t ledc_conf1 = {
        .gpio_num = LEDC_OUTPUT_IO1,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER1,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_conf1);

      // PWM timer settings for Timer 2
    ledc_timer_config_t timer_conf2 = {
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER2, // Use Timer 2 for channel 2
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf2);

    // PWM channel settings for channel 2
    ledc_channel_config_t ledc_conf2 = {
        .gpio_num = LEDC_OUTPUT_IO2,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL2,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER2,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_conf2);
    
      // PWM timer settings for Timer 3
    ledc_timer_config_t timer_conf3 = {
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER3, // Use Timer 3 for channel 3
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf3);

    // PWM channel settings for channel 3
    ledc_channel_config_t ledc_conf3 = {
        .gpio_num = LEDC_OUTPUT_IO3,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL3,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER3,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_conf3);
}