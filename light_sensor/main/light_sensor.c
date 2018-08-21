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

#define I2C_MASTER_SCL 					   18               /*!< GPIO for I2C master clock */
#define I2C_MASTER_SDA                     19               /*!< GPIO for I2C master data */
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000           /*!< I2C master clock frequency */

#define BH1750_SENSOR_ADDR                 0x23             /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START                   0x23             /*!< Command to set measure mode */
#define ESP_SLAVE_ADDR                     0x28             /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

static void i2c_master_init()
{
	int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;

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
