int lspeed = 250;
int rspeed = 250;

int mlfp = 23;
int mlrp = 25;
int mlpw = 5;

int mrfp = 27;
int mrrp = 29;
int mrpw =4;

void setup(){
	pinMode(mlfp,OUTPUT);
	pinMode(mlrp,OUTPUT);
	pinMode(mlpw,OUTPUT);

	pinMode(mrfp,OUTPUT);
	pinMode(mrrp,OUTPUT);
	pinMode(mrpw,OUTPUT);

	Serial.begin(9600);
	Serial.println("Pro Start");
}


void loop(){
	analogWrite(mlpw,lspeed);
	analogWrite(mrpw,rspeed);


	relase();
}

void relase(){
	digitalWrite(mrfp,HIGH);
	digitalWrite(mrrp,LOW);
}

void Catch(){
	digitalWrite(mlfp,HIGH);
	digitalWrite(mlrp,HIGH);
	digitalWrite(mrfp,LOW);
	digitalWrite(mrrp,HIGH);
}

void up(){
	digitalWrite(mlfp,LOW);
	digitalWrite(mlrp,HIGH);
	digitalWrite(mrfp,HIGH);
	digitalWrite(mrrp,HIGH);
}

void down(){
	digitalWrite(mlfp,HIGH);
	digitalWrite(mlrp,LOW);
	digitalWrite(mrfp,HIGH);
	digitalWrite(mrrp,HIGH);
}

void breakall(){

	digitalWrite(mlfp,HIGH);
	digitalWrite(mlrp,HIGH);
	digitalWrite(mrfp,HIGH);
	digitalWrite(mrrp,HIGH);
}
