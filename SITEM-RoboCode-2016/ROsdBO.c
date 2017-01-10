// include the library code
#include <LiquidCrystal.h>
LiquidCrystal lcd(11,10,9,8,7,6);
// This code is for check the sensor input
int SRR = 10; // SensorReadRate
// Motor speed controller
int LHS = 200;
int LMS = 100;
int LLS = 0;
int RHS = 200;
int RMS = 100;
int RLS = 0;

// Forward sensors
int sf1 = A0;
int sf2 = A1;
int sf3 = A2;
int sf4 = A3;
int sf5 = A4;
int sf6 = A5;
int sf7 = A6;
int sf8 = A7;
// back sensors
int sb1 = A8;
int sb2 = A9;
int sb3 = A10;
int sb4 = A11;
int sb5 = A12;
// Driver motor
// Left Moter
int DML1 = 37;
int DML2 = 35;
int DMLS = 2;
// Right Moter
int DMR1 = 33;
int DMR2 = 31;
int DMRS = 3;

int i; // Just a rubbish intiger for loops
int x = 1; // just a rubbish inteter for delay
int sfh[8]; // SensorForwardHigh
int sfl[8]; // SenosrForwardLow
char SFA[8]; // SensorForwardArray

int sbh[5]; // SensorBackHigh
int sbl[5]; // SenosrBackLow
char SBA[5]; // SensorBackArray


long FwBinory;
long FFwB;
int BwBinory;

void setup(){
  PINReset();
  CrystalReset();
  Serial.begin(9600);
}

void loop(){
  SensorCal(); // clibration point found
  delay(10);
  BlackWiteChange();
  delay(10);
  while(true){
    FwBwSensorRead();
    SensorBinory();
    //    Serial.println(FwBinory);
    lineFollower();

  }
}


void PINReset(){
  // forward sensors
  pinMode(sf1,INPUT);
  pinMode(sf2,INPUT);
  pinMode(sf3,INPUT);
  pinMode(sf4,INPUT);
  pinMode(sf5,INPUT);
  pinMode(sf6,INPUT);
  pinMode(sf7,INPUT);
  pinMode(sf8,INPUT);
  // backword sensors
  pinMode(sb1,INPUT);
  pinMode(sb2,INPUT);
  pinMode(sb3,INPUT);
  pinMode(sb4,INPUT);
  pinMode(sb5,INPUT);
  // motor pints
  pinMode(DMLS,OUTPUT);
  pinMode(DMRS,OUTPUT);
  pinMode(DML1,OUTPUT);
  pinMode(DML2,OUTPUT);
  pinMode(DMR1,OUTPUT);
  pinMode(DMR2,OUTPUT);
}

void CrystalReset(){
  lcd.begin(16,2);
  lcd.print("R2D2 Restarted");
}

