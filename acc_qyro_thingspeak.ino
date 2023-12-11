#include <Arduino_LSM6DS3.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

const char *ssid = "JioFiber-QhM58";
const char *password = "9550286930";
const char *thingSpeakApiKey ="CA3MN4QRPK7554G5";
const unsigned long channelId = 2370870;

WiFiClient client; // Declare a client object

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    while (1);
  }

  ThingSpeak.begin(client); // Pass the client object to ThingSpeak

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();
  Serial.println("Acceleration in g's\tGyroscope in degrees/second");
  Serial.println("X\tY\tZ\tX\tY\tZ");
}

void loop() {
  float ax, ay, az, gx, gy, gz;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

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

    // Send data to ThingSpeak
    ThingSpeak.setField(1, ax);
    ThingSpeak.setField(2, ay);
    ThingSpeak.setField(3, az);
    ThingSpeak.setField(4, gx);
    ThingSpeak.setField(5, gy);
    ThingSpeak.setField(6, gz);

    int response = ThingSpeak.writeFields(channelId, thingSpeakApiKey);

    if (response == 200) {
      Serial.println("Data sent to ThingSpeak successfully!");
    } else {
      Serial.println("Error sending data to ThingSpeak. HTTP error code: " + String(response));
    }

    //delay(15000);  // Adjust the delay as needed
  }
}
