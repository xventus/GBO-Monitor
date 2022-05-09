/**
 * @file gbo_client.cpp
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief MODBUS getter
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */


#include "gbo_client.h"


// the onDataHandler callback cannot pass the session pointer to the object, 
// so it is necessary to create a global variable
GBOData  gGBOData;

GBOData* GBOClient::getGBOData() {
    return &gGBOData;
}

bool GBOClient::init(const char* ssid, const char* passwd, const char* ip) {
        WiFi.begin(ssid, passwd);
    _mbc = new ModbusClientTCP(_wifi);
    if (_mbc) {
        _mbc->onDataHandler(&handleData);
        _mbc->onErrorHandler(&handleError);
        _mbc->setTimeout(2000, 1000);
        _mbc->begin();
        IPAddress iP;
        iP.fromString(ip);
        _mbc->setTarget(iP, 502);
    }
    
    return (_mbc);
}

bool GBOClient::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

bool GBOClient::isBlocked() {
    return (gGBOData._blockIO || gGBOData._accepted);
}

bool GBOClient::getData() {
    Error err = SUCCESS;

    if (gGBOData._accepted) {
       gGBOData._accepted = false;
       Serial.printf(" - accepted -> send magic\n");
       sendMagic(); 
    }

    _rotation++;
    
    if (_rotation == 1) {
        err = _mbc->addRequest(_rotation, 1, READ_HOLD_REGISTER, 0, 10); // GBO
        if (err!=SUCCESS) {
            ModbusError e(err);
            Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }
    }
    
    if (_rotation == 2) {
        err = _mbc->addRequest(_rotation, 1, READ_HOLD_REGISTER, 20, 36); // GBO
        if (err!=SUCCESS) {
            ModbusError e(err);
            Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }
    }

    if (_rotation == 3) {
        err = _mbc->addRequest(_rotation, 1, READ_INPUT_REGISTER, 0, 45); // GBO
        if (err!=SUCCESS) {
            ModbusError e(err);
            Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }
    }

    if (_rotation >= 3) _rotation = 0;

    return (err == SUCCESS);
}


void GBOClient::handleData(ModbusMessage response, uint32_t token)  {
    uint8_t shift = 3;
    Serial.printf("response: serverID=%d, FC=%d, token=%08X, len=%d:\n", response.getServerID(), response.getFunctionCode(), token, response.size());
    if (token == 1) {
        // time&relay addr:0, size:10 -  FC3
        gGBOData._sec = response[1+shift];
        gGBOData._min = response[0+shift];
        gGBOData._hour = response[3+shift];
        gGBOData._dayweek = response[2+shift];
        gGBOData._day = response[7+shift];
        gGBOData._month = response[6+shift];
        gGBOData._year =  ((uint16_t)response[8+shift] << 8) | response[9+shift];
        gGBOData._relay = ((uint16_t)response[10+shift] << 8) | response[11+shift];
    } else if (token == 2) {
        // confuguration addr:20, size:36 - FC3
        gGBOData._cfg = ((uint16_t)response[0+shift] << 8) | response[1+shift];
        gGBOData._firmware = ((uint16_t)response[14+shift] << 8) | response[15+shift];
        gGBOData._relEnable = ((uint16_t)response[40+shift] << 8) | response[41+shift];
        gGBOData._time2Start0M = response[33+shift];
        gGBOData._time2Start0H = response[32+shift]; 
        gGBOData._time2Stop0M = response[35+shift];
        gGBOData._time2Stop0H = response[34+shift];
        gGBOData._time2Start1M = response[37+shift];
        gGBOData._time2Start1H = response[36+shift]; 
        gGBOData._time2Stop1M = response[39+shift];
        gGBOData._time2Stop1H = response[38+shift];
        gGBOData._hdoStart0M = response[25+shift];
        gGBOData._hdoStart0H = response[24+shift];
        gGBOData._hdoStart1M = response[29+shift];
        gGBOData._hdoStart1H = response[28+shift];
        gGBOData._hdoStop0M = response[27+shift];
        gGBOData._hdoStop0H = response[26+shift];
        gGBOData._hdoStop1M = response[31+shift];
        gGBOData._hdoStop1H = response[30+shift];

        gGBOData._changeInterval = gGBOData._time2Start0M +  gGBOData._time2Start0H + gGBOData._time2Stop0M + gGBOData._time2Stop0H + 
        gGBOData._time2Start1M +  gGBOData._time2Start1H + gGBOData._time2Stop1M + gGBOData._time2Stop1H;
    } else if (token == 3) {
        // measure addr:0, size:45 - FC4
        gGBOData._rawI1 = ((uint16_t)response[0+shift] << 8) | response[1+shift];
        gGBOData._rawI2 = ((uint16_t)response[2+shift] << 8) | response[3+shift];
        gGBOData._rawI3 = ((uint16_t)response[4+shift] << 8) | response[5+shift];
        gGBOData._ssr1 = response[18+1+shift];
        gGBOData._ssr2 = response[36+1+shift];
        gGBOData._ssr3 = response[52+1+shift];
    } else if (token == 100) {
         Serial.printf("change accepted \n");
         gGBOData._accepted = true;
    } else if (token == 200) { 
        gGBOData._blockIO = false;
        Serial.printf("magic accepted \n");
    }
      else {
        /*      response 1st byte -> Unit identifier
                2nd byte -> Function code
                3rd byte -> Number of bytes in the response
        */
        Serial.printf("GBO - unknown  %02X %02X %02X len=%d ", response[0], response[1], response[2], response.size()); 
        for (auto& byte : response) {
            Serial.printf("%02X ", byte);
        }
        Serial.println("");
    }
}



