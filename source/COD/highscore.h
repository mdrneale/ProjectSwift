#ifndef CODHIGHSCORE
#define CODHIGHSCORE

#include <vector>

class Score
{
public:
	Score(float t,int b): time(t), bonus(b) {}
	float time;
	int bonus;
};

class HighScore
{
public:

	HighScore();

	void Load(const char * filename);
	void Save(const char * filename);

	bool IsHighScore(int levelnumber, Score score);

	std::vector<Score> scores;
};

#endif