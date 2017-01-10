/** @name FinalCode.ino
 *  @brief This code will calculate the voltage & frequancy of
 *         AC suplay and display it on LCD display.
 *         The data will save on SD card.
 *         The data saving is done once every second.
 *         For the clock, I have used the internal clock as interrupt.
 *
 *  @author Amila Sampath Pelaketigamage <pgasampath/@gmail.com>
 *
 *  @bug   The internel clock on Arduino boad will not be exact clock time.
 *         Therefore saving data can be happen about 2 savings in 1 second.
 *
 *  @log
 *  2016 - 08 - 20 Create this file <br />
 *  2016 - 08 - 29 Create voltage calculating funciton <br />
 *  2016 - 09 - 10 Create display part <br />
 *  2016 - 09 - 20 Create frequancy calculating <br />
 *  2016 - 09 - 25 Create display ploting <br />
 *  2016 - 10 - 02 Create data saving <br />
 *  2016 - 10 - 18 Finalized the code <br />
 */

#include <SD.h>
#include <SPI.h>
#include <UTFT.h>
#include <Keypad.h>

//******************PIN DEFINING******************************
int R1 = 9, R2 = 8, R3 = 7, R4 = 6; //connect to the row pinouts of the keypad
int C1 = 5, C2 = 4, C3 = 3, C4 = 2; //connect to the column pinouts of the keypad
int F_pin = 10;   // pin for read frequancy
int V_pin = A0;  // pin for read Voltage
int SD_pin = 53; // sd card pin
int Buzzer = 12; // pin for Buzzer
// ----LCD DISPLAY----
UTFT myGLCD(ILI9481, 38, 39, 40, 41);
//*********************REFERENCES*****************************
double Fref = 187500; // Frequancy Reference
double Vref = 2.66666666667;      // Voltage Reference
double VFactor = 2.67;
double Vavg;
int FL = 45;  // Low Frequancy Level
int FH = 55;  // High Frequancy Level
//*********************VARIABLES******************************




// timer

//yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
int timer1_counter;

//Frequancy
int Fin, T, x, y;
int buf[478];
double F;

//Voltage
int Vin, t, samples, Vsamp;
double V, Vtemp;

// Time Settings
String Minit = "00";
String Hour = "13";
String Day = "11";
String Month = "10";
String year = "2016";
int yy=2017, mm=01, dd=01, hh, mins, ss = 00;

// File Settings
String FileName="Log.txt";
File myFile;

// Graph
int v1, v2, v3, v4, v5, v6, v7, v8, v9, v10;
int f1, f2, f3, f4, f5, f6, f7, f8, f9, f10;
//*********************DISPLAY SETTINGS***********************

// ----FONTS----
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// ----LOGO----
int Logox, Letx, Lety;

//*****************************************************//
//*                                                   *//
//*     ----DO NOT EDIT BELOW FORM THIS POINT!----    *//
//*                                                   *//
//*****************************************************//

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3, C4};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
///
/// \brief setup
///        This will initialized the settings.
///
void setup() {
  initialization();
  TimerInitialization();
}

///
/// \brief loop
///        This code will run forever.
///        If the frequancies went away in correct reagion,
///        The buzzer will trun on.
///
void loop() {
  if(F<FL || F>FH){
    digitalWrite(Buzzer,HIGH);
    delay(2000);
  }
    digitalWrite(Buzzer,LOW);
}

//***********Voltage Calculate******************
///
/// \brief Voltage_Cal_Sample_RMS
///        This function will calculate the RMS vlotage
///
void Voltage_Cal_Sample_RMS() {
   //Window();
   Vsamp=0;
   V=0;
   Vavg=0;
  while(Vsamp<10){
      if(digitalRead(F_pin)== HIGH){
       
     //   T=0;
        while(digitalRead(F_pin) == HIGH){
          /*Vin = analogRead(V_pin);
          Vtemp = (int)Vin*Vref;
          V += Vtemp*Vtemp;
          T+=1;*/
          Vin = analogRead(V_pin);
          if(V<Vin){
            V=Vin;
          }
        }
      //  while(digitalRead(F_pin) == LOW){
          /*Vin = analogRead(V_pin);
          Vtemp = (int)Vin*Vref;
          V += Vtemp*Vtemp;
          T+=1;*/
       // }
        /*V = V / T;
        V = sqrt(V);
        V = V * VFactor ; */
        Vin = sqrt(2);
        V=V/(double)Vin;
        V=V/(double)VFactor;
        Vavg +=V;
        break;
      }
      
    }
    Vsamp+=1;
    Vavg =Vavg;
    V=Vavg;
}

