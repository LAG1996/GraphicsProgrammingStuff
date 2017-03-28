#include<iostream>
using namespace std;


char ** Allocate_PointerOfPointers(int w, int h)
{
	char * c = new char[h];
	char ** cp = new char*[w];

	for (int i = 0; i < w; i++)
		cp[i] = c;

	return cp;
}

void Fill_PointerOfPointers(char ** C, int w, int h, char c)
{
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			C[i][j] = c;
}

int main()
{

	char ** C = Allocate_PointerOfPointers(4, 5);
	Fill_PointerOfPointers(C, 4, 5, 'a');

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
			cout << C[i][j];

		cout << "\n";
	}
		
	return 0;
}