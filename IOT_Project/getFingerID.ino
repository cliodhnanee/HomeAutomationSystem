uint8_t getFingerID()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("Waiting for vailid finger");
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    Serial.println("Messy Image, Try Again");
    invalidScan();
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Invaild Finger, Try Again");
    invalidScan();
    error++;
    return -1;
  }

  validScan();
  return finger.fingerID;
}
