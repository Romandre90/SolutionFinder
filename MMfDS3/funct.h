#ifndef _FUNCT_
#define _FUNCT_

#include<iostream>
#include<string>
#include<fstream>
#include <sstream>
#include<algorithm>
#include<vector>
#include<list>
#include<cstdlib>

using namespace std;

int PT[250][30];
int TS[30][251][251];

int a[2];
int b[2];

vector<list<int>> SOL;
vector<list<int>> SOL_DEF;
vector<list<int>> SOL_DEF_TOT;

vector <vector<list<int>>> History;

int hole = 0;
int t = 0;		//tempo t
int Tex = 0;	// Tempo esecuzione 
int n = 0;		// n° JOB
int m = 0;		// n° MACCHINE
int val = 0;	// int in input
int Cmax = 1000000;	// Completation Time
int Cmax_TOT = 1000000;
int C[30];		// Tempi di completamento sulle macchine
int CB[30];		// Tempi migliori di completamento sulle macchine
string line;	// string in input
string file;

void resetSOL()
{
	Cmax = 1000000;
	SOL.clear();
	SOL_DEF.clear();
	list<int> a;
	for (int i = 0; i < m; i++)
	{
		SOL.push_back(a);
		SOL_DEF.push_back(a);
	}
	for (int i = 0; i < n; i++)
	{
		SOL[rand() % m].push_back(i);
	}
}

const vector<string> explode(const string& s, const char& c)
{
	string buff{ "" };
	vector<string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
		if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

string intToString(int value)
{
	stringstream ss;
	ss << value;
	return ss.str();
}

int initMatrix(int job, int mac)
{
	resetSOL();
	//inizializzazione matrice PROCESSING TIME
	for (int i = 0; i < job; i++)
	{
		for (int j = 0; j < mac; j++)
		{
			PT[i][j] = 0;

		}
	}
	list<int> a;
	//inizializzazione matrice PROCESSING TIME e SOLUZIONE
	for (int i = 0; i < mac; i++)
	{
		SOL_DEF_TOT.push_back(a);
		for (int j = 0; j < job; j++)
		{
			for (int k = 0; k < job; k++)
			{
				TS[i][j][k] = 0;
			}
		}

	}

	return 1;
}

bool openfile(string file)
{
	int val = 0;
	string line;
	ifstream input;
	input.open("input/" + file);
	
	if (!input.is_open())
	{
		cout << "errore apertura file" << endl;
		return false;
	}
	//FILE APERTO CORRETTAMENTE
	input >> n;
	input >> m;
	// inizializzo le MATRICI
	if (!initMatrix(n, m)){ cout << "errore inizializzazione" << endl; return false; }
	//salto i valori inutili
	input >> val;
	input >> val;
	//riempio la matrice dei Processing Time
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			input >> val;
			input >> PT[i][j];
		}
	}
	//controllo il fine matrice
	input >> line;
	if (line != "SSD")
	{
		cout << "errore linea SSD non trovata" << endl;
		return false;
	}
	//riempimento matrice Set-Up time
	for (int i = 0; i < m; i++)
	{
		input >> line;
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				input >> TS[i][j][k];
			}
		}
	}
	input.close();
	return true;
	//chiudo il file in lettura
}

bool output(string time)
{
	ofstream outputSol;
	ofstream output;
	if (n < 40)
	{
		//istante small
		output.open("output/small/output_small.csv", ios::app); //nome istanza;t;Tex;Cmax
		outputSol.open("output/small/" + time + "/" + file + ".sol");
	}
	else
	{
		//istanze large
		output.open("output/large/output_large.csv", ios::app); //nome istanza;t;Tex;Cmax
		outputSol.open("output/large/" + time + "/" + file + ".sol");
	}
	

	for (int i = 0; i < SOL_DEF_TOT.size(); i++)
	{
		for (list<int>::iterator j = SOL_DEF_TOT[i].begin(); j != SOL_DEF_TOT[i].end(); ++j)
		{
			if (*j != -1)
			{
				outputSol << *j;
				outputSol << " ";
			}
		}
		outputSol << '\n';
	}
	output << file << ";" << t << ";" << Tex << ";" << Cmax_TOT << '\n';	//nome istanza;t;Tex;Cmax
	output.close();
	outputSol.close();
	return true;
}

