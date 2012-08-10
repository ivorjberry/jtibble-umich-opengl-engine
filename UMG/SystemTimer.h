#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class SystemTimer 
{
	clock_t start;
	
public:
	void startTimer(void)
	{
		start = clock();
	}
	float elapsedTime(void)
	{
		return ((float)(clock()-start))/(float)CLOCKS_PER_SEC;
	}
};
#endif