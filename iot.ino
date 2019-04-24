
/*
 Name:		iot.ino
 Created:	4/24/2019 4:29:12 PM
 Author:	MIIX 510
*/
// the setup function runs once when you press reset or power the board
#include <DHT.h>

#include <WiFi.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>



#include <dummy.h>

#define URL_UPDATE "http://35.240.207.36/"
#define URL_UPDATE "http://35.240.207.36/Device/PostLog"
#define WIFI_SSID "Firhan Wibianto"
#define WIFI_PASS "jarangpulang"

#define DHT_PORT 0
#define DHT_TYPE 11

bool isInternetOK()
{
	HTTPClient http;
	http.begin("http://35.240.207.36/");
	int code = http.GET();
	http.end();
	return (code == HTTP_CODE_OK);

}

bool PostData()
{
	HTTPClient http;
	DHT dht(DHT_PORT, DHT_TYPE);
	JSONVar json;

	dht.begin();

	json["DeviceId"] = 1;
	json["Temperature"] = dht.readTemperature();
	json["Humidity"] = dht.readHumidity();
	json["Sound"] = 0;
	
	http.begin("http://35.240.207.36/Device/PostLog");
	http.addHeader("Content-Type", "application/json; charset=utf-8");
	int code = http.POST(JSON.stringify(json));
	http.end();
	Serial.println(http.getString());

	return (code == HTTP_CODE_OK);
}

void setup() 
{
	Serial.begin(9600);
	Serial.write("OK\n");
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	Serial.write("Connecting");
	int i = 0;
	while (WiFi.status() != WL_CONNECTED && i < 40)
	{
		delay(500);
		Serial.write(".");
		i++;
	}
	if (i > 40) Serial.write("Failed to connect to designated WiFI!");
	
	if (isInternetOK())
		Serial.write("Koneksi OK!");
	else
		Serial.write("Fail to connect to example.com");

}

// the loop function runs over and over again until power down or reset
void loop() 
{
	while (true)
	{
		delay(5000);
		PostData();
	}
}
