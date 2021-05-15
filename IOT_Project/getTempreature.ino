float getTempreature()
{
  double voltage = 0, tempC = 0;
  value = analogRead(ANALOG_IN);
  voltage = (value / 2048.0) * 3300; // 5000 to get millivots.
  tempC = voltage * 0.1;
  Serial.println(tempC);
  return tempC;
}
