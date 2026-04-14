# ESP32-S3 ZX Spectrum Emulator Wiring Guide (Atomic14 Port)

Ez a leírás tartalmazza a pontos hardver bekötéseket a te ESP32-S3 + ILI9341 + Analóg Joystick + PAM8403 hardver konfigurációdhoz. A projekt az atomic14/esp32-zxspectrum kódbázisára épül, így egy teljes értékű emulátort kapsz menürendszerrel és képernyőre rajzolt virtuális billentyűzettel (On-Screen Keyboard).

## Szükséges Alkatrészek
1.  **ESP32-S3 Modul**
2.  **ILI9341 TFT Kijelző (SPI)** + **SD Kártyaolvasó** (a hátulján)
3.  **Analóg Joystick Modul** (KY-023)
4.  **2x Mikrokapcsoló (Tact gomb)** + A Joystick beépített gombja
5.  **PAM8403 3W Erősítő Modul** + Hangszóró
6.  **RC Szűrő Alkatrészek:** 1x 1kOhm Ellenállás, 1x 0.1uF Kondenzátor (opcionális, de ajánlott a tisztább hanghoz)
7.  **Powerbank** (az ESP32-S3 USB portjára csatlakoztatva)

---

## Lábkiosztás (Pinout)

### 1. ILI9341 Kijelző és SD Kártya (Közös SPI busz)

A kijelző és az SD kártya ugyanazokat az SPI adat- és órajel vonalakat használja, de a Chip Select (CS) lábakkal választja ki az ESP32, hogy éppen melyikkel kommunikál.

| ESP32-S3 Pin | Kijelző (ILI9341) | SD Kártyaolvasó | Leírás                  |
| :---         | :---              | :---            | :---                    |
| **GPIO 11**  | SDI (MOSI)        | MOSI            | SPI Adat Ki (Master Out)|
| **GPIO 13**  | SDO (MISO)        | MISO            | SPI Adat Be (Master In) |
| **GPIO 12**  | SCK               | SCK             | SPI Órajel (Clock)      |
| **GPIO 10**  | CS                | -               | Kijelző Kiválasztás (CS)|
| **GPIO 5**   | -                 | CS              | SD Kártya Kiválasztás   |
| **GPIO 9**   | DC                | -               | Kijelző Adat/Parancs    |
| **GPIO 8**   | RESET             | -               | Kijelző Reset           |
| **GPIO 4**   | LED / BL          | -               | Háttérvilágítás (3.3V)  |
| **3.3V**     | VCC               | VCC             | Tápfeszültség           |
| **GND**      | GND               | GND             | Földelés                |

*(Megjegyzés: A kijelzőn lévő T_DO, T_DIN, T_CS, T_CLK, T_IRQ lábak az érintőpanelhez tartoznak, ezeket hagyd szabadon, nem használjuk).*

### 2. Analóg Joystick

| ESP32-S3 Pin | Joystick Pin | Leírás                                |
| :---         | :---         | :---                                  |
| **GPIO 1**   | VRX (X)      | Analóg X tengely (Balra/Jobbra mozgás)|
| **GPIO 2**   | VRY (Y)      | Analóg Y tengely (Fel/Le mozgás)      |
| **GPIO 40**  | SW (Gomb)    | **Menü gombként** fogjuk használni    |
| **3.3V**     | +5V / VCC    | **Fontos:** 3.3V-ot adj neki, ne 5V-ot!|
| **GND**      | GND          | Földelés                              |

### 3. Extra Gombok (Akció gombok)

Ezeket a mikrokapcsolókat a GND (Föld) és az adott GPIO láb közé kell kötni. Nincs szükség külső ellenállásra, a kód belső pull-up ellenállást használ.

| ESP32-S3 Pin | Funkció               | Bekötés                             |
| :---         | :---                  | :---                                |
| **GPIO 41**  | "A" Gomb (Lövés/Enter)| Egyik láb a GPIO41-re, másik a GND-re|
| **GPIO 42**  | "B" Gomb (Space)      | Egyik láb a GPIO42-re, másik a GND-re|

### 4. Hang (PAM8403)

Mivel az ESP32-S3-nak nincs fizikai analóg hangkimenete (DAC), PWM jelet generálunk. Ezt érdemes egy egyszerű "RC aluláteresztő szűrővel" analóg jellé alakítani, mielőtt az erősítőbe megy.

**A szűrő bekötése:**
1. A **GPIO 6** lábat kösd rá egy **1kOhm ellenállás** egyik végére.
2. Az ellenállás MÁSIK végét kösd a PAM8403 erősítő **L (vagy R) bemenetére**. Ez lesz a hangjel.
3. Fogj egy **0.1uF kondenzátort**, és kösd az erősítő L (vagy R) bemenete és a GND (Föld) közé.

**A PAM8403 bekötése:**
| Bekötés                      | PAM8403 Pin |
| :---                         | :---        |
| **RC Szűrőből jövő hangjel** | L (vagy R)  |
| **GND (ESP32 Föld)**         | GND (Audio) |
| **5V (ESP32 VBUS/5V lába)**  | 5V / VCC    |
| **GND (ESP32 Föld)**         | GND (Táp)   |
| **Hangszóró kábelei**        | L+ / L-     |

### 5. Tápellátás
A rendszert a legegyszerűbben úgy tudod táplálni, ha az ESP32-S3 USB-C (vagy Micro-USB) portjára rádugsz egy Powerbankot. Az ESP32 lapkán lévő **5V (vagy VBUS/VIN)** lábról tudod megtáplálni a hangosítót (PAM8403), a **3.3V** lábról pedig az összes többi modult (Kijelző, SD, Joystick).
