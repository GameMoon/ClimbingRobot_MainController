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


#include "ws2812.h"
#include "servo_controller.h"

// calibration_t cal = {
//     .mag_offset = {.x = 25.183594, .y = 57.519531, .z = -62.648438},
//     .mag_scale = {.x = 1.513449, .y = 1.557811, .z = 1.434039},
//     .accel_offset = {.x = 0.020900, .y = 0.014688, .z = -0.002580},
//     .accel_scale_lo = {.x = -0.992052, .y = -0.990010, .z = -1.011147},
//     .accel_scale_hi = {.x = 1.013558, .y = 1.011903, .z = 1.019645},

//     // .gyro_bias_offset = {.x = 0.303956, .y = -1.049768, .z = -0.403782}};
//     .gyro_bias_offset = {.x = 0.320405, .y = -0.977974, .z = -0.439018}
// };

#include "driver/touch_pad.h"

static const char *TAG = "robot_main";

#define TOUCH_PAD_NO_CHANGE (-1)
#define TOUCH_THRESH_NO_USE (0)
#define TOUCH_FILTER_MODE_EN (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

// Touch pins
// leg 1 - IO4 TOUCH0
// leg 2 - IO2 TOUCH2
// leg 3 - IO27 TOUCH7
// leg 4 - IO14 TOUCH6

uint8_t touch_indexes[] = { 0, 2, 7, 6};

static void tp_example_read_task(void *pvParameter)
{
    uint16_t touch_value;
    uint16_t touch_filter_value;
#if TOUCH_FILTER_MODE_EN
    printf("Touch Sensor filter mode read, the output format is: \nTouchpad num:[raw data, filtered data]\n\n");
#else
    printf("Touch Sensor normal mode read, the output format is: \nTouchpad num:[raw data]\n\n");
#endif
    while (1) {
        for (int i = 0; i < 4; i++) {
            touch_pad_read_raw_data(touch_indexes[i], &touch_value);
            touch_pad_read_filtered(touch_indexes[i], &touch_filter_value);
            printf("T%d:[%4d,%4d] ", touch_indexes[i], touch_value, touch_filter_value);
        }

        printf("\n");
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

static void tp_example_touch_pad_init(void)
{
    for (int i = 0;i< 4;i++) {
        touch_pad_config(touch_indexes[i], TOUCH_THRESH_NO_USE);
    }
}


void app_main()
{
   
    // Init i2cdev library
    // ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(touch_pad_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    robot_controller_init();

    wifi_init();
    ESP_LOGI(TAG, "port: %d", servoConn.port);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, &servoConn, 5, NULL);
    xTaskCreate(tcp_server_task, "camera_server", 4096, &camConn, 5, NULL);
    // xTaskCreatePinnedToCore(tcp_server_task, "camera_tcp", 4096, &camConn, 5, NULL, 1);

    // TEST

    //TOUCH
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    tp_example_touch_pad_init();
   // ESP_LOGI(TAG, "sizeof float %d", sizeof(float));
    while(1){
        ESP_LOGI(TAG, "PSRAM total: %d",heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        ESP_LOGI(TAG, "PSRAM free: %d", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

        ESP_LOGI(TAG, "Internal total: %d", heap_caps_get_total_size(MALLOC_CAP_INTERNAL));
        ESP_LOGI(TAG, "Internal free: %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
    // touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    // xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 2048, NULL, 5, NULL);

    // rgbVal color = makeRGBVal(0,0,255);
    // ws2812_init(GPIO_NUM_0);
    // ws2812_setColors(1,&color);
    // ESP_LOGW("LED", "led_init_ok %d",led_init_ok);
    ////xTaskCreatePinnedToCore(pca9685_test, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);

    /* SERVO_CONTROLLER_TEST*/
   
    //uint8_t data[8];
   //read_servo_positions(data);
   //set_single_servo_position(0,100);
   /* init_servo_controller();
    set_servo_psu(1);
    uint8_t test_data[12] = {0x43};
    set_servo_positions(test_data);*/
    /*set_coils(0x0F);
    deinit_servo_controller();*/
    /*i2c_mpu9250_init(&cal);
    calibrate_accel();*/

    /*
    while (1)
    {
        ESP_LOGI(TAG, "Taking picture...");
        camera_fb_t *pic = esp_camera_fb_get();

        // use pic->buf to access the image
        ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", pic->len);
        esp_camera_fb_return(pic);

        vTaskDelay(5000 / portTICK_RATE_MS);
    }*/
}