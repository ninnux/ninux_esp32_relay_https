
#include "ninux_esp32_relay_https.h"
#include "driver/gpio.h"

static const char *TAG = "ninux https";
#define GPIO_PORTS ((1ULL<<18)|(1ULL<<19))
void my_gpio_init(int ioports){
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    //io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pin_bit_mask = ioports;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void gpio_out(int ioport,int value){
    gpio_set_level(ioport, value);
}

/* An HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t *req)
{
if(!esp32_web_basic_auth(req)){
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Hello Secure World2!</h1>", -1); // -1 = use strlen()
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
    gpio_out(18,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>0 on</h1>", -1); // -1 = use strlen()
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
    gpio_out(18,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>0 off</h1>", -1); // -1 = use strlen()
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
    gpio_out(18,0);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_out(18,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>0 reset</h1>", -1); // -1 = use strlen()
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
    gpio_out(19,1);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>1 on</h1>", -1); // -1 = use strlen()
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
    gpio_out(19,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>1 off</h1>", -1); // -1 = use strlen()
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
    gpio_out(19,0);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_out(19,0);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>1 reset</h1>", -1); // -1 = use strlen()
}
else{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
}
    return ESP_OK;
}
static const httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler
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

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server");

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
        ESP_LOGI(TAG, "Error starting server!");
        return NULL;
    }
    my_gpio_init(GPIO_PORTS);
    // Set URI handlers
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &zero_on);
    httpd_register_uri_handler(server, &zero_off);
    httpd_register_uri_handler(server, &zero_reset);
    httpd_register_uri_handler(server, &one_on);
    httpd_register_uri_handler(server, &one_off);
    httpd_register_uri_handler(server, &one_reset);
    return server;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_ssl_stop(server);
}


void ninux_esp32_https_task(void * pvParameter)
{
    ESP_LOGI(TAG, "ninux https web server");

    /* Wait for the callback to set the CONNECTED_BIT in the
       event group.
    */
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to WiFi network! Attempting to connect to server...");

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
