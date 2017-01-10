// Driver Motor
// Left motor
int DML1 = 37;
int DML2 = 35;
int DMLS = 2;
// Right motor
int DMR1 = 33;
int DMR2 = 31;
int DMRS = 3;


void setup(){
// motor pints
 pinMode(DMLS,OUTPUT);
 pinMode(DMRS,OUTPUT);
 pinMode(DML1,OUTPUT);
 pinMode(DML2,OUTPUT);
 pinMode(DMR1,OUTPUT);
 pinMode(DMR2,OUTPUT);
}

void loop(){
digitalWrite(DML1,LOW);
digitalWrite(DML2,HIGH);
analogWrite(DMLS,150);
}
