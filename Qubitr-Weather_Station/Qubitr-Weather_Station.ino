#include <TinyGPS++.h>
#include <QubitroMqttClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <ClosedCube_OPT3001.h> // Click here to get the library: http://librarymanager/All#OPT3001
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h> // Click to install library: http://librarymanager/All#Adafruit_BME680

#define OPT3001_ADDRESS 0x44
#define SEALEVELPRESSURE_HPA (1010.0)

Adafruit_BME680 bme;
ClosedCube_OPT3001 g_opt3001;
TinyGPSPlus  gps;

// WiFi Client
WiFiClient wifiClient;
// Qubitro Client
QubitroMqttClient mqttClient(wifiClient);

double lux, luminosity;
float Lat, Lng
String  lat_str , lng_str;

// Device Parameters
char deviceID[] = "3726d909-47e5-42ae-b8da-a479ea7780aa";
char deviceToken[] = "Hb6zA3E9AvJmWM8fSED1QGrRAX3OXDZevzjv5F9c";

// WiFi Parameters
const char* ssid = "ELDRADO";
const char* password = "amazon123";


void configureSensor()
{
  OPT3001_Config newConfig;

  newConfig.RangeNumber = B1100;
  newConfig.ConvertionTime = B0;
  newConfig.Latch = B1;
  newConfig.ModeOfConversionOperation = B11;

  OPT3001_ErrorCode errorConfig = g_opt3001.writeConfig(newConfig);

  OPT3001_Config sensorConfig = g_opt3001.readConfig();

}

void opt3001_read_data()
{
  OPT3001 result = g_opt3001.readResult();
  if (result.error == NO_ERROR)
  {
    luminosity = result.lux;
  }
  else
  {
    printError("OPT3001", result.error);
  }
}


void printError(String text, OPT3001_ErrorCode error)
{
  Serial.print(text);
  Serial.print(": [ERROR] Code #");
  Serial.println(error);
}


void bme680_init()
{
  Wire.begin();

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    return;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void bme680_get()
{
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  opt3001_read_data();

  Serial.print("Light = ");
  Serial.print(luminosity);
  Serial.println(" lux");

  printFloatlat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloatlng(gps.location.lng(), gps.location.isValid(), 12, 6);

  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < 1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

  Serial.println();

  // Send telemetry
  String payload = "{  \"Temperature\":" + String(bme.temperature)
                   + ",\"Pressure\":" + String(bme.pressure / 100.0)
                   + ",\"Humidity\":" + String(bme.humidity)
                   + ",\"Gas\":" + String(bme.gas_resistance / 1000.0)
                   + ",\"Light\":" + luminosity
                   + ",\"Latitude\":" + lat_str
                   + ",\"Langitue\":" + lng_str
                   + "}";
  mqttClient.poll();
  mqttClient.beginMessage(deviceID);
  mqttClient.print(payload);
  mqttClient.endMessage();
  digitalWrite(2, HIGH);
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
  digitalWrite(12, HIGH);
  mqttClient.subscribe(deviceID);
}
void setup()
{
  Serial.begin(115200);

  //gps init
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, 0);
  delay(1000);
  digitalWrite(WB_IO2, 1);
  delay(1000);

  Serial1.begin(9600);
  while (!Serial1);
  Serial.println("GPS uart init ok!");

  // Initialize the built in LED
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize wireless connectivity
  wifi_init();
  // Initialize Qubitro
  qubitro_init();
  time_t serial_timeout = millis();
  bme680_init();
  /* opt3001 init */
  g_opt3001.begin(OPT3001_ADDRESS);
  configureSensor();
}

void printFloatlat(float val, bool valid, int len, int prec)
{
  Lat = val;
  lat_str = String(Lat , 6);
  Serial.print("Latitude = ");
  Serial.println(lat_str);
}
void printFloatlng(float val1, bool valid, int len, int prec)
{
  Lng = val1;
  lng_str = String(Lng , 6);
  Serial.print("Longitude = ");
  Serial.println(lng_str);
}


void loop()
{

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  bme680_get();
  delay(90000);
  digitalWrite(2, LOW);
}
