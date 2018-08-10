deps_config := \
	/Users/gideon/esp/esp-idf/components/app_trace/Kconfig \
	/Users/gideon/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/gideon/esp/esp-idf/components/bt/Kconfig \
	/Users/gideon/esp/esp-idf/components/driver/Kconfig \
	/Users/gideon/esp/esp-idf/components/esp32/Kconfig \
	/Users/gideon/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/Users/gideon/esp/esp-idf/components/esp_http_client/Kconfig \
	/Users/gideon/esp/esp-idf/components/ethernet/Kconfig \
	/Users/gideon/esp/esp-idf/components/fatfs/Kconfig \
	/Users/gideon/esp/esp-idf/components/freertos/Kconfig \
	/Users/gideon/esp/esp-idf/components/heap/Kconfig \
	/Users/gideon/esp/esp-idf/components/http_server/Kconfig \
	/Users/gideon/esp/esp-idf/components/libsodium/Kconfig \
	/Users/gideon/esp/esp-idf/components/log/Kconfig \
	/Users/gideon/esp/esp-idf/components/lwip/Kconfig \
	/Users/gideon/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/gideon/esp/esp-idf/components/mdns/Kconfig \
	/Users/gideon/esp/esp-idf/components/openssl/Kconfig \
	/Users/gideon/esp/esp-idf/components/pthread/Kconfig \
	/Users/gideon/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/gideon/esp/esp-idf/components/spiffs/Kconfig \
	/Users/gideon/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/gideon/esp/esp-idf/components/vfs/Kconfig \
	/Users/gideon/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/gideon/esp/esp-idf/Kconfig.compiler \
	/Users/gideon/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/gideon/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/gideon/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/gideon/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
