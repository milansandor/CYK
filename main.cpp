/*
Fordítása és futtatása következő éppen lehetséges:
	g++ -o cyk main.cpp -std=c++11
	./cyk g1.txt e1.txt
*/


#include<iostream>
#include<string>
#include<fstream>
#include<set>
#include<algorithm>
#include<cctype>
using namespace std;

#define MAX 100

string gram[MAX][MAX];  // nyelv eltárolására
int np;					

//concatenálja egyedi nem terminalisokat
string concat(string a, string b)
{
	string r = a;
	for (int i = 0; i < b.length(); i++)
		if (r.find(b[i]) > r.length())
			r += b[i];
	return r;
}


//osszes kombinaciojat elkesziti a variacioknak
string gen_comb(string a, string b)
{
	string pri, re = "";
	for (int i = 0; i < a.length(); i++)
		for (int j = 0; j < b.length(); j++)
		{
			pri = "";
			pri = pri + a[i] + b[j];

			// keresi hogy a generált production létre lehet hozni változókból
			for (int j = 0; j < np; j++)
			{
				for (int k = 1; gram[j][k] != ""; k++)
				{
					if (gram[j][k] == pri)
					{
						re = concat(re, gram[j][0]);
					}
				}
			}
		}
	return re;
}

int main(int argc, char** argv)
{
	int i, pt, j, l, k;
	string a, str, r, pr, start = "S";
	char buff[100];
	np = 0;
	bool prod_found=false;

	if (argv[1] == nullptr || argv[2] == nullptr) {
		cout << "Nincs input megadva";
		return 1;
	}		
	
	
	ifstream ifile(argv[1]);

	while (ifile.getline(buff, 99))
	{
		a = buff;
		if (a.find("SZABALYOK:") < a.length())
		{	
			prod_found=true;
			break;
		}
	}
	
	if(!prod_found)
	{
		cout<<"Nem találja szabályokat\n";
		ifile.close();
		
		return 1;
	}

	while (ifile.getline(buff, 99)) {
		a = buff;
		a.erase(remove_if(a.begin(), a.end(), ::isspace), a.end());
		a.erase(remove(a.begin(), a.end(), ';'), a.end());
		pt = a.find("->");

		//bal oldal
		gram[np][0] = a.substr(0, pt);
		a = a.substr(pt + 2, a.length());
		//jobb oldal
		for (j = 0; a.length()!=0; j++)
		{
			int i = a.find("|");
			if (i > a.length())
			{
				gram[np][j + 1] = a;
				a = "";
			}
			else
			{
				gram[np][j + 1] = a.substr(0, i);
				a = a.substr(i + 1, a.length());
			}
		}

		np++;
	}
	ifile.close();

	string table[MAX][MAX], st;
	set<string> strings;

	
	ifstream ifile1(argv[2]);
	while (ifile1.getline(buff, 999)) {
		str = buff;
		strings.insert(str);
	}
	ifile1.close();

	//Beolvassa megadott stringet
	for (set<string>::iterator it = strings.begin(); it != strings.end(); ++it) {
		str = *it;

		
		for (i = 0; i < str.length(); i++)
		{
			r = "";
			st = "";
			st += str[i];

			for (j = 0; j < np; j++)
			{
				for (k = 1; gram[j][k] != ""; k++)
				{
					if (gram[j][k] == st)
					{
						r = concat(r, gram[j][0]);
					}
				}
			}
			table[i][i] = r;
		}

		
		for (k = 1; k < str.length(); k++)
		{
			for (j = k; j < str.length(); j++)
			{
				r = "";
				for (l = j - k; l < j; l++)
				{
					pr = gen_comb(table[j - k][l], table[l + 1][j]);
					r = concat(r, pr);
				}
				table[j - k][j] = r;
			}
		}

		
		//matrix kiirasa
		for (i = 0; i < str.length(); i++)
		{
			k = 0;
			l = str.length() - i - 1;
			for (j=l; j<str.length(); j++)
			{
				cout << k+1 << "," << j+1 << " " << table[k++][j] << " | ";
			}
			cout << endl;
		}
		

		//Ellenorzes: megnezi hogy elso sor utolso eleme S-e
		if (table[0][str.length() - 1].find(start) <= table[0][str.length() - 1].length())
			cout << "Része a nyelvnek\n";
		else
			cout << "Nem része a nyelvnek\n";
	}

	
	return 0;
}