#ifndef __STOPWATCH__
#define __STOPWATCH__

#include "GameTime.h"

typedef void(*pFunction)(void);

class StopWatch
{
public:
	StopWatch();
	~StopWatch();

	bool isFinish();
	bool isTimeLoop(float time);
	bool isStopWatch(float time);
	void timeLoopAction(float milisecond, pFunction action);
	void restart();

private:
	float	_stopWatch;
	bool	_isStart;
	bool	_isFinish;
};

#endif // !__STOPWATCH__
