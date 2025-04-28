#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"

const char *SSID = "Silva";
const char *PASSWORD = "12345678";

#define LED_PIN 12

int connectWifi(void){
    int ret;
    do{
        printf("Tentando conectar a Rede: %s\n");
        ret = cyw43_arch_wifi_connect_timeout_ms(SSID, PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
    } while(ret != 0);

    printf("conectado com sucesso");
    gpio_put(LED_PIN, 1);
    return ret;
}

void monitorwifi(void){
    while(true){
        if(netif_default == NULL || (netif_default->flags & NETIF_FLAG_LINK_UP)){
            printf("Conexão perdid, tentando reconectar");
            gpio_put(LED_PIN, 0);
            connectWifi();
        }
        sleep_ms(1000);

    }
}

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    if(cyw43_arch_init()){
        printf("Erro ao iniciar o hardware wifi");
        return -1;
    }
    cyw43_arch_enable_sta_mode();

    connectWifi();
    monitorwifi();

    return 0;
}