// *******************Calculate Frequancy*********************
///
/// \brief Frequancy_Cal
///        This faunction will calculate the frequnacy
///
void Frequancy_Cal() {
  T = pulseIn(F_pin, HIGH);
  F = 1 / (double)T;
  F = F * Fref;

  if(F>1000){
    F=0;
    T=100;
  }
}


//**************TIMER INTERRUPT DATA SAVE*************
///
/// \brief ISR
///        This is the interrunt, to save data once every 1 sec.
///
ISR(TIMER1_OVF_vect)
{
  TCNT1 = timer1_counter;
 
  Voltage_Cal_Sample_RMS();
  Serial.println(samples);
  Frequancy_Cal();
  GraphArea();
  ploting();
  ClockTime();
  SaveDataFile();
  Graphy();
}

///
/// \brief SineWave
///        This will plot a sin wave
///
void SineWave() {
  for (int i = 1; i < 479; i++ )
  {
    y = 159 + (sin(((i * 1.13) * 3.14) * F / 3000 / 1.8) * (95)); //y co-ord
    myGLCD.setColor(0, 255, 255); // cyan pen
    myGLCD.drawPixel(i, y); // draw the wave
  }
}


//**************ININITALIZATION**********************
///
/// \brief initialization
///        This will initialize arduino board
///
void initialization() {
  Logo();
  GraphInitzialize();
  Loading();

  //**************PIN DEFINE************************
  pinMode(F_pin, INPUT);
  pinMode(Buzzer, OUTPUT);
  //************************************************
  Serial.begin(9600);

  SDcardInitialization();

  MenuIcon();

  Window();
}

//***************GraphInitizialize***********
///
/// \brief GraphInitzialize
///        This will initialized the graphs
///
void GraphInitzialize(){
    v1 = 200;
  v2 = 200;
  v3 = 200;
  v4 = 200;
  v5 = 200;
  v6 = 200;
  v7 = 200;
  v8 = 200;
  v9 = 200;
  v10 = 200;
  f1 = 200;
  f2 = 200;
  f3 = 200;
  f4 = 200;
  f5 = 200;
  f6 = 200;
  f7 = 200;
  f8 = 200;
  f9 = 200;
  f10 = 200;
}

//***************TIMER************************
///
/// \brief TimerInitialization
///        This will initialized the inturrupt timer
///
void TimerInitialization() {
  // initialize timer1
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  //TCNT1  = 0;

  timer1_counter = 65000;
  // TCCR1B |= (1 << WGM12);
  TCNT1 = timer1_counter;
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1);
  interrupts();
}
// ****************DATE TIME SETTINGS*******************

// ****************Save Data******************************
///
/// \brief SaveDataFile
///        This will save data
///
void SaveDataFile() {
  myFile.close();
  myFile = SD.open(FileName, FILE_WRITE);
  if (myFile) {
    //Serial.println("Writing the data on file now");
    myFile.print(yy);
    Serial.print(yy);
    myFile.print("-");
    Serial.print("-");
    myFile.print(mm);
    Serial.print(mm);
    myFile.print("-");
    Serial.print("-");
    myFile.print(dd);
    Serial.print(dd);
    myFile.print("-");
    Serial.print("-");
    myFile.print(mins);
    Serial.print(mins);
    myFile.print("-");
    Serial.print("-");
    myFile.print(ss);
    Serial.print(ss);
    myFile.print("-");
    Serial.print("-");
    myFile.print(V);
    Serial.print(V);
    myFile.print("-");
    Serial.print("-");
    myFile.println(F);
    Serial.println(F);
    delay(0);
  } else {
    myGLCD.setBackColor(VGA_GRAY);
    myGLCD.setColor(255, 255, 0);
    myGLCD.print("* Error on opening file & saving data *", CENTER, 307);
    delay(400);
  }
  myFile.close();
}

// ***************Menu Settings**************************
///
/// \brief MenuIcon
///        Thiw will build the main menu
///
void MenuIcon() {
  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 0, 255);
  myGLCD.fillRoundRect (35, 80, 200, 270);
  myGLCD.fillRoundRect (240, 80, 445, 270);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 80, 200, 270);
  myGLCD.drawRoundRect (240, 80, 445, 270);
  myGLCD.setBackColor(255, 0, 255);
  myGLCD.print("(A) Start", 40, 170);
  myGLCD.print("(B) Set Time", 250, 170);
  while (true) {
    char y = keypad.getKey();
    if (y == 'A') {
      InitializeTime();
      break;
    } else if (y == 'B') {
      TurnOnMenu();
      InitializeTime();
      break;
    } else {
    }
  }
}

