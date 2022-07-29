/**
 * @file main.cpp
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief Energy Dice - A simple indicator of the solar power plant status and free energy. As a server it uses NODE RED
 * @version 0.2 - without M5 framework
 * @date 2022-04-04
 * 
 * @copyright Copyright (c) 2022 Petr Vanek 
 * 
 */

//#include <Arduino.h>
#include <M5Unified.h>
#include <esp_log.h>
#include "gbo_client.h"
#include "ap.h"
#include "file_sys.h"
#include "configuration.h"
#include "cfg_server.h"
#include "screen.h"
#include "prediction_client.h"
#include "utils.h"
#include "beep.h"

/**
 * @brief set of global objects for running
 * 
 */
GBOClient     gbo;
Configuration cfg;
ItemFS        ifs;
Screen        sc;
PredictionClient pc;

bool gfirstrun = true;
uint8_t gnextMin = 0;

uint8_t gskipcycle = 0;

/**
 * @brief load configuration
 * 
 * @return true - OK
 * @return false - some item is not defined
 */
bool loadConfig() {
  bool rc = true;
  cfg.ssid = ifs.readItem(ItemFS::Data::ssid);
  cfg.pass = ifs.readItem(ItemFS::Data::password);
  cfg.ip = ifs.readItem(ItemFS::Data::ip);
  cfg.key = ifs.readItem(ItemFS::Data::apikey);
  cfg.lat = ifs.readItem(ItemFS::Data::lat);
  cfg.lon = ifs.readItem(ItemFS::Data::lon);

  if (cfg.ssid.isEmpty() || cfg.pass.isEmpty() || cfg.ip.isEmpty() || cfg.lon.isEmpty() || cfg.key.isEmpty() || cfg.lat.isEmpty()) rc = false;
  return rc;
}

/**
 * @brief Creates an AccessPoint with a web server for configuration. 
 * 
 */
void webConfig() {
  
    AP ap;
    CfgServer ws(&ifs);

    if (ap.init()) {
       Serial.printf("Configuration IP: %s\n", ap.getIP().toString().c_str());
       sc.ap(ap.getIP().toString().c_str(), ap.getSSID());

       ws.init(80);
       ws.serveCfgPage();
       while(true)  { 
          sc.redraw(*gbo.getGBOData());
          delay(200);
       }

    }
    while(true) { delay(1000); }
}

/**
 * @brief startup
 * 
 */

void setup(void) {
 
 M5.begin();
 sc.init();
 Beep::init();
 sc.splash().redraw(*gbo.getGBOData());
 Serial.begin(115200);
 Serial.flush();
 sc.wifi().redraw(*gbo.getGBOData());
 if (!ifs.init() || !ifs.isRequiredFileExists()) {
     Serial.printf("Invalid file system, please 'Upload Filesystem Image'! \nSTOP\n");
     sc.error(Constants::txtimgerr);
     while(true) {
        delay(1000);
     }
  }
  
  // debug dump
  Serial.printf("Filesystem content:\n"); 
  ifs.dumpFiles();

  if (!loadConfig()) {
    Serial.printf("configuration not found, use web config\n"); 
    webConfig();
  } 

  Serial.printf("connecting to AP\n");
  gbo.init(cfg.ssid.c_str(), cfg.pass.c_str(), cfg.ip.c_str());
  gfirstrun = true;
  gnextMin = 0;
  gskipcycle = 0;
  Beep::bootup();   
}

// cycle
void loop(void) {

  M5.update();

  // buttons handling
  if (M5.BtnA.wasClicked() || M5.BtnA.wasHold()) {
    // interval 1
    sc.button(Screen::Btn::A);
    if (gbo.isBlocked())  Beep::clickErr(); else  Beep::click();
    gbo.chnageInterval1();
  } 

  if (M5.BtnB.wasClicked() ||  M5.BtnB.wasHold()) {
    // interval 2
    sc.button(Screen::Btn::B);
    if (gbo.isBlocked())  Beep::clickErr(); else  Beep::click();
    gbo.chnageInterval2();
  } 

  if (M5.BtnC.wasClicked() ||  M5.BtnC.wasHold()) {
    // mode
    sc.button(Screen::Btn::C);
    if (gbo.isBlocked())  Beep::clickErr(); else Beep::click();
    gbo.chnageMode();
  } 
  
  if (M5.BtnA.wasHold() && M5.BtnC.wasHold()) {
    sc.button(Screen::Btn::AC);
    webConfig();
    Beep::hold();
  }
  
  // download data
  if (gskipcycle == 0) {
    if (gbo.isConnected()) {
      sc.overview();
      if (gbo.getData()) {   
        if (Utils::timesIsReady(*gbo.getGBOData())) {
          if (gfirstrun || (((gbo.getGBOData()->_hour==6) || (gbo.getGBOData()->_hour==18)) && (gbo.getGBOData()->_min==gnextMin)) ) {
              if (pc.getData(cfg.key.c_str(),cfg.lat.c_str(),cfg.lon.c_str(), *gbo.getGBOData())) {
                sc.updatePrediction(pc.getToday(), pc.getTommorow());
                gfirstrun = false;
                gnextMin = 0;
              } else { 
                gnextMin++;
                if (gnextMin > 5) {
                  gnextMin = 0;
                  gfirstrun = false;
                  // wait for next
                }

                gnextMin++;
                Serial.printf("gnext min =%d \n",gnextMin);
                delay(500);
              }
          }
        }

        if (!gbo.isBlocked()) {
          // unlock buttons
          sc.button(Screen::Btn::none);
        }
        delay(100);
      } else {
        // error    
        delay(300);
      }
      
    } 
  } 
  
  gskipcycle++;
  if (gskipcycle > 10) gskipcycle =0; 
  sc.redraw(*gbo.getGBOData());
  M5.update();
  delay(200);

}


