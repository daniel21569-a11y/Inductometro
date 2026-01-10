#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <algorithm>


LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int transistorPin = PA1;
const int measurePin = PA0;

const double C_ref = 0.968e-9;
const double PI_SQ = 9.8696044;

void setup() {
  Wire.begin(PB7, PB6);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("L-Meter v3.7 MST");

  pinMode(transistorPin, OUTPUT);
  digitalWrite(transistorPin, LOW);
  pinMode(measurePin, INPUT);

  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

inline bool isHigh() { return (GPIOA->IDR & GPIO_PIN_0); }

// Estructura simplificada
struct Result {
  uint32_t cycles;
  int quality; // Conteo de oscilaciones detectables
};

Result analyzeInductor() {
  Result res = {0, 0};

  // 1. CARGA
  digitalWrite(transistorPin, HIGH);
  delayMicroseconds(500);

  noInterrupts();
  // 2. DISPARO
  digitalWrite(transistorPin, LOW);
  uint32_t tStart = DWT->CYCCNT;

  uint32_t timeout = 500000; // 5ms total

  // 3. MEDIR EL PRIMER PERIODO COMPLETO (Timing critico)
  // Ciclo 1: Esperar primera subida
  while (!isHigh() && (DWT->CYCCNT - tStart < timeout))
    ;
  uint32_t t0 = DWT->CYCCNT;

  // Ciclo 1: Esperar bajada
  while (isHigh() && (DWT->CYCCNT - t0 < timeout))
    ;
  // Ciclo 1: Esperar segunda subida (Fin ciclo 1)
  while (!isHigh() && (DWT->CYCCNT - t0 < timeout))
    ;
  uint32_t t1 = DWT->CYCCNT;

  // Ciclo 2: Esperar bajada
  while (isHigh() && (DWT->CYCCNT - t1 < timeout))
    ;
  // Ciclo 2: Esperar tercera subida (Fin ciclo 2)
  while (!isHigh() && (DWT->CYCCNT - t1 < timeout))
    ;
  uint32_t t2 = DWT->CYCCNT;

  if (t2 > t1 && (t2 - t1 < 100000)) {
    res.cycles = (t2 - t1);
  }

  // 4. CONTAR OSCILACIONES (Calidad / R interna)
  // Seguimos contando mientras haya energía
  int count = 2; // Ya hemos visto 2 ciclos
  uint32_t lastT = t2;
  while (count < 100) {
    while (isHigh() && (DWT->CYCCNT - lastT < 20000))
      ; // Esperar bajada
    if (DWT->CYCCNT - lastT >= 20000)
      break; // Si tarda mucho, se acabó
    while (!isHigh() && (DWT->CYCCNT - lastT < 20000))
      ; // Esperar subida
    if (DWT->CYCCNT - lastT >= 20000)
      break;
    lastT = DWT->CYCCNT;
    count++;
  }
  res.quality = count;

  interrupts();
  return res;
}

void loop() {
  const int SAMPLES = 30;
  uint32_t cyclesReadings[SAMPLES];
  int qReadings[SAMPLES];
  int count = 0;

  for (int i = 0; i < SAMPLES; i++) {
    Result r = analyzeInductor();
    if (r.cycles > 10) {
      cyclesReadings[count] = r.cycles;
      qReadings[count] = r.quality;
      count++;
    }
    delay(10);
  }

  lcd.setCursor(0, 0);
  if (count < 5) {
    lcd.print("SIN OSCILACION  ");
    lcd.setCursor(0, 1);
    lcd.print("R muy baja?     ");
  } else {
    std::sort(cyclesReadings, cyclesReadings + count);
    std::sort(qReadings, qReadings + count);

    uint32_t cycles = cyclesReadings[count / 2];
    int qual = qReadings[count / 2];

    double freq = 100000000.0 / (double)cycles;
    double L = 1.0 / (4.0 * PI_SQ * freq * freq * C_ref);
    double L_uH = L * 1e6;

    // Linea 0: L y Freq
    lcd.print("L:");
    if (L_uH > 999.9) {
      lcd.print(L_uH / 1000.0, 2);
      lcd.print("mH");
    } else {
      lcd.print(L_uH, 1);
      lcd.print("uH");
    }
    lcd.print(" ");
    lcd.print((int)(freq / 1000));
    lcd.print("kHz   ");

    // Linea 1: Resistencia estimada
    lcd.setCursor(0, 1);
    // Aproximación: una bobina de 1 1 Ohm suele dar unas 15-20 oscilaciones.
    // Una de 0.2 Ohm se apaga casi inmediato. Una de 5 Ohm dura poco.
    lcd.print("Eco:");
    lcd.print(qual);
    lcd.print(" ciclos  ");

    lcd.setCursor(12, 1);
    if (qual > 20)
      lcd.print("OK ");
    else if (qual > 5)
      lcd.print("LOW");
    else
      lcd.print("BAD");
  }

  delay(200);
}
