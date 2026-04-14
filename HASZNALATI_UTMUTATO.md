# Használati Útmutató az ESP32-S3 ZX Spectrum Emulátorhoz

Ez a dokumentum részletesen leírja, hogyan kell használni a frissen lefordított emulátort, hova kell tenni a játékokat, mik a támogatott fájlok és hogyan működik a joystick/gombok párosítása a játékokkal.

A kód forrásfájljai itt találhatók (ha bele akarsz nézni):
- A fő belépési pont: `src/main.cpp`
- A képernyők (menük): `src/Screens/`
- A joystick és billentyűzet modulok: `src/Input/` (itt található az én egyedi `AnalogJoystick.cpp` fájlom is)
- A konfiguráció: `platformio.ini` (itt található a `esp32-s3-custom-joystick` profil, amit használnod kell)

## 1. SD Kártya és Játékok előkészítése

Az emulátor egy egyszerű SPI SD kártyaolvasót használ (ami az ILI9341 kijelződ hátán van).
1. **Formázás:** Az SD kártyát (maximum 32 GB ajánlott) számítógépen formázd meg **FAT32** fájlrendszerre.
2. **Mappák:** A kártya gyökerébe tetszés szerinti mappákat hozhatsz létre, pl. `games`, `roms`, de a gyökérbe is teheted a fájlokat. Érdemes ABC sorrendben, mappákba rendezni őket, mert a menüben könnyebb lesz a navigáció.
3. **Támogatott Játék Fájlformátumok:**
   - Szalagos (Tape) formátumok: `.tap`, `.tzx` és `.cas` fájlok. Ezek a betöltéskor automatikusan elindulnak.
   - Snapshot (Mentés) formátumok: `.sna` és `.z80` fájlok. Ezek a memóriamentések egyből elindulnak (töltés nélkül), ezért a legnépszerűbb formátumok ezen a rendszeren.

Másold rá a kedvenc játékaidat, tedd be az olvasóba, és adj áramot (USB-n) az ESP32-S3-nak.

## 2. Az Emulátor Irányítása és Használata

Amikor az emulátor bekapcsol, az SD kártya beolvasása után a főmenü ("File Picker") jelenik meg, ahol kilistázza a kártyádon lévő mappákat és fájlokat.

### A Menüben
- **Joystick Fel/Le:** Görgetés a listában
- **Joystick Balra/Jobbra:** Gyors görgetés (oldalanként)
- **A Gomb (GPIO 41):** Belépés a mappába, vagy játék kiválasztása és indítása.
- **Menü Gomb (GPIO 40 - Joystick közepe nyomás):** Vissza / Kilépés.

### Játék Közben (Alapértelmezett Kempston Irányítás)
A ZX Spectrum játékok döntő többsége támogatja a *Kempston* joystickot. A rendszer úgy tesz, mintha te egy igazi Kempston joystickkal játszanál:
- **Joystick Mozgatás:** Megegyezik a Kempston Fel/Le/Balra/Jobbra irányokkal.
- **A Gomb (GPIO 41):** Tűz / Akció gomb (Kempston Fire). Ezen felül a menükben és "Press Enter to start" képernyőkön ez az **ENTER** gombként is funkcionál!
- **B Gomb (GPIO 42):** SPACE (szóköz) gomb. Hasznos ugráshoz, vagy ha a játék a SPACE-t kéri valamihez.

### Játékból való kilépés
- Ha játék közben RÖVIDEN megnyomod a **Menü Gombot (GPIO 40)** (például a joystick lenyomásával), a játék megáll, és feljön az emulátor belső (fekete) menüje.
- Itt elmentheted az állást, betölthetsz mentést, reszetelheted a gépet, vagy a "Quit"-et választva visszatérhetsz a játékválasztó listához.

## 3. A Virtuális Billentyűzet (On-Screen Keyboard - OSK)

A régi ZX Spectrum játékoknál gyakori probléma, hogy a kezdőképernyőn meg kell nyomni egy adott számot a menüben (pl. "Press 1 for Keyboard, 2 for Kempston"). Mivel nincs fizikai billentyűzeted, ezért elkészítettem egy virtuálisat.

**Előhívása:**
1. Játék közben **nyomd meg és tartsd lenyomva a B Gombot (GPIO 42 / Space gomb)** legalább **0.8 másodpercig**, majd engedd el.
2. A játék képernyője helyén megjelenik egy **4x10-es betű/szám rács** fekete alapon, piros kurzorral.

**Használata:**
1. A **Joystick** segítségével tudod mozgatni a kurzort (piros hátterű kocka) a betűk és számok között.
2. Amint ráálltál a kívánt gombra (pl. a "2"-esre a Kempstonhoz), nyomd meg az **A Gombot (GPIO 41)**.
3. Ez szimulálja a konkrét Spectrum gomb lenyomását. A virtuális billentyűzet ekkor azonnal **eltűnik**, te pedig visszajutsz a játékba, ahol a gombnyomás hatására elindult a megfelelő játékmód!
4. Ha véletlenül hoztad be a billentyűzetet, a **Menü Gomb** rövid megnyomásával be is tudod zárni anélkül, hogy lenyomnál bármilyen gombot rajta.

Ezzel a trükkel az összes játék elindítható, irányítható és élvezhető billentyűzet csatlakoztatása nélkül is!
