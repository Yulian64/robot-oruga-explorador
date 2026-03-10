# 🤖 Robot Oruga Explorador

Proyecto de robótica desarrollado para la exploración remota mediante control inalámbrico.
El sistema utiliza un **ESP32** como microcontrolador principal y permite controlar el movimiento del robot desde una **interfaz web a través de WiFi**.

El robot integra además una **cámara en tiempo real**, iluminación LED y un sistema de control de motores mediante un driver **L298N**, lo que lo convierte en una plataforma versátil para experimentación en robótica e IoT.

---

## 🚀 Características principales

- Control remoto mediante **interfaz web**
- Conexión inalámbrica **WiFi**
- Transmisión de **video en tiempo real**
- Sistema de iluminación LED
- Control de motores con **driver L298N**
- Plataforma móvil tipo **oruga**

---

## 🛠 Tecnologías utilizadas

### Hardware

- ESP32 / ESP32 Wrover Dev
- Driver de motores L298N
- Motores DC
- LEDs
- Cámara para ESP32
- Fuente de alimentación

### Software

- Arduino IDE
- HTML
- CSS
- JavaScript
- Comunicación WiFi

---

## ⚙️ Funcionamiento del sistema

1. El ESP32 crea o se conecta a una red WiFi.
2. Se accede a una **interfaz web** desde un navegador.
3. Desde la interfaz se envían comandos al ESP32.
4. El microcontrolador controla el **driver L298N** para mover los motores.
5. La cámara transmite **video en tiempo real** al usuario.

---

## 📂 Estructura del proyecto

```
robot-oruga-explorador
│
├── firmware
│   └── robot_oruga.ino
│
├── images
│   ├── robot1.jpg
│   ├── robot2.jpg
│   └── robot3.jpg
│
└── README.md
```

---

## 🎯 Objetivo del proyecto

Desarrollar una plataforma robótica capaz de ser controlada de forma remota mediante tecnologías web, integrando **hardware, programación y comunicación inalámbrica**.

---

## 👨‍💻 Autor

**Evis Yulian Tovar Peluffo**
Tecnólogo en Análisis y Desarrollo de Software

Proyecto desarrollado durante prácticas en **Tecnoparque SENA**.
