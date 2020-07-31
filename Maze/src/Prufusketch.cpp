#include <Arduino.h>
#include <WiFi.h>
#include <Player.h>

#define LED 2

int iterations = 0;

WiFiClient client;

void setup()
{
	// put your setup code here, to run once:
	pinMode(2, OUTPUT);

	Serial.begin(115200);
	WiFi.begin("FROZENNet", "frozen22");

	while (WiFi.status() != WL_CONNECTED)
		delay(100);
	Serial.println("Connected to WiFi");
	delay(1000);
}

void loop()
{

	//client.connect("192.168.1.247", 2000);
	client.connect("130.208.71.135", 4510);

	if (client.connected())
	{
		Serial.println("Connected to server");
		Player player = Player(client);
		// Server should ask for id ----------------------------------------
		digitalWrite(LED, HIGH);

		player.waitForInput();
		player.readInput();
		if (strcmp(player.getBuffer(), "id\r\n"))
			player.identify_self();
		// Server should have received Id ----------------------------------

		//Server should asks for name --------------------------------------
		player.waitForInput();
		player.readInput(); // 'Welcome to the Maze'

		player.waitForInput();
		player.readInput(); // 'Your name?
		player.answerServer("Your name?\r\n", "Frozen\r\n");
		// Client should have sent kennitala.-------------------------------

		// Server asks for Kennitala ---------------------------------------
		player.waitForInput();
		player.readInput();
		player.answerServer("Your kennitala?\r\n", "101010-1010\r\n");

		player.play();
	}

	client.stop();
	digitalWrite(LED, LOW);
	delay(30000);
}
