# Horloge LCD

Projet **open-source complet** d'horloge LCD basé sur un microcontrôleur **STM32U083RC**. 

Ce dépôt regroupe la totalité du projet :
* 📐 **Conception 3D** : Boîtier modulaire pour impression 3D.
* 🔌 **Électronique (V1 & V2)** : Fichiers KiCad, Gerbers, BOM, schémas PDF et modèles 3D.
* 💻 **Firmware** : Code source C/C++ optimisé pour STM32 (projet STM32CubeIDE / CubeMX).
* 📖 **Documentation** : Manuel utilisateur et fiches techniques (*datasheets*).

---

## 🖼️ Aperçu du projet

| Boîtier complet |
| :---: |
| ![Horloge LCD](pictures/20260919_160751.jpg) |
| ![Horloge LCD](pictures/20260919_161016.jpg) |

---

## 📂 Structure du projet

```text
Moisture-sensor/
├── 3D_design/                            # Fichiers CAO pour impression 3D
│   ├── moisture_sensor_capot.3mf         # Capot principal (3MF)
│   ├── moisture_sensor_capot_ecran.stl   # Capot pour intégration d'un écran (STL)
│   └── moisture_sensor_capot_pile.stl    # Trappe / compartiment pile (STL)
│
├── Electronics/                          # Fichiers de conception électronique
│   ├── datasheet/                        # Documentation technique des composants
│   ├── Moisture_sensorV1/                # Première révision
│   └── Moisture_sensorV2/                # Révision V2 (Principale)
│       ├── 3D_model/                     # Modèle 3D STEP du PCB complet
│       ├── BOM/                          # Liste des composants (Bill of Materials)
│       ├── gerber_file/                  # Fichiers Gerber pour la fabrication du PCB
│       ├── KICAD/                        # Projet CAO KiCad (Schéma + PCB)
│       └── pdf_version/                  # Schémas électroniques au format PDF
│
├── Program/                              # Firmware & Code source
│   └── STM32U073CB/                      # Projet STM32CubeIDE / CubeMX
│       ├── Core/                         # Code source applicatif (Inc, Src, Main)
│       ├── Drivers/                      # Pilotes HAL et CMSIS STM32
│       ├── Capacitive_Moisture_sensor.ioc# Configuration STM32CubeMX
│       ├── Capacitive_Moisture_sensor Debug.launch # Config de débogage
│       └── STM32U073CBTX_FLASH.ld        # Linker script Flash
│
├── pictures/                             # Visuels du projet
│   ├── moisture_sensor.jpg
│   └── populated_PCB.jpg
│
├── Moisture_sensor_User_Manual.pdf       # Manuel utilisateur complet (PDF)
├── Moisture_sensor_User_Manual.odt       # Manuel utilisateur éditable (ODT)
└── README.md                             # Documentation principale du dépôt
```

---

## ⚡ Caractéristiques techniques

* **Microcontrôleur** : STM32U073CB (Ultra-low-power ARM Cortex-M0+).
* **Technologie de mesure** : Capacitive (résistante à la corrosion par rapport aux capteurs résistifs).
* **Conception PCB** : Développée sous **KiCad** (Version V2 disponible avec gerbers prêts à la fabrication).
* **Alimentation** : Conçu pour fonctionner sur pile avec trappe d'accès rapide.
* **Affichage (optionnel)** : Prise en charge d'un écran grâce au capot adapté.

---

## 🖨️ Impression 3D du boîtier

Les pièces sont situées dans le dossier `3D_design/`.

| Pièce | Format | Description |
| :--- | :--- | :--- |
| `moisture_sensor_capot.3mf` | `.3mf` | Capot plein standard |
| `moisture_sensor_capot_ecran.stl` | `.stl` | Capot découpé pour intégrer un écran |
| `moisture_sensor_capot_pile.stl` | `.stl` | Logement pour le compartiment d'alimentation |

---

## 💻 Firmware & Développement

Le code source réside dans `Program/STM32U073CB/`.

### Prérequis
* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (version récente)
* Sonde de programmation (ST-Link V2 / V3)

### Compilation et Flash
1. Ouvrez **STM32CubeIDE**.
2. Importez le projet à partir de `Program/STM32U073CB`.
3. Si vous souhaitez modifier la configuration des broches ou des périphériques, ouvrez le fichier `Capacitive_Moisture_sensor.ioc` avec STM32CubeMX.
4. Compilez (`Build Project`) puis flashez sur le microcontrôleur via la configuration de débogage incluse (`Capacitive_Moisture_sensor Debug.launch`).

---

## 🏭 Fabrication du PCB (Hardware V2)

Pour faire fabriquer la carte électronique V2 :
1. Les fichiers prêts pour l'usine (JLCPCB, PCBWay, etc.) se trouvent dans `Electronics/Moisture_sensorV2/gerber_file/`.
2. La liste des composants nécessaires pour le brasage est disponible dans `Electronics/Moisture_sensorV2/BOM/`.
3. Les schémas de principe sont lisibles via KiCad ou directement dans `Electronics/Moisture_sensorV2/pdf_version/`.

---

## 📖 Manuel Utilisateur

Pour en savoir plus sur l'étalonnage, l'installation et l'utilisation quotidienne du capteur, référez-vous au document :
📄 [Moisture_sensor_User_Manual.pdf](Moisture_sensor_User_Manual.pdf)

---

## 📜 Licence

Ce projet est distribué sous licence open-source. Vous êtes libre de l'utiliser, le modifier et le distribuer.
