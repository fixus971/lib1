/*
 * InDebounced.cpp
 */


#include "InDebounced.h"


void InDebounced::setup() {
  //Serial.println("InDebounced("+(String)pin+"): Setup");
  if( activeLow ) {
    pinMode(pin, INPUT_PULLUP);
    state = HIGH;
  } else {
    pinMode(pin, INPUT);
    state = LOW;
  }
  debounceState = state;
}


void InDebounced::loop() {
  changed = false;
  int actualState = digitalRead(pin);
  if (actualState != debounceState) {
    debounceState = actualState;
    debounceTime = millis();  //Reset the debouncing timer
  } else if ( (state != actualState) && ((millis() - debounceTime) > kDebounceTime) ) {
    state = actualState;
    changed = true;
//    Serial.print("Input "); Serial.print(name);
//    Serial.print(" (pin");  Serial.print(pin);  Serial.print(")" ); 
//    Serial.print(" changed to "); Serial.println(state?"1":"0");
  }
}  //EndOf loop

