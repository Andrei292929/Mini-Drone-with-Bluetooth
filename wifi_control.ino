#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "DroneESP32";
const char* password = "12345678";

void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<body>
  <h2>Control drona ESP32</h2>

  <button onclick="sendCmd('W')">W</button><br><br>

  <button onclick="sendCmd('A')">A</button>
  <button onclick="sendCmd('X')">X</button>
  <button onclick="sendCmd('D')">D</button><br><br>

  <button onclick="sendCmd('S')">S</button><br><br>

  <button onclick="sendCmd('Q')">Throttle -</button>
  <button onclick="sendCmd('E')">Throttle +</button>

  <script>
    function sendCmd(cmd) {
      fetch('/cmd?key=' + cmd);
    }

    document.addEventListener('keydown', function(event) {
      let key = event.key.toUpperCase();

      if ("WASDQEX".includes(key)) {
        sendCmd(key);
      }
    });

    document.addEventListener('keyup', function(event) {
      let key = event.key.toUpperCase();

      if ("WASD".includes(key)) {
        sendCmd('0');
      }
    });
  </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void handleCommand() {
  if (!server.hasArg("key")) {
    server.send(400, "text/plain", "Missing key");
    return;
  }

  String key = server.arg("key");

  pitchCmd = 0;
  rollCmd = 0;

  if (key == "W") {
    pitchCmd = 20;
  }
  else if (key == "S") {
    pitchCmd = -20;
  }
  else if (key == "A") {
    rollCmd = 20;
  }
  else if (key == "D") {
    rollCmd = -20;
  }
  else if (key == "Q") {
    throttle -= 10;
  }
  else if (key == "E") {
    throttle += 10;
  }
  else if (key == "X") {
    throttle = 0;
    pitchCmd = 0;
    rollCmd = 0;
  }
  else if (key == "0") {
    pitchCmd = 0;
    rollCmd = 0;
  }

  throttle = constrain(throttle, 0, 255);

  Serial.print("Comanda: ");
  Serial.print(key);
  Serial.print(" | throttle: ");
  Serial.print(throttle);
  Serial.print(" | pitchCmd: ");
  Serial.print(pitchCmd);
  Serial.print(" | rollCmd: ");
  Serial.println(rollCmd);

  server.send(200, "text/plain", "OK");
}

void setupWiFiControl() {
  WiFi.softAP(ssid, password);

  Serial.println("WiFi pornit.");
  Serial.print("Retea: ");
  Serial.println(ssid);
  Serial.print("Parola: ");
  Serial.println(password);
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);

  server.begin();

  Serial.println("Server web pornit.");
}

void readWiFiControl() {
  server.handleClient();
}