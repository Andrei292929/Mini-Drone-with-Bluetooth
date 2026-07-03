#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "DroneESP32";
const char* password = "12345678";

void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Control drona ESP32</title>

  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f4f4f4;
      color: #222;
      padding: 25px;
    }

    h2 {
      margin-bottom: 30px;
    }

    .controller {
      display: grid;
      grid-template-columns: 70px 70px 70px 70px 230px;
      grid-template-rows: 70px 70px 70px 70px;
      gap: 12px;
      align-items: center;
    }

    button {
      width: 60px;
      height: 55px;
      font-size: 22px;
      font-weight: bold;
      border: 2px solid #333;
      border-radius: 8px;
      background: white;
      cursor: pointer;
      box-shadow: 0 4px 0 #aaa;
      transition: 0.05s;
    }

    button:active,
    button.pressed {
      transform: translateY(3px);
      box-shadow: 0 1px 0 #777;
      background: #ddd;
    }

    .q { grid-column: 1; grid-row: 1; }
    .w { grid-column: 2; grid-row: 1; }
    .e { grid-column: 3; grid-row: 1; }

    .a { grid-column: 1; grid-row: 2; }
    .s { grid-column: 2; grid-row: 2; }
    .d { grid-column: 3; grid-row: 2; }

    .x {
      grid-column: 2;
      grid-row: 4;
      background: #ffdddd;
      border-color: #aa0000;
      color: #aa0000;
    }

    .x.pressed,
    .x:active {
      background: #ff9999;
    }

    .line {
      height: 2px;
      background: #444;
      width: 55px;
    }

    .desc {
      border: 2px solid #333;
      background: white;
      padding: 8px 12px;
      border-radius: 8px;
      font-size: 15px;
      min-width: 180px;
    }

    .line-q { grid-column: 4; grid-row: 1; }
    .desc-q { grid-column: 5; grid-row: 1; }

    .line-a { grid-column: 4; grid-row: 2; }
    .desc-a { grid-column: 5; grid-row: 2; }

    .line-x { grid-column: 4; grid-row: 4; }
    .desc-x { grid-column: 5; grid-row: 4; }

    .legend {
      margin-top: 25px;
      font-size: 14px;
      color: #555;
    }
  </style>
</head>

<body>
  <h2>Control drona ESP32</h2>

  <div class="controller">
    <button id="btnQ" class="q">Q</button>
    <button id="btnW" class="w">W</button>
    <button id="btnE" class="e">E</button>

    <button id="btnA" class="a">A</button>
    <button id="btnS" class="s">S</button>
    <button id="btnD" class="d">D</button>

    <button id="btnX" class="x">X</button>

    <div class="line line-q"></div>
    <div class="desc desc-q">Q = throttle down<br>E = throttle up</div>

    <div class="line line-a"></div>
    <div class="desc desc-a">W = forward<br>S = backward<br>A = left<br>D = right</div>

    <div class="line line-x"></div>
    <div class="desc desc-x">X = stop motors</div>
  </div>

  <p class="legend">
    Use the on-screen buttons or the physical keyboard. Combinations like W + A or W + D are supported.
  </p>

  <script>
    const movementKeys = {
      W: false,
      A: false,
      S: false,
      D: false
    };

    function sendControlState() {
      let pitch = 0;
      let roll = 0;

      if (movementKeys.W) pitch += 20;
      if (movementKeys.S) pitch -= 20;
      if (movementKeys.A) roll += 20;
      if (movementKeys.D) roll -= 20;

      fetch('/cmd?pitch=' + pitch + '&roll=' + roll);
    }

    function sendThrottle(delta) {
      fetch('/cmd?throttle=' + delta);
    }

    function sendStop() {
      fetch('/cmd?stop=1');
    }

    function pressButton(key) {
      let btn = document.getElementById('btn' + key);
      if (btn) {
        btn.classList.add('pressed');
      }
    }

    function releaseButton(key) {
      let btn = document.getElementById('btn' + key);
      if (btn) {
        btn.classList.remove('pressed');
      }
    }

    document.addEventListener('keydown', function(event) {
      let key = event.key.toUpperCase();

      if ("WASD".includes(key)) {
        event.preventDefault();

        if (!movementKeys[key]) {
          movementKeys[key] = true;
          pressButton(key);
          sendControlState();
        }
      }

      if (key === "Q") {
        event.preventDefault();
        pressButton("Q");
        sendThrottle(-10);
      }

      if (key === "E") {
        event.preventDefault();
        pressButton("E");
        sendThrottle(10);
      }

      if (key === "X") {
        event.preventDefault();
        pressButton("X");
        sendStop();
      }
    });

    document.addEventListener('keyup', function(event) {
      let key = event.key.toUpperCase();

      if ("WASD".includes(key)) {
        event.preventDefault();
        movementKeys[key] = false;
        releaseButton(key);
        sendControlState();
      }

      if (key === "Q" || key === "E" || key === "X") {
        event.preventDefault();
        releaseButton(key);
      }
    });

    function setupButtonHold(key) {
      let btn = document.getElementById('btn' + key);

      btn.addEventListener('mousedown', function() {
        pressButton(key);

        if ("WASD".includes(key)) {
          movementKeys[key] = true;
          sendControlState();
        } else if (key === "Q") {
          sendThrottle(-10);
        } else if (key === "E") {
          sendThrottle(10);
        } else if (key === "X") {
          sendStop();
        }
      });

      btn.addEventListener('mouseup', function() {
        releaseButton(key);

        if ("WASD".includes(key)) {
          movementKeys[key] = false;
          sendControlState();
        }
      });

      btn.addEventListener('mouseleave', function() {
        releaseButton(key);

        if ("WASD".includes(key)) {
          movementKeys[key] = false;
          sendControlState();
        }
      });

      btn.addEventListener('touchstart', function(event) {
        event.preventDefault();
        pressButton(key);

        if ("WASD".includes(key)) {
          movementKeys[key] = true;
          sendControlState();
        } else if (key === "Q") {
          sendThrottle(-10);
        } else if (key === "E") {
          sendThrottle(10);
        } else if (key === "X") {
          sendStop();
        }
      });

      btn.addEventListener('touchend', function(event) {
        event.preventDefault();
        releaseButton(key);

        if ("WASD".includes(key)) {
          movementKeys[key] = false;
          sendControlState();
        }
      });
    }

    ["Q", "W", "E", "A", "S", "D", "X"].forEach(setupButtonHold);
  </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void handleCommand() {
  if (server.hasArg("pitch")) {
    pitchCmd = server.arg("pitch").toInt();
  }

  if (server.hasArg("roll")) {
    rollCmd = server.arg("roll").toInt();
  }

  if (server.hasArg("throttle")) {
    throttle += server.arg("throttle").toInt();
  }

  if (server.hasArg("stop")) {
    throttle = 0;
    pitchCmd = 0;
    rollCmd = 0;
  }

  throttle = constrain(throttle, 0, 255);
  pitchCmd = constrain(pitchCmd, -50, 50);
  rollCmd = constrain(rollCmd, -50, 50);

  Serial.print("WiFi control | throttle: ");
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
}#include <WiFi.h>
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
