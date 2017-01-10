
// variables
int x;
int y;

// Sensors
// forward set
int SF1 = A0;
int SF2 = A1;
int SF3 = A2;
int SF4 = A3;
int SF5 = A4;
int SF6 = A5;
int SF7 = A6;
int SF8 = A7;
// back set
int SB1 = A8;
int SB2 = A9;
int SB3 = A10;
int SB4 = A11;
int SB5 = A12;
//int SB6 = A13;
/* / medium set
int SM1 = A13;
int SM2 = A14;
int SM3 = A15;
*/
// Setup
void setup(){
// sensor pins
 pinMode(SF1,INPUT);
 pinMode(SF2,INPUT);
 pinMode(SF3,INPUT);
 pinMode(SF4,INPUT);
 pinMode(SF5,INPUT);
 pinMode(SF6,INPUT);
 pinMode(SF7,INPUT);
 pinMode(SF8,INPUT);
/*
 pinMode(SM1,INPUT);
 pinMode(SM2,INPUT);
 pinMode(SM3,INPUT);
*/
 pinMode(SB1,INPUT);
 pinMode(SB2,INPUT);
 pinMode(SB3,INPUT);
 pinMode(SB4,INPUT);
 pinMode(SB5,INPUT);

 Serial.begin(9600);
}

void loop(){
 delay(1);
 x= analogRead(SF1);
 delay(1);
 y= analogRead(SF2);
 delay(1);

 Serial.println(y);
}


