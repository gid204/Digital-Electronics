/* Digital Electronics Spring 2018 Project
 * Gideon kanikevich - 11655988
 *
 * Development board used: ESP32-Devkit-C V4.
 * Light sensor used: BH1750 (model GY-30)
 *
 * ESP32 will utilisie i2c to read data from light sensor and save
 * to NVS (flash). This can then be saved to an external PC as a
 * .csv file using a USB connection.
*/


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define I2C_SLAVE_SCL_IO          		   26               /*!<gpio number for i2c slave clock  */
#define I2C_SLAVE_SDA_IO           		   25               /*!<gpio number for i2c slave data */

#define I2C_MASTER_SCL_IO 				   19               /*!< GPIO for I2C master clock */
#define I2C_MASTER_SDA_IO                  18               /*!< GPIO for I2C master data */
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000           /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF                  0                /*!< I2C master does not need buffer */
#define I2C_MASTER_RX_BUF                  0                /*!< I2C master does not need buffer */

#define BH1750_SENSOR_ADDR                 0x23             /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START                   0x23             /*!< Command to set measure mode */
#define ESP_SLAVE_ADDR                     0x28             /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

/**
 * @brief i2c master initialization
 */
static void i2c_master_init()
{
	int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
	i2c.config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SLAVE_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF, I2C_MASTER_TX_BUF, 0);
}

void app_main()
{
	while(1)
	{
	//countdown
	for (int i = 5; i >= 0; i--) {
	        printf("\nCountdown: %d seconds...\n", i);
	        vTaskDelay(1000 / portTICK_PERIOD_MS);
	    }
	 printf("\nHello_World!\n");
	}
}
