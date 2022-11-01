#ifndef UTYX_H
#define UTYX_H


#include <sstream>
#include <stdint.h>

#include <Arduino.h>


#define ARY_LEN(a) (sizeof(a)/(sizeof(a[0])))


typedef	unsigned short	ushort;		/* System V compatibility */
typedef	unsigned int	  uint;		/* System V compatibility */
typedef	unsigned long	  ulong;		/* System V compatibility */

/*
inline static char str[100];


inline void slogingX( const char* String ) {
  sprintf( str, "  t=%6lu  ", millis() );
  Serial.print( str );
  Serial.println( String );
}
*/

#endif  //UTYX_H