// Sonar specifi
#define echoPin 12
#define trigPin 13
long duration, distance;
// include the library code
#include <LiquidCrystal.h>
LiquidCrystal lcd(11,10,9,8,7,6);
// This code is for check the sensor input
int SRR = 10; // SensorReadRate
// Motor speed controller
int LHS = 250;
int LMS = 155;
//int LLS = 0;
int RHS = 240;
int RMS = 148;
//int RLS = 0;

// sensor cut offs
int FwCUToff = 250;
int BwCUToff = 250;

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
int sb6 = A13;
// Driver motor
// Left Moter
int DML1 = 37;
int DML2 = 35;
int DMLS = 2;
// Right Moter
int DMR1 = 33;
int DMR2 = 31;
int DMRS = 3;

int MaxDis = 6;
int MinDis = 4;
long FullTurn = 8;

int i; // Just a rubbish intiger for loops
int x = 1; // just a rubbish inteter for delay
int sfh[8]; // SensorForwardHigh
int sfl[8]; // SenosrForwardLow
char SFA[8]; // SensorForwardArray

int sbh[6]; // SensorBackHigh
int sbl[6]; // SenosrBackLow
char SBA[6]; // SensorBackArray


long FwBinory = 0;
long FFwB = 100000000;
long BwBinory = 0;
long BBwB = 1000000;

void setup(){
  PINReset();
  CrystalReset();
  sonarRest();
  Serial.begin(9600);
}

void loop(){
  // clibration point found
  //SensorCalFront();   
  //SensorCalBack();
  //delay(10);
  BlackWiteChange();
  lcd.clear();
  lcd.print("R2D2");
  delay(1000);
  lcd.clear();
  lcd.print("start now");
  delay(1000);

  // The Hobbit
  while(true){
    SonarRead();
    if (distance > MaxDis){
      lineFollower();
    } 
    else if(distance < MinDis){
      driver(-LMS,-RMS);	
    } 
    else {
      driver(0,0);
      lcd.clear();
      lcd.print("Load OF Rings");
      delay(3000);	
      // In here ring will lift up {Load Of The Rings}
      lcd.clear();
      lcd.print("Star Wars");
      //SmoothLineFollower();
      StarWars();
      while(true){
        SonarRead();
        if(distance > MaxDis){
          lineFollower();
        }
        else if (distance < MinDis){
          driver(-LMS,-RMS);
        } 
        else{
          lcd.clear();
          lcd.print("Downloading");
          driver(0,0);
          delay(3000);
          // Limit switch checking here 
          // {Return Of Kings}
          break;
        }
      }
      break;
    }
  }

  // Turn 180
  Turn180();



  // Back TO The FUture 1
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[0]== '1' || SFA[7]=='1'){
     // SmoothLineFollower();
      StarWars();
    }
    else if(FFwB == 100000000){

      if (BBwB == 1111111){
        driver(0,0);
        break;
      }
      else{
        driver(LMS,RMS);
      } 
    }
    else {
      lineFollower();
    }

  }

  // Turn 90
  Turn90();

  // Get ring form one side
  GetBlue();

  // Turn 180;
  Turn180C();

// Lpassing
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[7] == '1'){
      //driver(0,0);
      Lpass();
      break;
    } else {
      lineFollower();
    }
  }
// after L pass
  while (true){
    FwBwSensorRead();
    SensorBinory();
    if(FFwB == 11111111 || FFwB == 100000000){
      GetRed();
      break;
    } else {
      lineFollower();
    }
  }







/*
  // T junction 1
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(  SFA[7]=='1' ){
      lineFollower();
    }
    else if (FFwB = 111111111 || FFwB == 100000000){
      GetRed();
      break;
    } 
    else {
      lineFollower();
    }
  }
*/
  // 180 turn
  Turn180B();

// Up to here done


while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[0] == '1'){
      //driver(0,0);
      Rpass();
      break;
    } else {
      lineFollower();
    }
  }

