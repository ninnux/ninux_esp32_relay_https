
#include "ninux_esp32_relay_https.h"
#include "ninux_esp32_ota.h"
#include "driver/gpio.h"
#include "ninux_esp32_mqtt.h"

static const char *TAG_HTTP = "ninux https";

#define ZERO_PIN 23
#define ONE_PIN 17 


void gpio_out(int ioport,int value){
    gpio_pad_select_gpio(ioport);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(ioport, GPIO_MODE_OUTPUT);
    gpio_set_level(ioport, value);
}

/* An HTTP GET handler */
static esp_err_t update_get_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Hello Secure World2!</h1>", -1); // -1 = use strlen()
    ninux_esp32_ota();

}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}

static esp_err_t zero_on_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ZERO_PIN,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>0 on</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/0","on");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static esp_err_t zero_off_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ZERO_PIN,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>0 off</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/0","off");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static esp_err_t zero_reset_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ZERO_PIN,0);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_out(ZERO_PIN,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>0 reset</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/0","off");
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/0","on");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}

static esp_err_t one_on_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ONE_PIN,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>1 on</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/1","on");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static esp_err_t one_off_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ONE_PIN,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>1 off</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/1","off");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static esp_err_t one_reset_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ONE_PIN,0);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_out(ONE_PIN,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>1 reset</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/1","off");
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/1","on");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static esp_err_t all_on_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ONE_PIN,1);
    gpio_out(ZERO_PIN,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>all off</h1>", -1); // -1 = use strlen()
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static esp_err_t all_off_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    gpio_out(ONE_PIN,0);
    gpio_out(ZERO_PIN,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>all off</h1>", -1); // -1 = use strlen()
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/0","off");
    ninux_mqtt_publish("controllo/lampadasanfelice/ports/1","off");
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static const httpd_uri_t update = {
    .uri       = "/update",
    .method    = HTTP_GET,
    .handler   = update_get_handler
};


static const httpd_uri_t zero_on = {
    .uri       = "/0/on",
    .method    = HTTP_GET,
    .handler   = zero_on_handler
};

static const httpd_uri_t zero_off = {
    .uri       = "/0/off",
    .method    = HTTP_GET,
    .handler   = zero_off_handler
};

static const httpd_uri_t zero_reset = {
    .uri       = "/0/reset",
    .method    = HTTP_GET,
    .handler   = zero_reset_handler
};

static const httpd_uri_t one_on = {
    .uri       = "/1/on",
    .method    = HTTP_GET,
    .handler   = one_on_handler
};

static const httpd_uri_t one_off = {
    .uri       = "/1/off",
    .method    = HTTP_GET,
    .handler   = one_off_handler
};


static const httpd_uri_t one_reset = {
    .uri       = "/1/reset",
    .method    = HTTP_GET,
    .handler   = one_reset_handler
};

static const httpd_uri_t all_on = {
    .uri       = "/all/on",
    .method    = HTTP_GET,
    .handler   = all_on_handler
};

static const httpd_uri_t all_off = {
    .uri       = "/all/off",
    .method    = HTTP_GET,
    .handler   = all_off_handler
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;

    // Start the httpd server
    ESP_LOGI(TAG_HTTP, "Starting server");

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();

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
    //my_gpio_init(GPIO_PORTS);
    // Set URI handlers
    ESP_LOGI(TAG_HTTP, "Registering URI handlers");
    httpd_register_uri_handler(server, &update);
    httpd_register_uri_handler(server, &zero_on);
    httpd_register_uri_handler(server, &zero_off);
    httpd_register_uri_handler(server, &zero_reset);
    httpd_register_uri_handler(server, &one_on);
    httpd_register_uri_handler(server, &one_off);
    httpd_register_uri_handler(server, &one_reset);
    //httpd_register_uri_handler(server, &all_on);
    httpd_register_uri_handler(server, &all_off);
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

    start_webserver();
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void ninux_esp32_https()
{
    static httpd_handle_t server = NULL;

    xTaskCreate(&ninux_esp32_https_task, "ninux_esp32_https_task", 8192, NULL, 5, NULL);


//    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
 //   ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
}
