#include "./Player.h"
#include <string.h>
#include <Arduino.h>

Player::Player(WiFiClient &client)
{
	this->client = client;
}

bool Player::isPlaying()
{
	return t_playing;
}

uint8_t Player::markExit(uint8_t direction)
{
	uint8_t directionBack = 0;
	if (direction == NORTH)
		directionBack = SOUTH;
	else if (direction == SOUTH)
		directionBack = NORTH;
	else if (direction == EAST)
		directionBack = WEST;
	else if (direction == WEST)
		directionBack = EAST;

	if (directionBack)
		backtrack.push(directionBack);

	return directionBack;
}

bool Player::searchRoom(uint8_t direction)
{
	Serial.println("Room being searched");
	if (direction != NONE)
	{
		sendOutput(parseDirection(direction));
		sendOutput("\r\n");
	}

	uint8_t directionBack = markExit(direction);
	bool exits_parsed = false;
	char *token;
	SmallStack st = SmallStack();

	while (!exits_parsed)
	{
		waitForInput();
		readInput();
		token = strtok(getBuffer(), ":");
		if (token != NULL && !strcmp(token, "Exits"))
		{
			while ((token = strtok(NULL, " ")) != NULL)
			{
				uint8_t direction = parseDirection(token);
				if (direction != directionBack)
					st.push(direction);
			}
			exits_parsed = true;
		}
	}

	while (client.connected())
	{
		waitForInput();
		readInput();
		if (strcmp(getBuffer(), "The treasure is here!") == 0)
		{
			Serial.println("Treasure has been found!");
			return true;
		}
		if (getBuffer()[strlen(getBuffer()) - 1] == '>')
		{
			break; //Leave loop when asked for prompt
		}
	}

	while (!st.isEmpty())
	{
		if (searchRoom(st.pop()))
			return true;
		else
		{
			sendOutput(parseDirection(backtrack.pop()));
			sendOutput("\r\n");
			while (client.connected())
			{
				waitForInput();
				readInput();
				if (getBuffer()[strlen(getBuffer()) - 1] == '>')
				{
					break; //Leave loop when asked for prompt
				}
			}
		}
	}

	return false;
}

uint8_t Player::parseDirection(char *token)
{
	if (strcmp(token, "north") == 0)
		return NORTH;
	if (strcmp(token, "east") == 0)
		return EAST;
	if (strcmp(token, "south") == 0)
		return SOUTH;
	if (strcmp(token, "west") == 0)
		return WEST;

	return 0;
}

const char *Player::parseDirection(uint8_t direction)
{
	if (direction == NORTH)
		return "north";
	if (direction == EAST)
		return "east";
	if (direction == SOUTH)
		return "south";
	if (direction == WEST)
		return "west";

	return "";
}

void Player::play()
{
	searchRoom(0);

	while (client.connected())
	{
		waitForInput();
		readInput();
		if (getBuffer()[strlen(getBuffer()) - 1] == '>')
		{
			break; //Leave loop when asked for prompt
		}
	}

	while (!backtrack.isEmpty())
	{
		Serial.println("Backtracking.........");
		sendOutput(parseDirection(backtrack.pop()));
		sendOutput("\r\n");

		while (client.connected())
		{
			waitForInput();
			readInput();
			if (getBuffer()[strlen(getBuffer()) - 1] == '>')
			{
				break; //Leave loop when asked for prompt
			}
		}
	}

	Serial.println("Game over!");
}

void Player::waitForInput()
{
	while (client.connected() && !(client.available()))
		delay(5);
}

void Player::sendOutput(const char *out)
{
	client.print(out);
	Serial.print(">");
	Serial.print(out);
}

void Player::readInput()
{
	int i = 0;
	int c = '\0';

	while ((c = client.read()) != '\0')
	{
		buffer[i] = c;
		i++;
	}
	buffer[i] = c;
	Serial.println(getBuffer());
}

void Player::answerServer(const char *server_in, const char *answer)
{
	if (strcmp((char *)buffer, server_in))
		sendOutput(answer);
}

void Player::identify_self()
{
	//Swapfor "260794-3459 26A07B94C34D59EF "
	sendOutput("260794-3459 26A07B94C34D59EF ");
	sendOutput(WiFi.macAddress().c_str());
	sendOutput(" 21\r\n");
}

char *Player::getBuffer()
{
	return (char *)buffer;
}
