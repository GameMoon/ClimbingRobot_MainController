#ifndef CAM_CONTROLLER_H
#define CAM_CONTROLLER_H

#include "esp_camera.h"

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



esp_err_t init_camera();

void cam_write_callback(int socket);
void cam_read_callback();
void cam_client_connected();
void cam_client_disconnected();

#endif