void GBOClient::handleError(Error error, uint32_t token)  {
    ModbusError me(error);
    if (token==100) {
        gGBOData._blockIO = false;
    }

    Serial.printf("Error response: %02X - %s\n", (int)me, (const char *)me);
}

void GBOClient::chnageInterval1() {
    Error err = SUCCESS;
    if (gGBOData._blockIO) return;
   
    gGBOData._blockIO = true;
 
    if (gGBOData._hdoStart0H == gGBOData._time2Start0H  && gGBOData._hdoStart0M == gGBOData._time2Start0M) {
        // star time timer 2 is the same as HDO, now switch to OFF interval
        uint16_t wData[] = {  (uint16_t)(((uint16_t)gGBOData._hdoStop0H << 8) | gGBOData._hdoStop0M), (uint16_t)(((uint16_t)gGBOData._hdoStop0H << 8) | gGBOData._hdoStop0M)  };
        err = _mbc->addRequest(100, 1, WRITE_MULT_REGISTERS, 52, 2, 4, wData);
    } else {

        uint16_t wData[] = {  (uint16_t)(((uint16_t)gGBOData._hdoStart0H << 8) | gGBOData._hdoStart0M), (uint16_t)(((uint16_t)gGBOData._hdoStop0H << 8) | gGBOData._hdoStop0M)  };
        err = _mbc->addRequest(100, 1, WRITE_MULT_REGISTERS, 52, 2, 4, wData);
    }
    
    if (err!=SUCCESS) {
        ModbusError e(err);
        Serial.printf("Error chnageInterval1: %02X - %s\n", (int)e, (const char *)e);
    }

    _rotation = 1;
}


void GBOClient::chnageInterval2() {
    Error err = SUCCESS;
    if (gGBOData._blockIO) return;
    gGBOData._blockIO = true;
    
    if (gGBOData._hdoStart1H == gGBOData._time2Start1H  && gGBOData._hdoStart1M == gGBOData._time2Start1M) {
        // star time timer 2 is the same as HDO, now switch to OFF interval
        uint16_t wData[] = {  (uint16_t)(((uint16_t)gGBOData._hdoStop1H << 8) | gGBOData._hdoStop1M), (uint16_t)(((uint16_t)gGBOData._hdoStop1H << 8) | gGBOData._hdoStop1M)  };
        err = _mbc->addRequest(100, 1, WRITE_MULT_REGISTERS, 56, 2, 4, wData);
    } else {

        uint16_t wData[] = {  (uint16_t)(((uint16_t)gGBOData._hdoStart1H << 8) | gGBOData._hdoStart1M), (uint16_t)(((uint16_t)gGBOData._hdoStop1H << 8) | gGBOData._hdoStop1M)  };
        err = _mbc->addRequest(100, 1, WRITE_MULT_REGISTERS, 56, 2, 4, wData);
    }
    
    if (err!=SUCCESS) {
        ModbusError e(err);
        Serial.printf("Error chnageInterval2: %02X - %s\n", (int)e, (const char *)e);
    }
    _rotation = 1;
}



void GBOClient::chnageMode() {
    Error err = SUCCESS;
    if (gGBOData._blockIO) return;
    gGBOData._blockIO = true;
    if (gGBOData._cfg & (1 << 5)) {
        // clear bit  
        uint16_t wData[] = { (uint16_t)(((uint16_t)(gGBOData._cfg & (~(1 << 5))) | (1 << 13))) };
        err = _mbc->addRequest(100, 1, WRITE_MULT_REGISTERS, 20, 1, 2, wData);
        Serial.printf("chnageMode: CLR %02X\n", wData[0]);
    } else {
        // set bit
        uint16_t wData[] = { (uint16_t)(((uint16_t)(gGBOData._cfg | (1 << 5)))  & (~(1 << 13))) };
        err = _mbc->addRequest(100, 1, WRITE_MULT_REGISTERS, 20, 1, 2, wData);
        Serial.printf("chnageMode: SET %02X\n", wData[0]);
    }
    
    if (err!=SUCCESS) {
        ModbusError e(err);
        Serial.printf("Error chnageMode: %02X - %s\n", (int)e, (const char *)e);
    }
    _rotation = 1;
}


void GBOClient::sendMagic() {
     Error err = SUCCESS;
    uint16_t wData[] = { 0x55aa };
    err = _mbc->addRequest(200, 1, WRITE_MULT_REGISTERS, 76, 1, 2, wData);
     if (err!=SUCCESS) {
        ModbusError e(err);
        Serial.printf("Error sendMagic: %02X - %s\n", (int)e, (const char *)e);
    }
}