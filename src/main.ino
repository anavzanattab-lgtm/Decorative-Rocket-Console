#include <LiquidCrystal.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// =====================================================
// LCD 16x2
// RS, E, D4, D5, D6, D7
// =====================================================

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// =====================================================
// RTC DS1302
// DAT, CLK, RST
// =====================================================

ThreeWire myWire(7, 6, 8);
RtcDS1302<ThreeWire> Rtc(myWire);


// =====================================================
// LEDs
// =====================================================

const byte leds[] = {
  A3,
  A2,
  A1,
  A0,
  10,
  9
};

const byte cantidadLeds = 6;


// =====================================================
// BOTONES
// =====================================================

const byte boton10 = 0;
const byte boton20 = 1;
const byte boton30 = A4;
const byte botonPower = A5;


// =====================================================
// ESTADO DE LOS BOTONES
// =====================================================

bool estadoAnterior10 = HIGH;
bool estadoAnterior20 = HIGH;
bool estadoAnterior30 = HIGH;
bool estadoAnteriorPower = HIGH;


// =====================================================
// ESTADO DE LOS LEDS
// =====================================================

bool ledsEncendidos = false;


// =====================================================
// CUENTA REGRESIVA
// =====================================================

bool cuentaActiva = false;

unsigned long inicioCuenta = 0;
unsigned long duracionCuenta = 0;

int ultimoLedApagado = -1;


// =====================================================
// SETUP
// =====================================================

void setup() {

  // -------------------------
  // LCD
  // -------------------------

  lcd.begin(16, 2);
  lcd.clear();


  // -------------------------
  // RTC
  // -------------------------

  Rtc.Begin();

  Rtc.SetIsWriteProtected(false);
  Rtc.SetIsRunning(true);

  // Solo establece la hora si el RTC no tiene una
  // fecha/hora válida.
  if (!Rtc.IsDateTimeValid()) {
    RtcDateTime compiled(__DATE__, __TIME__);
    Rtc.SetDateTime(compiled);
  }


  // -------------------------
  // LEDs
  // -------------------------

  for (byte i = 0; i < cantidadLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }


  // -------------------------
  // Botones
  // -------------------------

  pinMode(boton10, INPUT_PULLUP);
  pinMode(boton20, INPUT_PULLUP);
  pinMode(boton30, INPUT_PULLUP);
  pinMode(botonPower, INPUT_PULLUP);


  // -------------------------
  // Pantalla inicial
  // -------------------------

  lcd.setCursor(3, 0);
  lcd.print("HAIL  MARY");

  delay(1000);
}


// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  // El reloj se actualiza siempre
  actualizarReloj();


  // Si hay una cuenta activa,
  // solamente actualizamos esa cuenta.
  if (cuentaActiva) {

    actualizarCuenta();

    return;
  }


  // Si no hay cuenta activa,
  // revisamos los botones.

  controlarBotonPower();

  controlarBoton10();

  controlarBoton20();

  controlarBoton30();
}


// =====================================================
// ACTUALIZAR RELOJ
// =====================================================

void actualizarReloj() {

  RtcDateTime now = Rtc.GetDateTime();

  lcd.setCursor(3, 0);
  lcd.print("HAIL  MARY");

  lcd.setCursor(4, 1);

  if (now.Hour() < 10) {
    lcd.print("0");
  }

  lcd.print(now.Hour());
  lcd.print(":");

  if (now.Minute() < 10) {
    lcd.print("0");
  }

  lcd.print(now.Minute());
  lcd.print(":");

  if (now.Second() < 10) {
    lcd.print("0");
  }

  lcd.print(now.Second());

  lcd.print("        ");
}


// =====================================================
// BOTÓN ON / OFF
// =====================================================

void controlarBotonPower() {

  bool estadoActual = digitalRead(botonPower);

  if (estadoAnteriorPower == HIGH &&
      estadoActual == LOW) {

    ledsEncendidos = !ledsEncendidos;

    if (ledsEncendidos) {
      encenderTodos();
    }
    else {
      apagarTodos();
    }

    delay(50);
  }

  estadoAnteriorPower = estadoActual;
}


// =====================================================
// BOTÓN 10 SEGUNDOS
// =====================================================

void controlarBoton10() {

  bool estadoActual = digitalRead(boton10);

  if (estadoAnterior10 == HIGH &&
      estadoActual == LOW) {

    iniciarCuenta(10000);

    delay(50);
  }

  estadoAnterior10 = estadoActual;
}


// =====================================================
// BOTÓN 20 SEGUNDOS
// =====================================================

void controlarBoton20() {

  bool estadoActual = digitalRead(boton20);

  if (estadoAnterior20 == HIGH &&
      estadoActual == LOW) {

    iniciarCuenta(20000);

    delay(50);
  }

  estadoAnterior20 = estadoActual;
}


// =====================================================
// BOTÓN 30 SEGUNDOS
// =====================================================

void controlarBoton30() {

  bool estadoActual = digitalRead(boton30);

  if (estadoAnterior30 == HIGH &&
      estadoActual == LOW) {

    iniciarCuenta(30000);

    delay(50);
  }

  estadoAnterior30 = estadoActual;
}


// =====================================================
// INICIAR CUENTA
// =====================================================

void iniciarCuenta(unsigned long duracion) {

  cuentaActiva = true;

  inicioCuenta = millis();

  duracionCuenta = duracion;

  ultimoLedApagado = -1;

  encenderTodos();
}


// =====================================================
// ACTUALIZAR CUENTA
// =====================================================

void actualizarCuenta() {

  unsigned long tiempoTranscurrido =
    millis() - inicioCuenta;


  // -------------------------
  // FIN DE LA CUENTA
  // -------------------------

  if (tiempoTranscurrido >= duracionCuenta) {

    apagarTodos();

    cuentaActiva = false;

    ledsEncendidos = false;

    return;
  }


  // -------------------------
  // Calcular cuántos LEDs
  // deberían estar apagados
  // -------------------------

  int ledsApagados =
    (tiempoTranscurrido * cantidadLeds)
    / duracionCuenta;


  // -------------------------
  // Apagar LEDs progresivamente
  // -------------------------

  while (ultimoLedApagado < ledsApagados - 1) {

    ultimoLedApagado++;

    digitalWrite(
      leds[ultimoLedApagado],
      LOW
    );
  }
}


// =====================================================
// ENCENDER TODOS
// =====================================================

void encenderTodos() {

  for (byte i = 0; i < cantidadLeds; i++) {
    digitalWrite(leds[i], HIGH);
  }

  ledsEncendidos = true;
}


// =====================================================
// APAGAR TODOS
// =====================================================

void apagarTodos() {

  for (byte i = 0; i < cantidadLeds; i++) {
    digitalWrite(leds[i], LOW);
  }

  ledsEncendidos = false;
}
