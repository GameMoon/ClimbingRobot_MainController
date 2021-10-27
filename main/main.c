#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <string.h>
#include <esp_log.h>
#include "nvs_flash.h"

#include "robot_controller.h"
#include "tcp.h"
#include "wifi_setup.h"


#include "mpu9250.h"
#include "calibrate.h"


// calibration_t cal = {
//     .mag_offset = {.x = 25.183594, .y = 57.519531, .z = -62.648438},
//     .mag_scale = {.x = 1.513449, .y = 1.557811, .z = 1.434039},
//     .accel_offset = {.x = 0.020900, .y = 0.014688, .z = -0.002580},
//     .accel_scale_lo = {.x = -0.992052, .y = -0.990010, .z = -1.011147},
//     .accel_scale_hi = {.x = 1.013558, .y = 1.011903, .z = 1.019645},

//     // .gyro_bias_offset = {.x = 0.303956, .y = -1.049768, .z = -0.403782}};
//     .gyro_bias_offset = {.x = 0.320405, .y = -0.977974, .z = -0.439018}
// };


void test_read_callback(){

}

void app_main()
{
   
    // Init i2cdev library
    // ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT_STA_GOT_IP, ESP_EVENT_ANY_ID, robot_event_handler, NULL, NULL));
  

    wifi_init();
    robot_controller_init();
    ////xTaskCreatePinnedToCore(pca9685_test, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);

    /* SERVO_CONTROLLER_TEST*/
   
    /*uint8_t data[8];
   read_servo_positions(data);
   set_single_servo_position(0,100);
   uint8_t test_data[12] = { 0x43 };
   set_servo_positions(test_data);
   set_coils(0x0F);
   deinit_servo_controller();*/
    /*i2c_mpu9250_init(&cal);
    calibrate_accel();*/
}