bool findCmax()
{
	int Cx = 0;
	int Rx = 10000;
	int MAX = 0;
	int min = 0;
	for (int i=0; i < m; i++)
	{
		if (Cx < C[i])
			Cx = C[i];
	}
	if (Cx < Cmax)
	{
		Cmax = Cx;
		hole = 0;
		for (int i = 0; i < m; i++){ CB[i] = C[i]; }
		SOL_DEF = SOL;
		if (Cmax < Cmax_TOT)
		{
			Cmax_TOT = Cmax;
			SOL_DEF_TOT = SOL_DEF;
		}
	}
	else
	{
		hole++;
		SOL = SOL_DEF;
		for (int i = 0; i < m; i++){ C[i] = CB[i]; }
	}
	return false;
}

void calCtot()
{
	int prev = -1;
	int x = 0;
	int Cx = 0;
	for (unsigned i = 0; i < SOL.size(); i++)
	{
		for (list<int>::iterator j = SOL[i].begin(); j != SOL[i].end(); ++j)
		{
			if (*j != -1)
			{
				if (prev == -1)
				{
					x = PT[*j][i];
					prev = *j;
				}
				else
				{
					x += TS[i][prev][*j] + PT[*j][i];
					prev = *j;
				}
			}
		}
		prev = -1;
		C[i] = x ;
		CB[i] = x;
	}
	findCmax();
}

void swap_toki() //Swappiamo per muoverci verso il minimo
{

	list<int>::iterator j;
	a[0] = rand() % SOL.size();
	while (SOL[a[0]].empty()){
		a[0] = rand() % SOL.size();
	}
	a[1] = rand() % SOL.size();

	b[0] = rand() % SOL[a[0]].size();

	if (!SOL[a[1]].empty())
		b[1] = rand() % SOL[a[1]].size();
	else
		b[1] = 0;

	int del, p, n, x = 0;

	//erase
	if (SOL[a[0]].size() == 1)				//aggiornamento C della macchina se unico
	{
		del = SOL[a[0]].back();
		SOL[a[0]].pop_back();
		C[a[0]] = 0;												
	}												
	else if (b[0] == 0)						//aggiornamento C della macchina se rimosso all'inizio
	{
		del = SOL[a[0]].front();
		SOL[a[0]].pop_front();
		n = SOL[a[0]].front();
		C[a[0]] = C[a[0]] - PT[del][a[0]] - TS[a[0]][del][n];		
	}
	else if (b[0] == SOL[a[0]].size()-1)	//aggiornamento C della macchina se rimosso in fondo
	{
		del = SOL[a[0]].back();
		SOL[a[0]].pop_back();
		p = SOL[a[0]].back();
		C[a[0]] = C[a[0]] - PT[del][a[0]] - TS[a[0]][p][del];
	}
	else									//aggiornamento C della macchina se rimosso in centro
	{
		j = SOL[a[0]].begin();
		advance(j, b[0]);
		del = *j;
		p = *--j++;
		n = *j--;
		C[a[0]] = C[a[0]] - TS[a[0]][p][*j] - PT[*j][a[0]] - TS[a[0]][*j][n] + TS[a[0]][p][n];
		SOL[a[0]].erase(j);
	}
	//insert
	if (SOL[a[1]].size() == 0)				//aggiunto in una macchina vuota
	{
		C[a[1]] = PT[del][a[1]];
		SOL[a[1]].push_back(del);
	}
	else if (b[1] == 0)						//aggiunto in testa
	{
		x = SOL[a[1]].front();
		C[a[1]] = C[a[1]] + PT[del][a[1]] + TS[a[1]][del][x];
		SOL[a[1]].push_front(del);
	}
	else if (b[1] >= SOL[a[1]].size()-1)	// aggiunto in coda
	{
		x = SOL[a[1]].back();
		C[a[1]] = C[a[1]] + PT[del][a[1]] + TS[a[1]][x][del];
		SOL[a[1]].push_back(del);
	}
	else									//aggiornamento C della macchina se rimosso in centro
	{
		j = SOL[a[1]].begin();
		advance(j, b[1]);
		p = *--j;
		n = *++j;
		C[a[1]] = C[a[1]] - TS[a[1]][p][n] + PT[del][a[1]] + TS[a[1]][del][n] + TS[a[1]][p][del];
		SOL[a[1]].insert(j, del);
	}
}

int deep_swap(int num) //Swappiamo per muoverci verso il minimo
{
	for (int i = 0; i < num; i++)
	{
		swap_toki(); 
	}
	if (num > 2)
	{
		Cmax = 100000;
		calCtot();
	}
	findCmax();
	return -1;
}

#endif