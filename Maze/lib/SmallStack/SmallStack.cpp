#include "./SmallStack.h"
#include <algorithm>
#include <iterator>

SmallStack::SmallStack()
{
	m_size = 32;
	m_reduce_margin = 8;
	m_stack = new uint8_t[m_size];
	m_index = 0;
}

SmallStack::~SmallStack()
{
	delete[] m_stack;
}

bool SmallStack::isEmpty()
{
	return m_empty;
}

void SmallStack::enlarge()
{
	if (m_size == SmallStack::MAX_SIZE)
	{
		throw("SmallStack can't get any bigger");
	}

	m_size *= 2;
	m_reduce_margin *= 2;

	uint8_t *oldStack = m_stack;
	m_stack = new uint8_t[m_size];

	std::copy(oldStack, oldStack + m_index, m_stack);
	delete oldStack;
}

void SmallStack::reduce()
{
	if (m_size == SmallStack::MIN_SIZE)
		return;

	m_size /= 2;
	m_reduce_margin /= 2;

	uint8_t *oldStack = m_stack;
	m_stack = new uint8_t[m_size];

	std::copy(oldStack, oldStack + m_index + 1, m_stack);
	delete oldStack;
}

void SmallStack::push(uint8_t value)
{
	m_stack[m_index] = value;
	m_empty = false;

	m_index++;
	if (m_index == m_size)
		enlarge();
}

uint8_t SmallStack::pop()
{
	m_index--;
	if (m_index == 0)
		m_empty = true;

	if (m_index == m_reduce_margin)
		reduce();

	return m_stack[m_index];
}