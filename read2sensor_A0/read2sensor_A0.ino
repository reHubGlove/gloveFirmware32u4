
#define S1 8
#define S2 9
#define I0 A0

void setup() {
  Serial.begin( 115200 );
  while (!Serial);
  Serial.println("Calibrazione");
  delay(1000);
  digitalWrite( S1,0 );
  digitalWrite( S2,0 );
  Serial.println("Start");
  
}

void loop() {
  digitalWrite( S1,1 );
  digitalWrite( S2,0 );
  delay(100);
  Serial.print( analogRead( A0 ) );
  Serial.print(" - ");
  digitalWrite( S1,0 );
  digitalWrite( S2,1 );
  delay(100);
  Serial.print( analogRead( A0 ) );
  Serial.print("\n");
  
}
