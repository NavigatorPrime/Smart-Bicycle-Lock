#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>   //Internet
#include <ArduinoHttpClient.h>  

#include <SoftwareSerial.h>

#include <Wire.h>                  // Include the Wire library for I2C communication
#include <Adafruit_Sensor.h>        // Include the Adafruit Sensor library
#include <Adafruit_ADXL345_U.h> 

#include <TinyGPS++.h>//GPS

SoftwareSerial sim800(7, 8);  // RX, TX pins for the SIM800
SoftwareSerial gpsSerial(9,10);//RX, TX pins for the GPS

//Sending Messages
String Phone_no="+94";
String msg="apple";

//Internet
const char WebServer_HOST[] = "ekaivalabs.com";  // HTTP server host
const String WebServer_PATH = "/";  // Path for HTTP GET request
const int HTTP_PORT = 80;  // HTTP port

TinyGsm modem(sim800);  // Initialize the modem
TinyGsmClient gsm_client(modem);  // Use TinyGsmClient for HTTP (non-secure connection)
HttpClient http_client = HttpClient(gsm_client, WebServer_HOST, HTTP_PORT);  // Initialize HTTP client

char apn[]  = "mobitel";  // Carrier APN (Mobitel in this case)
char user[] = "";  // Username for GPRS connection (usually empty)
char pass[] = "";  // Password for GPRS connection (usually empty)

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();//making accel object to use accelerometer

//unsigned long previousMillis = 0;

//GPS
TinyGPSPlus gps;
float lattitude,longitude;

void setup() {

  // Initialize serial monitor
  Serial.begin(115200);  
  Serial.println("Device serial initialize");

  //Making Initializations for internet
  sim800.begin(9600);  // Initialize SIM800 serial
  Serial.println("SIM800L serial initialize");

  Serial.println("Initializing modem...");
  modem.restart();  // Restart the modem
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);

  http_client.setHttpResponseTimeout(10 * 1000);  // Set HTTP timeout to 10 seconds
  Serial.print(F("Connecting to APN: "));
  Serial.print(apn);

  //Initialinzing Accelerometer
  if (!accel.begin()) {
    Serial.println("Couldn't find the ADXL345 sensor");
    while (1);  // Infinite loop if sensor isn't found
  }
  Serial.println("ADXL345 Accelerometer Test");
  accel.setRange(ADXL345_RANGE_16_G); // Set the range of the accelerometer to 16G (updated constant)

}

void loop() {
  Internet();
  float x,y,z;
  Accelerometer(x,y,z);
  GPS(lattitude,longitude); 
}

void  Internet(){
   if (!modem.gprsConnect(apn, user, pass)) {  // Attempt to connect to GPRS
    Serial.print(F("Connecting to "));
    Serial.print(apn);
    Serial.println(" failed");
    delay(1000);
    return;  // Retry if connection fails
  } 
  else {
    Serial.println("GPRS Connected");
    delay(1000);  // Give some time for the connection to stabilize
    
    // Send an HTTP request after GPRS is connected
    Serial.println("Sending HTTP request...");
    http_client.get(WebServer_PATH);  // Send a GET request to the specified path
    
    int statusCode = http_client.responseStatusCode();  // Get the status code of the response
    String response = http_client.responseBody();  // Get the response body
    
    Serial.print("HTTP Status Code: ");
    Serial.println(statusCode);  // Print the HTTP status code
    Serial.print("Response: ");
    Serial.println(response);  // Print the response body
    
    delay(10000);  // Wait 10 seconds before sending another request
  }
}

void SMS(){
  sim800.println("AT+CMGF=1");
  delay(1000);
  sim800.println("AT+CMGS=\"" + Phone_no +"\"\r");
  delay(1000);
  sim800.println(msg);
  delay(100);
  sim800.println((char)26);
  delay(1000);
  Serial.println("Message Sent");
}


void Accelerometer(float &x,float &y,float &z){
  sensors_event_t event;
  accel.getEvent(&event); // Get the accelerometer data

  // Print the X, Y, and Z values of the accelerometer
  Serial.print("X: ");
  x=event.acceleration.x;
  Serial.print(x);
  Serial.print(" m/s^2  ");

  Serial.print("Y: ");
  y=event.acceleration.y;
  Serial.print(y);
  Serial.print(" m/s^2  ");

  Serial.print("Z: ");
  z=event.acceleration.z;
  Serial.print(z);
  Serial.println(" m/s^2");

  delay(500); // Delay of 500ms before the next reading
}

void GPS(float &lattitude,float &longitude){
  while (gpsSerial.available())
  {
    int data = gpsSerial.read();
    if (gps.encode(data))
    {
      lattitude = (gps.location.lat());
      longitude = (gps.location.lng());
      Serial.print ("lattitude: ");
      Serial.println (lattitude);
      Serial.print ("longitude: ");
      Serial.println (longitude);
    }
  }
}
