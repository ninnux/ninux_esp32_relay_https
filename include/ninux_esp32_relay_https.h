#ifndef NINUX_RELAY_HTTPS
#define NINUX_RELAY_HTTPS

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

//#include <esp_wifi.h>
//#include <esp_event_loop.h>
#include <esp_log.h>
//#include <esp_system.h>
//#include <nvs_flash.h>
//#include <sys/param.h>
//#include "tcpip_adapter.h"
//#include "esp_eth.h"
//#include "protocol_examples_common.h"

#include <esp_https_server.h>
#include "esp32_web_basic_auth.h"

extern EventGroupHandle_t wifi_event_group;
extern const int CONNECTED_BIT;


static esp_err_t update_get_handler(httpd_req_t *req);
static esp_err_t zero_on_handler(httpd_req_t *req);
static esp_err_t zero_off_handler(httpd_req_t *req);
static esp_err_t zero_reset_handler(httpd_req_t *req);

static esp_err_t one_on_handler(httpd_req_t *req);
static esp_err_t one_off_handler(httpd_req_t *req);
static esp_err_t one_reset_handler(httpd_req_t *req);

static esp_err_t all_off_handler(httpd_req_t *req);

void gpio_out(int ioport,int value);
static httpd_handle_t start_webserver(void);
static void stop_webserver(httpd_handle_t server);
void ninux_esp32_https_task(void * pvParameter);
void ninux_esp32_https();
#endif
