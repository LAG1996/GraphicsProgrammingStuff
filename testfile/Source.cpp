#include<iostream>
#include<fstream>
using namespace std;

int main()
{
	ifstream inFile;
	inFile.open("RectLocas.txt");

	int x;
	int y;
	int z;
	int count = 0;

	if (inFile.good())
		cout << "We good" << endl;
	while (inFile >> x)
	{
		inFile >> y;
		inFile >> z;
		
		cout << "Count ["<<count<<"]:"<< x << " " << y << " " << z <<  endl;

		count++;

	}

	inFile.close();

	return 0;
}