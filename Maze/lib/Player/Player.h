#include <WiFi.h>
#include <SmallStack.h>

#define NONE 0
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

class Player
{
private:
	bool t_playing = true;
	WiFiClient client;
	uint8_t buffer[256];
	SmallStack backtrack = SmallStack();

public:
	Player(WiFiClient &client);
	void play();
	bool isPlaying();
	bool searchRoom(uint8_t direction); //Should return true after treasure is found
	uint8_t markExit(uint8_t direction);
	void sendOutput(const char *out);
	void readInput();
	void answerServer(const char *server_in, const char *answer);
	void identify_self();
	void waitForInput();
	uint8_t parseDirection(char *token);
	const char *parseDirection(uint8_t direction);
	char *getBuffer();
};
