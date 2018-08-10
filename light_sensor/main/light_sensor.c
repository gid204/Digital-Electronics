/* Digital Electronics Spring 2018 Project
 * Gideon kanikevich - 1655988
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
