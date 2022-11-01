#ifndef InLogic_h
#define InLogic_h

#include <Arduino.h>
#include <vector>
#include <Runnable.h>


// Push e Release -> (InPin)  ShortClick (<200ms>) Long  Double  #Clks(ReStart if dt>200ms)
// Seq Clicks if dtPressed<200 & dtReleased<200

// 0->1 tPressStart
// 1->0 tPressStop   ->dtPressed   if dt>200 -> LongClick

// delay <200 ->++Clicks
// delay >200 ->##Clicks


class InLogic : public Runnable {

  private:
    static const  uint16_t tLongTime   = 200;
    /*
    static inline uint16_t tLongClick;
    static inline uint16_t tMultiDelay;
    */
    static inline uint16_t tLongClick   = 500;
    static inline uint16_t tMultiDelay  = 250;

    struct data_s {
        ulong   clickStartTime;
        uint8_t clicks;
        bool    evClick;
        bool    evLongClick;
    } ;

        data_s  readData, evData, oldData;

  private:
        bool    active;

        bool    changed;    // comprende ShortClick LongPush e DoublePush
        ulong   tChangedOld;
        ulong   tChanged;
        //      n*=New reading
        //      e*=Event data 1CpuLoop
        //      o*=Old ev data
        /*
        ulong   nClickStartTime;
        ulong   eClickStartTime;
        uint8_t nClicks;
        uint8_t eClicks;
        bool    eClick;
        bool    eLongClick;
        */
        void    setEvClicks()       { evClicks    = nClicks; nClicks = 0;  }

        void    setClick()          { Serial.print("Click"); Serial.print(nClicks); Serial.print("! ");
                                      evClick     = true; setEvClicks(); }
        void    setLongClick()      { Serial.print("ClickLong! ");
                                      evLongClick = true; setEvClicks(); }

        bool    waitNextClick;   // attesa eventuale click sucessivo

  public:

        InLogic() {}  // Constructor

        bool    isActive()          { return( active ); }
        bool    isChanged()         { return( changed ); }  // 1CicleTimeMemory
        bool    isChToActive()      { return( changed &&  isActive() ); }
        bool    isChToNotActive()   { return( changed && !isActive() ); }

        ulong   getTChanged()       { return( tChanged ); }
        ulong   getDTChanged()      { return( tChanged -tChangedOld ); }
        
        void    setChanged() { changed = true; tChangedOld = tChanged; tChanged = millis(); }

        void    setActive( bool activeNew ) {
                  if( activeNew ) Serial.print("Push_");
                      else        Serial.print("_Rel+ ");
                  if( active != activeNew ) {
                      active  = activeNew;  setChanged();
                      if( !active ) {
                          nClicks++;
                          //Serial.print("Clk! ");
                          if( getDTChanged() >= tLongClick ) setLongClick();
                          else waitNextClick = true;
                      }
                  }
                }

        void    setup() {}
        
        void    loop() {
                  changed     = false;
                  evClick     = false;
                  evLongClick = false;
                  evClicks    = 0;
                  if( waitNextClick && ( (millis()-tChanged) > tMultiDelay ) ) {
                      waitNextClick = false;
                      setClick();
                  }
        }  //EndOf loop


};

/*
uint16_t InLogic::tLongClick  = InLogic::tLongTime;
uint16_t InLogic::tMultiDelay = InLogic::tLongTime;
*/

#endif