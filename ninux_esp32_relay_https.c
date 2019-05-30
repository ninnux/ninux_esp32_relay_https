
#include "ninux_esp32_relay_https.h"
#include "ninux_esp32_ota.h"
#include "ninux_esp32_mqtt.h"

static const char *TAG_HTTP = "ninux https";

static httpd_handle_t start_webserver(httpd_uri_t * handlers[])
{
    httpd_handle_t server = NULL;

    // Start the httpd server
    ESP_LOGI(TAG_HTTP, "Starting server");

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
    conf.httpd.uri_match_fn = httpd_uri_match_wildcard;

    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
    extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
    conf.cacert_pem = cacert_pem_start;
    conf.cacert_len = cacert_pem_end - cacert_pem_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
    extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
    conf.prvtkey_pem = prvtkey_pem_start;
    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    esp_err_t ret = httpd_ssl_start(&server, &conf);
    if (ESP_OK != ret) {
        ESP_LOGI(TAG_HTTP, "Error starting server!");
        return NULL;
    }
    // Set URI handlers
    ESP_LOGI(TAG_HTTP, "Registering URI handlers");
    int i=0;
    for(i=0;i<sizeof(handlers);i++){
    	httpd_register_uri_handler(server, handlers[i]);
    }
    return server;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_ssl_stop(server);
}


void ninux_esp32_https_task(void * pvParameter)
{
    ESP_LOGI(TAG_HTTP, "ninux https web server");

    /* Wait for the callback to set the CONNECTED_BIT in the
       event group.
    */
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG_HTTP, "Connected to WiFi network! Attempting to connect to server...");

    start_webserver(pvParameter);
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void ninux_esp32_https(httpd_uri_t * handlers[])
{
    static httpd_handle_t server = NULL;

    xTaskCreate(&ninux_esp32_https_task, "ninux_esp32_https_task", 8192, handlers, 5, NULL);
    //xTaskCreate(&ninux_esp32_https_task, "ninux_esp32_https_task", 8192, NULL, 5, NULL);


}