Turn90();
/*
  // T junction
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[0] == '1'|| SFA[1]=='1'){
      while(true){
        if(SBA[7] == '1' || SBA[6] == '1'){
          Turn90();
          break;
        }
        else{
          driver(LMS,RMS);
        }
      }
      break;
    } 
    else {
      lineFollower();
    }
  }
  */

  // GO to HELL
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[0]=='1' || SFA[7]=='1'){
    //  SmoothLineFollower();
      StarWars();
      FinalDestination();
      break;
    } 
    else{
      lineFollower();
    }
  }

  // Turn 180
  Turn180();

  // Back to the future II
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[0]== '1' || SFA[7]=='1'){
      StarWars();
    }
    else if(FFwB == 100000000 || FFwB == 111111111){
      driver(0,0);
      break;
    }
    else {
      lineFollower();
    }

  }

  // All coads come over here
  // Bread at the WOrlds End
  At_Worlds_End();
}

void Rpass(){
  while (true){
    FwBwSensorRead();
    SensorBinory();
    if(SBA[0]=='1' || SBA[5] == '1'){
      driver(0,0);
      break;
    } else{
      driver(LMS,RMS);
    }
  }
}

void Lpass(){
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(SFA[7] == '1'){
      driver(LMS,RMS);
      //Lpass();
      
    } else {
      break;
    }
  }
}

void FinalDestination(){
  while(true){
    FwBwSensorRead();
    SensorBinory();
    SonarRead();
    if(SFA[0]=='1' || SFA[7]=='1'){
      break;
    } 
    else {
      if(distance > MaxDis){
        lineFollower();
      } 
      else if(distance < MinDis){
        driver(-LMS,-RMS);
      } 
      else{
        driver(0,0);
        lcd.clear();
        lcd.print("Hi Bro!");
        delay(3000);
        // Unloading both rings
        lcd.clear();
        lcd.print("Going to HELL!");
        break;
      }
    }
  }
}

void GetRed(){
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if(FFwB == 111111111 || FFwB == 100000000){
      while(true){
        SonarRead();
        if(distance > MaxDis){
          driver(LMS,RMS);
        } 
        else if(distance < MinDis){
          driver(-LMS,-RMS);
        } 
        else{
          driver(0,0);
          lcd.clear();
          lcd.print("REloading..");
          delay(3000);
          // Load the ring here
          lcd.clear();
          lcd.print("R2D2");
          break;
        }
      }
      break;
    } 
    else{
      if(SFA[7] == '1' ){
        driver(0,0);
      }
      else{
      lineFollower();
    }
    }
  }
}

void GetBlue(){
  while(true){
    FwBwSensorRead();
    SensorBinory();

    if(FFwB == 111111111 || FFwB == 100000000){
      while(true){
        FwBwSensorRead();
        SensorBinory();
        SonarRead();
        if(distance > MaxDis){
          if(FFwB == 11111111 || FFwB == 100000000){
            driver(LMS,RMS);
          }
          else{
            lineFollower();
          }
        } 
        else if(distance < MinDis){
          driver(-LMS,-RMS);
        } 
        else{
          driver(0,0);
          lcd.clear();
          lcd.print("Loading..");
          delay(3000);
          // Load the ring here
          lcd.clear();
          lcd.print("R2D2");
          break;
        }
      }
      break;
    } 
    else{
      lineFollower();
    }
  }
}

void Turn90(){
  while(true){
    FwBwSensorRead();
    SensorBinory();
    if (SFA[3]=='1' || SFA[4]=='1' || SFA[5]=='1'){
      //driver(0,0);
      break;
    }
    else{
      driver(LMS,-RMS);
    }
  }
}



void At_Worlds_End(){
  while (true){
    driver(0,0);
  }
}

