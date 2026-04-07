#include <EncButton.h>
#include <func.h>
#include <Arduino.h>

void test(EncButton &encoder) {
    uint64_t time, buzz_del;
    uint64_t work_time = 0;
    bool buzz_turn;
    while(1) {
        if(millis() - time >= 5) {
            time = millis();
            encoder.tick();
            if(encoder.turn()) {
                if(encoder.left()) work_time -= default_step;
                if(encoder.right()) work_time += default_step;
                if(encoder.leftH()) work_time -= hold_step;
                if(encoder.rightH()) work_time += hold_step;
                Serial.println(F("encoder tuned"));
                buzz_del = millis();
                buzz_turn = 1;
                tone(buzz_pin, 700);
                display(work_time, 1);
            }
        }
        if(buzz_turn == 1 and millis() - 50 >= buzz_del) {
        buzz_turn = 0;
        noTone(buzz_pin);
        }
    }
}