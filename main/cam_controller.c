#include "cam_controller.h"
#include "esp_log.h"
#include "tcp.h"

static const char *CAMERA_TAG = "camera";

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

    .pixel_format = PIXFORMAT_GRAYSCALE, // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_VGA, // QQVGA-UXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 12, // 0-63 lower number means higher quality
    .fb_count = 1,      // if more than one, i2s runs in continuous mode. Use only with JPEG
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

esp_err_t init_camera()
{
    // initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(CAMERA_TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

void cam_write_callback(int socket){
    ESP_LOGI(CAMERA_TAG, "cam_write");

    camera_fb_t *pic = esp_camera_fb_get();

    int sent = send(socket, pic->buf,pic->len, 0);
    ESP_LOGI(CAMERA_TAG, "Picture taken! Its size was: %zu bytes, %d", pic->len, sizeof(camera_fb_t));
    ESP_LOGI(CAMERA_TAG, "Width: %d, Height: %d", pic->width, pic->height);
    ESP_LOGI(CAMERA_TAG, "Sent: %d, %d", sent, socket);
    esp_camera_fb_return(pic);
}

void cam_read_callback(){
    ESP_LOGI(CAMERA_TAG, "cam_read");
}
void cam_client_connected(){
    ESP_LOGI(CAMERA_TAG, "Client connected to camera");
}

void cam_client_disconnected(){
    ESP_LOGI(CAMERA_TAG, "Client disconnected from the camera");
}