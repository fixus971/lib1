/*
 * InPin.h
 */
#ifndef OutPin_h
#define OutPin_h


#include <Arduino.h>
#include <vector>
#include <Runnable.h>


inline void digitalWriteF( uint8_t ulPin, uint8_t ulVal ) {
  /*
  //Specifico per Nordic nrf5x:  //0.021us
  ///if (ulPin >= PINS_COUNT) { return; }
  ulPin = g_ADigitalPinMap[ulPin];
  NRF_GPIO_Type * port = nrf_gpio_pin_port_decode(&ulPin);
  switch ( ulVal ) {
    case LOW: port->OUTCLR = (1UL << ulPin);
    break;
    default:  port->OUTSET = (1UL << ulPin);
    break;
  }
  */
  digitalWrite( ulPin, ulVal );
}


enum OutType {
  Out_ActiveHigh = 0x00,
  Out_ActiveLow  = 0x01,
};


class OutPin : public Runnable {

  private:
          char    simbolStr[2];
    const char*   name;
    const byte    pin;
    const bool    activeLow;
    bool          oldState;
    bool          actualState;  //Stato Logico
    bool          changed;
    ulong         t_Change;


  public:


    static std::vector<OutPin*> OutPins;

/*
    OutPin(const char* aName, byte aPin, boolean activeLow) :
      name(aName),
      pin(aPin),
      activeLow(activeLow)
      {
        OutPins.push_back( this );
      }
*/

    OutPin(const char aSimbol, const char* aName, byte aPin, boolean aActiveLow) :
      simbolStr("o"),
      name(aName),
      pin(aPin),
      activeLow(aActiveLow)
      {
        simbolStr[0] = aSimbol;
        OutPins.push_back( this );
      }

    char* getSimbolStr() { return( simbolStr    ); }
    char  getSimbol()    { return( simbolStr[0] ); }

    bool  isActive()     { return( actualState  ); }

    bool  isChanged()    { 
      changed = (oldState != actualState);
                 oldState  = actualState;
      return (changed);
    }
	
    ulong getChangeT()   {
      return( t_Change );
    }
	
    void set( bool state ) {
        if( state != oldState ) { t_Change = millis(); }
        actualState = state;
        digitalWriteF(pin, actualState ^activeLow);
    }
	/*
    void invert->Toggle() {
        actualState = !actualState;
        digitalWriteF(pin, actualState);
    }
	*/
    void activate()   { set( true  ); }
    void deactivate() { set( false ); }
/*
    void activate() {
      if( activeLow ) {
          actualState = LOW;
        } else {
          actualState = HIGH;
        }
        digitalWriteF(pin, actualState);
    }
    void deactivate() {
      if( activeLow ) {
          actualState = HIGH;
        } else {
          actualState = LOW;
        }
        digitalWriteF(pin, actualState);
    }
*/


    void setup() {
      pinMode(pin, OUTPUT);
      /*
      if( activeLow ) {
        actualState = HIGH;
      } else {
        actualState = LOW;
      }
      digitalWriteF(pin, actualState);
      */
      set( false );  //Logicamente Disattivo al Setup
    }


    void loop() {
    }  //EndOf loop


};

#endif

