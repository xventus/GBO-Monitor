/**
 * @file led.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief LED
 * @version 0.1
 * @date 2022-04-27
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#pragma once

#include <M5GFX.h>

#define LEDCOLOR TFT_GREEN
/**
 * @brief LED
 * 
 */
class LED {

 private:
    M5GFX * _display {nullptr};
    bool     _value  {false};   ///> state
    int     _posx {0};          ///> position X
    int     _posy {0};          ///> position Y
    bool   _round {false};
 public: 

    /**
     * @brief Construct a new Gauge object
     * 
     * @param dsp display
     * @param x x position
     * @param y y position
     */
    explicit LED( M5GFX* dsp, int x, int y, bool round=false) : 
        _display(dsp), 
        _posx(x), 
        _posy(y),
        _round(round){
    } 

    /**
     * @brief display LED
     * 
     * @param label - label
     */
    void show(const char *label) {

        //_display->setTextColor(LEDCOLOR, TFT_BLACK);
        _display->setTextDatum(textdatum_t::middle_left);
         _display->setFont(&fonts::Font2);
         _display->setTextColor(TFT_WHITE, TFT_BLACK);
        _display->drawString(label, _posx + 30, _posy);
    }

    /**
     * @brief update internal value
     * 
     * @param val 
     * @return LED& 
     */
    LED& updateValue(bool val) {
        _value = val;
        return *this;
    }

    /**
     * @brief display LED 
     * 
     */
    void redraw() {
        if (_value) {
             if (_round) {
                _display->fillCircle(_posx, _posy, 10, LEDCOLOR);
             } else {
                _display->fillRoundRect(_posx, _posy, 20, 8, 2, LEDCOLOR);
             }
        } else {
             if (_round) {
                _display->fillCircle(_posx, _posy, 10, TFT_BLACK);
                _display->drawCircle(_posx, _posy, 10, LEDCOLOR);
             } else {
                _display->fillRoundRect(_posx, _posy, 20, 8, 2, TFT_BLACK);
               _display->drawRoundRect(_posx, _posy, 20, 8, 2, LEDCOLOR);
             }
        }
    }
    
};