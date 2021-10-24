#ifndef __MAX34409_H__
#define __MAX34409_H__

#include <stdbool.h>
#include <i2cdev.h>
#include <esp_err.h>

esp_err_t max34409_init_desc(i2c_dev_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);
esp_err_t max34409_free_desc(i2c_dev_t *dev);
//esp_err_t max34409_get_current(i2c_dev_t *dev, uint8_t *current);
esp_err_t max34409_get_year(i2c_dev_t *dev, uint8_t *year);

#endif
