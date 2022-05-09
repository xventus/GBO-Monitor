
/**
 * @file constants.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief predefined strings & numbers
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */


#pragma once

#include <M5Unified.h>

// colors - Screen
#define WIFICOLOR       TFT_BLUE
#define BARSCOLOR       TFT_BLUE
#define TEXTCOLOR       TFT_WHITE
#define TEXTWARNCOLOR   TFT_RED
#define TEXTACTCOLOR    TFT_GREEN
#define BACKCOLOR       TFT_BLACK
#define INDIINACTIVE    TFT_LIGHTGREY

// Constants for running and texts
namespace Constants {

    const int gridSize = 40;              ///<  panel area m2 - TODO!!!!! change according to the size of the panel - corresponds to approximately 10kWp
    const int predictionRedLevel = 15;    ///<  limits for warning indication of low prediction in kWh
    const float efficiencyOfPanels = 0.25;  ///<  panel efficiency in energy conversion
    const float devconst = gridSize * efficiencyOfPanels;

    // these texts can be translated 
    const char* const txtGbofw   = "GBO FW:";
    const char* const txttryconect  = "trying to connect Wifi"; 
    const char* const txtap  = "AP:";
    const char* const txtname  = "GBO-AKU";
    const char* const txtcontrl  = "monitor";
    const char* const txtver  = "ver. 0.6";
    const char* const txtint1  = "Interval 1";
    const char* const txtint2  = "Interval 2";
    const char* const txtmode  = "Mode";
    const char* const txtl1  = "L1";
    const char* const txtl2  = "L2";
    const char* const txtl3  = "L3";
    const char* const txtwm  = "Winter mode"; 
    const char* const txtlt  = "Low tariff"; 
    const char* const txtin1  = "interval 1:"; 
    const char* const txtin2  = "interval 2:"; 
    const char* const txtpredt  = "Today [kWh]:"; 
    const char* const txtpredtt  = "Tommorow [kWh]:"; 
    const char* const txtoffdt  = "OFF"; 
    const char* const txtimgerr  = "Upload Filesystem Image!";

    // do not translate - for communication with the portal 
    const char* const txthttp  = "http://";
    const char* const forecastServer =  "http://www.pvforecast.cz/api/?key=[KEY]&lat=[LAT]&lon=[LON]&format=json&start=today&type=day&number=2";
    const char* const forecastKey = "[KEY]";
    const char* const forecastLat = "[LAT]";
    const char* const forecastLon = "[LON]"; 
    
};
