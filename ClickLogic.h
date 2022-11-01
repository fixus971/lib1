#ifndef ClickLogic_h
#define ClickLogic_h

#include <Arduino.h>
#include <vector>
#include <Runnable.h>
#include <StateChangeInt.h>


// Push e Release -> (InPin)  ShortClick (<200ms>) Long  Double  #Clks(ReStart if dt>200ms)
// Seq Clicks if dtPressed<200 & dtReleased<200

// 0->1 tPressStart
// 1->0 tPressStop   ->dtPressed   if dt>200 -> LongClick

// delay <200 ->++Clicks
// delay >200 ->##Clicks





class ClickLogic : public Runnable {

  private:

        //static inline uint16_t tLongClick   = 500;
        //static inline uint16_t tMultiDelay  = 300;
        static const uint16_t tLongClick   = 500;
        static const uint16_t tMultiDelay  = 300;

        struct  data_s          {
                                  ulong   time1   = 0;
                                  ulong   time2   = 0;
                                  uint8_t clicks  = 0;
                                  bool    evClick     = false;
                                  bool    evLongPress = false;  // Not Released
                                  ulong   duration()    { return (time2-time1); }
                                } ;
        data_s  readingData, readedData, evData, oldData, resetData;

        void    setClick()      { Serial.print(" \nClick"); Serial.print(readingData.clicks); Serial.print("! \n");
                                  readingData.evClick     = true; setEv(); }

        void    setLongPress()  { Serial.print(" \nLongPress! \n");
                                  readingData.evLongPress = true; setEv(); }

        void    setEv()         { readedData  = readingData;
                                  readingData = resetData; }

        bool    waitNextClick = false;  // si attiva dopo rilascio click per un tot di tempo

  public:
        
                ClickLogic()    {}  // Constructor

        void    setup()         {}

        uint8_t getClicks()     { return evData.clicks; }
        bool    getEvClick()    { return evData.evClick; }
        bool    getEvLongPress(){ return evData.evLongPress; }

        void    pushChange_cb( bool state ) {
                  if( state ) pushStart(); else pushStop();
                }

        void    pushStart()     {
                  readingData.time1 = millis();
                  waitNextClick = false;  // nn deve scattare fine clicks se sto premendo
                }

        void    pushStop()      {
                  if( readingData.time1 ) { // se rilevato Long, Resetta T1! e quindi ignora click
                      readingData.time2 = millis();
                      readingData.clicks++;
                      waitNextClick = true;
                  }
                }

        void    loop()          {
                  if( waitNextClick
                    && (readingData.time2 > readingData.time1)
                    && ( (millis()-readingData.time2) > tMultiDelay ) ) {
                      waitNextClick = false;
                      setClick(); // Determina Fine Seq Clicks!: Dati -> ReadedData
                  }
                  if( readingData.time1 && ((millis()-readingData.time1) >= tLongClick)
                      && (readingData.clicks==0)  // Nn sta contando una seq di clicks
                    ) {  // Scatta mentre premo
                    setLongPress();
                  }
                  if(     evData.clicks ) { oldData = evData; }  // se cera un evento lo salva come vecchio
                  if( readedData.clicks || readedData.evLongPress ) {  // aggiorna evento
                          evData = readedData;  readedData = resetData;
                  } else {
                          evData = resetData;
                  }
                }

};


#endif