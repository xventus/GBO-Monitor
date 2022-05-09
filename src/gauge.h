/**
 * @file gauge.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief vertical gauge meter
 * @version 0.1
 * @date 2022-04-27
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#pragma once

#include <M5GFX.h>

/**
 * @brief Gauge
 * 
 */
class Gauge {

 private:
    M5GFX * _display {nullptr};
    int     _value  {0};        ///> converted value    0-100%
    int     _old_value {100};   ///> last known coverted value 0-100%
    int     _showValue{0};      ///> true value
    int     _posx {0};          ///> position X
    int     _posy {0};          ///> position Y
    int     _width {0};         ///> device width
    int     _height {0};        ///> device height
    int     _minValue {0};      ///> minimum value
    int     _maxValue {100};    ///> maximum value
    

 public: 

    /**
     * @brief Construct a new Gauge object
     * 
     * @param dsp display
     * @param x x position
     * @param y y position
     * @param w device width 
     * @param h device height 
     * @param min minimum value
     * @param max maximum value
     */
    explicit Gauge( M5GFX* dsp, int x, int y, int w, int h, int min=0, int max=100) : 
    _display(dsp), 
    _posx(x), 
    _posy(y), 
    _width(w), 
    _height(h), 
    _minValue(min), 
    _maxValue(max)   {
    } 

    /**
     * @brief dispaly gauge
     * 
     * @param label - label
     */
    void show(const char *label) {
        _display->drawRect(_posx, _posy, _width, _height, TFT_LIGHTGREY);
        _display->fillRect(_posx + 2, _posy + 18, _width - 3, _height - 36, TFT_WHITE);
        _display->setTextColor(TFT_CYAN, TFT_BLACK);
        _display->setTextDatum(textdatum_t::middle_center);
        _display->setFont(&fonts::Font2);
        _display->setTextPadding(_display->textWidth("100"));
        _display->drawString(label, _posx + _width / 2, _posy + 10);
        auto plot_height = _height - (19*2);

        for (auto i = 0; i <= 10; i ++) {
            _display->drawFastHLine(_posx + _width/2, _posy + 19 + (i+1) * plot_height / 12, _width*(3+(0==(i%5)))/16, TFT_BLACK);
        }
    }

    /**
     * @brief update interna value
     * 
     * @param val 
     * @return Gauge& 
     */
    Gauge& updateValue(int val) {
        _showValue = val;
        _value = (val - _minValue) * 100 / (_maxValue - _minValue); 
        if (val > _maxValue) _value = _maxValue;
        if (val < _minValue) _value = _minValue;
        return *this;
    }

    /**
     * @brief display gauge hand
     * 
     */
    void redraw(bool trueVal=true) {
        _display->setFont(&fonts::Font2);
        _display->setTextColor(TFT_GREEN, TFT_BLACK);
        _display->setTextDatum(textdatum_t::middle_right);
        float plot_height = (_height - (19*2)) ;
        _display->drawNumber((trueVal)?_showValue:_value, _posx + (_width/2) + 15 , _posy + _height - 10);
        auto dx = _posx + 2;
        float incr = plot_height / 120;
        auto dy =  _posy + 18 + (_old_value+2) * incr;
        _display->fillTriangle(dx, dy - 5, dx, dy + 5, dx +  (_width/3), dy, TFT_WHITE);
        _old_value = 110 - _value;
        dy = _posy + 18 + (_old_value+2) * incr;
        _display->fillTriangle(dx, dy - 5, dx, dy + 5, dx + (_width/3) , dy, TFT_RED);
    }
    
};