#include "robot_controller.h"
#include "tcp.h"
#include "servo_controller.h"
#include "ws2812.h"

static const char *ROBOT_TAG = "robot_controller";

void robot_event_handler(void *handler_args, esp_event_base_t event_base, int32_t event_id, void *event_data){

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGW(ROBOT_TAG, "WIFI connected and got IP");
        robot_status = 2;
    }
}

void read_callback(uint8_t *data, uint8_t len){
    ESP_LOGI(ROBOT_TAG, "INCOMING DATA");
    if(len == 14){
        ESP_LOGW(ROBOT_TAG, "Data: %d", data[12]>>4);
        set_servo_psu((data[12] & 0x10) >> 4);
        set_servo_positions(data);
    }
}

void write_callback(int socket){
    read_servo_positions(&servo_positions);
    send(socket, servo_positions, NUMBER_OF_SERVOS,0);
}

void client_connected(){
    robot_status = 3;
    ESP_LOGI(ROBOT_TAG, "Client connected");
}

void client_disconnected(){
    robot_status = 2;
}

void robot_controller_init(){
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT_STA_GOT_IP, ESP_EVENT_ANY_ID, robot_event_handler, NULL, NULL));

    tcp_read_callback = read_callback;
    tcp_write_callback = write_callback;
    tcp_client_connected = client_connected;
    tcp_client_disconnected = client_disconnected;

    init_servo_controller();
    //soft_servo_stop();
    //set_servo_psu(1);
    xTaskCreate(led_status_task, "led_status", 1000, (void *)AF_INET, 5, NULL);
    ESP_LOGW(ROBOT_TAG, "Controller is ready");
}

void led_status_task(void *pvParameters)
{
    uint8_t old_status = 0;
    
    rgbVal color = makeRGBVal(0, 0, 0);
    ws2812_init(GPIO_NUM_0);
    ws2812_setColors(1, &color);
    while(1){

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
        }
        else if (robot_status == 2){
            color.r = 0;
            color.g = 255;
            color.b = 0;
            ws2812_setColors(1, &color);
        }
        else if(robot_status == 3){
            color.r = 0;
            color.g = 0;
            color.b = 255;
            ws2812_setColors(1, &color);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            color.b = 0;
            ws2812_setColors(1, &color);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(NULL);
}