#include <DHT.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/*
 Name:		iot.ino
 Created:	4/24/2019 4:29:12 PM
 Author:	MIIX 510
*/

#include <Arduino_JSON.h>


#include <dummy.h>

#define URL_UPDATE "http://35.240.207.36/"
#define URL_CHECK  "http://35.240.207.36/"
#define WIFI_SSID  "AndroidAPff7a"
#define WIFI_PASS  "poliklinik"

#define DHT_PORT 0
#define DHT_TYPE 11



bool isInternetOK()
{
	if (WiFi.status() != WL_CONNECTED) return false;

	HTTPClient http;
	http.begin(URL_CHECK);
	int code = http.GET();
	auto payload = http.getString();
	Serial.println(payload);
	Serial.println(String(code).c_str());
	http.end();
	return (code == HTTP_CODE_OK);
}

bool PostData()
{
	HTTPClient http;
	DHT dht(DHT_PORT, DHT11);
	JSONVar json;

	dht.begin();

	json["DeviceId"] = 1;
	json["Temperature"] = dht.readTemperature();
	json["Humidity"] = dht.readHumidity() / 100.0;
	json["Sound"] = 0;

	http.begin("http://35.240.207.36/Device/PostLog");
	http.addHeader("Content-Type", "application/json; charset=utf-8");

	int code = http.POST(JSON.stringify(json));
	Serial.println(http.getString());
	http.end();


	return (code == HTTP_CODE_OK);
}

void setup()
{
	Serial.begin(9600);
	Serial.write("OK\n");
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	Serial.write("Connecting");
	int i = 0;
	while (WiFi.status() != WL_CONNECTED && i < 100)
	{
		delay(500);
		Serial.write(".");
		i++;
	}
	if (i > 100) Serial.write("Failed to connect to designated WiFI!");

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
		if (PostData())
			Serial.println("Post sukses");
		else
			Serial.println("Post failed");
	}
}