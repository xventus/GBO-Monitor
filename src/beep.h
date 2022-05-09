/**
 * @file beep.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief beeper 
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */

#pragma once

#include <M5Unified.h>

class Beep {
    public:

    static void init() {
        M5.Speaker.begin();
        M5.Speaker.setVolume(0);
    }

    static void bootup() {
        if (M5.Speaker.isEnabled()) {
            M5.Speaker.setVolume(64);
            M5.Speaker.tone(440, 200);
            while (M5.Speaker.isPlaying()) { delay(1); }
            M5.Speaker.tone(659, 200);
            while (M5.Speaker.isPlaying()) { delay(1); }
            M5.Speaker.setVolume(0);
        }
    }

    static void click() {
        if (M5.Speaker.isEnabled()) {
            M5.Speaker.setVolume(64);
            M5.Speaker.tone(659, 100);
            while (M5.Speaker.isPlaying()) { delay(1); }
        }
    }

     static void clickErr() {
        if (M5.Speaker.isEnabled()) {
            M5.Speaker.setVolume(64);
            M5.Speaker.tone(300, 300);
            while (M5.Speaker.isPlaying()) { delay(1); }
        }
    }

    static void hold() {
        if (M5.Speaker.isEnabled()) {
            M5.Speaker.setVolume(64);
            M5.Speaker.tone(440, 1000);
            while (M5.Speaker.isPlaying()) { delay(1); }
        }
    }

};