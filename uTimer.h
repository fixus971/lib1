#ifndef UTimer_H
#define UTimer_H


#include <Arduino.h>
#include <Utyx.h>



class UTimer {


  private:  // Singleton:
    UTimer() {}                             // L'Oggetto Singleton si deve creare da solo e nn da altri quindi nascondo il costruttore
    UTimer(UTimer const&) {}                // copy con"uctor is private // Bisogna evitare che venga copiato l'oggetto come capita durante il passaggio di parametri
    UTimer& operator = (UTimer const&) { return(*this); }  // assignment operator is private
  public:
    static UTimer& getInstance() {
      static UTimer _instance;
      return (_instance);
    };


	public:

///#define uTimer_IntInterval 10  //1000uS=1ms  //Min=2us  //Max=262ms  //Rumore10uS  //BuchiRandom100us
// IntInterval=101  =>  2Cicli   // IntInterval=100  =>  1Cicli
// IntInterval=101  => 11Cicli   // IntInterval=100  => 10Cicli
		static const    unsigned long IntInterval = 20;  //10->40us  100->23us
/*
tLoop calc come 1S/70000#Loop = 14us
a Riposo con IntInterval = 250000 = 1/4S   fa  87500 loops/Sec tLoop=11.43
a Riposo con IntInterval =    250 = 1/4mS  fa  87200 loops/Sec tLoop=11.47
a Riposo con IntInterval =     50          fa  83500 loops/Sec tLoop=11.97
a Riposo con IntInterval =     20          fa  77700 loops/Sec tLoop=12.87
a Riposo con IntInterval =     10          fa  68500 loops/Sec tLoop=14.6
*/
		volatile        unsigned long uTimer, uTimerLoops;
		                unsigned long tDebug;  //usabile ovunque per attivare il debugPrint
		                unsigned long uNow, uOld;
						         long uDiff, mDiff, xDiff;
								 long pDiff;

				        unsigned long loops;
		                unsigned long mNow, mOld, mMed, mMed1;

						unsigned long uLoop, uLoopMax, uLoopMin, uLoopMed;
						unsigned long uLoopMaxN, uLoops;
		                ///unsigned long tNow;
						//TODO: add nLoopsPerSec

		void reset() {
			uOld  = uTimer;
			mOld  = millis();
			loops = 0;
		}

		void Inc() {
			uTimer += IntInterval;
		}

/*
#define loopsChkCycle 10000  // 10000 =ogni> 200ms
		void LoopChk00() {
			uNow = uTimer;
			///mNow = millis();           //O_O => 0.403us
			//TODO: Legger ogni 50loops
			mNow = tick2ms(xTaskGetTickCount()); //0.138us vs 0.403 of millis()

			loops++;
			if( loops>=loopsChkCycle ) {  //ex100000
				mDiff = mNow-mOld;      // = 1400ms
				mMed1 = mDiff *(1000000/loopsChkCycle);      // =14000ms
				if ( mMed==0 )
					mMed  = mMed1;
				  else
					mMed  = ( mMed + mMed1 ) /2;
				uDiff = uNow-uOld;
				xDiff = mDiff*1000 -uDiff;
				pDiff = 100-( (uDiff*100) / (mDiff*1000.f) );
				if (pDiff<0) pDiff = 0;  //se legge piu us che ms
				uOld  = uNow;
				mOld  = mNow;
				loops = 0;
				DebugPrint();
			}
		}
*/

#define loopsChkCycleMS 500  //250OK;  200 da -LostInInt  // 10000 =ogni> 200ms
		void LoopChk() {
			uNow = uTimer;
			mNow = tick2ms( xTaskGetTickCount() );  //0.138us vs 0.403 of millis()
			loops++;
			if( mNow>=mOld+loopsChkCycleMS ) {
				mDiff = (mNow-mOld) *1000;    // 500 000 us
				mMed1 = (mDiff*1000) /loops;  // 500 000 000  /  25 000  =  20us

				///if ( mMed==0 )
					mMed  = mMed1;
				  //else
					///mMed  = ( mMed + mMed1 ) /2;
				
				uDiff = (uNow-uOld);  //*.80f;
				pDiff = ((mDiff-uDiff) *1000) / mDiff;
			///	pDiff = 1000-((1000*uDiff)/mDiff);
				if (pDiff<0) pDiff = 0;  //se legge piu us che ms
				///DebugPrint();
				uOld  = uNow;
				mOld  = mNow;
				loops = 0;
			}
		}


		void DebugPrint() {
			///Serial.println();
			sprintf(str, "%3lus", mNow/1000);  Serial.print(str);
			///sprintf(str, "%3luusL", mMed/1000);  Serial.print(str);
			sprintf(str, "%3luu", mMed/1000);  Serial.print(str);
			///sprintf(str, "%7luloops", loops);       Serial.print(str);
			//NB: i valori *Diff sono di tipo long("%ld") e nn unsigned("%lu") long o int(%d)
			///sprintf(str," %6ld", uDiff);  Serial.print(str);
			///sprintf(str,"/%6ld", mDiff);  Serial.print(str);
	  		///sprintf(str,"=%3.1f%%I", pDiff/10.f);  Serial.print(str);
			if( pDiff<10 )
			  { sprintf(str, ".%1ldi",  pDiff       );  Serial.print(str); }
			else
			  { sprintf(str,  "%2ldi", (pDiff+5)/10 );  Serial.print(str); }  //+5 fa Round()
			yield();  //= USB flush: mangia 8us anche se già fatto: usare solo dove serve
		}


		void DebugAfter(long t) { tDebug = mNow +t; }


};


//Loop min 7us  25500loops/200ms


#endif