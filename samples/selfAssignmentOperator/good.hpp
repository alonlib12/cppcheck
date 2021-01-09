#include <algorithm>
class Bug
{
	int length;
	int* array;

public:
	Bug(int l);
	~Bug();
	Bug(const Bug& rhs) = delete;
	Bug& operator=(const Bug& rhs);	
};