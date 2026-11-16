#include <iostream>
using namespace std;

int main()
{
    int arr[100] = { 1,2,3,4,5,6,7};
    int sum = 0;
    
    double avarage = 0;

    for (int i = 1; i <=7; i++)
    {
        sum += arr[i];
    }

    avarage = sum / 7;
    
    cout << "sum = " << sum;
    cout << "\navarege = " << avarage;
    cout << "\nlength = " << 7;
}