///
/// \brief TurnONMENUicon
///         This will set the menu
///
void TurnONMENUicon() {
  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 0, 255);

  myGLCD.fillRoundRect (10, 30, 150, 90);
  myGLCD.fillRoundRect (170, 30, 310, 90);
  myGLCD.fillRoundRect (330, 30, 470, 90);
  myGLCD.fillRoundRect (10, 100, 230, 160);
  myGLCD.fillRoundRect (250, 100, 470, 160);
  myGLCD.fillRoundRect (10, 170, 470, 230);

  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 30, 150, 90);
  myGLCD.drawRoundRect (170, 30, 310, 90);
  myGLCD.drawRoundRect (330, 30, 470, 90);
  myGLCD.drawRoundRect (10, 100, 230, 160);
  myGLCD.drawRoundRect (250, 100, 470, 160);
  myGLCD.drawRoundRect (10, 170, 470, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 255);
  myGLCD.print("(1)YEAR", 20, 50);
  myGLCD.print("(2)MONTH", 180, 50);
  myGLCD.print("(3)DAY", 340, 50);
  myGLCD.print("(4)HOURS", 20, 120);
  myGLCD.print("(5)MINUTES", 260, 120);
  myGLCD.print("(D)START TRAKING", 20, 190);
}

///
/// \brief TurnOnMenu
///        This is time setting menu
///
void TurnOnMenu() {
  TurnONMENUicon();
  while (true) {
    char y = keypad.getKey();
    if (y == '1') {
      Serial.println("Set Year");
      SetYear();
    } else if (y == '2') {
      Serial.println("Set Month");
      SetMonth();
    } else if (y == '3') {
      Serial.println("Set Day");
      SetDay();
    } else if (y == '4') {
      Serial.println("Set Hour");
      SetHour();
    } else if (y == '5') {
      Serial.println("Set Minit");
      SetMinit();
    } else if (y == 'D') {
      void InitializeTime();
      break;
    }
  }
}

///
/// \brief InitializeTime
/// initializing time
///
void InitializeTime() {
  yy = (int)year.toInt();
  mm = (int)Month.toInt();
  dd =  (int)Day.toInt();
  hh =  (int)Hour.toInt();
  mins =  (int)Minit.toInt();
  if (yy == 0) {
    yy = 2016;
  }
  if (mm > 12) {
    mm = 1;
  }
  if (dd > 31) {
    dd = 1;
  }
  if (hh > 24) {
    hh = 00;
  }
  if (mins > 59) {
    mins = 00;
  }
  FileNameCheck();
}

///
/// \brief SetMinit
///     set minitues
///
void SetMinit() {
  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Enter Minutes", 150, 100);
  myGLCD.setFont(SevenSegNumFont);
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Minit[0] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Minit, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Minit[1] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Minit, 200, 150);
      delay(100);
      TurnONMENUicon();
      break;
    }

  }
}


///
/// \brief SetHour
///set hours
///
void SetHour() {
  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Enter Hour", 150, 100);
  myGLCD.setFont(SevenSegNumFont);
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Hour[0] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Hour, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Hour[1] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Hour, 200, 150);

      delay(100);
      TurnONMENUicon();
      break;
    }
  }

}

///
/// \brief SetDay
///         set days
///
void SetDay() {
  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Enter Date", 150, 100);
  myGLCD.setFont(SevenSegNumFont);
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Day[0] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Day, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Day[1] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Day, 200, 150);

      delay(100);
      TurnONMENUicon();
      break;
    }
  }
}

///
/// \brief SetMonth
///         set months
///
void SetMonth() {
  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Enter Month", 150, 100);
  myGLCD.setFont(SevenSegNumFont);
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Month[0] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Month, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      Month[1] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(Month, 200, 150);

      delay(100);
      TurnONMENUicon();
      break;
    }
  }
}

///
/// \brief SetYear
/// set years
///
void SetYear() {

  Window();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Enter Year", 150, 100);
  myGLCD.setFont(SevenSegNumFont);
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      year[0] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(year, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      year[1] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(year, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      year[2] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(year, 200, 150);
      break;
    }
  }
  while (true) {
    char y1 = keypad.getKey();
    if (y1) {
      year[3] = y1;
      myGLCD.setColor(0, 0, 255);
      myGLCD.print(year, 200, 150);
      delay(100);
      TurnONMENUicon();
      break;
    }
  }
}


