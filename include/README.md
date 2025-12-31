# VEML7700 + STM32 Blue Pill (PlatformIO)

Dieses Repository zeigt ein **funktionierendes Beispielprojekt** zum Auslesen eines  
**VEML7700 Umgebungslichtsensors** mit einem  
**STM32F103C8 („Blue Pill“)**
unter **PlatformIO / Arduino**.

Das Projekt dient als **saubere Referenz** für:
- UART-Debugging mit FTDI
- I²C-Kommunikation
- STM32 Blue Pill + PlatformIO
- strukturierte Projekt- und GitHub-Dokumentation

---

## Hardware

- STM32 Blue Pill (STM32F103C8)
- VEML7700 Umgebungslichtsensor (I²C)
- USB-UART Adapter (FTDI / CP2102 / CH340, **3.3 V Pegel**)
- Verbindungskabel (Dupont)

---

## Aufbau / Verdrahtung

<p align="center">
  <img src="images/setup.jpg" width="600">
</p>

<p align="center">
  <em>STM32 Blue Pill mit VEML7700 (I²C) und FTDI-Adapter für UART-Debug</em>
</p>

---

## Pinbelegung

### I²C – VEML7700
| Signal | STM32 Blue Pill |
|------|------------------|
| SCL  | PB6              |
| SDA  | PB7              |
| VCC  | 3.3 V            |
| GND  | GND              |

> Hinweis: Pull-Up-Widerstände (typ. 4.7 kΩ nach 3.3 V) müssen vorhanden sein  
> (oft bereits auf dem VEML7700-Breakout).

---

### UART – Debug (FTDI)

| FTDI | STM32 Blue Pill |
|-----|------------------|
| TXD | PA10 (RX)        |
| RXD | PA9  (TX)        |
| GND | GND              |

> ⚠️ FTDI **nicht** zur Spannungsversorgung verwenden.  
> Die Blue Pill wird separat versorgt (z. B. über ST-LINK oder 3.3 V-Regler).

---

## Funktion

- I²C-Scan (Erkennung des VEML7700 auf Adresse `0x10`)
- Initialisierung des Sensors
- Auslesen von:
  - ALS-Rohwert (Umgebungslicht)
  - WHITE-Rohwert (breiteres Spektrum)
- Näherungsweise Lux-Berechnung
- Ausgabe der Messwerte über UART (FTDI → Serial Monitor)

---

## Build, Flash & Monitor

### Build
```bash
pio run
