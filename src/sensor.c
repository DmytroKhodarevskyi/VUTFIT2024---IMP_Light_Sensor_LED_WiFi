#include "sensor.h"

float read_bh1750() {

    uint8_t data[2];

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ////////// Measure light pipeline
    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, (BH1750_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, BH1750_CMD_START, true);

    i2c_master_stop(cmd);

    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));

    i2c_cmd_link_delete(cmd);
    //////////

    vTaskDelay(pdMS_TO_TICKS(180));  // Wait for measurement to complete

    cmd = i2c_cmd_link_create();

    ////////// Retrieve measurment pipeline
    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, (BH1750_SENSOR_ADDR << 1) | I2C_MASTER_READ, true);

    i2c_master_read(cmd, data, 2, I2C_MASTER_LAST_NACK);

    i2c_master_stop(cmd);

    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / pdMS_TO_TICKS(1000));

    i2c_cmd_link_delete(cmd);
    //////////

    // Convert data to lux
    int16_t raw_lux = (data[0] << 8) | data[1];
    return raw_lux / 1.2;  // Convert raw value to lux
}