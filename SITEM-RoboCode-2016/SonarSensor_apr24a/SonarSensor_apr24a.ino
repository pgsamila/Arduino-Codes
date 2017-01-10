#define echoPin 12
#define trigPin 13


long duration, distance;

void setup(){
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
}

void loop(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  
  distance = duration/58.2;
  
  Serial.println(distance);
  
  delay(50);
}
