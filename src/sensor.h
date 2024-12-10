#include <stdio.h> 
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0 // I2C port number

#define BH1750_SENSOR_ADDR 0x23 // BH1750 I2C address
#define BH1750_CMD_START 0x10 // BH1750 start measurement command

float read_bh1750();
