#include <Wire.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

char ssid[] = "Itreek";
char pass[] = "7893031134";
char server[] = "script.google.com";
int port = 443;  // Use port 443 for HTTPS

WiFiSSLClient wifi;
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

   while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  // Connect to WiFi

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); 
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
void loop() {
  // Your logic to set aax, aay, aaz, ggx, ggy, ggz
   while(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
  float ax, ay, az,gx,gy,gz;
  IMU.readAcceleration(ax, ay, az);
  IMU.readGyroscope(gx, gy, gz);

  String aax=String(ax,6);
  String aay=String(ay,6);
  String aaz=String(az,6);
  
  String ggx=String(gx,6);
  String ggy=String(gy,6);
  String ggz=String(gz,6);
  
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  
  int secondsInMinute = seconds % 60;
  int minutesInHour = (seconds / 60) % 60;
  int hoursInDay = (seconds / 3600) % 24;

  Serial.print(hoursInDay);
  Serial.print(':');
  Serial.print(minutesInHour);
  Serial.print(':');
  Serial.println(secondsInMinute);

  Serial.print(ax);
  Serial.print('\t');
  Serial.print(ay);
  Serial.print('\t');
  Serial.print(az);
  Serial.print('\t');

  Serial.print(gx);
  Serial.print('\t');
  Serial.print(gy);
  Serial.print('\t');
  Serial.println(gz);

  // Create the GET request path
  String path = "/macros/s/AKfycbzqRJyYBuHc7ZbzmABRvVh66R38eENF4_xzoQZmx4ni1AziJwhVvULp2D-A3cZv_83F/exec";
  path += "?ax=" + String(aax) +
          "&ay=" + String(aay) +
          "&az=" + String(aaz) +
          "&gx=" + String(ggx) +
          "&gy=" + String(ggy) +
          "&gz=" + String(ggz);

  Serial.println("Making GET request");
unsigned long startTime = millis();
  // Make the GET request
  if (wifi.connectSSL(server, port)) {
    wifi.print("GET " + path + " HTTP/1.1\r\n");
    wifi.print("Host: " + String(server) + "\r\n");
    wifi.print("Connection: close\r\n\r\n");

    // Read and process the response (assuming it's a simple text response)
    while (wifi.available()) {
      char c = wifi.read();
      // Process the response data as needed
    }

    unsigned long duration = millis() - startTime;
    Serial.print("GET request took ");
    Serial.print(duration);
    Serial.println(" milliseconds");

    wifi.stop();
  }
  }
  // Wait for some time before the next request
  delay(1000);
}
