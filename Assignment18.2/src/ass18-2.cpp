#include "WiFi.h"
// Blink led when packets are exchanged (optional)
//#define LED_PIN 33 // ESP32 -CAM
//#define LED_PIN 2 // ESP32 Dev board?
const char *ssid = "TOL026M";		   // Course Wifi network
const char *server_ip = "192.168.0.1"; // Course Server
const int server_port = 3001;
const int my_port = 2020;
WiFiUDP udp;
#define TIMEOUT_MS 5000 // 5sec
#define MAX_LENGTH 200
char buf[MAX_LENGTH + 1];

int exp_count = 10000;		 //------------------Bætt við
int count = 0;				 //------------------Bætt við
uint64_t experiments[10000]; //------------------Bætt við

void setup()
{
#if defined(LED_PIN)
	pinMode(LED_PIN, OUTPUT);
#endif
	Serial.begin(115200);
	delay(250);
	// Setup a Wifi network
	WiFi.disconnect(true);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid); // no PSK
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println(" connected");
	Serial.print("My IP address: ");
	Serial.println(WiFi.localIP());
	udp.begin(my_port); // set up UDP
}

void calculateMedian()
{
	uint64_t sum = 0;
	uint64_t medianms = 0;
	for (int i = 0; i < exp_count; i++)
	{
		sum += experiments[i];
	}

	medianms = (sum / exp_count) / 1000;

	Serial.print(exp_count);
	Serial.println(" experiments concluded.");
	Serial.print("Median in ms: ");
	Serial.println((long)medianms);
}

void loop()
{
	uint64_t t0;
	uint64_t t1;
	int i;
#if defined(LED_PIN)
	digitalWrite(LED_PIN, LOW);
	delay(500);
	digitalWrite(LED_PIN, HIGH);
	delay(500);
#else
	delay(1000);
#endif
	udp.beginPacket(server_ip, server_port); // send a "hello" message
	strcpy(buf, "ping");
	udp.write((const uint8_t *)buf, strlen(buf));
	udp.endPacket();
	t0 = esp_timer_get_time();
	while ((esp_timer_get_time() - t0) / 1000 < TIMEOUT_MS)
	{
		if (udp.parsePacket() <= 0)
		{
			delay(100); // no response , check again later!
			continue;
		}
		if ((i = udp.read(buf, MAX_LENGTH)) > 0) // got a response!
		{
			buf[i] = '\0';
			t1 = esp_timer_get_time(); // -------------------- Bætt við
			Serial.print("Server response: ");
			Serial.println((char *)buf);

			experiments[count] = t1 - t0; //------------ Bætt við
			count++;					  //------------ Bætt við
			if (count == exp_count)		  //------------ Bætt við
			{							  //------------ Bætt við
				calculateMedian();		  //------------ Bætt við
			}							  //------------ Bætt við

			return;
		}
	}

	Serial.println("Time out - no response");
}