void Turn180(){
  lcd.clear();
  lcd.print("Turn180");
  while(true){
    SonarRead();

    if(distance < FullTurn){
      driver(-LMS,-RMS);
    }
    else{
      while(true){		
        FwBwSensorRead();
        SensorBinory();
        if(FFwB == 100000000){
          break;
        }
        else{
          driver(LMS,-RMS);
        }
      }
      while(true){
        FwBwSensorRead();
        SensorBinory();
        if(  SFA[3]=='1'  || SFA[4]=='1' || SFA[2] == '1'){
          break;
        }
        else{
          driver(LMS,-RMS);
        }
      }
      break;
    }
  }
}
void Turn180C(){
  lcd.clear();
  lcd.print("Turn180");
  while(true){
    SonarRead();

    if(distance < 15){
      driver(-LMS,-RMS);
    }
    else{
      while(true){    
        FwBwSensorRead();
        SensorBinory();
        if(FFwB == 100000000){
          break;
        }
        else{
          driver(LMS,-RMS);
        }
      }
      while(true){
        FwBwSensorRead();
        SensorBinory();
        if(  SFA[3]=='1'  || SFA[4]=='1' || SFA[2] == '1'){
          break;
        }
        else{
          driver(LMS,-RMS);
        }
      }
      break;
    }
  }
}

void Turn180B(){
  lcd.clear();
  lcd.print("Turn180");
  while(true){
    SonarRead();

    if(distance < 15){
      driver(-LMS,-RMS);
    }
    else{
      while(true){    
        FwBwSensorRead();
        SensorBinory();
        if(FFwB == 100000000){
          break;
        }
        else{
          driver(-LMS,RMS);
        }
      }
      while(true){
        FwBwSensorRead();
        SensorBinory();
        if(  SFA[3]=='1'  || SFA[4]=='1' || SFA[5] == '1'){
          break;
        }
        else{
          driver(-LMS,RMS);
        }
      }
      break;
    }
  }
}



void StarWars(){

  while(true){
    FwBwSensorRead();
    SensorBinory();
    //SonarRead();
    if(BBwB == 1111111){
      while(true){
        FwBwSensorRead();
        SensorBinory();
        if(SFA[0] == '1' || SFA[7]== '1'){
          driver(LMS,RMS);
        } 
        else {
          break;
        }
      }
      break;
    }
    else {
      driver(LMS,RMS);
    }
  }

}

