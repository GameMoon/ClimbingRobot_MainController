#include "robot_controller.h"
#include "tcp.h"
#include "servo_controller.h"
#include "ws2812.h"
#include "wifi_setup.h"
#include "cam_controller.h"

static const char *ROBOT_TAG = "robot_controller";
uint8_t wifi_try = 0;

void robot_event_handler(void *handler_args, esp_event_base_t event_base, int32_t event_id, void *event_data){

    if (event_base == IP_EVENT && (event_id == IP_EVENT_STA_GOT_IP || event_id == IP_EVENT_AP_STAIPASSIGNED))
    {
        ESP_LOGW(ROBOT_TAG, "WIFI or client connected and got IP");
        robot_status = 2;
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (wifi_try == ESP_MAXIMUM_RETRY)
        {
            robot_status = 0;
        }
        else wifi_try++;
    }
}

void read_callback(uint8_t *data, uint8_t len){
    ESP_LOGI(ROBOT_TAG, "INCOMING DATA");
    if(len == 14){
        ESP_LOGW(ROBOT_TAG, "Data: %d", data[12]>>4);
        //set_servo_psu((data[12] & 0x10) >> 4);
        set_servo_positions(data);
        set_coils(data[12]);
    }
    else if(len == 2){
        set_single_servo_position(data[0], data[1]);
    }
}

void write_callback(int socket){
    read_servo_positions(&servo_positions);
    send(socket, servo_positions, NUMBER_OF_SERVOS,0);
}

void client_connected(){
    robot_status = 3;
    ESP_LOGI(ROBOT_TAG, "Client connected");
    set_servo_psu(1);
}

void client_disconnected(){
    robot_status = 2;
}


void robot_controller_init(){
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT_STA_GOT_IP, ESP_EVENT_ANY_ID, robot_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, robot_event_handler, NULL, NULL));

    servoConn.read_callback = read_callback;
    servoConn.write_callback = write_callback;
    servoConn.client_connected = client_connected;
    servoConn.client_disconnected = client_disconnected;
    servoConn.port = SERVO_CONN_PORT;
    
    camConn.read_callback = cam_read_callback;
    camConn.write_callback = cam_write_callback;
    camConn.client_connected = cam_client_connected;
    camConn.client_disconnected = cam_client_disconnected;
    camConn.port = CAM_CONN_PORT;

    init_servo_controller();
    init_camera();
    //soft_servo_stop();
    //set_servo_psu(1);
    xTaskCreate(led_status_task, "led_status", 1000, NULL, 5, NULL);
    ESP_LOGW(ROBOT_TAG, "Controller is ready");
}

void led_status_task(void *pvParameters)
{
    uint8_t old_status = 0;

    rgbVal color = makeRGBVal(0, 0, 0);
    ws2812_init(GPIO_NUM_0);
    ws2812_setColors(1, &color);

    while (1)
    {
        if (robot_status == 0)
        {
            color.r = 255;
            color.g = 0;
            color.b = 0;
            ws2812_setColors(1, &color);
        }
        else if (robot_status == 1)
        {
            color.r = 255;
            color.g = 255;
            color.b = 0;
            ws2812_setColors(1, &color);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            color.r = 0;
            color.g = 0;
            ws2812_setColors(1, &color);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else if (robot_status == 2 && old_status != robot_status)
        {
            color.r = 0;
            color.g = 255;
            color.b = 0;
            ws2812_setColors(1, &color);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            color.g = 0;
            ws2812_setColors(1, &color);
            old_status = robot_status;
        }
        else if (robot_status == 3)
        {
            color.r = 0;
            color.g = 0;
            color.b = 255;
            ws2812_setColors(1, &color);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            color.b = 0;
            ws2812_setColors(1, &color);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
        else
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        if (old_status == 2 && robot_status != 2)
            old_status = robot_status;
    }
    vTaskDelete(NULL);
}
