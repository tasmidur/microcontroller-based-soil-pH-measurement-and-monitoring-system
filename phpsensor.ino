#include FirebaseESP8266.h
#include ESP8266WiFi.h
#include Wire.h
#include Adafruit_GFX.h
#include Adafruit_SSD1306.h
#include math.h

#include DHT.h



#define FIREBASE_HOST phsensor2019.firebaseio.com Without http or https schemes
#define FIREBASE_AUTH 8zJ6BrYNFm4BEClURQbq5ht0o2pfScwBFgVnDPbP
#define WIFI_SSID itmansion1
#define WIFI_PASSWORD itmansion1

#define SCREEN_WIDTH 128  OLED display width, in pixels
#define SCREEN_HEIGHT 64  OLED display height, in pixels

 Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SensorPin A0            pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40  

#define DHTPIN D6

#define DHTTYPE DHT11

times of collection
int pHArray[ArrayLenth];   Store the average value of the sensor feedback
int pHArrayIndex=0;


Define FirebaseESP8266 data object
FirebaseData firebaseData;

FirebaseJson json;

DHT dht(DHTPIN, DHTTYPE);


String path = PHSENSOR;

double data;

void setup()
{

  Serial.begin(9600);
  dht.begin();
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print(Connecting to Wi-Fi);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(.);
    delay(300);
  }
  Serial.println();
  Serial.print(Connected with IP );
  Serial.println(WiFi.localIP());
  Serial.println();
  
  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Set the size of WiFi rxtx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000  60);
  tiny, small, medium, large and unlimited.
  Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, tiny);
  
  
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  
 Firebase.pushInt(firebaseData, path + PushInt,value)
 
 Firebase.setDouble(firebaseData, path + alive +1.00);


  
}

void loop()
{
   data=phValue();
  
   
  
  float t = dht.readTemperature();
  Serial.print(F(%  Temperature ));
  Serial.print(t);
  float h = dht.readHumidity();
  pushValue(data,t,h);
  Serial.print(h);
  displayodel(phvalue);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  Address 0x3D for 128x64
    Serial.println(F(SSD1306 allocation failed));
    for(;;)
;
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
   Display static text
  display.print(PH);
  display.println(data);
  display.print(TEMP);
  display.println(t);
  
  Serial.print(display);
  Serial.println(data);
  display.display(); 
  delay(1800);
}

void pushValue(double ph,double temperature,double humidity){
  
  if(Firebase.setDouble(firebaseData, path + PH_LIVE,ph)){
    Serial.println(PH VALUE ADDED);
  }else{
      Serial.println(PH ADDED FAILED);
      Serial.println(REASON  + firebaseData.errorReason());
      Serial.println(------------------------------------);
      Serial.println();
  }

  if(Firebase.setDouble(firebaseData, path + PH_HUMIDITY_LIVE,humidity)){
    Serial.println(HUMIDITY_LIVE VALUE ADDED);
  }else{
      Serial.println(HUMIDITY_LIVE ADDED FAILED);
      Serial.println(REASON  + firebaseData.errorReason());
      Serial.println(------------------------------------);
      Serial.println();
  }

  if(Firebase.setDouble(firebaseData, path + PH_TEMPERATURE_LIVE,temperature)){
    Serial.println(TEMPERATURE VALUE ADDED);
  }else{
      Serial.println(TEMPERATURE ADDED FAILED);
      Serial.println(REASON  + firebaseData.errorReason());
      Serial.println(------------------------------------);
      Serial.println();
  }

  if(Firebase.pushDouble(firebaseData, path + PH_TEMPERATURES,temperature)){
    Serial.println(TEMPERATURE VALUE PUSHED);
  }else{
      Serial.println(TEMPERATURE PUSHED FAILED);
      Serial.println(REASON  + firebaseData.errorReason());
      Serial.println(------------------------------------);
      Serial.println();
  }
  if(Firebase.pushDouble(firebaseData, path + PH_HUMIDITYS,humidity)){
    Serial.println(PH_HUMIDITYS VALUE PUSHED);
  }else{
      Serial.println(PH_HUMIDITYS PUSHED FAILED);
      Serial.println(REASON  + firebaseData.errorReason());
      Serial.println(------------------------------------);
      Serial.println();
  }

  if (Firebase.pushDouble(firebaseData, path + PH,ph))
    {
        Serial.println(PUSHED);
    }
    else
    {
      Serial.println(PUSH FAILED);
      Serial.println(REASON  + firebaseData.errorReason());
      Serial.println(------------------------------------);
      Serial.println();
    }
}

double phValue(){
  double result_phvalue=0.0;
   static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime  samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)5.01024;
      pHValue = 3.5voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime  printInterval)   Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print(Voltage);
    Serial.print(voltage,2);
    Serial.print(    pH value );
    Serial.println(pHValue,2);
    result_phvalue=pHValue;
    printTime=millis();
  }
  return result_phvalue;
}

double avergearray(int arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number=0){
    Serial.println(Error number for the array to avraging!n);
    return 0;
  }
  if(number5){   less than 5, calculated directly statistics
    for(i=0;inumber;i++){
      amount+=arr[i];
    }
    avg = amountnumber;
    return avg;
  }else{
    if(arr[0]arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;inumber;i++){
      if(arr[i]min){
        amount+=min;        arrmin
        min=arr[i];
      }else {
        if(arr[i]max){
          amount+=max;    arrmax
          max=arr[i];
        }else{
          amount+=arr[i]; min=arr=max
        }
      }if
    }for
    avg = (double)amount(number-2);
  }if
  return avg;
}