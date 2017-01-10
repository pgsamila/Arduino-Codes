// sensor checking
// R2D2
// include the library code
#include <LiquidCrystal.h>
LiquidCrystal lcd(11,10,9,8,7,6);
// This code is for check the sensor input
int s1 = A0;
int s2 = A1;
int s3 = A2;
int s4 = A3;
int s5 = A4;
int s6 = A5;
int s7 = A6;
int s8 = A7;
int sb1 = A8;
int sb2 = A9;
int sb3 = A10;
int sb4 = A11;
int sb5 = A12;
int sb6 = A13;

int sh[8];
int i;
int sl[8];
int SA[8];
int SRR = 10;
int s;

void setup(){
  SensorReset();
  CrystalReset();
  Serial.begin(9600);
}

void loop(){
  //SenCal(); // clibration point found
  //delay(10);
  //BlackWiteChange();
 // delay(10);
  while(true){
    //FwSensorRead();
    Serial.print(analogRead(sb1) );
    Serial.print("  ");
    Serial.print(analogRead(sb2) );
    Serial.print("  ");
    Serial.print(analogRead(sb3) );
    Serial.print("  ");
    Serial.print(analogRead(sb4) );
    Serial.print("  ");
    Serial.print(analogRead(sb5));
    Serial.print("  ");
    Serial.print(analogRead(sb6));
    Serial.print("  ");
    Serial.print("  ");
    Serial.print(analogRead(s1) );
    Serial.print("  ");
    Serial.print(analogRead(s2) );
    Serial.print("  ");
    Serial.print(analogRead(s3) );
    Serial.print("  ");
    Serial.print(analogRead(s4) );
    Serial.print("  ");
    Serial.print(analogRead(s5) );
    Serial.print("  ");
    Serial.print(analogRead(s6) );
    Serial.print("  ");
    Serial.print(analogRead(s7) );
    Serial.print("  ");
    Serial.println(analogRead(s8) );
  }
}


void SensorReset(){
  pinMode(s1,INPUT);
  pinMode(s2,INPUT);
  pinMode(s3,INPUT);
  pinMode(s4,INPUT);
  pinMode(s5,INPUT);
  pinMode(s6,INPUT);
  pinMode(s7,INPUT);
  pinMode(s8,INPUT);
  pinMode(sb1,INPUT);
  pinMode(sb2,INPUT);
  pinMode(sb3,INPUT);
  pinMode(sb4,INPUT);
  pinMode(sb5,INPUT);
  pinMode(sb6,INPUT);
}

void CrystalReset(){
  lcd.begin(16,2);
  lcd.print("R2D2 Restarted");
}

// calibration point finding
void SenCal(){
  lcd.clear();
  lcd.print("Keep on Wite");
  delay(5000);
  lcd.clear();
  lcd.print("WWWW");
  //lcd.print(millis()/5);
  delay(1000);
  for(int i=0;i<8;i++){
    sh[i]=0;
  }
  delay(5000);
  for(int i=0; i<10;i++){
    sh[0] += analogRead(s1);
    sh[1] += analogRead(s2);
    sh[2] += analogRead(s3);
    sh[3] += analogRead(s4);
    sh[4] += analogRead(s5);
    sh[5] += analogRead(s6);
    sh[6] += analogRead(s7);
    sh[7] += analogRead(s8);
  }
  for(int i=0;i<8;i++){
    sh[i]=sh[i]/10;
  }
  Serial.println("Wite color sensed");
  lcd.clear();
  delay(2000);

  lcd.print("Keep on Black");
  delay(5000);
  lcd.clear();
  lcd.print("BBBB");
  delay(1000);
  for(int i=0;i<8;i++){
    sl[i]=0;
  }
  delay(5000);
  for(int i=0; i<10;i++){
    sl[0] += analogRead(s1);
    sl[1] += analogRead(s2);
    sl[2] += analogRead(s3);
    sl[3] += analogRead(s4);
    sl[4] += analogRead(s5);
    sl[5] += analogRead(s6);
    sl[6] += analogRead(s7);
    sl[7] += analogRead(s8);
  }
  for(int i=0;i<8;i++){
    sl[i]=sl[i]/10;
  }
  Serial.println("Black color sensed");
  delay(5000);
  lcd.clear();
  delay(100);
  lcd.print("Calibration done");
}


void BlackWiteChange(){
  for(int i=0;i<8;i++){
    sh[i]= sh[i]+ sl[i];
    sh[i]= sh[i]/2;
  }
}

// Read sensor
int ReadSensor(int Sensor,int CheckRate){
  int sum = 0;
  for( i=0;i<CheckRate;i++){
    sum += analogRead(Sensor);
  }
  sum = sum/CheckRate;
  return sum;
}

// Forward Sensor Array
void FwSensorRead(){
  if(ReadSensor(s1,SRR) < sh[0]){
    SA[0] = 0;
  } 
  else {
    SA[0] = 1;
  }
  if(ReadSensor(s2,SRR) < sh[1]){
    SA[1] = 0;
  } 
  else {
    SA[1] = 1;
  }
  if(ReadSensor(s3,SRR) < sh[2]){
    SA[2] = 0;
  } 
  else {
    SA[2] = 1;
  }
  if(ReadSensor(s4,SRR) < sh[3]){
    SA[3] = 0;
  } 
  else {
    SA[3] = 1;
  }
  if(ReadSensor(s5,SRR) < sh[4]){
    SA[4] = 0;
  } 
  else {
    SA[4] = 1;
  }
  if(ReadSensor(s6,SRR) < sh[5]){
    SA[5] = 0;
  } 
  else {
    SA[5] = 1;
  }
  if(ReadSensor(s7,SRR) < sh[6]){
    SA[6] = 0;
  } 
  else {
    SA[6] = 1;
  }
  if(ReadSensor(s8,SRR) < sh[7]){
    SA[7] = 0;
  } 
  else {
    SA[7] = 1;
  }
  
  
  delay(10);
}



