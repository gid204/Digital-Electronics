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

#define DATA_LENGTH                        512              /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH                     129              /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define DELAY_TIME_BETWEEN_ITEMS_MS        10000             /*!< delay time between different test items */


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

#define STORAGE_NAMESPACE "storage"

int32_t SensorValue = 0;

SemaphoreHandle_t print_mux = NULL;



/**
 * @brief i2c master initialization
 */
static void i2c_master_init()
{
	int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
	i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF, I2C_MASTER_TX_BUF, 0);
}

/**
 * @brief test code to write esp-i2c-slave
 *
 * 1. set mode
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
static esp_err_t i2c_master_sensor_test(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, BH1750_CMD_START, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t save_sensor_value()
{
	nvs_handle my_handle;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Open
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) return err;

	// Read
		printf("Reading light intensity from NVS ... ");
		int32_t lightSensor = 0; // value will default to 0, if not set yet in NVS
		err = nvs_get_i32(my_handle, "lightSensor", &lightSensor);
		switch (err) {
			case ESP_OK:
				printf("Done\n");
				printf("Light Intensity = %d\n", lightSensor);
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				printf("The value is not initialized yet!\n");
				break;
			default :
				printf("Error (%s) reading!\n", esp_err_to_name(err));
		}

	// Write
	printf("Updating light intensity reading in NVS with = %d\n", SensorValue);
	err = nvs_set_i32(my_handle, "lightSensor", SensorValue);
	printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
	// Commit written value.
	// After setting any values, nvs_commit() must be called to ensure changes are written
	// to flash storage. Implementations may write to storage at other times,
	// but this is not guaranteed.
	printf("Committing updates in NVS ... ");
	err = nvs_commit(my_handle);
	printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

	// Close
	nvs_close(my_handle);

	printf("\n");
	fflush(stdout);
	//esp_restart();
	return ESP_OK;
}


/**
 * @brief i2c task to read data from sensor
 */
static void i2c_task(void* arg)
{
	esp_err_t err;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    while (1) {
        printf("test cnt: %d\n", cnt++);
        ret = i2c_master_sensor_test( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if(ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
        } else if(ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ SENSOR( BH1750 )\n", task_idx);
            printf("*******************\n");
            printf("data_h: %02x\n", sensor_data_h);
            printf("data_l: %02x\n", sensor_data_l);
            int32_t sensor_val = ((sensor_data_h << 8 | sensor_data_l) / 1.2);
            SensorValue = sensor_val;
            printf("sensor val: %d\n", sensor_val);
        } else {
            printf("%s: No ack, sensor not connected...skip...\n", esp_err_to_name(ret));
        }

        err = save_sensor_value();
       	if (err != ESP_OK) printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));

        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);
    }
}



void app_main()
{

    print_mux = xSemaphoreCreateMutex();
    i2c_master_init();
    xTaskCreate(i2c_task, "i2c_task_0", 1024 * 2, (void* ) 0, 10, NULL);

}
