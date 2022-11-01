/*
 * Runnable.h
 * Aggancia automaticamente in coda le classi "Runnable" durante la creazione
 */
#ifndef Runnable_h
#define Runnable_h


#include <Arduino.h>


class Runnable {

  private:

  static        Runnable *headRunnable;
                Runnable *nextRunnable;

  public:

                Runnable();  // Aggancia automaticamente in coda le classi "Runnable" durante la creazione

  virtual void  setup() = 0;
  virtual void  loop()  = 0;

  static  void  setupAll();
  static  void  loopAll();
  static  void  printAll();

                //Nomi:
  private:
          char  _simStr[2] = {0,0}; //Char usabile come Str
    const char* _name;              //const contenuto
  public:
          void  setSimbol( const char  aSimbol )  { _simStr[0] = aSimbol;    }
          void  setSimStr( const char* aSimStr )  { _simStr[0] = aSimStr[0]; }
          void  setName  ( const char* aName   )  { _name      = aName;      }
          char  getSimbol() { return( _simStr[0] ); }
          char* getSimStr() { return( _simStr    ); }
    const char* getName()   { return( _name ); }
    
  
};


#endif

