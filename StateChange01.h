#ifndef StateChange_h
#define StateChange_h

#include <Arduino.h>
#include <functional>
#include <Runnable.h>

	//StateControll
class StateChange : public Runnable {

  private:

          bool    state = 0;
          bool    changed = 0;
          ulong   tChanged = 0;
          ulong   tChangedOld = 0;

          void    setChanged()        { changed = true; tChangedOld = tChanged; tChanged = millis();
                                        callback( state );
                                      }

          typedef std::function<void(bool)> callback_t;
                  callback_t callback;

  public:

                  StateChange()       {}  // Constructor

          void    setup()             {}
          
          void    loop()              { changed = false; }  //EndOf loop

          bool    isActive()          { return( state ); }
          bool    isChanged()         { return( changed ); }  // 1CicleTimeMemory
          bool    isChToActive()      { return( changed &&  isActive() ); }
          bool    isChToNotActive()   { return( changed && !isActive() ); }

          ulong   getTChanged()       { return( tChanged ); }
          ulong   getDTChanged()      { return( tChanged -tChangedOld ); }
          
          void    setActive( bool activeNew ) {
                    if( state != activeNew ) {
                        if( activeNew ) Serial.print("1");
                                else    Serial.print("0");
                        state  = activeNew;  setChanged();
                    }
                  }

          void setCallBack( callback_t callback_ ) {
                  callback = callback_;
          }

};


#endif