// *************SD CARD SETTINGS*****************
///
/// \brief SDcardInitialization
///         inizialized SD card
///
void SDcardInitialization() { // sd card initialization in initialization

  Serial.print("Initializing SD card...");
  myGLCD.print("Initializing SD card...", CENTER, 300);
  delay(400);
  pinMode(SD_pin, OUTPUT);
  if (!SD.begin(SD_pin)) {
    myGLCD.clrScr();
    myGLCD.fillScr(VGA_RED);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.setColor(VGA_YELLOW);
    Serial.println("Initializing Faild");
    myGLCD.print("Initializing SD card Faild!", CENTER, 200);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("Try reset the device", CENTER, 100);
    myGLCD.print("or check your SD card", CENTER, 120);
    myGLCD.print("to torbleshoot", CENTER, 140);
    while (true) {
      // options to SD card problem
    }
    delay(1000);
    return;
  }
  Serial.println("Initializing Done");
  myGLCD.print("____Initializing Done!____", CENTER, 300);
  delay(400);
}

// **************DISPLAY FIEATURES*****************
// View data
///
/// \brief viewData
///         view data
///
void viewData() {  // view frequancy and voltage in text
  // V and F are voltage and frequancy
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print("Volt :", 10, 15);
  myGLCD.print(String(V), 150, 15);
  myGLCD.setColor(0, 255, 0);
  myGLCD.print("Freq :", 250, 15);
  //wF=random(40,60);
  myGLCD.print(String(F), 400, 15);
  // Serial.print("Voltage: ");
  // Serial.print(V);
  // Serial.print(" Frequancy: ");
  // Serial.println(F);
}

// Axis
///
/// \brief ploting
/// plot data
///
void ploting() { // X,Y axis of graph
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(239, 15, 239, 304); // y axis
  myGLCD.drawLine(1, 250, 478, 250);  // x axis
  for (int i = 9; i < 480; i += 10)
    myGLCD.drawLine(i, 248, i, 252);  // Y axis
  for (int i = 19; i < 320; i += 10)
    myGLCD.drawLine(237, i, 241, i);  // x axis
  //Sidebar();
  viewData();
}
///
/// \brief GraphArea
///         graphe area
void GraphArea() {
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRoundRect(0, 40, 479, 300);
}

// Side Bar
///
/// \brief Sidebar
///
void Sidebar() {
  myGLCD.setBackColor(255, 255, 255);
  myGLCD.setColor(VGA_PURPLE);
  myGLCD.fillRoundRect(10, 270, 229, 300);
  myGLCD.fillRoundRect(251, 270, 469, 300);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(10, 270, 229, 300);
  myGLCD.drawRoundRect(251, 270, 469, 300);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.print("MENU", 80, 280);
  myGLCD.print("CONNECT DEVICE", 300, 280);
}

// Window
///
/// \brief Window
///     display window
///
void Window() { // up/down bars
  myGLCD.clrScr();
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRect(0, 0, 480, 13);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRect(0, 306, 479, 319);
  myGLCD.setColor(255, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("** Elec Check - Home Electrical Check **", CENTER, 1);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(255, 255, 0);
  myGLCD.print("* System is now Online *", CENTER, 307);
}


// Loading
///
/// \brief Loading
///         loading window
///
void Loading() { // lading bar in initialization
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 0, 0);
  for (int i = 0; i < 480; i += 10) {
    delay(50);
    myGLCD.print("=", i, 270);
  }
}

// LOGO
///
/// \brief Logo
///         logo window
///
void Logo() { // logo in initialization
  myGLCD.InitLCD(); // initialization of LCD
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setFont(BigFont);

  Lety = 75;
  Letx = 115;
  LetterE();
  Letx = 185;
  LetterL();
  Letx = 255;
  LetterE();
  Letx = 325;
  LetterC();

  Lety = 185;
  Letx = 80;
  LetterC();
  Letx = 150;
  LetterH();
  Letx = 220;
  LetterE();
  Letx = 290;
  LetterC();
  Letx = 360;
  LetterK();
  delay(1000);
}
///
/// \brief LetterK
///
void LetterK() { // logo letter K
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx + Logox, Lety              , Letx + Logox, Lety + 60);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + Logox + 30      , Letx + 40   , Lety + Logox);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + 30 - Logox , Letx + 40   , Lety + 60 - Logox);
  }
}
///
/// \brief LetterH
///
void LetterH() { // logo letter H
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx + Logox, Lety              , Letx + Logox, Lety + 60);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx + Logox + 40, Lety              , Letx + Logox + 40, Lety + 60);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + 30 + Logox , Letx + 40   , Lety + 30 + Logox);
  }
}
///
/// \brief LetterC
///
void LetterC() { // logo letter C
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx + Logox, Lety              , Letx + Logox, Lety + 60);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + Logox      , Letx + 40   , Lety + Logox);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + 60 + Logox , Letx + 40   , Lety + 60 + Logox);
  }
}
///
/// \brief LetterL
///
void LetterL() { // logo letter L
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx + Logox, Lety              , Letx + Logox, Lety + 60);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + 60 + Logox , Letx + 40   , Lety + 60 + Logox);
  }
}
///
/// \brief LetterE
///
void LetterE() { // logo letter E
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx + Logox, Lety              , Letx + Logox, Lety + 60);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + Logox      , Letx + 40   , Lety + Logox);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + 30 + Logox   , Letx + 20   , Lety + 30 + Logox);
  }
  for (Logox = 0; Logox < 5; Logox++) {
    myGLCD.drawLine(Letx        , Lety + 60 + Logox , Letx + 40   , Lety + 60 + Logox);
  }
}

