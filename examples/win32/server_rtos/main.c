/**
 * \file            main.c
 * \brief           Main file
 */

/*
 * Copyright (c) 2018 Tilen Majerle
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of ESP-AT.
 *
 * Before you start using WIN32 implementation with USB and VCP,
 * check esp_ll_win32.c implementation and choose your COM port!
 */
#include "stdafx.h"
#include "esp/esp.h"
#include "station_manager.h"
#include "netconn_client.h"

static espr_t esp_callback_func(esp_evt_t* evt);
static espr_t esp_server_callback_func(esp_evt_t* evt);

/**
 * \brief           Program entry point
 */
int
main(void) {
    printf("Starting ESP application!\r\n");

    /*
     * Initialize ESP with default callback function
     */
    esp_init(esp_callback_func, 1);

    /*
     * Connect to access point.
     *
     * Try unlimited time until access point accepts up.
     * Check for station_manager.c to define preferred access points ESP should connect to
     */
    connect_to_preferred_access_point(1);

    /*
     * Start server on port 80
     */
    esp_set_server(1, 80, ESP_CFG_MAX_CONNS, 0, esp_server_callback_func, 1);

    /*
     * Do not stop program here as we still need to wait
     * for commands to be processed
     */
    while (1) {
        esp_delay(1000);
    }

    return 0;
}

/**
 * \brief           Callback function for server connection events
 * \param[in]       evt: Event information with data
 * \return          espOK on success, member of \ref espr_t otherwise
 */
static espr_t
esp_server_callback_func(esp_evt_t* evt) {
    esp_conn_p conn;

    conn = esp_conn_get_from_evt(evt);          /* Get connection handle from event */
    switch (esp_evt_get_type(evt)) {
        case ESP_EVT_CONN_ACTIVE: {             /* Connection just active */
            printf("Connection %d active as server!\r\n", (int)esp_conn_getnum(conn));
            break;
        }
        case ESP_EVT_CONN_DATA_RECV: {          /* Connection data received */
            esp_pbuf_p p;
            p = esp_evt_conn_data_recv_get_buff(evt);   /* Get received buffer */
            if (p != NULL) {
                printf("Server connection %d data received with %d bytes\r\n",
                    (int)esp_conn_getnum(conn), (int)esp_pbuf_length(p, 1));
            }
            esp_conn_close(conn, 0);            /* Close connection */
            break;
        }
        case ESP_EVT_CONN_CLOSED: {             /* Connection closed */
            printf("Server connection %d closed!\r\n", (int)esp_conn_getnum(conn));
            break;
        }
    }
    return espOK;
}

/**
* \brief           Event callback function for ESP stack
* \param[in]       evt: Event information with data
* \return          espOK on success, member of \ref espr_t otherwise
*/
static espr_t
esp_callback_func(esp_evt_t* evt) {
    switch (esp_evt_get_type(evt)) {
        case ESP_EVT_AT_VERSION_NOT_SUPPORTED: {
            esp_sw_version_t v_min, v_curr;

            esp_get_min_at_fw_version(&v_min);
            esp_get_current_at_fw_version(&v_curr);

            printf("Current ESP8266 AT version is not supported by library!\r\n");
            printf("Minimum required AT version is: %d.%d.%d\r\n", (int)v_min.major, (int)v_min.minor, (int)v_min.patch);
            printf("Current AT version is: %d.%d.%d\r\n", (int)v_curr.major, (int)v_curr.minor, (int)v_curr.patch);
            break;
        }
        case ESP_EVT_AT_VERSION_NOT_SUPPORTED: {
            esp_sw_version_t v_min, v_curr;

            esp_get_min_at_fw_version(&v_min);
            esp_get_current_at_fw_version(&v_curr);

            printf("Current ESP8266 AT version is not supported by library!\r\n");
            printf("Minimum required AT version is: %d.%d.%d\r\n", (int)v_min.major, (int)v_min.minor, (int)v_min.patch);
            printf("Current AT version is: %d.%d.%d\r\n", (int)v_curr.major, (int)v_curr.minor, (int)v_curr.patch);
            break;
        }
        case ESP_EVT_INIT_FINISH: {
            printf("Library initialized!\r\n");
            break;
        }
        case ESP_EVT_RESET: {
            printf("Device reset detected!\r\n");
            break;
        }
        default: break;
    }
    return espOK;
}