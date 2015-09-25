#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<thread>

#include"funct.h"

using namespace std;

int main(int argc, char* argv[])
{
	int start_G = clock();
	file = argv[1];
	t = atoi(argv[2]);
	//apro il file per i dati
	openfile(file);
	//elaborazione soluzione
	int op=0;
	int op1 = 0;
	int stop = n*n/2;
	calCtot();
	int start_s = clock();
	while ((clock() - start_s) / double(CLOCKS_PER_SEC) * 1000 < n*m*t/2)
	{
		while (hole<stop)
		{
			op++;
			deep_swap(1);
		}
		hole = 0;
		deep_swap(5);
	}
	
	//creo i file per la consegna
	vector<string> v{ explode(file, '.') };
	file = v[v.size() - 2];
	Tex = clock() - start_G;
	output(argv[2]);
	return 0;
}
