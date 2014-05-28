#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void main()
{
	string star[10];
	//string a = "*";
	/*
	string star1[10];
	string star2[10];
	for(int i = 0; i < 10; i++)
	{
		star1[i] = "*" ;
		star2[i] = " " ;
	}
	*//*
	int b;
	cin >> b;
	for (int i = 0; i < b; i++)
	{
		star[i] = a;
		a += "*";
	}
	*//*
	for (int i = 9; i > -1; i--)
	{
		string t = star1[i];
		star1[i] = star2[i];
		star2[i] = t;
		for(int k = 0; k < 10; k++)
		{
			cout << star1[k];
		}
		cout << endl;
	}
	*//*
	for (int i = 0; i < 10; i++)
	{
		for(int k=10; k>i; k--)
		{
			cout << "*";
			if(i==9)
			{
				cout << endl << " ";
			}
		}

		cout << endl;
	}
	*//*
	for(int i=0; i<10; i++)
	{
	cout << star[i] << endl;
	}
	*//*
	string star1[21];
	string star2[21];
	int a=11;
	int b=11;
	for(int i = 0; i < 21; i++)
	{
		star1[i] = "*";
		if(i==11)
		{
			star2[i] = "*";
		}
		else
		{
			star2[i] = " ";
		}
	}

	for (int i = 0; i < 10; i++)
	{
		string t = star1[a];
		star1[a] = star2[a];
		star2[a] = t;
		a++;
		t = star1[b];
		star1[b] = star2[b];
		star2[b] = t;
		b--;
		for(int k = 0; k < 21; k++)
		{
			cout << star2[k];
		}
		cout << endl;
	}
	*//*
	for (int i = 21; i > 0; i-=2)
	{
		for(int k=0; k<i; k+=2)
		{
			cout << " ";
		}
		for(int k=20; k>i; k--)
		{
			cout << " ";
		}
		cout << endl;
	}
	*/
	for (int i =41; i>0; i-=2)
	{
		if(i > 29)
		{
			for(int k=0; k<i; k+=2)
			{
				cout << "7";
			}
			for(int k=40; k>i; k--)
			{
			cout << "*";
			}
		}
		if(31 > i)
		{
			for(int k=31; k>i; k-=2)
			{
				cout << "9";
			}
			for(int k=0; k<i; k++)
			{
			cout << "1";
			}
			if(i <19)
				break;
		}
		if(17 > i)
		{
			for(int k=31; k>i; k-=2)
			{
				cout << "6";
			}
			for(int k=0; k<i; k++)
			{
			cout << "2";
			}
		}
		cout << endl;
	}
	
}