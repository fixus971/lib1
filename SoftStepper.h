// SoftStepper.h

#ifndef SoftStepper_h
#define SoftStepper_h

#include <stdlib.h>
#include <Arduino.h>

// Correct the bug in macro round() of "Arduino.h": Erra con i grossi floating point and returns a long integer.
// La versione di math.h funza meglio e occupa la metà!   https://github.com/arduino/Arduino/issues/6098
#undef round

// Acc [Steps/Sec2]
// Vel [Steps/Sec]
// Pos [Steps]

// Acc [um/Sec2]
// Vel [um/Sec]
// Pos [um]



class SoftStepper : public Runnable {


  ////////
  private:

    const byte     oStepPin;
    const byte     oDirPin;

    bool           oStep;
    bool           oDir;

    bool           oStepChanged = 0;
    bool           oDirChanged = 0;

    float          stepSize = 1;   // Dim di un Full Step in [um]
    float          homePos = 0;    // Pos to assign when found home

    long           posSteps;
    float          pos;            // [um]
    float          posToGo;

    float          posVelMax;
    float          velVelMax;

    float          vel;            // [um/Sec] velToGo
    float          velAtt;
    float          velFwRv;        // x Mov a Vel Fissa
    float          velMax;         // x MoveTo point

    float          acc;            // [um/Sec2]
    float          acc_sqrt_twoa;  // Precomputed sqrt(2*_acceleration)

    int            moveMode;

    unsigned long  stepInterval;   // interval between steps in microseconds.
    unsigned long  stepChangeTime; // The last step time in microseconds

    long  velCalc_n;  // The step counter for speed calculations
    float _c0;    // Initial step size in microseconds
    float _cn;    // Last step size in microseconds
    float _cmin;  // Min step size in microseconds based on maxSpeed at max speed 

    char str[100];

  //////////
  protected:

    typedef enum {
      DIRECTION_CCW = 0,  ///< Counter-Clockwise
      DIRECTION_CW  = 1   ///< Clockwise
    } Direction;
    boolean _direction; // 0 == CW

/*
    int oDirOld = 0;
    void setOutputPins(uint8_t mask) {
      if( oDirOld != oDir ) {
          oDirOld  = oDir;
          digitalWrite(oDirPin, oDir);
      }
      digitalWrite(oStepPin, oStep);
    }
*/

    void step(long step);  // chiamato per Steppare


  ///////
  public:

    SoftStepper(byte aStepPin, byte aDirPin) :
      oStepPin(aStepPin), oDirPin(aDirPin)
      {}

    void    setStepSize(float s) {stepSize = s;}
    float   getStepSize()        {return stepSize;}

    void    setAcc(float a)      {acc = a;}
    float   getAcc()             {return acc;}

    void    setVel(float v)      {vel = v; moveMode = 0;}
    float   getVel()             {return vel;}

    void    setVelAtt(float v)   {velAtt = v; moveMode = 0;}
    float   getVelAtt()          {return velAtt;}
    
//    void    setVelPerc(float v)  {setVel((v*velMax)/100);}
    float   getVelPerc()         {return ((vel*100)/velMax);}
      
    void    setVelMax(float v)   {velMax = v;}
    float   getVelMax()          {return velMax;}

    void    setHomePos(float v)  {homePos = v;}  // Setta Home in pos attuale
    void    setHomeHere()        {pos = homePos; posSteps = 0;}  // Set pos di riferimento
    void    setHomeAt(float p)   {float d = pos-p;  // posAttuale - posMediaCorrettà
      posSteps = d /stepSize;
      pos = homePos + posSteps *stepSize;
      }  // Set pos di riferimento

    void    setPos(float p)      {pos = p;}
    float   getPos()             {return pos;}
    long    getPosSteps()        {return posSteps;}

    void    setVelFwRv(float v)  {velFwRv = v;}

    void    moveFw()             {setVel( velFwRv); velAtt = vel;}  //mov mono vel
    void    moveRv()             {setVel(-velFwRv); velAtt = vel;}  //mov mono vel

