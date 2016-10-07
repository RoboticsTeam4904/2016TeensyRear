#include <Encoder.h>
#include <FlexCAN.h>
#include <TeensyCANBase.h>

Encoder rightEncoder(8, 7);
Encoder leftEncoder(5, 6);

long lastReadRight = 0;
long posRight = -999;
long rateRight = 0;
long lastReadLeft = 0;
long posLeft = -999;
long rateLeft = 0;

const int ultrasonicPort = 0;
long distance = 0;

int rightEncoderFunc(byte* msg, byte* resp) {
  if (msg[0] == 0) {
    resp[0] = posRight & 0xff;
    resp[1] = (posRight >> 8) & 0xff;
    resp[2] = (posRight >> 16) & 0xff;
    resp[3] = (posRight >> 24) & 0xff;

    resp[4] = 0; // Mode

    for (int i = 5; i < 8; i++) {
      resp[i] = 0;
    }

    return 0;
  }
  else if (msg[0] == 1) {
    resp[0] = rateRight & 0xff;
    resp[1] = (rateRight >> 8) & 0xff;
    resp[2] = (rateRight >> 16) & 0xff;
    resp[3] = (rateRight >> 24) & 0xff;

    resp[4] = 1; // Mode

    for (int i = 5; i < 8; i++) {
      resp[i] = 0;
    }

    return 0;
  }
  else if (msg[0] == 0x72 && msg[1] == 0x65 && msg[2] == 0x73 && msg[3] == 0x65 && msg[4] == 0x74 && msg[5] == 0x65 && msg[6] == 0x6e && msg[7] == 0x63) {
    rightEncoder.write(0);
    posRight = 0;
    rateRight = 0;
    Serial.println("reset");
    return 1;
  }
  return 1;
}

int leftEncoderFunc(byte* msg, byte* resp) {
  if (msg[0] == 0) {
    resp[0] = posLeft & 0xff;                                                                                                                                                                                                                 
    resp[1] = (posLeft >> 8) & 0xff;                                                                                                                                                                                                          
    resp[2] = (posLeft >> 16) & 0xff;                                                                                                                                                                                                         
    resp[3] = (posLeft >> 24) & 0xff;                                                                                                                                                                                                         

    resp[4] = 0; // Mode                                                                                                                                                                                                                      

    for (int i = 5; i < 8; i++) {                                                                                                                                                                                                             
      resp[i] = 0;                                                                                                                                                                                                                            
    }

    return 0;                                                                                                                                                                                                                                 
  }
  else if (msg[0] == 1) {
    resp[0] = rateLeft & 0xff;                                                                                                                                                                                                                
    resp[1] = (rateLeft >> 8) & 0xff;                                                                                                                                                                                                         
    resp[2] = (rateLeft >> 16) & 0xff;                                                                                                                                                                                                        
    resp[3] = (rateLeft >> 24) & 0xff;                                                                                                                                                                                                        

    resp[4] = 1; // Mode                                                                                                                                                                                                                      

    for (int i = 5; i < 8; i++) {                                                                                                                                                                                                             
      resp[i] = 0;                                                                                                                                                                                                                            
    }

    return 0;                                                                                                                                                                                                                                 
  }
  else if (msg[0] == 0x72 && msg[1] == 0x65 && msg[2] == 0x73 && msg[3] == 0x65 && msg[4] == 0x74 && msg[5] == 0x65 && msg[6] == 0x6e && msg[7] == 0x63) {
    leftEncoder.write(0);                                                                                                                                                                                                                     
    posLeft = 0;                                                                                                                                                                                                                              
    rateLeft = 0;                                                                                                                                                                                                                             
    Serial.println("reset");                                                                                                                                                                                                                  
    return 1;                                                                                                                                                                                                                                 
  }
  return 1;                                                                                                                                                                                                                                   
}

int ultrasonicFunc(byte* msg, byte* resp){
  if(msg[0] == 0){
    resp[0] = posLeft & 0xff;                                                                                                                                                                                                                 
    resp[1] = (posLeft >> 8) & 0xff;                                                                                                                                                                                                          
    resp[2] = (posLeft >> 16) & 0xff;                                                                                                                                                                                                         
    resp[3] = (posLeft >> 24) & 0xff;

    for(int i = 4; i < 8; i++){
      resp[i] = 0;
    }

    return 0;
  }

  return 1;
}

TeensyCANBase rightEncoderCAN(0x603, &rightEncoderFunc);
TeensyCANBase leftEncoderCAN(0x602, &leftEncoderFunc);
TeensyCANBase ultrasonicCAN(0x604, &ultrasonicFunc);

void setup(void) {
  TeensyCANBase::begin();
  delay(1000);
  Serial.println("Teensy 3.X CAN Encoder");
}


void loop(void) {
  TeensyCANBase::update();

  long newPos = rightEncoder.read();
  if (newPos != posRight) {
    rateRight = ((double) 1000000.0 * (newPos - posRight)) / ((double) (micros() - lastReadRight));
    posRight = newPos;
    Serial.print("Right: ");
    Serial.println(posRight);
    lastReadRight = micros();
  }
  else {
    if ((micros() - lastReadRight) > 1000) {
      rateRight = 0;
    }
  }
  
  newPos = leftEncoder.read();
  if (newPos != posLeft) {
    rateLeft = ((double) 1000000.0 * (newPos - posLeft)) / ((double) (micros() - lastReadLeft));
    posLeft = newPos;
    Serial.print("Left: ");
    Serial.println(posLeft);
    lastReadLeft = micros();
  }
  else {
    if ((micros() - lastReadLeft) > 1000) {
      rateLeft = 0;
    }
  }

  distance = pulseIn(ultrasonicPort, HIGH) / 58; // Pulse is 58us per cm
}

