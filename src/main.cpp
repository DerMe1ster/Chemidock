#include <Arduino.h>
#include <func.h>
#include <testing.h>
#include <EncButton.h>

#define enc_pin_A 3
#define enc_pin_B 4
#define enc_Button 2

EncButton encoder(enc_pin_A, enc_pin_B, enc_Button);

uint64_t time_disp, time_enc, buzz_del;
int64_t universal;
uint64_t work_time, left_time;

bool buzz_turn, buzz_mode;

void setup() {
  init(); //надо, чтобы ардуинка завелась, встроенная функция
  MCUSR = 0; //watchdog сброс, или я хз
  Serial.begin(115200); //инициализия com порта
  encoder.setHoldTimeout(400);
  encoder.counter = 0;

  pinMode(buzz_pin, OUTPUT);
  pinMode(rele_pin, OUTPUT);
  digitalWrite(rele_pin, 0); //на всякий случай
  Serial.println(F("stated"));
  disp_begin();
}

int main() {
  setup(); //чисто "железная" часть
  //test(encoder);
  while(1) {
    if(millis() - time_enc >= 3) {
      time_enc = millis();
      encoder.tick();
      if(encoder.click() || encoder.press() || encoder.turn()) {
        digitalWrite(rele_pin, 1);
        Serial.println(F("turned on"));
        tone(buzz_pin, 800);
        delay(100);
        noTone(buzz_pin);
        disp_clear();
        break;
      }
    }
    //disp_start_string(350);

  }

  work_time = (millis() / 1000) + default_time; //устанавливаем время работы относительно 0
  universal = millis() + 150;
  time_enc = millis();
  //Serial.print(F("millis(): ")); Serial.println(millis());
  //Serial.print(F("time_enc: ")); Serial.println(time_enc);

  while(1) {
    if(millis() - time_disp >= 20) {
      time_disp = millis();
      left_time = work_time - (millis() / 1000); //получаем оставшееся время в с.
      display(left_time, 1);
    }

    if(millis() - time_enc >= 3) {
      time_enc = millis();

      encoder.tick();
      if(encoder.turn()) {
        if(encoder.left()) {
          if(work_time - (millis() / 1000) <= default_step) {
            work_time = (millis() / 1000) + 10;
          }
          else {
            work_time -= default_step;     
          }
          buzz_mode = 0;
          tone(buzz_pin, 800);
        }
        if(encoder.right()) {
          work_time += default_step;
          buzz_mode = 0;
          tone(buzz_pin, 800);
        }
        if(encoder.leftH()) {
          if(work_time - (millis() / 1000) <= hold_step) {
            work_time = (millis() / 1000) + 10;
          }
          else {
            work_time -= hold_step;     
          }
          buzz_mode = 1;
          tone(buzz_pin, 500);
        }
        if(encoder.rightH()) {
          work_time += hold_step;
          buzz_mode = 1;
          tone(buzz_pin, 500);
        }

        if(work_time - (millis() / 1000) > 5999) {
          work_time = (millis() / 1000) + 5999;
        }

        buzz_del = millis();
        buzz_turn = 1;
      }
      if(encoder.click() && universal - millis() <= 0) {
        end();
      }
    }

    if(work_time - (millis() / 1000) <= 0) {
      if(preparing_to_escape(encoder)) {
        work_time = millis() / 1000 + 300;
        universal = millis() + 1000;
      }
      else {
        end();
      } 
    }

    if(buzz_turn == 1) {
      if(buzz_mode == 1 && millis() - buzz_del >= 50) {
        buzz_turn = 0;
        noTone(buzz_pin);
      }
      if(buzz_mode == 0 && millis() - buzz_del >= 15) {
        buzz_turn = 0;
        noTone(buzz_pin);
      }
    }
  }
}