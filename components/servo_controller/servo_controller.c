#include <stdio.h>
#include <esp_log.h>
#include "servo_controller.h"
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"

static gpio_num_t i2c_gpio_sda = 5;
static gpio_num_t i2c_gpio_scl = 18;
static uint32_t i2c_frequency = 400000; // 400 khz
static i2c_port_t i2c_port = I2C_NUM_0;
static uint8_t chip_addr = 0x08;
static const char *SERVO_TAG = "servo_controller";

static uint8_t coil_state = 0;

static esp_err_t i2c_master_driver_initialize(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = i2c_gpio_sda,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = i2c_gpio_scl,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = i2c_frequency,
        //.clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    return i2c_param_config(i2c_port, &conf);
}

void init_servo_controller(){
    //init gpu pin
    gpio_set_direction(PSU_PIN,GPIO_MODE_OUTPUT);
    gpio_set_level(PSU_PIN,0); 

    //init i2c 
    i2c_driver_install(i2c_port, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    i2c_master_driver_initialize();
    //i2c_set_timeout(i2c_port, 1800000);
    i2c_set_timeout(i2c_port, 0xFFFFF);
}

void deinit_servo_controller(){
    i2c_driver_delete(i2c_port);
}

uint8_t read_servo_positions(uint8_t * data){
    uint8_t data_addr = 0x01;
    uint8_t len = 12;
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    
    i2c_master_write_byte(cmd, chip_addr << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data_addr, ACK_CHECK_EN);
    i2c_master_start(cmd);
    
    i2c_master_write_byte(cmd, chip_addr << 1 | READ_BIT, ACK_CHECK_EN);
    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data + len - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_OK) {
        for (int i = 0; i < len; i++) {
            printf("0x%02x ", data[i]);
            if ((i + 1) % 16 == 0) {
                printf("\r\n");
            }
        }
        if (len % 16) {
            printf("\r\n");
        }
        return 1;
    } else if (ret == ESP_ERR_TIMEOUT) {
        ESP_LOGW(SERVO_TAG, "Bus is busy");
    } else {
        ESP_LOGW(SERVO_TAG, "Read failed");
    }
    return 0;
}

uint8_t send_i2c_command(uint8_t *command,uint8_t len){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, chip_addr << 1 | WRITE_BIT, ACK_CHECK_EN);
    
    for (int i = 0; i < len; i++)
    {
        i2c_master_write_byte(cmd, command[i], ACK_CHECK_EN);
    }
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_OK)
    {
        ESP_LOGI(SERVO_TAG, "Write OK");
        return 1;
    }
    else if (ret == ESP_ERR_TIMEOUT)
    {
        ESP_LOGW(SERVO_TAG, "Bus is busy");
    }
    else
    {
        ESP_LOGW(SERVO_TAG, "Write Failed");
    }
    return 0;
}

uint8_t set_single_servo_position(uint8_t index, uint8_t value){
    uint8_t command[] = {WRITE_SINGLE_REG, index, value};
    return send_i2c_command(command,sizeof(command));
}

uint8_t set_servo_positions(uint8_t *positions){
    uint8_t command[13] = {0x00};
    command[0] = WRITE_ALL_REG;

    command[3] = positions[0]; //coxa
    command[2] = positions[1]; //femur
    command[1] = positions[2]; //tiba

    command[4] = positions[3];
    command[5] = positions[4];
    command[6] = positions[5];

    command[7] = positions[6];
    command[8] = positions[7];
    command[9] = positions[8];

    command[12] = positions[9];
    command[11] = positions[10];
    command[10] = positions[11];
    //memcpy(&command[1],positions,12);
    return send_i2c_command(command, sizeof(command));
}

uint8_t set_coils(uint8_t value){
    if(coil_state == value) return 1;

    coil_state = value;
    uint8_t command[] = {WRITE_COIL, value};
    return send_i2c_command(command, sizeof(command));
}

void set_servo_psu(uint8_t state){
    gpio_set_level(PSU_PIN,state);
}

void soft_servo_stop(){
    uint8_t stop_pos[12] = {0x01};
    set_servo_positions(stop_pos);
}