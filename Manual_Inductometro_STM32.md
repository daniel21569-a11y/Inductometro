# Manual Técnico y de Usuario: Inductómetro Digital STM32 (v3.7 MST)

Este documento detalla la construcción, operación y especificaciones del inductómetro de alta precisión basado en el microcontrolador STM32F411CE "Black Pill".

## 1. Especificaciones Técnicas
| Característica | Valor |
| :--- | :--- |
| **Microcontrolador** | STM32F411CE (100 MHz) |
| **Resolución Temporal** | 10 nanosegundos (vía DWT Core) |
| **Rango de Medida L** | 1 $\mu$H a ~800 mH |
| **Frecuencia de Test** | 200 Hz a 2 MHz |
| **Capacitancia de Tanque** | 0.968 nF (Polipropileno recomendado) |
| **Alimentación** | 3.3V (desde STM32) |
| **Precisión** | < 5% (en rango 10uH - 100mH) |

---

## 2. Diagrama de Conexión (Hardware)

El sistema utiliza un driver de corriente NPN para compensar bobinas de baja resistencia interna.

### Componentes Necesarios:
- 1x STM32F411CE Black Pill.
- 1x Pantalla LCD 16x2 con adaptador I2C.
- 1x Transistor NPN **BC549B** (o similar como BC547).
- 1x Resistencia de **1k$\Omega$** (Base del transistor).
- 1x Condensador de **0.968 nF** (Referencia C).

### Pinout:
1. **Transistor:**
   - **Emisor:** A GND.
   - **Base:** Conectada a **PA1** mediante la resistencia de 1k$\Omega$.
   - **Colector:** Conectado a **PA0** y al circuito tanque.
2. **Circuito Tanque (LC):**
   - La bobina (L) y el condensador (C) se conectan en **paralelo**.
   - Un extremo del paralelo va al **Colector** del transistor.
   - El otro extremo del paralelo va a **3.3V**.
3. **LCD I2C:**
   - **SCL:** PB6.
   - **SDA:** PB7.
   - **VCC/GND:** 5V o 3.3V según modelo.

---

## 3. Instrucciones de Uso

### Inicio:
Al encender el dispositivo, la pantalla mostrará **"L-Meter v3.7 MST"**. Asegúrese de que no haya ninguna bobina conectada para verificar que el sistema marca "SIN OSCILACION".

### Medición:
1. Conecte la bobina en los terminales de prueba.
2. La pantalla mostrará:
   - **L:** El valor de inductancia (en uH o mH).
   - **kHz:** La frecuencia de resonancia actual.
   - **Eco:** El número de ciclos que la bobina es capaz de oscilar.

### Diagnóstico de Calidad (Eco):
- **Eco > 30:** Bobina de excelente calidad (baja resistencia, alto Q).
- **Eco 10-25:** Bobina estándar.
- **Eco < 5 / "LOW":** Bobina con alta resistencia interna o pérdidas.
- **"R MUY BAJA":** La bobina tiene una resistencia tan baja (<0.2$\Omega$) que el circuito no puede establecer la oscilación.

---

## 4. Calibración
Si desea ajustar la precisión con una bobina de referencia conocida:
1. Localice en el código: `const double CALIBRATION = 1.0;`.
2. Si el aparato mide de más (ej. marca 105 para 100), cambie a `0.95`.
3. Si mide de menos, aumente el valor.

---

## 5. Mantenimiento y Consejos
- Mantenga los cables de prueba lo más cortos posible para evitar capacitancias parásitas que afecten a medidas de uH.
- Use condensadores de tipo **Styroflex** o **Polipropileno** para el condensador de referencia (0.968nF) para obtener la mejor estabilidad térmica.
