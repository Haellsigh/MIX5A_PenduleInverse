#include "SensorFusion.h"

/**
 * Valeurs en cm
 */
float Dmax = 198;
float Dmin = 2;
float Dseuil = 12;

float fusion(float senL, float senR) {
  if(senL < Dseuil) {
    
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  int senL = analogRead(A0);
  int senR = analogRead(A1);

  // put your main code here, to run repeatedly:
  Serial.print("Gauche: ");
  Serial.println(senL);
  Serial.print("Droite: ");
  Serial.println(senR);
  delay(50);
}
