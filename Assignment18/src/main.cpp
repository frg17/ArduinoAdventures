#include <WiFi.h>
// Blink led when packets are exchanged (optional)
#define LED_PIN 33 // ESP32 -CAM
//#define LED_PIN 2 // ESP32 Dev board?
#if defined LED_PIN
int led = 0;
#endif
const char *ssid = "TOL026M";		// Course Wifi network
IPAddress myip(192, 168, 0, 1);		// Server IP address
IPAddress subnet(255, 255, 255, 0); // Subnet mask
WiFiUDP udp;
#define MAX_LENGTH 200
char buf[MAX_LENGTH + 1];

void setup()
{
#if defined(LED_PIN)
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
#endif
	Serial.begin(115200);
	delay(250);
	WiFi.mode(WIFI_AP); // Setup a Wifi network
	WiFi.softAP(ssid);	// no PSK
	delay(200);			// give AP some time to start
	WiFi.softAPConfig(myip, myip, subnet);
	delay(200);
	IPAddress IP = WiFi.softAPIP();
	udp.begin(3001);
	Serial.print("Listening UDP port 3001 on IP: ");
	Serial.println(IP);
}

void loop()
{
	if ((udp.parsePacket() <= 0))
		return;

#if defined(LED_PIN)
	led = led ? 0 : 1;
	digitalWrite(LED_PIN, led);
#endif
	if (udp.read(buf, MAX_LENGTH) > 0)
	{
		Serial.println("Datagram received -- sending a reply");
		udp.beginPacket(udp.remoteIP(), udp.remotePort());
		udp.print("pong");
		udp.endPacket(); // send it!
	}
}