void sonarRest(){
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
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
  pinMode(sb6,INPUT);
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
void SensorCalFront(){
  lcd.clear();
  lcd.print("Forward calibration");
  delay(2000);
  lcd.clear();
  lcd.print("Keep on White");
  Serial.println("Keep on white");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on W 4");
  delay(1000);
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
  delay(1000);
  lcd.print("white Forward INT"); //forward initialized
  for(int i=0;i<8;i++){
    sfh[i]=0;
  }
  delay(1000);
  lcd.clear();
  lcd.print("White FW Reading...");
  delay(1000);
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
  lcd.clear();
  lcd.print("White Forward DONE!!");
  delay(1000);

  Serial.println("Wite color sensed");
  lcd.clear();
  lcd.print("Keep on Black");
  Serial.println("Keep on Black");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on B 4");
  delay(1000);
  lcd.clear();
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
  lcd.clear();
  lcd.print("Black Forward DONE !!");
  delay(1000);
  Serial.println("Black color sensed");
  lcd.clear();
  lcd.print("Black Color Sensed");
  delay(1000);
}

void SensorCalBack(){
  lcd.clear();
  lcd.print("Backword calibration");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on White");
  Serial.println("Keep on white");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on W 4");
  delay(1000);
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
  delay(1000);
  lcd.print("white Backword INT");
  for(int i=0;i<6;i++){
    sbh[i]=0;
  }
  delay(1000);





  lcd.clear();
  lcd.print("white BW Reading...");
  delay(1000);
  for(int i=0; i<10;i++){
    sbh[0] += analogRead(sb1);
    sbh[1] += analogRead(sb2);
    sbh[2] += analogRead(sb3);
    sbh[3] += analogRead(sb4);
    sbh[4] += analogRead(sb5);
    sbh[5] += analogRead(sb6);
  }
  for(int i=0;i<6;i++){
    sbh[i]=sbh[i]/10;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("white Backword DONE!!");
  delay(1000);
  Serial.println("Wite color sensed");
  //delay(2000); // White color reading is done
  lcd.clear();
  lcd.print("Keep on Black");
  Serial.println("Keep on Black");
  delay(1000);
  lcd.clear();
  lcd.print("Keep on B 4");
  delay(1000);
  lcd.clear();
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
  for(int i=0;i<6;i++){
    sbl[i]=0;
  }
  //delay(1000);
  lcd.clear();
  lcd.print("Black BW Reading...");
  delay(1000);
  for(int i=0; i<10;i++){
    sbl[0] += analogRead(sb1);
    sbl[1] += analogRead(sb2);
    sbl[2] += analogRead(sb3);
    sbl[3] += analogRead(sb4);
    sbl[4] += analogRead(sb5);
    sbl[5] += analogRead(sb6);
  }
  for(int i=0;i<6;i++){
    sbl[i]=sbl[i]/10;
  }
  lcd.clear();
  lcd.print("Black Backword DONE !!");
  Serial.println("Black color sensed");
  lcd.clear();
  delay(1000);
  lcd.clear();
  lcd.print("calibration compleated");
  delay(1000);
}


void BlackWiteChange(){
  for(int i=0;i<8;i++){
    //sfh[i]= sfh[i] + sfl[i];
    //sfh[i]= sfh[i] / 2;
    sfh[i] = FwCUToff;  // Static value
  }
  for(int i=0;i<6;i++){
    //sbh[i]= sbh[i] + sbl[i];
    //sbh[i]= sbh[i] / 2;
    sbh[i] = BwCUToff;  // static value
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
  if(ReadSensor(sb6,SRR) < sbh[5]){
    SBA[5] = '0';
  } 
  else {
    SBA[5] = '1';
  }

  delay(10);
}
// Convert sensor values to binory
void SensorBinory(){

  //FwBinory=atoi(SFA)+100000000;
  //BwBinory=atoi(SBA);
  FFwB = 100000000;
  FwBinory = 0;
  BBwB = 1000000;
  BwBinory = 0;
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


  if(SBA[0]=='1'){
    BBwB+=1;
    BwBinory -= 5;
  }
  if(SBA[1]=='1'){
    BBwB+=10;
    BwBinory -= 3;
  }
  if(SBA[2]=='1'){
    BBwB+=100;
    BwBinory += 1;
  }
  if(SBA[3]=='1'){
    BBwB+=1000;
    BwBinory += 1;
  }
  if(SBA[4]=='1'){
    BBwB+=10000;
    BwBinory += 3;
  }
  if(SBA[5]=='1'){
    BBwB+=100000;
    BwBinory += 5;
  }



  // FwBinory+=800000000;
  //BwBinory+=500000;

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

void SonarRead(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);

  distance = duration/58.2;
  delay(10);
}

void lineFollower(){
  FwBwSensorRead();
  SensorBinory();
  lcd.clear();
  /*if(FwBinory == 2||){
   	driver(LHS,RHS);
   	}
   	else */
  if(FwBinory==0) {
    driver(0,0);
  }  
  // turn right
  else if(FwBinory < 1){
    driver(LHS,RHS+(30*(FwBinory)));
  }
  // turn Left
  else if(FwBinory > 2) {
    driver(LHS-(30*(FwBinory)),RHS);
  }
  else {
    driver(LHS,RHS);
  }
  lcd.print(FFwB);
}

void SmoothLineFollower(){
 
  while(true){
  FwBwSensorRead();
  SensorBinory();
    if(FwBinory == 2 ){
      driver(LMS,RMS);
    } else if(FwBinory < 5 && FwBinory > 2 ){
            driver(LMS,RMS-(30*(FwBinory)));
     
    } else if(FwBinory > -5 && FwBinory <2 ){
 driver(LMS-(30*(FwBinory)),RMS);
    } else{
      break;
    }
  }
}

void BackFollower(){
  FwBwSensorRead();
  SensorBinory();
  lcd.clear();
  if(BwBinory == 2){
    driver(-LHS,-RHS);
  }
  else if(BwBinory == 0){
    driver(0,0);
  }
  else if (BwBinory > 3){
    driver(-LHS,RHS/(BwBinory));
  }
  else if (BwBinory < -1){
    driver(-LHS/BwBinory,-RHS);
  }
  lcd.print(BBwB);
}

