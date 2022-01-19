#include "Score.h"



Score::Score(const char* fileName)
{
	scoreFile = new File(fileName);
}


Score::~Score()
{
	if (scoreFile)
	{
		delete scoreFile;
		scoreFile = nullptr;
	}
}
//Tests if the given score is the new high score, if it is write it to the file
void Score::NewHighScore(int newScore)
{
	File highScoreFile("assets/HighScore.txt");

	highScoreFile.Read();

	highScore = std::stoi(highScoreFile.fileLines[0]);


	if (newScore > highScore)
	{
		highScore = newScore;

		highScoreFile.WriteInt(highScore);
	}
}
//Returns the high score from the file
int Score::GetHighScore()
{
	File highScoreFile("assets/HighScore.txt");

	highScoreFile.Read();

	return std::stoi(highScoreFile.fileLines[0]);
}