//Clock
///
/// \brief ClockTime
///         clock for save data
///
void ClockTime() {
  ss += 1;
  if (ss == 60) {
    ss = 0;
    mins += 1;
    if (mins == 60) {
      hh += 1;
      if (hh == 24) {
        hh = 0;
        dd += 1;
        if (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12) {
          if (dd == 31) {
            dd = 1;
            mm += 1;
            FileNameCheck();
            //SD.remove(FileName);
          }
        } else if (mm == 2) {
          if (dd == 28) {
            dd = 1;
            mm += 1;
            FileNameCheck();
            //SD.remove(FileName);
          }
        } else {
          if (dd == 30) {
            dd = 1;
            mm += 1;
            FileNameCheck();
            //SD.remove(FileName);
          }
        }
      }
      if (mm == 12) {
        mm = 1;
        yy += 1;
        FileNameCheck();
        //SD.remove(FileName);
      }
    }
  }
}

///
/// \brief FileNameCheck
///         this will set the data saving file name
///
void FileNameCheck(){
  if(mm==1){
    FileName="January.txt";
  }else if(mm==2){
    FileName="February.txt";
  }else if(mm==3){
    FileName="March.txt";
  }else if(mm==4){
    FileName="April.txt";
  }else if(mm==5){
    FileName="May.txt";
  }else if(mm==6){
    FileName="June.txt";
  }else if(mm==7){
    FileName="July.txt";
  }else if(mm==8){
    FileName="August.txt";
  }else if(mm==9){
    FileName="September.txt";
  }else if(mm==10){
    FileName="October.txt";
  }else if(mm==11){
    FileName="November.txt";
  }else{
    FileName="December.txt";
  }
}

//**************Graph*******************
///
/// \brief Graphy
///     this will draw graph
///
void Graphy() {
  v1 = v2;
  v2 = v3;
  v3 = v4;
  v4 = v5;
  v5 = v6;
  v6 = v7;
  v7 = v8;
  v8 = v9;
  v9 = v10;
  f1 = f2;
  f2 = f3;
  f3 = f4;
  f4 = f5;
  f5 = f6;
  f6 = f7;
  f7 = f8;
  f8 = f9;
  f9 = f10;

  v10 = 480 - (int)V;
  f10 = 255 - (int)F;
  myGLCD.setColor(255, 0, 0); // to voltage plot
  myGLCD.drawLine(0,   v1, 50,  v2);
  myGLCD.drawLine(50,  v2, 100, v3);
  myGLCD.drawLine(100, v3, 150, v4);
  myGLCD.drawLine(150, v4, 200, v5);
  myGLCD.drawLine(200, v5, 250, v6);
  myGLCD.drawLine(250, v6, 300, v7);
  myGLCD.drawLine(300, v7, 350, v8);
  myGLCD.drawLine(350, v8, 400, v9);
  myGLCD.drawLine(400, v9, 450, v10);
  // for(int i=0;i<19;i++){
  //  myGLCD.drawLine(20*i,Gv[i],20*(i+1),Gv[i+1]);
  // }


  myGLCD.setColor(0, 255, 0); // to frequency plot
  // for(int i=0;i<19;i++){
  //  myGLCD.drawLine(20*i,Gf[i],20*(i+1),Gf[i+1]);
  //}

  myGLCD.drawLine(0,   f1, 50,  f2);
  myGLCD.drawLine(50,  f2, 100, f3);
  myGLCD.drawLine(100, f3, 150, f4);
  myGLCD.drawLine(150, f4, 200, f5);
  myGLCD.drawLine(200, f5, 250, f6);
  myGLCD.drawLine(250, f6, 300, f7);
  myGLCD.drawLine(300, f7, 350, f8);
  myGLCD.drawLine(350, f8, 400, f9);
  myGLCD.drawLine(400, f9, 450, f10);
}
