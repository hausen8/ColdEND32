# Wire
Modified Arduino 1.6.5 Wire library

  Added timeout for bus errors in Wire.endTransmission() and Wire.requestFrom()
  Added lastError for bus errors
  Changed onRequestEvent() to allow multiple calls to Wire.write()
  Changed onRequestEvent() to support repeated calls during a single I2C read transaction.  This allows a Arduino to support random length read requests.

Source: https://github.com/stickbreaker/Wire