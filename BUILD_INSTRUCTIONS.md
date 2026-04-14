# Telepítési és Fordítási Útmutató (ESP32-S3 ZX Spectrum)

Ez az útmutató bemutatja, hogyan tudod az elkészült egyedi firmware-t lefordítani és feltölteni az ESP32-S3 lapkádra.

## 1. Előkészületek (Szoftver telepítése)
Mivel ez a projekt **PlatformIO** alapú (nem a sima Arduino IDE), a következőt kell tenned a számítógépeden:
1. Töltsd le és telepítsd a **Visual Studio Code (VS Code)** nevű ingyenes kódszerkesztőt: https://code.visualstudio.com/
2. Nyisd meg a VS Code-ot, menj bal oldalt a bővítmények (Extensions - a kis kocka ikon) fülre.
3. Keress rá a **PlatformIO IDE** nevű bővítményre és telepítsd. A telepítés után indítsd újra a VS Code-ot (várj, amíg a jobb alsó sarokban befejezi a belső telepítéseket).

## 2. A Projekt Megnyitása
1. Töltsd le a teljes kódbázist, amiben ezt a fájlt találod (git clone, vagy zip letöltése, és kicsomagolása egy mappába).
2. A VS Code-ban kattints a `File` -> `Open Folder...` menüpontra, és válaszd ki azt a főkönyvtárat, ami tartalmazza a `platformio.ini` fájlt.

## 3. Fordítás és Feltöltés (Upload)
A projektben létrehoztam egy egyedi profilt a te hardveredhez, aminek a neve `esp32-s3-custom-joystick`.

1. Csatlakoztasd az ESP32-S3 modult a számítógépedhez USB kábellel.
2. A VS Code legalján lévő kék PlatformIO sávban kattints az **"Env: ..."** feliratra (ha van ilyen), és válaszd ki az **`env:esp32-s3-custom-joystick`** profilt a listából.
3. Kattints a jobbra mutató nyíl ikonra (➡) az alsó sávban, vagy a bal oldali PlatformIO menüben az `Upload` gombra.
4. A PlatformIO most letölti a szükséges könyvtárakat, lefordítja a ZX Spectrum magot, és automatikusan feltölti a firmware-t az ESP32-S3-ra. Ez eltarthat 1-2 percig.

Kész! Ezután csatlakoztasd a hardvereket a WIRING.md alapján, és élvezd a játékot a HASZNALATI_UTMUTATO.md alapján.
