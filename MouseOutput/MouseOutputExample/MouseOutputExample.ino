#include "MouseOutput.h"

Output out( 8, 7, 9 );

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  out.put("testing\n", 'a', 2 );
  delay(1000);
  out.put("again\n", 'g', 2 );
  delay(1000);
  out.ERROR("THIS SHIT", 4 );
  delay(4000);
  out.put( NULL, 'b', 0 );
  delay(2000);
  out.off('b');
}
