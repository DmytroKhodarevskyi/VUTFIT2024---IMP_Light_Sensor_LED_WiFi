#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "store"
// helper function to initialize the nvs
void init_nvs();

// esp_err_t save_to_nvs(const char *key, uint16_t value);
esp_err_t save_to_nvs(const char *key, uint16_t value);

uint16_t read_from_nvs(const char *key, esp_err_t *result);

esp_err_t remove_from_nvs(const char *key);