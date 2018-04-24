#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <map>

using namespace std;
//////////////////////////////////////////
const int l_grup = 20;
const int dl_alf = 36;

map<char, int> zliczacz;

string alf = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string klucz = "10Z210";
string grupy[l_grup];

double S[l_grup];

int licznik = 0;
int bufor;
int p_klucz[dl_alf];
//////////////////////////////////////////
string konwerter(string s);

char szyfr(char &c, int p);

void znajdz_klucz();
void czestowliwosc();
void metoda_ic(string s);
void mapuj(string s);
//////////////////////////////////////////

int main()
{
	string tekst = "";

	ifstream WE;
	WE.open("we.txt");
	if (WE.good() == true)
	{
		tekst.assign((istreambuf_iterator<char>(WE)), istreambuf_iterator<char>());
		WE.close();
	}

	for (int i = 0; i < tekst.size(); i++)
	{
		if ((int)tekst[i] >= (int)'a' && (int)tekst[i] <= (int)'z')
		{ // tylko du¿e litery
			tekst[i] = (char)tekst[i] - ((int)'a' - (int)'A');
		}
	}

	tekst = konwerter(tekst);

	ofstream WY;
	WY.open("wy.txt");
	if (WY.good() == true)
	{
		WY << tekst;
		WY.close();
	}

	system("pause");
	return 0;
}
//////////////////////////////////////////

void znajdz_klucz()
{
	for (int i = 0; i < klucz.length(); i++)
	{
		size_t pozycja = alf.find(klucz[i]);
		if (pozycja != string::npos)
		{
			p_klucz[i] = pozycja;
		}
	}
}

char szyfr(char &c, int p)
{
	bufor++;

	if (p > dl_alf)
	{
		p = p % dl_alf;
	}

	size_t pozycja = alf.find(c);
	if (pozycja != string::npos)
	{ // szyfr cezara
		zliczacz[c]++;
		licznik++;

		c = alf[(dl_alf + pozycja + p) % dl_alf];
	}
	else bufor--;

	return (char)c;
}

string konwerter(string s)
{
	znajdz_klucz();

	bufor = 0;
	for (int i = 0; i < s.size(); i++)
	{
		s[i] = szyfr(s[i], p_klucz[bufor % klucz.length()]);
	}

	//czestowliwosc();

	metoda_ic(s);
	return s;
}


void mapuj(string s)
{
	zliczacz.clear();
	for (int z = 0; z < s.length(); z++)
	{
		zliczacz[s[z]]++;
	}
}

void metoda_ic(string s)
{
	ofstream IC;
	string tekstj = s;

	IC.open("f.txt");
	if (IC.good() == true)
	{
		for (int i = 1; i <= 20; i++)
		{
			for (int j = 0; j < i; j++)
			{ // zerowanie tablic
				grupy[j] = "";
				S[j] = 0;
			}

			int k = 0;
			for (int j = 0; j < s.length(); j++)
			{ // podzial tekstu na grupy (pomijaj¹c znaki specjalne)
				size_t pozycja = alf.find(s[j]);
				if (pozycja != string::npos)
				{
					grupy[k%i] = grupy[k%i] + s[j];
					k++;
				}
			}

			for (int j = 0; j < i; j++)
			{ // zmapowanie grup, obliczenie kolejnych sum IC
				mapuj(grupy[j]);

				for (auto x : zliczacz)
				{
					S[j] += ((double)(x.second * (x.second - 1)) / (double)(grupy[j].length() * (grupy[j].length() - 1)));
				}
			}

			double suma = 0;
			for (int j = 0; j < i; j++)
			{ // obliczenie sumy IC
				suma += (double)S[j] / (double)i;
			}
			IC << "IC" << i << ": " << suma << endl;

		}

		for (int j = 0; j < 6; j++)
		{ // zerowanie tablic
			grupy[j] = "";
			S[j] = 0;
		}


		int k = 0;
		for (int j = 0; j < tekstj.length(); j++)
		{ // podzial tekstu na grupy (pomijaj¹c znaki specjalne)
			size_t pozycja = alf.find(tekstj[j]);
			if (pozycja != string::npos)
			{
				grupy[k % 6] = grupy[k % 6] + tekstj[j];
				k++;
			}
		}

		for (int j = 0; j < 6; j++)
		{ // zmapowanie grup, obliczenie kolejnych sum IC
			mapuj(grupy[j]);

			for (auto x : zliczacz)
			{
				S[j] += ((double)(x.second * (x.second - 1)) / (double)(grupy[j].length() * (grupy[j].length() - 1)));
			}
			//S[j] = S[j] * dl_alf;
		}

		double suma = 0;
		for (int j = 0; j < 6; j++)
		{ // obliczenie sumy IC
			suma += (double)S[j] / (double)6;
		}
		IC << "IC wzorcowy: " << suma << endl;

		IC.close();
	}
}

void czestowliwosc()
{
	ofstream F;

	F.open("zliczacz.txt");
	if (F.good() == true)
	{
		for (auto x : zliczacz)
		{
			F << x.first << " " << (double)x.second / (double)licznik * 100 << endl;
		}

		F.close();
	}
}
