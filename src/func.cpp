#include "signal.h"
#include <Arduino.h>
#include <GyverTM1637.h>
#include <avr/wdt.h>
#include <EncButton.h>

#define dio_pin 11
#define clk_pin 10

GyverTM1637 disp(clk_pin, dio_pin);

void disp_clear() {
    disp.clear();
    disp.point(0);
}

void disp_begin() {
    disp.brightness(7);
    disp_clear();
}

void disp_start_string(int time) {
    uint8_t data[] = {_t, _o, _u, _c, _h, 0x00, _t, _o, 0x00, _t, _u, _r, _n, 0x00, _o, _n};
    disp.runningString(data, sizeof(data), time);
}

void display(int time, bool dots) {
    int min = time / 60;
    int sec = time % 60;

    //Serial.print(F("min: ")); Serial.println(min);
    //Serial.print(F("sec: ")); Serial.println(sec);

    disp.point(dots);
    disp.displayClock(min, sec);
}

void reset() {
    wdt_enable(WDTO_15MS);
    while(1) {}
}

void end() {
    Serial.println(F("end function called"));
    digitalWrite(rele_pin, 0);
    disp_clear();
    tone(buzz_pin, 1000);
    delay(250);
    noTone(buzz_pin);

    reset();
}

bool preparing_to_escape(EncButton &encoder) {
    Serial.println(F("I'm preparing to escape"));
    bool flag;
    uint64_t last, enc_time, start;
    start = millis();
    disp.displayByte(0x00, _O, _F, _F);
    disp.point(0);
    while(1) {
        if(millis() - last > 150 && flag == 1) {
            last = millis();
            flag = !flag;
            noTone(buzz_pin);
            disp.clear();
        }
        if(millis() - last > 250 && flag == 0) {
            last = millis();
            flag = !flag;
            tone(buzz_pin, 1000);
            disp.displayByte(0x00, _O, _F, _F);
        }

        if(millis() - enc_time >= 3) {
            enc_time = millis();
            encoder.tick();
            if(encoder.turn() || encoder.press()) {
                noTone(buzz_pin);
                disp_clear();
                return 1;  //не выключаемся
            }
        }

        if(millis() - start > 15000) {
            noTone(buzz_pin);
            disp_clear();
            return 0; //выключемся
        }
    }   
}

