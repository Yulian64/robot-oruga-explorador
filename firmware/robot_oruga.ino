#include <WiFi.h>
#include <WebServer.h>

// Configura tus credenciales Wi-Fi
const char *ssid = "Robot_Camara";
const char *password = "12345678";

// Pines del L298N
const int ENA = 14;
const int IN1 = 12;
const int IN2 = 13;
const int ENB = 33;
const int IN3 = 15;
const int IN4 = 32;

// Otros pines
const int buzPin = 4;
const int ledPin = 2;

// Velocidad inicial de los motores (0-255)
int valSpeed = 200;

// Servidor Web
WebServer server(80);

// --- DECLARACIÓN DE FUNCIONES ---
void Stop();
void forward();
void backward();
void left();
void right();
void horn();
void handleRoot();
void setup()
{
    Serial.begin(115200);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(buzPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    // Enciende el LED al arrancar (indica que el robot está encendido)
    digitalWrite(ledPin, HIGH);
    ledcAttach(ENA, 5000, 8);
    ledcAttach(ENB, 5000, 8);
    Stop();

    // Configurar Wi-Fi como punto de acceso
    Serial.println("Creando punto de acceso WiFi...");
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP del robot: ");
    Serial.println(IP);

    // Rutas del servidor
    server.on("/", handleRoot);
    server.on("/F", []()
              { forward(); server.send(200, "text/plain", "Forward"); });
    server.on("/B", []()
              { backward(); server.send(200, "text/plain", "Backward"); });
    server.on("/L", []()
              { left(); server.send(200, "text/plain", "Left"); });
    server.on("/R", []()
              { right(); server.send(200, "text/plain", "Right"); });
    server.on("/S", []()
              { Stop(); server.send(200, "text/plain", "Stop"); });
    server.on("/Y", []()
              { horn(); server.send(200, "text/plain", "Horn"); });
    server.on("/U", []()
              { digitalWrite(ledPin, HIGH); server.send(200, "text/plain", "LED ON"); });
    server.on("/u", []()
              { digitalWrite(ledPin, LOW); server.send(200, "text/plain", "LED OFF"); });

    // Nueva ruta para actualizar la velocidad
    server.on("/speed", []()
              {
if (server.hasArg("value")) {
valSpeed = constrain(server.arg("value").toInt(), 0, 255);
Serial.printf("Nueva velocidad: %d\n", valSpeed);
server.send(200, "text/plain", "Velocidad actualizada");
} else {
server.send(400, "text/plain", "Falta parámetro 'value'");
} });
    server.begin();
    Serial.println("Servidor Web del Robot iniciado.");
}
void loop()
{
    server.handleClient();
}

// Página web principal
void handleRoot()
{
    String esp32_cam_ip = "192.168.4.2"; // Ajusta según ESP32-CAM
    String html = R"-(
<!DOCTYPE html>
<html lang="es">
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Control del Robot WiFi</title>
<style>
body {
text-align: center;
font-family: 'Segoe UI', sans-serif;
background: radial-gradient(circle at center, #001820 0%, #000 100%);
color: #00f6ff;
margin: 0;
padding: 20px;
}
h2, h3 {
color: #00f6ff;
text-shadow: 0 0 10px #00f6ff, 0 0 20px #00f6ff;
margin-bottom: 10px;
}
#videoStream {
width: 90%;
max-width: 640px;
border: 2px solid #00f6ff;
border-radius: 15px;
box-shadow: 0 0 15px #00f6ff;
margin: 50px 0 50px 0;
transform: rotate(-90deg); /* Rota 90° a la izquierda (corrige cámara girada a la derecha) */
transform-origin: center center;
}
/* 🔹 Animación de brillo neón */
@keyframes pulse {
0% { box-shadow: 0 0 10px #00f6ff; }
50% { box-shadow: 0 0 25px #00f6ff; }
100% { box-shadow: 0 0 10px #00f6ff; }
}
/* 🔸 Base para botones tipo neón */
.neon-btn {
display: inline-flex;
align-items: center;
justify-content: center;
width: 80px;
height: 80px;
margin: 20px;
border: 2px solid #00f6ff;
border-radius: 15px;
background: transparent;
color: #00f6ff;
font-weight: bold;
font-size: 16px;
text-shadow: 0 0 8px #00f6ff;
box-shadow: 0 0 15px #00f6ff, inset 0 0 10px rgba(0,255,255,0.3);
cursor: pointer;
transition: 0.2s;
animation: pulse 3s infinite;
}
.neon-btn:hover {
transform: scale(1.08);
background: rgba(0,255,255,0.1);
}
.neon-btn:active {
transform: scale(0.95);
box-shadow: 0 0 40px #00f6ff, inset 0 0 25px #00f6ff;
}
/* 🔺 Flechas con borde sólido y fondo transparente */
.arrow {
position: relative;
width: 80px;
height: 80px;
margin: 10px;
display: inline-flex;
align-items: center;
justify-content: center;
background: transparent;
border: 2px solid #00f6ff;
border-radius: 15px;
box-shadow: 0 0 15px #00f6ff, inset 0 0 10px rgba(0,255,255,0.3);
animation: pulse 3s infinite;
cursor: pointer;
transition: transform 0.2s;
}
.arrow::before {
content: '';
position: absolute;
width: 0;
height: 0;
border-style: solid;
}
.arrow.up::before {
border-width: 0 20px 30px 20px;
border-color: transparent transparent #00f6ff transparent;
}
.arrow.down::before {
border-width: 30px 20px 0 20px;
border-color: #00f6ff transparent transparent transparent;
}
.arrow.left::before {
border-width: 20px 30px 20px 0;
border-color: transparent #00f6ff transparent transparent;
}
.arrow.right::before {
border-width: 20px 0 20px 30px;
border-color: transparent transparent transparent #00f6ff;
}
.arrow:hover {
transform: scale(1.1);
background: rgba(0,255,255,0.1);
}
.arrow:active {
transform: scale(0.9);
box-shadow: 0 0 40px #00f6ff, inset 0 0 25px #00f6ff;
}
.controls {
display: flex;
flex-direction: column;
align-items: center;
justify-content: center;
margin: 30px;
}
.middle-row {
display: flex;
justify-content: center;
align-items: center;
}
.slider {
width: 80%;
margin: 15px auto;
accent-color: #00f6ff;
}
p {
color: #00f6ff;
text-shadow: 0 0 8px #00f6ff;
}
</style>
</head>
<body>
<h2>Robot Oruga</h2>
<h3>Live Camera</h3>
<img id="videoStream" src="http://)-" +
                  esp32_cam_ip + R"-(/stream" alt="ESP32-CAM
Stream">
<h3>Velocidad</h3>
<input type="range" id="speedSlider" class="slider" min="0" max="255" value="200"
oninput="updateSpeed(this.value)">
<p>Velocidad actual: <span id="speedValue">200</span></p>
<h3>Controles</h3>
<div class="controls">
<div class="arrow up" ontouchstart="send('F')" onmousedown="send('F')"></div>
<div class="middle-row">
<div class="arrow left" ontouchstart="send('L')" onmousedown="send('L')"></div>
<button class="neon-btn" ontouchstart="send('S')"
onmousedown="send('S')">STOP</button>
<div class="arrow right" ontouchstart="send('R')" onmousedown="send('R')"></div>
</div>
<div class="arrow down" ontouchstart="send('B')" onmousedown="send('B')"></div>
</div>
<div>
<button class="neon-btn" ontouchstart="send('Y')"
onmousedown="send('Y')">HORN</button>
<button class="neon-btn" ontouchstart="send('U')" onmousedown="send('U')">LED
ON</button>
<button class="neon-btn" ontouchstart="send('u')" onmousedown="send('u')">LED
OFF</button>
</div>
<div>
<h3>Designed By Yulian Tovar - Juan Aviles</h3>
<h3>Tecnoparque 2025 -_-</h3>
</div>
<script>
function send(cmd) {
fetch('/' + cmd)
.then(() => console.log('Comando enviado: ' + cmd))
.catch(err => console.error(err));
}
function updateSpeed(val) {
document.getElementById('speedValue').innerText = val;
fetch('/speed?value=' + val)
.then(() => console.log('Velocidad actualizada: ' + val))
.catch(err => console.error(err));
}
</script>
</body>
</html>
)-";
    server.send(200, "text/html", html);
}
// Funciones de movimiento
void forward()
{
    ledcWrite(ENA, valSpeed);
    ledcWrite(ENB, valSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
void backward()
{
    ledcWrite(ENA, valSpeed);
    ledcWrite(ENB, valSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void left()
{
    ledcWrite(ENA, valSpeed);
    ledcWrite(ENB, valSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
void right()
{
    ledcWrite(ENA, valSpeed);
    ledcWrite(ENB, valSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void Stop()
{
    ledcWrite(ENA, 0);
    ledcWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
void horn()
{
    digitalWrite(buzPin, HIGH);
    delay(200);
    digitalWrite(buzPin, LOW);
    delay(80);
    digitalWrite(buzPin, HIGH);
    delay(300);
    digitalWrite(buzPin, LOW);
}