    void    moveTo(float p)      {posToGo = p;
                                  vel = 0; velAtt =0; /////////////////
                                  posVelMax = ( (pos-getStopSpace()) + posToGo ) /2;           // = a metà tra PosPartenza e PosArrivo
                                  velVelMax = getVelBySpace( abs(posToGo -posVelMax) ) +velFwRv;  // +Boost per Avvio e Arrivo
                                  moveMode = 1;
                                  Serial.print("\n");
                                  Serial.print(" @");  Serial.print(pos);
                                  Serial.print(" moveTo");  Serial.print(p);
                                  //Serial.print("  posVelMax");  Serial.print(posVelMax);
                                  //Serial.print("  velVelMax");  Serial.print(velVelMax);
                                  //Serial.println();
                                 }
    void    moveBy(float pr)     {moveTo(pos +pr);}  //Relative
    //float   getMoveTo()          {return posToGo;}  // Può servire ma meglio far altrimenti
    float   getPosToGo()         {return posToGo;}
    float   getDistToGo()        {return (posToGo-pos);}

    float   getStopSpace()       {return( pow(velAtt,2) / (2*acc) );}

    float   getVelBySpace(float s) {return( sqrt( 2*acc *s ) );}

    void    stop()               {setVel(0); velAtt = 0;}  // Frenata d'Emergenza

    bool    isRunning()          {return (vel != 0);}
    bool    isInPos()            {return ( abs(posToGo - pos) < stepSize );}

    bool    getStepPinValue()    {return oStep;}



    void    setup() {
      pinMode(oDirPin,  OUTPUT);  digitalWrite(oDirPin,  LOW);
      pinMode(oStepPin, OUTPUT);  digitalWrite(oStepPin, LOW);
      homePos = 0;
      pos = homePos;
      vel = 0;
      velAtt = 0;
      moveMode = 0;
    }


    ulong loops;
    void    loop() {
 
      if( moveMode == 0) {
        // TODO: Calc Vel using Acc
        velAtt = (velAtt*99 + vel) /99.99999;  //con 999.99991 va da 0 a (500) in 2Sec   con 99.99999 0.5Sec
        
      } else {  // "ModeTo"
        float distVelMax = abs(posVelMax -pos);
        velAtt = velVelMax - getVelBySpace( distVelMax );
        if( pos > posToGo )          { velAtt = -velAtt; }  // dir vel per raggiungere dest
        if( isInPos() )  {
          Serial.println(); Serial.print("isInPos! "); 
          sprintf( str, "@%4d ", (int)pos );  Serial.print( str );
          //velAtt = 0;
          setVel( 0 );
         }
      }

      if( abs(velAtt) < 0.000001) {
        velAtt = 0;
        stepInterval = 0;
      } else {

        int  dir = (velAtt>=0)?1:-1;  // +1 o -1 //Valore Fisico
        //bool nDir = (dir==-1);        //  1 o 0  //Valore Logico Uscita
        bool nDir = (dir==1);        //  1 o -1 -> Valore Logico Uscita
        if( _direction ) { nDir = !nDir; };
        if( nDir != oDir) {
          oDir = nDir;
          oDirChanged = true;
          digitalWrite(oDirPin, oDir);
        } else {
          oDirChanged = false;
        }

        if( abs(velAtt) > velMax )  { velAtt = velMax *dir; }

        stepInterval = (500*stepSize) / abs(velAtt);  //StepSize=16
      //stepInterval = 5;
        unsigned long now = millis();
      //unsigned long now = loops;
        loops++;
        if( (now - stepChangeTime) >= stepInterval ) {
          stepChangeTime = now;
          oStep ^= 1;
          digitalWrite(oStepPin, oStep);
          if( oStep ) {
            posSteps += dir;
            pos = homePos + posSteps *stepSize;
          }
        }
      }
      
    }


    void printDebugLine00() {
      //print((int)float,3) scrive valori errati perchè il tipo int nn ha virgola
      //Serial.print(" Pos="); Serial.print((int)pos);
      //Serial.print(" Vel="); Serial.print((int)velAtt);
      //Serial.print("->"); Serial.print((int)vel);
      //Serial.print(" StInt="); Serial.print(stepInterval);  //19 con Vel=500
      //Serial.print(" StSize="); Serial.print(stepSize,3);
      sprintf(str, "  Pos=%4d", (int)pos);
      Serial.print(str);
      sprintf(str, "  Vel=%5d->%5d", (int)velAtt, (int)vel);
      Serial.print(str);
    }

    void printDebugLine() {
      sprintf( str, "@%4d", (int)pos );  Serial.print(str);
      sprintf( str, "->%4d", (int)posToGo );  Serial.print(str);
      //sprintf( str, " %5dva->%5dv", (int)velAtt, (int)vel);  Serial.print(str);
      //Serial.print(" StInt=");   Serial.print(stepInterval);  //Vel=500->SI=19 Vel=8000->SI=1
    }

};

#endif 

