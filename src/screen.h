/**
 * @file screen.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief SCREEN
 * @version 0.1
 * @date 2022-04-27
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#pragma once

#include <M5GFX.h>
#include <Math.h>
#include "gauge.h"
#include "led.h"
#include "gbo_data.h"
#include "constants.h"

/**
 * @brief a class responsible for displaying data
 * 
 */
class Screen {
 public:
     enum class Page { overview,
                       prediction, 
                       status,
                       ap,
                       wifi, 
                       splash,
                       error };
    enum class Btn { A,
                     B, 
                     C,
                     AC,
                     none, 
                     init};

     
   M5GFX   _display;
    private:
   uint8_t _rotation {0};
   Page    _page {Page::overview};
   LED     _winter;
   Gauge   _ssr1; 
   Gauge   _ssr2;
   Gauge   _ssr3;
   LED     _hdo;
   LED     _k1;
   LED     _k2;
   LED     _k3;
   LED     _k4;
   LED     _k5;
   int     _today {0};
   int     _tomorrow {0};
   Btn     _btn {Btn::init};
   uint8_t _lastInt {0xff};
  
 public: 

    Screen();
    Screen& error(const char* error);
    Screen& splash();
    Screen& wifi(); 
    Screen& overview();  
    Screen& ap(const char* ip, const char* ssid);
    Screen& updatePrediction(int td, int tm) { 
      float val = (td *  Constants::devconst) / 1000;
      _today =  (int) round(val);
      val = (tm  *  Constants::devconst) / 1000;
      _tomorrow = (int) round(val);
       return *this;
       }
    void redraw(const GBOData& gbo);
    Screen&  button(Screen::Btn btn);
    void init() ;

 private:
    void wifiupdate(const GBOData& gbo);
    void  currentSensor(int value, int xpost);
    void overviewUpdate(const GBOData& gbo);
    void cls() {_display.fillScreen(TFT_BLACK);}
    String makeTime(uint8_t hour, uint8_t min);
    String makeDate(uint8_t day, uint8_t month, uint16_t year);
    int16_t word2int(uint16_t val);
    
};