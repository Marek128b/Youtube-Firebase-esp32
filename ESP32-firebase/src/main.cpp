#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "ZTE_8EU23F"      // SSID
#define WIFI_PASSWORD "6mM5mJYY726" // PASSWORD

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCFGIi20VOO_krrHcP06PwChzrvF-g1knw" // API-KEY

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://test-35c8b-default-rtdb.europe-west1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app //Firebase-URL

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "test.123@gmail.com" // Firebase-Email
#define USER_PASSWORD "123456"          // Firebase-Password

#define LED_PIN 13
#define POT_PIN 33
int prevValue = 0; // variable to store the previous potentiometer value

// Define Firebase Data object
FirebaseData fbdo, fbdo_s1;

FirebaseAuth auth;
FirebaseConfig config;
String uid;

unsigned long sendDataPrevMillis = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  // begin Stream
  if (!Firebase.RTDB.beginStream(&fbdo_s1, "/led1"))
  {
    Serial.printf("Stram 1 error begin: %s\r\n", fbdo_s1.errorReason().c_str());
  }

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "")
  {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);
}

void loop()
{
  // Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
  // in all Firebase and FirebaseJson functions, unless F() macro is not supported.

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && Firebase.RTDB.getBool(&fbdo, "/led1")) //using polling database
  {
    digitalWrite(LED_PIN, fbdo.boolData());
  }

  int potValue = analogRead(POT_PIN); // read the potentiometer value
  if (Firebase.ready() && potValue != prevValue)
  {
    Serial.println(potValue);
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, "/pot1", potValue) ? "ok" : fbdo.errorReason().c_str());
    prevValue = potValue; // store the new value as the previous value
    delay(100);
  }
  delay(20);
}