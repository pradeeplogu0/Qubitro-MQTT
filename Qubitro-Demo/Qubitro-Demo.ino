#include <QubitroMqttClient.h>
#include <WiFi.h>
#include <M5StickC.h>

// WiFi Client
WiFiClient wifiClient;

// Qubitro Client
QubitroMqttClient mqttClient(wifiClient);

// Device Parameters
char deviceID[] = "e84f1894-ae5f-388b10";
char deviceToken[] = "sPnwDzA$DorW1K8qjXp4y$2VJz5Z";

// WiFi Parameters
const char* ssid = "ELDRADO";
const char* password = "amazon123";

void setup() {
  // Initialize the serial port
  serial_init();

  // Initialize wireless connectivity
  wifi_init();

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(7, 20, 2);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  // Initialize Qubitro
  qubitro_init();
}

void loop() {

  // Generate random values
  int sensor_value_1 = random(0, 100);
  int sensor_value_2 = random(0, 100);

  // Print the random values
  Serial.print("Sensor value 1: ");
  Serial.println(sensor_value_1);
  Serial.print("Sensor value 2: ");
  Serial.println(sensor_value_2);

  // Send telemetry
  String payload = "{\"Sensor 1\":" + String(sensor_value_1)
                   + ",\"Sensor 2\":" + String(sensor_value_2) + "}";
  mqttClient.poll();
  mqttClient.beginMessage(deviceID);
  mqttClient.print(payload);

  M5.Lcd.print("I'm Groot ");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(3, 2);
  M5.Lcd.print(payload);
  mqttClient.endMessage();

  // Delay
  delay(3000);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(1, 3);
  M5.Lcd.print("I'm Groot ");
  delay(2000);
  M5.Lcd.fillScreen(BLACK);
}

// Initialization code

void serial_init() {
  // Initiate serial port connection
  Serial.begin(115200);

  // Delay for stabilization
  delay(200);
}

void wifi_init() {
  // Set WiFi mode
  WiFi.mode(WIFI_STA);

  // Disconnect WiFi
  WiFi.disconnect();
  delay(100);

  // Initiate WiFi connection
  WiFi.begin(ssid, password);

  // Print connectivity status to the terminal
  Serial.print("Connecting to WiFi...");
  while (true)
  {
    delay(1000);
    Serial.print(".");
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println("WiFi Connected.");

      Serial.print("Local IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.println(WiFi.RSSI());
      break;
    }
  }
}

void qubitro_init() {
  char host[] = "broker.qubitro.com";
  int port = 1883;
  mqttClient.setId(deviceID);
  mqttClient.setDeviceIdToken(deviceID, deviceToken);
  Serial.println("Connecting to Qubitro...");
  //  M5.Lcd.print("Connecting to Qubitro.");
  //  M5.Lcd.fillScreen(BLACK);
  if (!mqttClient.connect(host, port))
  {
    Serial.print("Connection failed. Error code: ");
    Serial.println(mqttClient.connectError());
    Serial.println("Visit docs.qubitro.com or create a new issue on github.com/qubitro");
  }
  Serial.println("Connected to Qubitro.");

  mqttClient.subscribe(deviceID);
  //  M5.Lcd.fillScreen(BLACK);
}
