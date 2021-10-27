#include "robot_controller.h"
#include "tcp.h"
#include "servo_controller.h"

static const char *ROBOT_TAG = "robot_controller";

void robot_event_handler(void *handler_args, esp_event_base_t event_base, int32_t event_id, void *event_data){

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGW(ROBOT_TAG, "WIFI connected and got IP");
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

void robot_controller_init(){
    tcp_read_callback = read_callback;
    tcp_write_callback = write_callback;

    init_servo_controller();
    soft_servo_stop();
    set_servo_psu(0);
    ESP_LOGW(ROBOT_TAG, "Controller is ready");
}