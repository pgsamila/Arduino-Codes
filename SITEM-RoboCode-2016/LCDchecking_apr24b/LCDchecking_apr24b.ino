#include <LiquidCrystal.h>
LiquidCrystal lcd(11,10,9,8,7,6);

void CrystalReset(){
  lcd.begin(16,2);
  lcd.print("R2D2 Restarted");
}

void setup(){
 CrystalReset();
}
void loop(){
 lcd.setCursor(0, 1);
 lcd.print(millis()/1000);
}