// calibration point finding
void SensorCal(){
  //lcd.clear();
  //lcd.print("Keep on W 5");
  Serial.println("Keep on white");
/*  delay(1000);
  lcd.clear();
  lcd.print("Keep on W 4");
  delay(1000);*/
  lcd.clear();
  lcd.print("Keep on W 3");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on W 2");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on W 1");
  delay(1000);
  lcd.clear();
  lcd.print("White start...");
  lcd.clear();
  lcd.print("white Forward INT"); //forward initialized
  for(int i=0;i<8;i++){
    sfh[i]=0;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("White FW Reading...");
  //delay(1000);
  for(int i=0; i<10;i++){
    sfh[0] += analogRead(sf1);
    sfh[1] += analogRead(sf2);
    sfh[2] += analogRead(sf3);
    sfh[3] += analogRead(sf4);
    sfh[4] += analogRead(sf5);
    sfh[5] += analogRead(sf6);
    sfh[6] += analogRead(sf7);
    sfh[7] += analogRead(sf8);
  }
  for(int i=0;i<8;i++){
    sfh[i]=sfh[i]/10;
  }
  //delay(2000);
  lcd.clear();
  lcd.print("White Forward DONE!!");
  //delay(1000);
  lcd.clear();
  lcd.print("white Backword INT");
  for(int i=0;i<5;i++){
    sbh[i]=0;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("white BW Reading...");
  //delay(1000);
  for(int i=0; i<10;i++){
    sbh[0] += analogRead(sb1);
    sbh[1] += analogRead(sb2);
    sbh[2] += analogRead(sb3);
    sbh[3] += analogRead(sb4);
    sbh[4] += analogRead(sb5);
  }
  for(int i=0;i<5;i++){
    sbh[i]=sbh[i]/10;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("white Backword DONE!!");
  delay(1000);
  Serial.println("Wite color sensed");
  //delay(2000); // White color reading is done
  lcd.clear();
  //lcd.print("Keep on W 5");
  Serial.println("Keep on white");
  //delay(1000);
  //lcd.clear();
  //lcd.print("Keep on W 4");
  //delay(1000);
  //lcd.clear();
  lcd.print("Keep on B 3");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on B 2");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on B 1");
  delay(1000);
  lcd.clear();
  lcd.print("Black Forward INT");
  delay(1000);
  for(int i=0;i<8;i++){
    sfl[i]=0;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("Black FW Reading...");
  for(int i=0; i<10;i++){
    sfl[0] += analogRead(sf1);
    sfl[1] += analogRead(sf2);
    sfl[2] += analogRead(sf3);
    sfl[3] += analogRead(sf4);
    sfl[4] += analogRead(sf5);
    sfl[5] += analogRead(sf6);
    sfl[6] += analogRead(sf7);
    sfl[7] += analogRead(sf8);
  }
  for(int i=0;i<8;i++){
    sfl[i]=sfl[i]/10;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("Black Forward DONE !!");
  //delay(1000);
  lcd.clear();
  lcd.print("Black Backword INT");
  delay(1000);
  for(int i=0;i<5;i++){
    sbl[i]=0;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("Black BW Reading...");
  for(int i=0; i<10;i++){
    sbl[0] += analogRead(sb1);
    sbl[1] += analogRead(sb2);
    sbl[2] += analogRead(sb3);
    sbl[3] += analogRead(sb4);
    sbl[4] += analogRead(sb5);

  }
  for(int i=0;i<5;i++){
    sbl[i]=sbl[i]/10;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("Black Backword DONE !!");
  delay(1000);
  Serial.println("Black color sensed");
  delay(2000);
  lcd.clear();
  delay(100);
}


void BlackWiteChange(){
  for(int i=0;i<8;i++){
    sfh[i]= sfh[i] + sfl[i];
    sfh[i]= sfh[i] / 2;
  }
  for(int i=0;i<5;i++){
    sbh[i]= sbh[i] + sbl[i];
    sbh[i]= sbh[i] / 2;
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
// 0 = white
// 1 = black
void FwBwSensorRead(){
  // forward sensor reading
  if(ReadSensor(sf1,SRR) < sfh[0]){
    SFA[0] = '0';
  } 
  else {
    SFA[0] = '1';
  }
  if(ReadSensor(sf2,SRR) < sfh[1]){
    SFA[1] = '0';
  } 
  else {
    SFA[1] = '1';
  }
  if(ReadSensor(sf3,SRR) < sfh[2]){
    SFA[2] = '0';
  } 
  else {
    SFA[2] = '1';
  }
  if(ReadSensor(sf4,SRR) < sfh[3]){
    SFA[3] = '0';
  } 
  else {
    SFA[3] = '1';
  }
  if(ReadSensor(sf5,SRR) < sfh[4]){
    SFA[4] = '0';
  } 
  else {
    SFA[4] = '1';
  }
  if(ReadSensor(sf6,SRR) < sfh[5]){
    SFA[5] = '0';
  } 
  else {
    SFA[5] = '1';
  }
  if(ReadSensor(sf7,SRR) < sfh[6]){
    SFA[6] = '0';
  } 
  else {
    SFA[6] = '1';
  }
  if(ReadSensor(sf8,SRR) < sfh[7]){
    SFA[7] = '0';
  } 
  else {
    SFA[7] = '1';
  }
  // Backword sensor reading
  if(ReadSensor(sb1,SRR) < sbh[0]){
    SBA[0] = '0';
  } 
  else {
    SBA[0] = '1';
  }
  if(ReadSensor(sb2,SRR) < sbh[1]){
    SBA[1] = '0';
  } 
  else {
    SBA[1] = '1';
  }
  if(ReadSensor(sb3,SRR) < sbh[2]){
    SBA[2] = '0';
  } 
  else {
    SBA[2] = '1';
  }
  if(ReadSensor(sb4,SRR) < sbh[3]){
    SBA[3] = '0';
  } 
  else {
    SBA[3] = '1';
  }
  if(ReadSensor(sb5,SRR) < sbh[4]){
    SBA[4] = '0';
  } 
  else {
    SBA[4] = '1';
  }

  delay(10);
}
// Convert sensor values to binory
void SensorBinory(){
  lcd.clear();
  //FwBinory=atoi(SFA)+100000000;
  //BwBinory=atoi(SBA);
  FFwB = 100000000;
  FwBinory = 0;
  if(SFA[0]=='1'){
    FFwB+=1;
    FwBinory -= 9;
  }
  if(SFA[1]=='1'){
    FFwB+=10;
    FwBinory -= 5;
  }
  if(SFA[2]=='1'){
    FFwB+=100;
    FwBinory -= 3;
  }
  if(SFA[3]=='1'){
    FFwB+=1000;
    FwBinory += 1;
  }
  if(SFA[4]=='1'){
    FFwB+=10000;
    FwBinory += 1;
  }
  if(SFA[5]=='1'){
    FFwB+=100000;
    FwBinory += 3;
  }
  if(SFA[6]=='1'){
    FFwB+=1000000;
    FwBinory += 5;
  }
  if(SFA[7]=='1'){
    FFwB+=10000000;
    FwBinory += 9;
  }


  // FwBinory+=800000000;
  //BwBinory+=500000;
  lcd.print(FFwB);
}

// driver motor controlling 
void driver(float lspeed,float rspeed){
  // left motor
  if(lspeed>0){
    digitalWrite(DML1,HIGH);
    digitalWrite(DML2,LOW);
    analogWrite(DMLS,lspeed);
  }
  else if(lspeed==0){
    digitalWrite(DML1,LOW);
    digitalWrite(DML2,LOW);
    analogWrite(DMLS,lspeed);
  } 
  else {
    digitalWrite(DML1,LOW);
    digitalWrite(DML2,HIGH);
    analogWrite(DMLS,-lspeed);
  }
  // right motor 
  if(rspeed>0){
    digitalWrite(DMR1,HIGH);
    digitalWrite(DMR2,LOW);
    analogWrite(DMRS,rspeed);
  }
  else if(lspeed==0){
    digitalWrite(DMR1,LOW);
    digitalWrite(DMR2,LOW);
    analogWrite(DMRS,rspeed);
  } 
  else {
    digitalWrite(DMR1,LOW);
    digitalWrite(DMR2,HIGH);
    analogWrite(DMRS,-rspeed);
  }

}

void lineFollower(){
    /*
  if(FwBinory == 2){
    driver(LHS,RHS);
  }
  else if(FwBinory==0) {
    driver(0,0);
  }  
  // turn right
  else if(FwBinory < 1){
    driver(LHS,RHS/(-FwBinory));
  }
  // turn Left
  else if(FwBinory > 1) {
    driver(LHS/FwBinory,RHS);
  }

    */
    if(FwBinory == 0){
    driver(0,0);
  }
  else  {
    driver(LHS,RHS);
  }

}

