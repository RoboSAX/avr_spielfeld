# avr_spielfeld
Hauptprogramm zur Spielfeld-Steuerung und für die Teamproben.

Die Software ist jahresweise abgelegt und unterscheidet sich jeweils für das Spielfeld & die Teamprobe:

    RoboSAX
        ├── 2024
        │   ├── Spielfeld
        │   │   ├── RoboSAX_2024__Spielfeld__2025_02_03.hex
        │   │   └── src/
        │   └── Teamprobe
        │       ├── RoboSAX_2024__Teamprobe__2025_02_03.hex
        │       └── src/
        ├── 2023/
        ├── ...
        ├── 2018/
        └── modules/

Alle Spielfeldstände und die Teamproben teilen sich gemeinsame [Module](RoboSAX/modules/).



## Setup
- Repo auschecken

      git clone https://github.com/RoboSAX/avr_spielfeld spielfeld
      cd spielfeld

- RoboLib auschecken, Abhängigkeiten installieren und sourcen

      ./setup_robolib.sh

- Terminal erneut öffnen

      # <strg>+<d>
      # <strg>+<alt>+<t>

- gesamtes Repo bauen

      cd spielfeld/
      make

## Repo bauen

    # Pfad entsprechend des Setups anpassen
    cd spielfeld/
    make

## aktuellen Stand herunterladen

    # Pfad entsprechend anpassen
    #   1. Teil siehe Setup
    #   2. Teil ist immer "RoboSAX"
    #   3. Teil entspricht gewünschtem Jahr
    #   4. Teil unterscheided zw. Spielfeld & Teamprobe
    #   5. Teil ist immer "src"
    cd spielfeld/RoboSAX/2024/Teamprobe/src/

    robolib_avrdude

## gesicherten Stand herunterladen

    # Pfad entsprechend anpassen
    #   1. Teil siehe Setup
    #   2. Teil ist immer "RoboSAX"
    #   3. Teil entspricht gewünschtem Jahr
    #   4. Teil unterscheided zw. Spielfeld & Teamprobe
    cd spielfeld/RoboSAX/2024/Teamprobe/

    # gewünschten Stand auswählen und nach out.hex kopieren
    cp RoboSAX_2024__Teamprobe__2025_02_03.hex out.hex

    robolib_avrdude

