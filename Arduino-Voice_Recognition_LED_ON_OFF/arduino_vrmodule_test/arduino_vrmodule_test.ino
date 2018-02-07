int redPin = 9; 
int greenPin = 10;

int bluePin = 11; byte

com = 0; //reply from voice recognition

void setup() { Serial.begin(9600);

pinMode(redPin, OUTPUT);

pinMode(greenPin, OUTPUT);

pinMode(bluePin, OUTPUT);

delay(2000);

Serial.write(0xAA);

Serial.write(0x37);

delay(1000);

Serial.write(0xAA);

Serial.write(0x21);

}

void loop()

{

while(Serial.available())

{

com = Serial.read();

switch(com)

{

case 0x11:

digitalWrite(redPin, HIGH);

break;

case 0x12:

digitalWrite(greenPin, HIGH);

break;

case 0x13:

digitalWrite(bluePin, HIGH);

break;

case 0x14:

digitalWrite(redPin, HIGH);

digitalWrite(greenPin, HIGH);

digitalWrite(bluePin, HIGH);

break;

case 0x15:

digitalWrite(redPin, LOW);

digitalWrite(greenPin, LOW);

digitalWrite(bluePin, LOW);

break;

}

}

}
