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

#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
// calibration_t cal = {
//     .mag_offset = {.x = 25.183594, .y = 57.519531, .z = -62.648438},
//     .mag_scale = {.x = 1.513449, .y = 1.557811, .z = 1.434039},
//     .accel_offset = {.x = 0.020900, .y = 0.014688, .z = -0.002580},
//     .accel_scale_lo = {.x = -0.992052, .y = -0.990010, .z = -1.011147},
//     .accel_scale_hi = {.x = 1.013558, .y = 1.011903, .z = 1.019645},

//     // .gyro_bias_offset = {.x = 0.303956, .y = -1.049768, .z = -0.403782}};
//     .gyro_bias_offset = {.x = 0.320405, .y = -0.977974, .z = -0.439018}
// };

#define CAM_PIN_PWDN -1
#define CAM_PIN_RESET 13
#define CAM_PIN_XCLK 23 // MCLK
#define CAM_PIN_SIOD 5
#define CAM_PIN_SIOC 18

#define CAM_PIN_D7 22
#define CAM_PIN_D6 26
#define CAM_PIN_D5 25
#define CAM_PIN_D4 32
#define CAM_PIN_D3 34
#define CAM_PIN_D2 36
#define CAM_PIN_D1 39
#define CAM_PIN_D0 35
#define CAM_PIN_VSYNC 15
#define CAM_PIN_HREF 19 // HSYNC
#define CAM_PIN_PCLK 33

static const char *TAG = "robot_main";

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    // XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565, // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QVGA,     // QQVGA-UXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 12, // 0-63 lower number means higher quality
    .fb_count = 1,      // if more than one, i2s runs in continuous mode. Use only with JPEG
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

static esp_err_t init_camera()
{
    // initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}


void app_main()
{
   
    // Init i2cdev library
    // ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    robot_controller_init();

    init_camera();
    wifi_init();
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);
    
    // TEST
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