
/**
 * @file screen.cpp
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief data image from GBO
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#include "screen.h"
#include "constants.h"
#include "utils.h"




Screen::Screen() :_winter(&_display,140, 70), 
    _ssr1(&_display,0,  25, 35, 150,0,255), 
    _ssr2(&_display,36, 25, 35, 150,0,255), 
    _ssr3(&_display,72, 25, 35, 150,0,255),
    _hdo(&_display,140, 40),
    _k1(&_display,10,185),
    _k2(&_display,10,200),
    _k3(&_display,45,200),
    _k4(&_display,80,185),
    _k5(&_display,80,200) {
        _display.begin();
    } 

Screen&  Screen::button(Screen::Btn btn) {

    do {
        if (btn == _btn) break;
        _btn = btn;    
        _display.setTextColor(TEXTCOLOR, BARSCOLOR);
        _display.setTextDatum(textdatum_t::middle_left);
        _display.setFont(&fonts::Font2);
        auto xstep =  _display.width()/8;
    
        _display.fillRoundRect(xstep-15,_display.height()-20,xstep*2,20,5,BARSCOLOR);
        if (btn == Screen::Btn::A || btn == Screen::Btn::AC) {
        _display.setTextColor(TEXTWARNCOLOR, BARSCOLOR);  
        } else {
        _display.setTextColor(TEXTCOLOR, BARSCOLOR);  
        }
        _display.drawString(Constants::txtint1, xstep-6, _display.height()-10); 

        _display.fillRoundRect(3*xstep,_display.height()-20,xstep*2,20,5,BARSCOLOR);
        if (btn == Screen::Btn::B) {
        _display.setTextColor(TEXTWARNCOLOR, BARSCOLOR);    
        } else {
        _display.setTextColor(TEXTCOLOR, BARSCOLOR); 
        }
        _display.drawString(Constants::txtint2, 3*xstep+10, _display.height()-10); 

        _display.fillRoundRect(6*xstep-30,_display.height()-20,xstep*2,20,5,BARSCOLOR);
        if (btn == Screen::Btn::C || btn == Screen::Btn::AC) {
        _display.setTextColor(TEXTWARNCOLOR, BARSCOLOR);    
        } else {
        _display.setTextColor(TEXTCOLOR, BARSCOLOR);  
        }
        _display.drawString(Constants::txtmode, 6*xstep-10, _display.height()-10); 
    } while(false);
    return *this;
}

Screen&  Screen::wifi() { 
    _page = Page::wifi; 
    cls(); 
    _display.setTextColor(TEXTCOLOR, BACKCOLOR);
    _display.setTextDatum(textdatum_t::middle_left);
    _display.setFont(&fonts::Font4);
    _display.drawString(Constants::txttryconect, 30 ,_display.height() - 50);  
    return *this;  
}  

Screen& Screen::ap(const char* ip, const char* ssid) {
    _page = Page::wifi; 
    cls(); 
    _display.setTextColor(TEXTCOLOR, BACKCOLOR);
    _display.setTextDatum(textdatum_t::middle_left);
    _display.setFont(&fonts::Font4);
    _display.drawString(Constants::txtap, 70 , 50);  
    _display.drawString(ssid, 120, 50); 
    _display.drawString(Constants::txthttp, 50 ,_display.height() - 50); 
    _display.drawString(ip, 120 ,_display.height() - 50);  
    return *this;  
}

Screen& Screen::error(const char* error) {
    _page = Page::error; 
    cls(); 
    _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
    _display.setTextDatum(textdatum_t::middle_left);
    _display.setFont(&fonts::Font4);
    _display.drawString(error, 5, 50);  
    return *this;  
}


Screen& Screen::splash() { 
    _page = Page::splash; 
    cls(); 
    _display.setTextColor(TEXTCOLOR, BACKCOLOR);
    _display.setTextDatum(textdatum_t::middle_left);
    _display.setFont(&fonts::Font4);
    _display.drawString(Constants::txtname, 100, 50); 
    _display.drawString(Constants::txtcontrl, 110, 80);
    _display.drawString(Constants::txtver, 110, 110);   
    return *this;   
}  

Screen& Screen::overview() { 
    if (_page != Page::overview)  {
        _page = Page::overview; 
        cls(); 
        _display.setTextColor(TEXTCOLOR, BARSCOLOR);
        _display.setTextDatum(textdatum_t::middle_left);
        _display.setFont(&fonts::Font2);
        _display.fillRect(0,0,_display.width(),20, BARSCOLOR);
        
        Screen::button(Screen::Btn::none);

        _ssr1.show(Constants::txtl1);
        _ssr2.show(Constants::txtl2);
        _ssr3.show(Constants::txtl3);

        _winter.show(Constants::txtwm);
        _hdo.show(Constants::txtlt);
        _k1.show("");
        _k2.show("");
        _k3.show("");
        _k4.show("");
        _k5.show("");

        _display.drawString(Constants::txtin1, 140, 100);
        _display.drawString(Constants::txtin2, 140, 130);
        
        _display.drawString(Constants::txtpredt, 140, 160);
        _display.drawString(Constants::txtpredtt, 140, 190);


    }
    return *this;   
} 


void Screen::overviewUpdate(const GBOData& gbo) {
    
    _display.setTextColor(TEXTCOLOR, BARSCOLOR);
    _display.setTextDatum(textdatum_t::middle_left);
    _display.setFont(&fonts::Font2);
    
    // firmware
    _display.drawString(Constants::txtGbofw, 10, 10); 
    _display.drawNumber(gbo._firmware, 70, 10);

    // timedate
    auto dt = Utils::makeTime(gbo._hour,gbo._min);
    dt += " ";
    dt += Utils::makeDateDDMMYYYY(gbo._day, gbo._month, gbo._year);
    _display.drawString(dt.c_str(), _display.width()-130, 10); 

    // SSR 0-100%
    _ssr1.updateValue(gbo._ssr1).redraw(false);
    _ssr2.updateValue(gbo._ssr2).redraw(false);
    _ssr3.updateValue(gbo._ssr3).redraw(false);

    // relays
    _k1.updateValue(gbo._relay & (1 << 0) ).redraw();
    _k2.updateValue(gbo._relay & (1 << 1) ).redraw();
    _k3.updateValue(gbo._relay & (1 << 2) ).redraw();
    _k4.updateValue(gbo._relay & (1 << 3) ).redraw();
    _k5.updateValue(gbo._relay & (1 << 4) ).redraw();

    // HDO
    _hdo.updateValue(gbo._relay & (1 << 8) ).redraw();
    
    // HDO + excess power
    _winter.updateValue(! (gbo._cfg & (1 << 5))).redraw();

    if (_lastInt != gbo._changeInterval) {
        _lastInt = gbo._changeInterval; // prevent flickering
        // interval 1
        _display.setTextDatum(textdatum_t::middle_left);
        if (gbo._hdoStart0H == gbo._time2Start0H  && gbo._hdoStart0M == gbo._time2Start0M) {
            dt = Utils::makeTime(gbo._time2Start0H, gbo._time2Start0M); //timer 2  - interval 1
            dt += " - ";
            dt += Utils::makeTime(gbo._time2Stop0H, gbo._time2Stop0M); // timer 2 - interval 1
            _display.setTextColor(TEXTACTCOLOR, BACKCOLOR);
        } else {
        _display.fillRect(220, 90,_display.width()-220,20, BACKCOLOR);
        _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
        dt = Constants::txtoffdt;  
        }

        _display.drawString(dt.c_str(), 220, 100); 

        // interval 2
        if (gbo._hdoStart1H == gbo._time2Start1H  && gbo._hdoStart1M == gbo._time2Start1M) {
            dt = Utils::makeTime(gbo._time2Start1H, gbo._time2Start1M); // timer 2 - interval 2
            dt += " - ";
            dt += Utils::makeTime(gbo._time2Stop1H, gbo._time2Stop1M); // timer 2 - interval 2
            _display.setTextColor(TEXTACTCOLOR, BACKCOLOR);
        } else {
            _display.fillRect(220, 120,_display.width()-220,20, BACKCOLOR);
            _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
            dt = Constants::txtoffdt;  
        }
        
        _display.drawString(dt.c_str(), 220, 130); 
    }

    // energy prediction - today
    _display.setTextDatum(textdatum_t::middle_left);
    if (_today > 0) {
        if (_today < Constants::predictionRedLevel) {
            _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
        } else {
            _display.setTextColor(TEXTACTCOLOR, BACKCOLOR);
        }
        _display.drawNumber(_today, 250, 160);
    } 
    else {
        _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
        _display.drawString("?", 250, 160); 
    }

    // energy prediction - tomorrow
    _display.setTextDatum(textdatum_t::middle_left);
    if (_tomorrow > 0) {
        if (_tomorrow < Constants::predictionRedLevel) {
            _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
        } else {
            _display.setTextColor(TEXTACTCOLOR, BACKCOLOR);
        }
        _display.drawNumber(_tomorrow, 250, 190); 
    } 
    else {
        _display.setTextColor(TEXTWARNCOLOR, BACKCOLOR);
        _display.drawString("?", 250, 190);
    }    

   
    // current sensor - indicates the direction of current flow
    currentSensor(word2int(gbo._rawI1), 120);
    currentSensor(word2int(gbo._rawI2), 140);
    currentSensor(word2int(gbo._rawI3), 160);

}

int16_t Screen::word2int(uint16_t val) {
    int rc = val;
    if ( val >= 32767)  {
        rc = (65535 -  val)* (-1) ; 
    }
    return rc;
}

void Screen::currentSensor(int value, int xpost) {
     if (value > 1) {
         _display.fillCircle(xpost, 10, 5, TEXTACTCOLOR);
     } else if (value < -1) {
        _display.fillCircle(xpost, 10, 5, TEXTWARNCOLOR);
     } else {
        _display.fillCircle(xpost, 10, 5, BARSCOLOR);
        _display.drawCircle(xpost, 10, 5, INDIINACTIVE);
     }
}

void Screen::wifiupdate(const GBOData& gbo) {
    auto posy = 130;
    auto pos = _display.width() / 5;

    for (auto i=1; i<=4; i++) {
        _display.fillCircle( (pos*i), posy, 10, BACKCOLOR);
        _display.drawCircle( (pos*i), posy, 10, WIFICOLOR);
       
        if (_rotation == i) _display.fillCircle( (pos*i), posy,10, WIFICOLOR);    
    }
    
}

void Screen::redraw(const GBOData& gbo) {
    
    if (_rotation>=4) _rotation = 0;
    _rotation++;

    if (!_display.displayBusy()) {
       
       _display.startWrite();
       
       if (_page == Page::wifi) {
              wifiupdate(gbo);  
       }
       if (_page == Page::overview) {
              overviewUpdate(gbo);  
       }
       _display.endWrite();
    }
}