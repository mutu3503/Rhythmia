#pragma once

enum class JUDGMENT
{
	PERFECT,
	GREAT,
	GOOD,
	MISS,
};

struct PlayResult
{
	int score = 0;
	int perfectCount = 0;
	int greatCount = 0;
	int goodCount = 0;
	int missCount = 0;
};