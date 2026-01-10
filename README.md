# Inductómetro de Alta Precisión STM32 (v3.7 MST)

Este proyecto implementa un inductómetro digital profesional utilizando un **STM32F411CE (Black Pill)**, capaz de medir inductancias desde 1µH hasta ~1H con una precisión excepcional gracias al uso de hardware dedicado y captura por ciclos de CPU.

## 🚀 Características principales
- **Medición por Resonancia de Impulso:** Captura la frecuencia natural de un tanque LC excitado por un pulso de corriente.
- **Resolución de 10ns:** Utiliza el contador de ciclos del núcleo (DWT) para medir tiempos con precisión nanométrica.
- **Driver de Corriente NPN:** Incluye un diseño con transistor BC549 para permitir la medida de bobinas con muy baja resistencia interna (DCR).
- **Sistema de Diagnóstico "Eco":** Informa sobre el factor de calidad (Q) y la resistencia interna de la bobina basándose en el decaimiento de la oscilación.
- **Detección Automática:** Avisa si la bobina no es medible o si hay un circuito abierto.
- **Interfaz Visual:** Pantalla LCD 16x2 con adaptador I2C para lectura en tiempo real.

## 🛠️ Hardware: Componentes base
Para obtener resultados precisos, este software requiere:
- **Microcontrolador:** STM32F411CE (Black Pill).
- **Pantalla:** LCD 16x2 con módulo I2C (PCF8574).
- **Transistor Driver:** BC549B (o similar).
- **Pin de Pulso:** `PA1` (con resistencia de 1k a la base).
- **Pin de Medida:** `PA0` (conectado al colector y al tanque LC).
- **Tanque LC:** Bobina bajo prueba en paralelo con un condensador de **0.968 nF**.

## 📖 Documentación
Para instrucciones detalladas de montaje, pinout completo y guía de manejo, consulte el archivo:
👉 [**Manual de Usuario y Montaje**](Manual_Inductometro_STM32.md)

## ✍️ Créditos y Autoría
Proyecto desarrollado por **Daniel Rodriguez Gonzalez**.

## ⚖️ Licencia
Este proyecto es software libre bajo la **Licencia MIT**. Siéntete libre de usarlo, modificarlo y compartirlo, manteniendo siempre los créditos del autor original.
