#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <TroykaGPS.h>


GPS gps(Serial1);
RF24 radio(7,8);
float coords[3];

void setupGPS(){
  Serial1.begin(9600);
  Serial1.print("$CCCAS,1,5*55\r\n");
  delay(100);
  Serial1.begin(115200);
  Serial1.print("$CCINV,100,*60\r\n");
}

void setupRadio(){
  radio.begin();
  radio.setAutoAck(false);
  radio.openWritingPipe(0x1234567890LL);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}
 
void setup()
{ 
  setupGPS();
  setupRadio();
}
 
void loop()
{
  if (gps.available()) {
    gps.readParsing();
    switch(gps.getState()) {
      case GPS_OK:
        coords[0] = gps.getLatitudeBase10();
        coords[1] = gps.getLongitudeBase10();
        coords[2] = gps.getAltitude();
        radio.write(&coords,12);
        break;
      case GPS_ERROR_DATA:
        memset(coords, 0, 12);
        radio.write(&coords,12);
        break;
      case GPS_ERROR_SAT:
        memset(coords, 0, 12);
        radio.write(&coords,12);
        break;
    }
  }
}
