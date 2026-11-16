#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

enum enChooseLevel { Easy = 1, Med = 2, Hard = 3, Mix = 4};
enum enChooseOperation { Add = 1, sub = 2, Mul = 3, Div = 4, MixOp = 5};
enum enGamePass { Pass = 1, Fail = 2};

int RandomNumber(int From, int To)
{
	int RandNum = rand() % (To - From + 1) + From;
	return RandNum;
}

struct stResultsOFGame
{
	short NumberOFQuestions = 0;
	short NumberOfRightAns = 0;
	short NumberOfWrongAns = 0;
	enChooseLevel QuestionLevel;
	enChooseOperation OpType;
	enGamePass GamePass;
};



int ReadHowManyQuestion()
{
	int Question = 1;
	do {
		cout << "\n Enter How Many Questions ? (1 to 10) \n";
		cin >> Question;

	} while (Question < 1 || Question > 10);

	return Question;
}


enChooseLevel ReadPlayerLevel()
{
	short Level;
	do
	{
		cout << "\nChoose Level [ Easy : [1] , Med : [2] , Hard  : [3] , Mix : [4] ]? " << endl;
		cin >> Level;
	} while (Level < 1 || Level > 4);

	return enChooseLevel(Level);
}
enChooseOperation ReadPlayerOperation()
{
	short Operation;
	do
	{
		cout << "\n Enter the operation type (Add : [1] , Sub : [2] , Mul : [3] , Div : [4] , Mix : [5] ?\n\n____________\n\n";
		cin >> Operation;
	} while (Operation < 1 || Operation > 5);
	return enChooseOperation(Operation);
}

enChooseOperation GetRandomOperation()
{
	return enChooseOperation(RandomNumber(1, 4));
}
enChooseLevel GetRandomLevel()
{
	return enChooseLevel(RandomNumber(1, 3));
}
string GetOperationType(enChooseOperation ChooseOperation)
{
	if (ChooseOperation == Add)
		return "+";
	else if (ChooseOperation == sub)
		return "-";
	else if (ChooseOperation == Mul)
		return "*";
	else if (ChooseOperation == Div)
		return "/";
	else if (ChooseOperation == MixOp)
		return GetOperationType(GetRandomOperation());
}

int ComputerCalculate(int Number1, int Number2, enChooseOperation OpType )
{
	switch (OpType)
	{
	case Add:
		return Number1 + Number2;
	case sub:
		return Number1 - Number2;
	case Mul:
		return Number1 * Number2;
	case Div:
		return Number1 / Number2;
	}
}

stResultsOFGame FillGameResults(short GameQuestions,short &NumberOfRightQuestion , short &NumberOfWrongQuestion)
{

	stResultsOFGame GameResult;

	GameResult.NumberOFQuestions = GameQuestions;
	GameResult.NumberOfRightAns = NumberOfRightQuestion;
	GameResult.NumberOfWrongAns = NumberOfWrongQuestion;
	return GameResult;
}

//Show All Question in this game .


void GenerateQuestion(int& Number1, int & Number2,enChooseLevel Level) {
	switch (Level) {
	case enChooseLevel::Easy:
		Number1 = RandomNumber(1, 10);
		Number2 = RandomNumber(1, 10);
		break;
	case enChooseLevel::Med:
		Number1 = RandomNumber(10, 50);
		Number2 = RandomNumber(10, 50);

		break;
	case enChooseLevel::Hard:
		Number1 = RandomNumber(50, 100);
		Number2 = RandomNumber(50, 100);
		break;
	case enChooseLevel::Mix:
		GenerateQuestion(Number1, Number2, GetRandomLevel());
	}
}


void PrintResultOfRound(int UserAnswar,int ComputerAnswar)
{
	stResultsOFGame GameResult;
	if (UserAnswar == ComputerAnswar)
	{
		cout << "Right Answar :-)\n\n";
		GameResult.NumberOfRightAns++;
	}
	else
	{
		cout << "Wrong Answar :-(\n\n";
		GameResult.NumberOfWrongAns++;
	}
}

//Print Results of All Questions.



void SetWinnerScreenColor(enGamePass winner)
{
	switch (winner)
	{
	case enGamePass::Pass:
		system("color 2F");
		break;
	case enGamePass::Fail:
		system("color 4F");
		break;
	}
}

string Tabs(short HowManyTabs)
{
	string tab = "";
	for (int i = 0; i < HowManyTabs; i++)
		tab += '\t';

	return tab;
}

void ResetScreen()
{
	system("cls");
	system("color 0F");
}
void ShowGameOverOnScreen() {

	cout << Tabs(2) << "---------------------------------------------------------------\n";
	cout << Tabs(2) << "                     +++ Game Over +++\n";
	cout << Tabs(2) << "---------------------------------------------------------------\n";
}
void ExecuteMathGame()
{
	stResultsOFGame GameResult;
	int Number1= 0, Number2= 0;
	short NumberofQuestions = ReadHowManyQuestion();
	enChooseLevel PlayerLevel = ReadPlayerLevel();
	enChooseOperation OpType = ReadPlayerOperation();
	GameResult.OpType = OpType;
	GameResult.QuestionLevel = PlayerLevel;
	int Answar;
	short NumberOfRightQuestions = 0, NumberOfWrongQuestions = 0;
	
	for (int i = 1;i <= NumberofQuestions;i++)
	{
		GenerateQuestion(Number1, Number2, PlayerLevel);
		cout << "Questoin ["<<i<<"/" << NumberofQuestions << "]" << endl;
		enChooseOperation ActualOp = OpType;

		if (OpType == MixOp)
		{
			ActualOp = GetRandomOperation();
		}


		cout << Number1 << endl << Number2 << " " << GetOperationType(ActualOp) << endl << "_______________________\n";
		cin >> Answar;
		int CorrectAnswer = ComputerCalculate(Number1, Number2, ActualOp);

		PrintResultOfRound(Answar, CorrectAnswer);

		
	}

	FillGameResults(NumberofQuestions, NumberOfRightQuestions, NumberOfWrongQuestions);
}


void StartGame()
{
	char PlayAgain = 'M';
	do
	{
		ResetScreen();
		ExecuteMathGame();
		cout << "Are You Need To Play Again ( Y , N )? \n";
		cin >> PlayAgain;
	} while (PlayAgain == 'Y' || PlayAgain == 'y');
}

int main() {
	srand((unsigned)time(NULL));

	StartGame();
	return 0;
}