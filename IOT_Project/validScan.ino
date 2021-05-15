int validScan()
{
  error = 0;
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  lcd.clear();
  lcd.print("Door Opening");
  lcd.setCursor(0, 1);
  lcd.print("Welcome");
  delay(3000);

  Serial.println("Door opening");

  for (pos = 0; pos <= 180; pos += 1)
  { // goes from 0 degrees to 180 degrees
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
  delay(5000);

  lcd.clear();
  lcd.print("Door Closing");
  delay(3000);

  for (pos = 180; pos >= 0; pos -= 1)
  { // goes from 180 degrees to 0 degrees
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
}
