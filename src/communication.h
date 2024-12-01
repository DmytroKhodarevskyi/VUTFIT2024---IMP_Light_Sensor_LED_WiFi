#include <stdio.h> 
#include "esp_log.h"
#include "driver/i2c.h"


#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL  // GPIO for SCL (Clock)
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA  // GPIO for SDA (Data)
#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number
#define I2C_MASTER_FREQ_HZ 100000  // I2C clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0  // I2C master doesn't need buffer
#define I2C_MASTER_RX_BUF_DISABLE 0  // I2C master doesn't need buffer
#define I2C_MASTER_TIMEOUT_MS 1000

/**
 * @brief Initialize the I2C master
 * @return 
 */
esp_err_t i2c_master_init();
