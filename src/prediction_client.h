
/**
 * @file prediction_client.h
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
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "constants.h"
#include "gbo_data.h"
#include "utils.h"

/**
 * @brief http client for power prediction
 * 
 */
class PredictionClient {
private:
    int         _today {0};
    int         _tomorrow {0};

public:

    int getToday()      { return _today; }
    int getTommorow()   { return _tomorrow; }
   
    bool getData(const char* key, const char* lat, const char* lon, const GBOData& gbo) {
        bool rc = false;

        
        HTTPClient  http;
        String today = Utils::makeDateYYYYMMDD(gbo._day, gbo._month, gbo._year);
        today +=  " 00:00:00";
        
        String url = Constants::forecastServer;
        url.replace(Constants::forecastKey,key);
        url.replace(Constants::forecastLat,lat);
        url.replace(Constants::forecastLon,lon);
        Serial.printf("URL: %s\n", url.c_str());
        http.begin(url); 
        
        auto  httpCode = http.GET(); 
        if(httpCode == HTTP_CODE_OK) {  
            String payload = http.getString();
            Serial.printf("PAYLOAD: %s\n", payload.c_str());
            auto myArray = JSON.parse(payload);
            if (JSON.typeof(myArray) == "array" && myArray.length() == 2) {
                if ((JSON.typeof(myArray[0]) == "array") && (JSON.typeof(myArray[1]) == "array")) {
                    // correct possition
                    Serial.printf("TD: %s\n", today.c_str());
                    if (today.equals((const char*)myArray[0][0])) {
                        Serial.printf("HIT: %s\n", payload.c_str());
                        _today = (int) myArray[0][1];
                        _tomorrow = (int) myArray[1][1];
                    } else if (today == (const char*)myArray[1][0]) {
                        //unknown next day
                         _today = (int) myArray[1][1];
                         _tomorrow = 0;
                    } else {
                        _today = 0;
                        _tomorrow = 0;
                    }

                    rc = true;
                    
                } else {
                _today = -1; 
                _tomorrow = -1;
                }
            } else {
                _today = -1; 
                _tomorrow = -1;
            }
        } else {
           Serial.printf("HTTP CODE: %d\n", httpCode); 
            _today = -2;
            _tomorrow = -2;
        }
        
        http.end();
        return rc;
        
    }
};