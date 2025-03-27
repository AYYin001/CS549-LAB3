#include <WiFi.h>
#include <HttpClient.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

// This example downloads the URL "http://arduino.cc/"

char ssid[] = "Yihan";    // your network SSID (name) 
char pass[] = "cvdl7342"; // your network password (use for WPA, or use as key for WEP)

// Name of the server we want to connect to
const char kHostname[] = "18.117.121.106";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const int kPort = 5000;
const char kPath[] = "/?var=25";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

// Create AHT sensor instance
Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(9600);

  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  if (!aht.begin()) {
    Serial.println("Could not find AHT10/AHT20 sensor. Check wiring.");
    while (1) delay(10);
  } else {
    Serial.println("AHT sensor found.");
  }

}

void loop() {
    // Read humidity and temperature
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    float humidityValue = humidity.relative_humidity;
    float temperatureValue = temp.temperature;
  
    // Format the GET request path
    char path[100];
    snprintf(path, sizeof(path), "/?humidity=%.2f&temp=%.2f", humidityValue, temperatureValue);
    Serial.print("Sending request to: ");
    Serial.println(path);
  
    // Prepare HTTP request
    WiFiClient client;
    HttpClient http(client);
    int err = http.get(kHostname, kPort, path);
  
    if (err == 0) {
      Serial.println("Request started successfully");
  
      int statusCode = http.responseStatusCode();
      if (statusCode >= 0) {
        Serial.print("Status code: ");
        Serial.println(statusCode);
  
        int headerSkip = http.skipResponseHeaders();
        if (headerSkip >= 0) {
          Serial.println("Response body: ");
          unsigned long timeoutStart = millis();
  
          while ((http.connected() || http.available()) &&
                 (millis() - timeoutStart < kNetworkTimeout)) {
            if (http.available()) {
              char c = http.read();
              Serial.print(c);
              timeoutStart = millis(); // reset timeout
            } else {
              delay(kNetworkDelay);
            }
          }
        } else {
          Serial.print("Failed to skip headers: ");
          Serial.println(headerSkip);
        }
      } else {
        Serial.print("Failed status code: ");
        Serial.println(statusCode);
      }
    } else {
      Serial.print("Connection failed: ");
      Serial.println(err);
    }
  
    http.stop();
    Serial.println("HTTP connection closed\n");
    delay(5000); // wait before sending the next request
  }
  

// void loop() {


//   sensors_event_t humidity, temp;
//   aht.getEvent(&humidity, &temp);
//   float humidityValue = humidity.relative_humidity;
//   float temperatureValue = temp.temperature;

// //   Serial.print("💧 Humidity: ");
// //   Serial.print(humidityValue);
// //   Serial.println(" %");

//   char path[100];
//   snprintf(path, sizeof(path), "/?humidity=%.2f&temp=%.2f", humidityValue, temperatureValue);
//   Serial.print("📤 Final path: ");
//   Serial.println(path);



//   int err =0;
  
//   WiFiClient c;
//   HttpClient http(c);
  
//   err = http.get(kHostname, kPort, path);
//   if (err == 0)
//   {
//     Serial.println("startedRequest ok");

//     err = http.responseStatusCode();
//     if (err >= 0)
//     {
//       Serial.print("Got status code: ");
//       Serial.println(err);

//       // Usually you'd check that the response code is 200 or a
//       // similar "success" code (200-299) before carrying on,
//       // but we'll print out whatever response we get

//       err = http.skipResponseHeaders();
//       if (err >= 0)
//       {
//         int bodyLen = http.contentLength();
//         Serial.print("Content length is: ");
//         Serial.println(bodyLen);
//         Serial.println();
//         Serial.println("Body returned follows:");
      
//         // Now we've got to the body, so we can print it out
//         unsigned long timeoutStart = millis();
//         char c;
//         // Whilst we haven't timed out & haven't reached the end of the body
//         while ( (http.connected() || http.available()) &&
//                ((millis() - timeoutStart) < kNetworkTimeout) )
//         {
//             if (http.available())
//             {
//                 c = http.read();
//                 // Print out this character
//                 Serial.print(c);
               
//                 bodyLen--;
//                 // We read something, reset the timeout counter
//                 timeoutStart = millis();
//             }
//             else
//             {
//                 // We haven't got any data, so let's pause to allow some to
//                 // arrive
//                 delay(kNetworkDelay);
//             }
//         }
//       }
//       else
//       {
//         Serial.print("Failed to skip response headers: ");
//         Serial.println(err);
//       }
//     }
//     else
//     {    
//       Serial.print("Getting response failed: ");
//       Serial.println(err);
//     }
//   }
//   else
//   {
//     Serial.print("Connect failed: ");
//     Serial.println(err);
//   }
//   http.stop();
//   delay(5000);

//   // And just stop, now that we've tried a download
//   //while(1);
// }
