

/**
 * @file utils.h
 * @author Petr Vanek (petr@fotoventus.cz)
 * @brief utility
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022 Petr Vanek
 * 
 */


#pragma once

#include <Arduino.h>
#include "gbo_data.h"

class Utils {
    public:

    static bool timesIsReady(const GBOData& gbo) {
        return !(gbo._min==0 && gbo._hour==0 && gbo._month==0);
    }

    static String makeTime(uint8_t hour, uint8_t min) {
        String rc;
        if (hour > 23 || min > 59) return rc; 
        if (hour<10) rc += "0";
        rc += hour;
        rc += ":";
        if (min<10) rc += "0";
        rc += min;
        return rc;  
    }

    /**
     * @brief 
     * 
     * @param day 1 - 31
     * @param month 1 - 12
     * @param year 00-99
     * @return String 
     */
    static String makeDateDDMMYYYY(uint8_t day, uint8_t month, uint16_t year) {
        String rc;
        if (day > 31 || month > 12 || year > 99) return rc;
        if (day<10) rc += "0";
        rc += day;
        rc += ".";
        if (month<10) rc += "0";
        rc += month;
        rc += ".20";
        rc += year;
        return rc;  
    }

    static String makeDateYYYYMMDD(uint8_t day, uint8_t month, uint16_t year) {
        String rc;
        
        rc += "20";
        rc += year;
        rc += "-";
        if (month<10) rc += "0";
        rc += month;
        rc += "-";
        if (day<10) rc += "0";
        rc += day;
        
        return rc;  
    }
};