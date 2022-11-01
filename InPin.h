#ifndef InPin_h
#define InPin_h


#include <Arduino.h>
#include <vector>
#include <Runnable.h>


//NB: pesa 1us per ogni input dell app
//TODO: usare lettura via interrupt


inline int digitalReadF( uint8_t ulPin ) {
  /*
  //Specifico per Nordic nrf5x:
  ulPin = g_ADigitalPinMap[ulPin];
  NRF_GPIO_Type * port = nrf_gpio_pin_port_decode(&ulPin);
  uint32_t const bm = (1UL << ulPin);
  //return (bm  & ((port->DIR & bm) ? port->OUT : port->IN)) ? 1 : 0;
  return (bm & (port->IN)) ? 1 : 0;
  */
  return digitalRead( ulPin );
}


#define kDebounceTime 20


enum InPinType {
  In_ActiveHigh = 0x00,
  In_ActiveLow  = 0x01,
};




class InPin : public Runnable {

  private:
          char  simbolStr[2];
    const char* name;
    const byte  pin;
    const bool  activeLow;
          bool  oldState;
          bool  actualState;
          bool  debouncedState;
          bool  changed;
          ulong debounceTime;
          ulong debounceStartTime;
          ulong prevStateDuration;
          ulong changedTime = 0;

  public:

    static std::vector<InPin*> InPins;

/*
    InPin(const char* aName, byte aPin, boolean activeLow) :
      simbol('i'),
      name(aName),
      pin(aPin),
      activeLow(activeLow),
      debounceTime(kDebounceTime)
      {
        InPins.push_back( this );
      }
*/
    InPin(const char aSimbol, const char* aName, byte aPin, boolean activeLow) :
      simbolStr("i"),
      name(aName),
      pin(aPin),
      activeLow(activeLow),
      debounceTime(kDebounceTime)
      {
        simbolStr[0] = aSimbol;
        InPins.push_back( this );
      }


    char* getSimbolStr() { return( simbolStr    ); }
    char  getSimbol()    { return( simbolStr[0] ); }

//    bool getActualState() { return (actualState ^ activeLow); }
//    bool getState()       { return (state ^ activeLow); }

    bool isActiveNow()     { return( actualState ); }
    bool isActive()        { return( debouncedState ); }

    bool isChanged()       { return( changed ); }  // 1CicleTimeMemory
    bool isChToActive()    { return( changed && isActive() ); }
    bool isChToNotActive() { return( changed && (!isActive()) ); }
    unsigned long getChangeTime() { return( changedTime ); }

    void setDebounceTime( unsigned long t ) { debounceTime = t; }

    unsigned long getPrevStateDuration() { return( prevStateDuration ); }


    void setup() {
      ///Serial.println("InPin("+(String)pin+"): Setup");
      if( activeLow ) {
        pinMode(pin, INPUT_PULLUP);
//        actualState = HIGH;
      } else {
        pinMode(pin, INPUT);
//        actualState = LOW;
      }
      actualState = LOW;  // vien corretto in loop
      debouncedState = actualState;
    }


    void loop() { //TODO: sost millis() e leggere via interrupt per lasciar libero loop
      changed = false;
      oldState = actualState;
      actualState = digitalReadF(pin) ^ activeLow;  //Inverte la lettura se il pin è attivo Basso
      if (actualState != oldState) {
        debounceStartTime = millis();  //Reset the debouncing timer
      } else {  // if==
//		  if( (debouncedState != actualState) && (pin==2) && (actualState==true) ) {  //pStart=2
//			  Serial.print("Input "); Serial.print(name);
//			  Serial.print(" (pin");  Serial.print(pin);  Serial.print(")" ); 
//			  Serial.print(" ActDebTime "); Serial.println( ((millis() -debounceStartTime)) );
//		  }
        if ( (debouncedState != actualState) && ((millis() - debounceStartTime) >= debounceTime) ) {
          debouncedState = actualState;
          changed = true;
          prevStateDuration = millis() -changedTime; // debounceStartTime -changedTime;
          changedTime = millis(); //debounceStartTime;
          /*
          Serial.print("Input "); Serial.print(name);
          Serial.print(" (G");  Serial.print(pin);  Serial.print(")" ); 
          Serial.print(" changed to "); Serial.println(debouncedState?"1":"0");
          */
        }
      }
    }  //EndOf loop

};


#endif