int invalidScan()
{
  lcd.clear();
  lcd.print("Invalid Scan");
  lcd.setCursor(0, 1);
  lcd.print ("Try Again");
  digitalWrite(GREEN, LOW);

  for (int x = 0; x < 5; x++) { //flashes LED when fingerprint is not valid
    digitalWrite(RED, LOW);
    delay(500);
    digitalWrite(RED, HIGH);
    delay(500);
  }
}
