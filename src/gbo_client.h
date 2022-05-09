
/**
 * @file gbo_client.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief HTTP getter
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "ModbusClientTCP.h"
#include "gbo_data.h"

/**
 * @brief http client for energy status retrieval
 * 
 */
class GBOClient {
private:
    WiFiClient          _wifi;
    ModbusClientTCP     *_mbc {nullptr};
    uint8_t             _rotation{0};
public:

    GBOData* getGBOData();

    bool init(const char* ssid, const char* passwd, const char* ip);

    bool isConnected();

    bool getData();

    bool isBlocked();
    
    static void handleData(ModbusMessage response, uint32_t token);

    static void handleError(Error error, uint32_t token);

    void chnageInterval1();

    void chnageInterval2();
    
    void chnageMode();

    void sendMagic();

};