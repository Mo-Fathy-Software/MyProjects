#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>

struct stGameResults {
	int GameRounds = 0;
	int PlayerWinTimes = 0;
	int ComputerWinTimes = 0;
	int DrawTimes = 0;
};
	
using namespace std;
enum enStonePaperScissors {Stone = 1,Paper = 2,Scissors = 3};
int RandomNumber(int From, int To)
{
	int RandNum = rand() % (To - From + 1) + From;
	return RandNum;
}
int ReadPositiveNumber(string Message)
{
	int Number = 0;
	do
	{
		cout << Message << endl;
		cin >> Number;
	} while (Number <= 0 || Number > 10);
	return Number;
}

int ReadPositiveNumberFrom1To3(string Message)
{
	int Number = 0;
	do
	{
		cout << Message << endl;
		cin >> Number;
	} while (Number <= 0 || Number > 3);
	return Number;
}
string ReadAnswarOnly(string Message)
{
	string Answar = "";
	do
	{
		cout << Message << endl;
		cin >> Answar;
	} while (Answar != "y" && Answar != "Y" && Answar != "n" &&  Answar != "N");
	return Answar;
}
int GameRounds()
{
	int Number = ReadPositiveNumber("How Many Rounds (1,10): ");
	return Number;
}

int NumberOfChoice()
{
	int NumberOfChoice = ReadPositiveNumberFrom1To3("Your Choice : [1]:Stone, [2]:Paper, [3]:Scissors ?\n");
	return NumberOfChoice;
}

string PlayerChoice()
{
	int NumberChoiceOnce = NumberOfChoice();
	if (NumberChoiceOnce == enStonePaperScissors::Stone)
		return "Stone";
	else if (NumberChoiceOnce == enStonePaperScissors::Paper)
		return "Paper";
	else
		return "Scissors";

}

string ComputerChoice()
{
	int randnum = RandomNumber(1, 3);
	if (randnum == 1)
		return "Stone";
	else if (randnum == 2)
		return "Paper";
	else
		return "Scissors";

}

void RoundWinner(string Player ,string Computer,stGameResults &GameResults)
{
	if (Player == Computer)
	{
		cout << "[No Winner]\n";
		GameResults.DrawTimes++; // زيادة عداد التعادل
	}
	else if ((Player == "Stone" && Computer == "Scissors") ||
		(Player == "Paper" && Computer == "Stone") ||
		(Player == "Scissors" && Computer == "Paper"))
	{
		cout << "[Player 1 Winner]\n";
		GameResults.PlayerWinTimes++; // زيادة عداد فوز اللاعب
	}
	else
	{
		cout << "[Computer Winner]\n";
		GameResults.ComputerWinTimes++; // زيادة عداد فوز الكمبيوتر
	}
}


void PrintResults(string Player,string Computer,stGameResults &GameResults)
{
	cout << "Player 1 Choice : " << Player << endl;
	cout << "Computer Choice : " << Computer << endl;
	cout << "Round Winner : ";
	RoundWinner(Player,Computer,GameResults);

}

void ShowGameOverScreen(stGameResults GameResults)
{
	cout << "__________________________________________________\n";
	cout << "                 G A M E   O V E R                \n";
	cout << "__________________________________________________\n";
	cout << "Rounds Played        : " << GameResults.GameRounds << endl;
	cout << "Player 1 Won Times   : " << GameResults.PlayerWinTimes << endl;
	cout << "Computer Won Times   : " << GameResults.ComputerWinTimes << endl;
	cout << "__________________________________________________\n\n\n";
}

void StartGame()
{
	string Answar;
	string Player, Computer;
	do
	{
		stGameResults GameResults;

		int NumberOfAllRounds = GameRounds();
		GameResults.GameRounds = NumberOfAllRounds;
		for (int i = 1;i <= NumberOfAllRounds;i++)
		{
			Player = PlayerChoice();
			Computer = ComputerChoice();
			cout << "\n\nRound [" << i << "] begins:\n";
			cout << "______________Round [" << i << "]_____________\n\n";
			PrintResults(Player, Computer, GameResults);
			cout << "\n---------------\n\n";

		}
		ShowGameOverScreen(GameResults);
		
		Answar = ReadAnswarOnly("Are You Need To Play Again [Y/N] : \n");

	} while (Answar == "y" || Answar == "Y");
}

int main()
{
	srand((unsigned)time(NULL));
	StartGame();
}