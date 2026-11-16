#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;



class clsCalculator
{
private:
    
    int _Result = 0;
    int _NumberCalculating;
    int _Number;

    enum enCalculations
    {
        eClear = 0,
        eAdd = 1,
        eSubtract = 2,
        eMultiply = 3,
        eDivide = 4,
        ePrintFinalResults = 5
    };

    int DividedByZero(int Number)
    {
        if (Number == 0)
        {
            Number += 1;
            return Number;
        }
        else
        {
            return Number;
        }
    }

    string PrintRelates(int NumberCalculate)
    {

        if (NumberCalculate == enCalculations::eAdd)
        {
            return "Add";
        }
        else if (NumberCalculate == enCalculations::eClear)
        {
            return "Clear";
        }
        else if (NumberCalculate == enCalculations::eSubtract)
        {
            return "Subtracting";
        }
        else if (NumberCalculate == enCalculations::eMultiply)
        {
            return "Multipling";
        }
        else if (NumberCalculate == enCalculations::eDivide)
        {
            return "Dividing";
        }
    }

public:


    int Clear()
    {
        _Result = 0;
        _Number = 0;
        _NumberCalculating = 0;
        return _Result;
    }
    int Add(int Number)
    {
        _Number = Number;
        _Result += Number;
        _NumberCalculating = 1;

        return _Result;

    }
    int Subtract(int Number)
    {
        _Result -= Number;
        _Number = Number;
        _NumberCalculating = 2;

        return _Result;

    }
    int Multiply(int Number)
    {
        _Result *= Number;
        _NumberCalculating = 3;
        _Number = Number;

        return _Result;

    }
    int Divide(int Number)
    {
        DividedByZero(Number);
        _Result /= Number;
        _NumberCalculating = 4;
        _Number = Number;

        return _Result;

    }
    void PrintFinalResult()
    {
        cout << "Final Result: " << _Result << endl;
    }
    void PrintResult()
    {
        cout << "Result After " << PrintRelates(_NumberCalculating) << " " << _Number << " is: " << _Result << endl;
    }
};

int main()
{
    clsCalculator Calculator1;
    Calculator1.Add(10);
    Calculator1.PrintResult();

    Calculator1.Multiply(2);
    Calculator1.PrintResult();

    system("pause>0");
    return 0;
}
