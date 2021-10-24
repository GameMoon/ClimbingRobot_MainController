#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include <pca9685.h>

static const char *PWM_TAG = "pca9685_test";

#define ADDR PCA9685_ADDR_BASE

#define PWM_FREQ_HZ 49

#define SDA_GPIO 21
#define SCL_GPIO 22

void pwm_controller_init(i2c_dev_t * dev)
{
    //i2c_dev_t dev;
    memset(dev, 0, sizeof(i2c_dev_t));

    //ESP_ERROR_CHECK(max34409_init_desc(&dev, 0x10, 0, SDA_GPIO, SCL_GPIO));
    ESP_ERROR_CHECK(pca9685_init_desc(dev, ADDR, 0, SDA_GPIO, SCL_GPIO));
    ESP_ERROR_CHECK(pca9685_init(dev));

    ESP_ERROR_CHECK(pca9685_restart(dev));

    uint16_t freq;
    ESP_ERROR_CHECK(pca9685_set_pwm_frequency(dev, PWM_FREQ_HZ));
    ESP_ERROR_CHECK(pca9685_get_pwm_frequency(dev, &freq));

    ESP_LOGI(PWM_TAG, "Freq %dHz, real %d", PWM_FREQ_HZ, freq);
}

void pwm_controller_set(i2c_dev_t * dev, uint16_t * data){
    for(int k = 0; k < 16; k++){
        if (pca9685_set_pwm_value(dev, k, data[k]) != ESP_OK)
            ESP_LOGI(PWM_TAG, "Could not set PWM value to ch%d", k);
        else{
            ESP_LOGI(PWM_TAG, "%d PWM to ch%d set", data[k], k);
        }
    }
}

#endif