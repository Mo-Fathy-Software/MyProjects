#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

enum enQuestionsLevel {EasyLevel = 1, MidLevel = 2 , HardLevel =3 , Mix = 4};

enum enOperationType { Add = 1 ,Sub = 2, Mult = 3,Div = 4 ,MixOp=5};


int RandomNumber(int From, int To)
{
	return  rand() % (To - From + 1) + From;
}

struct stQuestion
{
	int Number1 = 0;
	int Number2 = 0;
	enOperationType OperationType;
	enQuestionsLevel QuestionLevel;
	int CorrectAnswar = 0;
	int PlayerAnswar = 0;
	bool AnswarResult = false;
};

struct stQuizz
{
	stQuestion QuestionList[100];
	short NumberOfQuestions;
	enQuestionsLevel QuestionLevel;
	enOperationType OpType;
	short NumberOfWrongAnswars = 0;
	short NumberOfRightAnswars = 0;
	bool isPass = false;
};

short ReadHowManyQuesions()
{
	short NumberOfQuestion;
	do
	{
		cout << "How Many Question do you want to Answar?\n";
		cin >> NumberOfQuestion;
	} while (NumberOfQuestion < 1 || NumberOfQuestion >10);
	return NumberOfQuestion;
}
enQuestionsLevel ReadQuestionsLevel()
{
	short QuestionLevel;
	do
	{
		cout << "Enter Question level [1] Easy , [2] Mid , [3] Hard ,[4] Mix ?";
		cin >> QuestionLevel;
	} while (QuestionLevel < 1 || QuestionLevel >4);
	return (enQuestionsLevel)QuestionLevel;
}

enOperationType ReadOperationType()
{
	short NumberOfOpType = 0;
	do
	{
		cout << "Please enter the Operation type [1] Add ,[2] Sub,[3] Mult,[4] Div,[5] Mix\n";
		cin >> NumberOfOpType;

	} while (NumberOfOpType < 1 || NumberOfOpType >5);
	return (enOperationType)NumberOfOpType;
}

enQuestionsLevel GenerateRandomLevel()
{
	return (enQuestionsLevel)RandomNumber(1, 3);
}

enOperationType GenerateOpType()
{
	return (enOperationType) RandomNumber(1, 4);
}

int SimpleCalculator(short  Number1, short  Number2, enOperationType OperationType)
{
	switch (OperationType)
	{
	case enOperationType::Add:
		return Number1 + Number2;
	case enOperationType::Sub:
		return Number1 - Number2;
	case enOperationType::Mult:
		return Number1 * Number2;
	case enOperationType::Div:
		return Number1 / Number2;
	default:
		return Number1 + Number2;
	}
}

stQuestion GenerateQuestion(enQuestionsLevel QuestionLevel, enOperationType OpType)
{
	stQuestion Question;
	if (QuestionLevel == enQuestionsLevel::Mix)
	{
		QuestionLevel = GenerateRandomLevel();
	}
	if (OpType == enOperationType::MixOp)
	{
		OpType = GenerateOpType();
	}

	Question.OperationType = OpType;
	if (QuestionLevel == enQuestionsLevel::EasyLevel)
	{
		Question.Number1 = RandomNumber(1, 10);
		Question.Number2 = RandomNumber(1, 10);
		Question.CorrectAnswar = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
		Question.QuestionLevel = QuestionLevel;
		return Question;

	}
	else if (QuestionLevel == enQuestionsLevel::MidLevel)
	{

		Question.Number1 = RandomNumber(10, 50);
		Question.Number2 = RandomNumber(10, 50);
		Question.CorrectAnswar = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
		Question.QuestionLevel = QuestionLevel;
		return Question;

	}

	else if (QuestionLevel == enQuestionsLevel::HardLevel)
	{

		Question.Number1 = RandomNumber(50, 100);
		Question.Number2 = RandomNumber(50, 100);
		Question.CorrectAnswar = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
		Question.QuestionLevel = QuestionLevel;
		return Question;
	}
}

void GenerateQuestions(stQuizz& Quizz)
{
	for (int Question = 0;Question < Quizz.NumberOfQuestions;Question++)
	{
		Quizz.QuestionList[Question] = GenerateQuestion(Quizz.QuestionLevel, Quizz.OpType);
	}
}

