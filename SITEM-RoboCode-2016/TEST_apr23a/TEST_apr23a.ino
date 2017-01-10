/*******
   *   *R2D2 
  *****vBeta
 *  ********
**/

// driver motor speeds
//Left Motor
int LHS = 255; // high speed
int LMS = 120; // meduim speed
int LLS = 0;   // low speed
//Right Motor
int RHS = 255; // high speed
int RMS = 120; // meduim speed
int RLS = 0;   // low speed

// Black - Wite Colour separation
int BlackWiteChange = 800;

// Sensor value getting rate
int SensorReadRate = 5;

// Program Delay
int x=1;


// Sensors
// forward set
int SF1 = A0;
int SF2 = A1;
int SF3 = A2;
int SF4 = A3;
int SF5 = A4;
int SF6 = A5;
int SF7 = A6;
// medium set
int SM1 = A7;
int SM2 = A8;
int SM3 = A9;
// back set
int SB1 = A10;
int SB2 = A11;
int SB3 = A12;
int SB4 = A13;
int SB5 = A14;
//int SB6 = A15;
// sensor specification
int s;

// Sensor input array
// forward set
char FwSensorArray[8];
// meduim set
char MeSensorArray[3];
// back set
char BwSensorArray[5];

// Binory sets
// Forward set
int FwBinary;
// Medium set
int MeBinary;
// Back set
int BwBinary;

// Driver Motor
// Left motor
int DML1 = 28;
int DML2 = 26;
int DMLS = 4;
// Right motor
int DMR1 = 22;
int DMR2 = 24;
int DMRS = 5;





// Setup
void setup(){
// sensor pins
 pinMode(SF1,INPUT);
 pinMode(SF2,INPUT);
 pinMode(SF3,INPUT);
 pinMode(SF4,INPUT);
 pinMode(SF5,INPUT);
 pinMode(SF6,INPUT);

 pinMode(SM1,INPUT);
 pinMode(SM2,INPUT);
 pinMode(SM3,INPUT);

 pinMode(SB1,INPUT);
 pinMode(SB2,INPUT);
 pinMode(SB3,INPUT);
 pinMode(SB4,INPUT);
 pinMode(SB5,INPUT);
// pinMode(SB6,INPUT);

// motor pints
 pinMode(DMSL,OUTPUT);
 pinMode(DMSR,OUTPUT);
 pinMode(DML1,OUTPUT);
 pinMode(DML2,OUTPUT);
 pinMode(DMR1,OUTPUT);
 pinMode(DMR2,OUTPUT);

// Serial setup
 Serial.Begin(9600);
}


// Loop
void loop(){
 linefollow();
 delay(x);
}


// Read sensor
void ReadSensor(int Sensor,int CheckRate){
 int sum = 0;
 for(int i=0;i<CheckRate;i++){
   sum += analogRead(Sensor);
 }
 sum = sum/CheckRate;
 return sum;
}

// Forward Sensor Array
void FwSensorRead(){
 for(int i=0;i<8;i++){
   if(i==0){
    s=SF1;
   }
   else if(i==1){
    s=SF2;
   }
   else if(i==2){
    s=SF3;
   }
   else if(i==3){
    s=SF4;
   }
   else if(i==4){
    s=SF5;
   }
   else if(i==5){
    s=SF6;
   }
   else if(i==6){
    s=SF7;
   }
   else if(i==7){
    s=SF8;
   }
   if(ReadSensor(s,SensorReadRate)>BlackWiteChange){
    FwSensorArray[i] = '1';
   } else {
    FwSensorArray[i] = '0';
   }
 }
 delay(x);
}

// Medium Sensor Array
void MeSensorRead(){
 for(int i=0;i<3;i++){
   if(i==0){
    s=SM1;
   }
   else if(i==1){
    s=SM2;
   }
   else if(i==2){
    s=SM3;
   }

   if(ReadSensor(s,SensorReadRate)>BlackWiteChange){
    MeSensorArray[i] = '1';
   } else {
    MeSensorArray[i] = '0';
   }
 }
}

// Back Sensor Array
void BwSensorRead(){
 for(int i=0;i<5;i++){
   if(i==0){
    s=SB1;
   }
   else if(i==1){
    s=SB2;
   }
   else if(i==2){
    s=SB3;
   }
   else if(i==3){
    s=SB4;
   }
   else if(i==4){
    s=SB5;
   }
   if(ReadSensor(s,SensorReadRate)>BlackWiteChange){
    FwSensorArray[i] = '1';
   } else {
    FwSensorArray[i] = '0';
   }
 }
}

// set Binory valuses for sensor arrays
void Sensors(){
 FwSensorRead();
 delay(x);
 MeSensorRead();
 delay(x);
 BwSensorRead();
 delay(x);
 FwBinory=atio(FwSensorArray);
 MeBinory=atio(MeSensorArray);
 BwBinory=atio(BwSensorArray); 
 FwBinory=FwBinory+80000000;
 MeBinory=MeBinory+3000;
 BwBinory=BwBinory+500000;
 delay(x);
}

// driver motor controlling 
void driver(float lspeed,float rspeed){
// left motor
 if(lspeed>0){
  digitalWrite(DML1,HIGH);
  digitalWrite(DML2,LOW);
  analogWrite(DMSL,lspeed);
 }
 else if(lspeed==0){
  digitalWrite(DML1,LOW);
  digitalWrite(DML2,LOW);
  analogWrite(DMSL,lspeed);
 } else {
  digitalWrite(DML1,LOW);
  digitalWrite(DML2,HIGH);
  analogWrite(DMSL,-lspeed);
 }
// right motor 
 if(rspeed>0){
  digitalWrite(DMR1,HIGH);
  digitalWrite(DMR2,LOW);
  analogWrite(DMSR,rspeed);
 }
 else if(lspeed==0){
  digitalWrite(DMR1,LOW);
  digitalWrite(DMR2,LOW);
  analogWrite(DMSr,rspeed);
 } else {
  digitalWrite(DMR1,LOW);
  digitalWrite(DMR2,HIGH);
  analogWrite(DMSR,-rspeed);
 }
 delay(x);
}

// line follow
void linefollow(){
 while(true){
  Sensors();
  // drive forward
  if(FwBinory==800011000){
    driver(LHS,RHS);
  // turn left low
  }else if(FwBinory==800111000 || FwBinory==800110000){
    driver(LMS,RHS);
  // turn left meduim
  }else if(FwBinory==801110000 || FwBinory==801100000){
    driver(LLS,RHS);
  // turn left high
  }else if(FwBinory==811110000 || FwBinory==811111000){
    driver(-LHS,RHS);
  // turn right low
  }else if(FwBinory==800011100 || FwBinory==800001100){
    dirver(LHS,RMS);
  // turn right meduim
  }else if(FwBinory==800001110 || FwBinory==800000110){
    driver(LHS,RLS);
  // turn right high
  }else if(FwBinory==800001111 || FwBinory==800000111){
    driver(LHS,-RHS);
  }
  else{
    driver(LS,LS);
    break;
  }
 }
}
