#ifndef StateChange_h
#define StateChange_h

#include <Arduino.h>
#include <functional>  //typedef std::function<void(int)> callback_t;
#include <Utyx.h>
#include <Runnable.h>

	//StateControll
class StateChange : public Runnable {

  private:

          int     changed  = 0;
          int     state    = 0;
          int     stateOld = 0;
          ulong   tChanged = 0;
          ulong   tChangedOld = 0;

//          void    setChanged() {          }

          typedef std::function<void(int)> callback_t;
                  callback_t callback;

          const char* const * stateNames;
          //char* [] stateNames;

  public:

                  StateChange()       {}  // Constructor

          void    setup()             {}
          
          void    loop()              {
            // var cambiate a metà loop devon restare 1.5 loop
            if( changed )  changed--;  else  changed = false;
          }  //EndOf loop
/*
          void    changeTest() { 
            changed = false;
          }  //EndOf loop
*/
          int     get()             { return( state ); }
          int     getOld()          { return( stateOld ); }
          bool    isChanged()       { return( changed ); }  // 1CicleTimeMemory
          ulong   getChangeT()      { return( tChanged ); }		            //Quando
          ulong   getElapsedT()     { return( millis() -tChanged ); }     //DaQuanto
          ulong   getPrevStateDT()  { return( tChanged -tChangedOld ); }  //DurataPrec
  
          void    set( int stateNew ) {
                    if( state != stateNew ) {
                      changed = 2;
                      stateOld = state;
                      state    = stateNew;
                      tChangedOld = tChanged;
                      tChanged    = millis();
                      //printChange();
                      callback( state );
                    }
                  }

          void    print() {
                    Serial.print( "St:" );  Serial.print( state );
                    Serial.print( "=" );    Serial.print( stateNames[ state ] );
                  }

          void    printChange() {
                    Serial.print( " StCh:" );
                    Serial.print( stateOld );  Serial.print( "->" );  Serial.print( state );
                    Serial.print( "=" );  Serial.print( stateNames[ state ] );
                    Serial.print( "\n" );
                  }

          void setCallBack( callback_t callback_ ) {
                  callback = callback_;
          }

          void setStateNames( const char* const stateNames_[] ) {
                  stateNames = stateNames_;
          }

};


#endif