#include <algorithm>
#include "bad.hpp"

Bug::Bug(int l) : length(l), array(new int[length])
{
}

Bug::~Bug()
{
	delete[] array;
}

Bug& Bug::operator=(const Bug& rhs)
{
	if (this == &rhs) // Oh no, inverse condition
	{
		length = rhs.length;
		delete[] array;
		array = new int[length];
		std::copy_n(rhs.array, length, array);
	}
	return *this;
}