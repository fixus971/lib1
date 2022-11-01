/*
 * InDebounced.h
 */
#ifndef InDebounced_h
#define InDebounced_h


#include <Arduino.h>
#include <Runnable.h>


#define kDebounceTime 50


class InDebounced : public Runnable {

    const byte pin;
    const char* name;
    int state;
    int debounceState;
    unsigned long debounceTime;
    boolean activeLow;
    boolean changed;

  public:

    InDebounced(const char* aName, byte aPin, boolean activeLow) :
      pin(aPin),
      name(aName),
      activeLow(activeLow)
      {}

    void setup();

    void loop();

    boolean getState() { return (state ^ activeLow); }

    boolean isChanged() { return changed; }

};


#endif