string GetOpTypeSymbol(enOperationType OpType)
{
	switch (OpType)
	{
	case enOperationType::Add:
		return "+";
	case enOperationType::Sub:
		return "-";
	case enOperationType::Mult:
		return "x";
	case enOperationType::Div:
		return "/";
	default:
		return "Mix";
	}
}

void PrintTheQuestion(stQuizz & Quizz, short Question)
{
	cout << "\n";
	cout << "Question [" << Question + 1 << "/" << Quizz.NumberOfQuestions << "] \n\n";
	cout << Quizz.QuestionList[Question].Number1 << endl;
	cout << Quizz.QuestionList[Question].Number2 << " ";
	cout << GetOpTypeSymbol(Quizz.QuestionList[Question].OperationType);
	cout << "\n_____________" << endl;
}

int ReadQuestionAns()
{
	int Answar = 0;
	cin >> Answar;
	return Answar;
}

void CorrectTheQuestionAnswars(stQuizz & Quizz, short Question)
{
	if (Quizz.QuestionList[Question].PlayerAnswar != Quizz.QuestionList[Question].CorrectAnswar)
	{
		Quizz.QuestionList[Question].AnswarResult = false;
		Quizz.NumberOfWrongAnswars++;

		cout << "Wrong Answar -(\n";
		cout << "The Right Ans Is : " << Quizz.QuestionList[Question].CorrectAnswar << endl;
	}
	else
	{
		Quizz.QuestionList[Question].AnswarResult = true;
		Quizz.NumberOfRightAnswars++;
		cout << "Right Answars -)\n";

	}
	cout << "\n";
}

void AskAndCorrectQuestionsListAnswars(stQuizz & Quizz)
{
	for (int Question = 0; Question < Quizz.NumberOfQuestions;Question++)
	{
		PrintTheQuestion(Quizz,Question);
		Quizz.QuestionList[Question].PlayerAnswar = ReadQuestionAns();
		CorrectTheQuestionAnswars(Quizz, Question);

	}
	Quizz.isPass = (Quizz.NumberOfRightAnswars >= Quizz.NumberOfWrongAnswars);
}

string GetFinalResultsText(bool Pass)
{
	if (Pass)
		return "Pass :-)";
	else
		return "Failed :-(";
}

string GetQuestionLevelText(enQuestionsLevel QuestionLevel)
{
	string arrQuestionLevelText[4] = { "Easy","Mid","Hard","Mix" };
	return arrQuestionLevelText[QuestionLevel - 1];
}

void PrintQuizzResults(stQuizz Quizz)
{
	cout << "\n__________________________\n\n";
	cout << "Final Results is " << GetFinalResultsText(Quizz.isPass);
	cout << "\n__________________________\n\n";

	cout << "Number Of Questions : " << Quizz.NumberOfQuestions << endl;
	cout << "Question Level      : " << GetQuestionLevelText(Quizz.QuestionLevel) << endl;
	cout << "OpType              : " << GetOpTypeSymbol(Quizz.OpType) << endl;
	cout << "Number Of Right Questions: " << Quizz.NumberOfRightAnswars << endl;
	cout << "Number Of Wrong Questions: " << Quizz.NumberOfWrongAnswars << endl;
	cout << "_____________________________\n";
}

void PlayMathGame()
{
	stQuizz Quizz;
	Quizz.NumberOfQuestions = ReadHowManyQuesions();

	Quizz.QuestionLevel = ReadQuestionsLevel();

	Quizz.OpType = ReadOperationType();

	GenerateQuestions(Quizz);
	
	AskAndCorrectQuestionsListAnswars(Quizz);

	PrintQuizzResults(Quizz);


}

void ResetScreen()
{
	system("cls");
	system("color 0");
}

void StartGame()
{
	char PlayGame = 'D';

	do
	{
		ResetScreen();
		PlayMathGame();
		
		cout << "\nDo You Wanna Play Again (Y/N).";
		cin >> PlayGame;

	} while (PlayGame == 'Y' || PlayGame == 'y');

}

int main()
{
	srand((unsigned)time(NULL));
	StartGame();
	return 0;
}
