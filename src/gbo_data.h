
/**
 * @file gbo_data.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief data image from GBO
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#pragma once

#include <Arduino.h>

struct GBOData {

    // time&relay addr:0, size:10 -  FC3
    uint8_t  _sec {0};        ///> [0]
    uint8_t  _min {0};        ///> [0]
    uint8_t  _hour {0};       ///> [1]
    uint8_t  _dayweek {0};    ///> [1]
    uint8_t  _day {0};        ///> [3]
    uint8_t _month {0};      ///> [3]
    uint16_t  _year {0};       ///> [4]
    uint16_t  _relay {0};      ///> [5]

    // confuguration addr:20, size:36 - FC3 
    uint16_t  _cfg {0};         ///> [0]
    uint16_t  _firmware {0};    ///> [7]
    uint8_t  _time2Start0M {0}; ///> [16]
    uint8_t  _time2Start0H {0}; ///> [16]      
    uint8_t  _time2Stop0M {0};  ///> [17] 
    uint8_t  _time2Stop0H {0};  ///> [17]      
    uint8_t  _time2Start1M {0}; ///> [18]   
    uint8_t  _time2Start1H {0}; ///> [18]      
    uint8_t  _time2Stop1M {0};  ///> [19]  
    uint8_t  _time2Stop1H {0};  ///> [19]  
    uint16_t  _relEnable {0};   ///> [20]
    uint8_t  _hdoStart0M {0};   ///> [12] 
    uint8_t  _hdoStart0H {0};   ///> [12] 
    uint8_t  _hdoStop0M {0};    ///> [13] 
    uint8_t  _hdoStop0H {0};    ///> [13]
    uint8_t  _hdoStart1M {0};   ///> [14] 
    uint8_t  _hdoStart1H {0};   ///> [14]      
    uint8_t  _hdoStop1M {0};    ///> [15] 
    uint8_t  _hdoStop1H {0};    ///> [15]      

    // measure addr:0, size:45 - FC4
    uint16_t _rawI1 {0};    ///> raw ADC current - ADC [0]  
    uint16_t _rawI2 {0};    ///> raw ADC current - ADC [1]
    uint16_t _rawI3 {0};    ///> raw ADC current - ADC [2]
    uint8_t  _ssr1 {0};     ///> converted to % [9]
    uint8_t  _ssr2 {0};     ///> converted to % [18]
    uint8_t  _ssr3 {0};     ///> converted to % [26]

    // transaction 
    bool _blockIO {false};  ///> waiting for enrolment to the device
    bool _accepted {false}; ///> waiting for finalization

    uint8_t _changeInterval {0};    ///> auxiliary sum for detecting changes in intervals

}; 
