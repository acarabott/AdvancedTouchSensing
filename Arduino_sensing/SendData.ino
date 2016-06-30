byte yMSB=0, yLSB=0, xMSB=0, xLSB=0, zeroByte=128, Checksum=0;

void SendData(int Command, unsigned int xValue, unsigned int yValue){
  /* >=================================================================<
     y = 01010100 11010100    (x & y are 2 Byte integers)
     yMSB      yLSB      send seperately -> reciever joins them
     >=================================================================<  */

  xLSB=lowByte(xValue);
  xMSB=highByte(xValue);
  yLSB=lowByte(yValue);
  yMSB=highByte(yValue);


 /* >=================================================================<
    Only the very first Byte may be a zero, this way allows the computer
    to know that if a Byte recieved is a zero it must be the start byte.
    If data bytes actually have a value of zero, They are given the value
    one and the bit in the zeroByte that represents that Byte is made
    high.
    >=================================================================< */

  zeroByte = 128;                                   // 10000000

  if(xLSB==0){ xLSB=1; zeroByte=zeroByte+1;}        // make bit 1 high
  if(xMSB==0){ xMSB=1; zeroByte=zeroByte+2;}        // make bit 2 high
  if(yLSB==0){ yLSB=1; zeroByte=zeroByte+4;}        // Make bit 3 high
  if(yMSB==0){ yMSB=1; zeroByte=zeroByte+8;}        // make bit 4 high


 /* >=================================================================<
    Calculate the remainder of: sum of all the Bytes divided by 255
    >=================================================================< */

  Checksum = (Command + xMSB + xLSB + yMSB + yLSB + zeroByte)%255;

  if( Checksum !=0 ){
    Serial.write(byte(0));            // send start bit
    Serial.write(byte(Command));      // command eg: Which Graph is this data for

    Serial.write(byte(xMSB));         // X value's most significant byte
    Serial.write(byte(xLSB));         // X value's least significant byte
    Serial.write(byte(yMSB));         // Y value's most significant byte
    Serial.write(byte(yLSB));         // Y value's least significant byte

    Serial.write(byte(zeroByte));     // Which values have a zero value
    Serial.write(byte(Checksum));     // Error Checking Byte
  }
}

void PlottArray(uint8_t cmd, float Array1[], uint16_t numValues) {
  SendData(cmd + 1, 1, 1);            // Tell PC an array is about to be sent
  delay(1);

  for(uint16_t x = 0; x < numValues; x++) {     // Send the arrays
    SendData(cmd, x, round(Array1[x]));
  }

  SendData(cmd + 2, 1, 1);                // Confirm arrrays have been sent
}
