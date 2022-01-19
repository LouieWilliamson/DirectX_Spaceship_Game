#pragma once
#include "File.h"

class Score
{
public:
	Score(const char* fileName);
	~Score();
	int GetScore() { return value; }
	void NewHighScore(int newScore);
	int GetHighScore();
	void AddToScore(int scoreUpdate) { value += scoreUpdate; }
private:
	int value = 0;
	int highScore = 0;
	File* scoreFile;
};

