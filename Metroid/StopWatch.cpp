﻿#include "StopWatch.h"

StopWatch::StopWatch()
{
	_stopWatch = 0.0f;
	_isStart = false;
	_isFinish = false;
}

StopWatch::~StopWatch()
{
}

bool StopWatch::isFinish()
{
	return this->_isFinish;
}

// đơn vị milisecond
bool StopWatch::isTimeLoop(float time)
{
	float _totalgametime = GameTime::getInstance()->getTotalGameTime();
	if (_isStart == false)
	{
		_stopWatch = time + _totalgametime;
		_isStart = true;
		return false;
	}

	float delta = _stopWatch - _totalgametime;
	if (delta <= 0)
	{
		_stopWatch = time + delta + _totalgametime;
		return true;
	}

	return false;
}

bool StopWatch::isStopWatch(float time)
{
	if (_isFinish == true)
		return false;

	float _totalgametime = GameTime::getInstance()->getTotalGameTime();
	if (_isStart == false)
	{
		_stopWatch = time + _totalgametime;
		_isStart = true;
		return false;
	}

	float delta = _stopWatch - _totalgametime;
	if (delta <= 0)
	{
		_isFinish = true;
		return true;
	}

	return false;
}

void StopWatch::restart()
{
	_isStart = false;
	_isFinish = false;
}

void StopWatch::timeLoopAction(float milisecond, pFunction action)
{
	if (isTimeLoop(milisecond))
	{
		(*action)();
	}
}
