#include <QubitroMqttClient.h>
#include <WiFi.h>
#include <M5StickC.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
// WiFi Client
WiFiClient wifiClient;
#define ONE_WIRE_BUS 33
// Qubitro Client
QubitroMqttClient mqttClient(wifiClient);
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
// Device Parameters
char deviceID[] = "996c4667-a2d2-4cec-8488-86c20c5214c9";
char deviceToken[] = "3$QQpSGvsxFLJ7a06TMY4$zgu97pIEkDvDoOSmPf";

// WiFi Parameters
const char* ssid = "ELDRADO";
const char* password = "amazon123";
int RawValue = 0;
double Voltage = 0;
double tempC = 0;
double Sum;

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
 Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.println("DONE"); 
/********************************************************************/
 Serial.print(sensors.getTempCByIndex(0)); 
  float sensor_value_1 = sensors.getTempCByIndex(0);

  // Print the random values
  Serial.print("Temperature value: ");
  Serial.println(sensor_value_1);


  // Send telemetry
  String payload = "{\"Temperature\": " + String(sensor_value_1) + "}";
  mqttClient.poll();
  mqttClient.beginMessage(deviceID);
  mqttClient.print(payload);

  M5.Lcd.print("I'm Groot ");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(3, 2);
  M5.Lcd.print(payload);
  mqttClient.endMessage();

  // Delay
  delay(5000);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(1, 3);
  M5.Lcd.print("I'm Groot ");
  delay(60000);
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

  if (!mqttClient.connect(host, port))
  {
    Serial.print("Connection failed. Error code: ");
    Serial.println(mqttClient.connectError());
    Serial.println("Visit docs.qubitro.com or create a new issue on github.com/qubitro");
  }
  Serial.println("Connected to Qubitro.");

  mqttClient.subscribe(deviceID);

}
