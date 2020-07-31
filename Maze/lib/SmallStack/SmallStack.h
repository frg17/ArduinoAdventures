#include <cstdint>

class SmallStack
{
private:
	uint8_t *m_stack; //Main stack
	uint32_t m_index; //Stack index
	uint32_t m_size;
	uint32_t m_reduce_margin;
	static const uint32_t MAX_SIZE = 0xFFFFFFFF; //2^32
	static const uint32_t MIN_SIZE = 32;
	void enlarge();
	void reduce();
	bool m_empty = true;

public:
	bool isEmpty();
	SmallStack();
	~SmallStack();
	uint8_t pop();
	void push(uint8_t value);
};