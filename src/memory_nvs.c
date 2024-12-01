#include "memory_nvs.h"

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

esp_err_t save_to_nvs(const char *key, uint16_t value) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        err = nvs_set_u16(nvs_handle, key, value);

        if (err == ESP_OK) {
            nvs_commit(nvs_handle);  // Ensure data is written
            fprintf(stderr, "Value saved with key '%s'\n", key);
        }

        nvs_close(nvs_handle);  // Close the handle when done
        return err;
    } else {
        fprintf(stderr, "Saving failed, handle issue\n");
        fprintf(stderr, "Error (%s) opening NVS handle! (write)\n", esp_err_to_name(err));
        return err;
    }
}

uint16_t read_from_nvs(const char *key, esp_err_t *result) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
    uint16_t value = 0;
    if (err == ESP_OK) {
        err = nvs_get_u16(nvs_handle, key, &value);

        if (err == ESP_ERR_NVS_NOT_FOUND) {
            fprintf(stderr, "Value not found for key '%s'\n", key);
            *result = ESP_ERR_NOT_FOUND;
            // nvs_close(nvs_handle);
            return 0;
        } 

        if (err != ESP_OK) {
            fprintf(stderr, "Error (%s) reading!\n", esp_err_to_name(err));
            *result = ESP_FAIL;
            nvs_close(nvs_handle);
            return 0;
        }

        fprintf(stderr, "Value read with key '%s' is %d\n", key, value);
        *result = ESP_OK;
        nvs_close(nvs_handle);
        return value;

    } else {
        fprintf(stderr, "Reading failed, handle issue\n");
        fprintf(stderr, "Error (%s) opening NVS handle! (read)\n", esp_err_to_name(err));
        *result = ESP_FAIL;
        return 0;

    }
}

esp_err_t remove_from_nvs(const char *key) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    err = nvs_erase_key(nvs_handle, key);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) erasing key '%s' from NVS!\n", esp_err_to_name(err), key);
        nvs_close(nvs_handle);
        return err;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) committing changes to NVS!\n", esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    return err;
}