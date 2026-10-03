#include <LiquidCrystal.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// DS1302: DAT, CLK, RST
ThreeWire myWire(7, 6, 8);
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  lcd.begin(16, 2);

  Rtc.Begin();

  Rtc.SetDateTime(RtcDateTime(2026, 10, 2, 20, 55, 0));

  Rtc.SetIsWriteProtected(false);
  Rtc.SetIsRunning(true);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HAIL MARY");
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  lcd.setCursor(0, 1);

  if (now.Hour() < 10) lcd.print("0");
  lcd.print(now.Hour());
  lcd.print(":");

  if (now.Minute() < 10) lcd.print("0");
  lcd.print(now.Minute());
  lcd.print(":");

  if (now.Second() < 10) lcd.print("0");
  lcd.print(now.Second());

  delay(500);
}
