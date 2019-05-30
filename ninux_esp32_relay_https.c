
#include "ninux_esp32_relay_https.h"
#include "ninux_esp32_ota.h"
//#include "driver/gpio.h"
#include "ninux_esp32_mqtt.h"

static const char *TAG_HTTP = "ninux https";

//#define ZERO_PIN 23
//#define ONE_PIN 17 


//void gpio_out(int ioport,int value){
//    gpio_pad_select_gpio(ioport);
//    /* Set the GPIO as a push/pull output */
//    gpio_set_direction(ioport, GPIO_MODE_OUTPUT);
//    gpio_set_level(ioport, value);
//}
//
//void do_action(char* portstr,char* action){
//	int port;
//	int i;
//    	char mqtt_topic[512];
//    	bzero(mqtt_topic,sizeof(mqtt_topic));
//	if(strcmp(portstr,"all")==0){
//		//for(i=MIN_PORT;i<MAX_PORT+1;i++){
//		//	do_action((char*)i,action);
//		//}
//		if(strcmp(action,"on")==0){
//			gpio_out(ZERO_PIN,0);
//			gpio_out(ONE_PIN,0);
//		}
//		if(strcmp(action,"off")==0){
//			gpio_out(ZERO_PIN,1);
//			gpio_out(ONE_PIN,1);
//		}
//		if(strcmp(action,"reset")==0){
//			gpio_out(ZERO_PIN,1);
//			gpio_out(ONE_PIN,1);
//			vTaskDelay(10000 / portTICK_PERIOD_MS);
//			gpio_out(ZERO_PIN,0);
//			gpio_out(ONE_PIN,0);
//		}
//		
//	}else{
//        port=atoi(portstr);
//        if(port <= MAX_PORT && port >= MIN_PORT ){
//		if(port==0 && strcmp(action,"on")==0){
//			gpio_out(ZERO_PIN,0);
//		}
//		if(port==0 && strcmp(action,"off")==0){
//			gpio_out(ZERO_PIN,1);
//		}
//		if(port==0 && strcmp(action,"reset")==0){
//			gpio_out(ZERO_PIN,1);
//			vTaskDelay(10000 / portTICK_PERIOD_MS);
//			gpio_out(ZERO_PIN,0);
//		}
//		if(port==1 && strcmp(action,"on")==0){
//			gpio_out(ONE_PIN,0);
//		}
//		if(port==1 && strcmp(action,"off")==0){
//			gpio_out(ONE_PIN,1);
//		}
//		if(port==1 && strcmp(action,"reset")==0){
//			gpio_out(ONE_PIN,1);
//			vTaskDelay(10000 / portTICK_PERIOD_MS);
//			gpio_out(ONE_PIN,0);
//		}
//	}
//	}
//	sprintf(mqtt_topic,"controllo/lampadasanfelice/ports/%s",portstr);
//        ninux_mqtt_publish(mqtt_topic,action);
//}
//
/////* An HTTP GET handler */
//static esp_err_t update_get_handler(httpd_req_t *req)
//{
//if(!esp32_web_basic_auth(req)){
//    httpd_resp_set_type(req, "text/html");
//    httpd_resp_send(req, "<h1>Hello Secure World2!</h1>", -1); // -1 = use strlen()
//    ninux_esp32_ota();
//
//}
//else{
//    httpd_resp_set_type(req, "text/html");
//    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
//}
//    return ESP_OK;
//}
//
//static esp_err_t port_management_handler(httpd_req_t *req)
//{
//if(!esp32_web_basic_auth(req)){
//    char myuri[512];
//    char http_message[512];
//    char command[8];
//    char *campo;
//    char portstr[8];
//    int port;
//    bzero(http_message,sizeof(http_message));
//    memcpy(myuri,req->uri,sizeof(myuri));
//    campo=strtok(myuri,"/");
//    printf("campo:%s\n",campo);
//    if(strcmp(campo,"ports")==0){
//        campo=strtok(NULL,"/");
//    	printf("campo:%s\n",campo);
//	memcpy(portstr,campo,strlen(campo));
//        port=atoi(campo);
//        if((port <= MAX_PORT && port >= MIN_PORT )|| strcmp(campo,"all")==0){
//            campo=strtok(NULL,"/");
//    	    printf("campo:%s\n",campo);
//            if(strcmp(campo,"on")==0 || strcmp(campo,"off")==0 || strcmp(campo,"reset")==0){
//                do_action(portstr,campo);
//		printf("do_action %d %s\n",port,campo);
//                httpd_resp_set_type(req, "text/html");
//                sprintf(http_message,"<h1>%d %s</h1>",port,campo);
//                httpd_resp_send(req, http_message, -1);
//            }else{
//                httpd_resp_set_type(req, "text/html");
//                httpd_resp_send(req, "<h1>command error</h1>", -1); // -1 = use strlen()
//            }
//
//         }else{
//                httpd_resp_set_type(req, "text/html");
//                httpd_resp_send(req, "<h1>port unrecognized</h1>", -1); // -1 = use strlen()
//         }
//     }else{
//        httpd_resp_set_type(req, "text/html");
//        httpd_resp_send(req, "<h1>invalid path</h1>", -1); // -1 = use strlen()
//
//     }
//}
//else{
//    httpd_resp_set_type(req, "text/html");
//    httpd_resp_send(req, "<h1>Culo2!</h1>", -1); // -1 = use strlen()
//}
//
//    return ESP_OK;
//}
//
//
//static const httpd_uri_t update = {
//    .uri       = "/update",
//    .method    = HTTP_GET,
//    .handler   = update_get_handler
//};
//
//static const httpd_uri_t port_management = {
//    .uri       = "/ports/*",
//    .method    = HTTP_GET,
//    .handler   = port_management_handler
//};

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
    //my_gpio_init(GPIO_PORTS);
    // Set URI handlers
    ESP_LOGI(TAG_HTTP, "Registering URI handlers");
    int i=0;
    for(i=0;i<sizeof(handlers);i++){
    	httpd_register_uri_handler(server, handlers[i]);
    }
    //httpd_register_uri_handler(server, &update);
    //httpd_register_uri_handler(server, &port_management);
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


//    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
 //   ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
}
