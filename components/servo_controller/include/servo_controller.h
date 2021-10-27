
#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include "driver/i2c.h"

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ    /*!< I2C master read */
#define ACK_CHECK_EN 0x1            /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0           /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                 /*!< I2C ack value */
#define NACK_VAL 0x1                /*!< I2C nack value */

#define READ_POS_REG 0x01 // Read all servo positions
#define WRITE_ALL_REG 0x02 // Set all servo position
#define WRITE_SINGLE_REG 0x03 //Set single servo position
#define WRITE_COIL 0x04 //Coil status modification

#define PSU_PIN 21



void init_servo_controller();
void deinit_servo_controller();

void set_servo_psu(uint8_t gpio_state);
void soft_servo_stop();

uint8_t read_servo_positions(uint8_t * data);
uint8_t set_single_servo_position(uint8_t index, uint8_t value);
uint8_t set_servo_positions(uint8_t * positions);
uint8_t set_coils(uint8_t value);


#endif