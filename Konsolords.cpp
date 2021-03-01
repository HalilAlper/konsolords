#include <iostream>
#include <conio.h>
#include <synchapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
using namespace std;

string TroopNames[] = {"Archer","Spearman","Cavalry","Swordsman","Scout","Enemy"};
int TroopStats[][8] = {
	{50, 5, 1, 20, 7, 4, 2, 10},
	{80, 15, 2, 0, 0, 2, 1, 15},
	{150, 30, 1, 0, 0, 3, 3, 30},
	{100, 20, 1, 0, 0, 1, 1, 20},
	{20, 5, 1, 0, 0, 5, 4, 10},
	{40, 0, 0, 0, 0, 0, 0, 0}
};
static const int MapLength = 60;
int SendRow = 4;
int SendSoldier = 0;
bool SendingAllowed = true;
int Coins[2] = {50, 50};
int Progress = 0;
bool Endgame = false, win;


string Initials = "ASCIsX";

// Health, MeleeDamage, MeleeRange, RangedDamage, RangedRange, Initiative, Speed, Price



class Troop
{
	public:
		int Health;
		int MeleeDamage;
		int MeleeRange;
		int RangedDamage;
		int RangedRange;
		int Initiative;
		int Speed;
		char Initial;
		int x, y;
		bool Player;
		
		Troop(string TroopType, int xx, int yy, bool P1)
		{
			if (TroopType == "Empty")
			{
				Health = 0;
				MeleeDamage = 0;
				MeleeRange = 0;
				RangedDamage = 0;
				RangedRange = 0;
				Initiative = 0;
				Speed = 0;
				Initial = 'X';
				x = 0;
				y = 0;
				Player = false;
				return;
			}
			for (int i = 0; i < 7; i++)
			{
				if (TroopNames[i] == TroopType)
				{
					Health = TroopStats[i][0];
					MeleeDamage = TroopStats[i][1];
					MeleeRange = TroopStats[i][2];
					RangedDamage = TroopStats[i][3];
					RangedRange = TroopStats[i][4];
					Initiative = TroopStats[i][5];
					Speed = TroopStats[i][6];
					Initial = Initials[i];
					x = xx;
					y = yy;
					Player = P1;
					break;
				}
			}
		}
		
		bool ScanMelee(void);
		bool ScanRanged(void);
		int ScanMovement(void);
};

struct TroopNode
{
	Troop *Tp;
	TroopNode *Next; 
};
TroopNode *AllTroops = new struct TroopNode;

bool Troop::ScanMelee(void)
{
	if (MeleeRange > 0);
	{
		TroopNode *Temp = AllTroops;
		while (Temp->Next != NULL)
		{
			Temp = Temp->Next;
			if (Temp->Tp->x == x && Temp->Tp->Player != Player)
			{
				if (Player)
				{
					for (int i = y + 1; i <= y + MeleeRange; i++)
					{
						if (Temp->Tp->y == i)
						{
							int DealDamage = MeleeDamage;
							if ((Initial == 'A' && Temp->Tp->Initial == 'S') || (Initial == 'S' && Temp->Tp->Initial == 'C') || (Initial == 'C' && Temp->Tp->Initial == 'I'))
								DealDamage*=2;
							Temp->Tp->Health = Temp->Tp->Health - DealDamage;
							if (Temp->Tp->Health <= 0)
							{
								TroopNode *Temp2 = AllTroops;
								while (Temp2->Next != Temp)
									Temp2 = Temp2->Next;
								Temp2->Next = Temp->Next;
								Temp->Next = NULL;
								delete Temp;
								return false;
							}
							return true;
						}
					}
				}
				else
				{
					for (int i = y - 1; i >= y - MeleeRange; i--)
					{
						if (Temp->Tp->y == i)
						{
							int DealDamage = MeleeDamage;
							if ((Initial == 'A' && Temp->Tp->Initial == 'S') || (Initial == 'S' && Temp->Tp->Initial == 'C') || (Initial == 'C' && Temp->Tp->Initial == 'I'))
								DealDamage*=2;
							Temp->Tp->Health = Temp->Tp->Health - DealDamage;
							if (Temp->Tp->Health <= 0)
							{
								TroopNode *Temp2 = AllTroops;
								while (Temp2->Next != Temp)
									Temp2 = Temp2->Next;
								Temp2->Next = Temp->Next;
								Temp->Next = NULL;
								delete Temp;
								return false;
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
bool Troop::ScanRanged(void)
{
	if (RangedRange > 0);
	{
		TroopNode *Temp = AllTroops;
		while (Temp->Next != NULL)
		{
			Temp = Temp->Next;
			if (Temp->Tp->x == x && Temp->Tp->Player != Player)
			{
				if (Player)
				{
					for (int i = y + 2; i <= y + RangedRange; i++)
					{
						if (Temp->Tp->y == i)
						{
							int DealDamage = RangedDamage;
							if ((Initial == 'A' && Temp->Tp->Initial == 'S') || (Initial == 'S' && Temp->Tp->Initial == 'C') || (Initial == 'C' && Temp->Tp->Initial == 'I'))
								DealDamage*=2;
							Temp->Tp->Health = Temp->Tp->Health - DealDamage;
							if (Temp->Tp->Health <= 0)
							{
								TroopNode *Temp2 = AllTroops;
								while (Temp2->Next != Temp)
									Temp2 = Temp2->Next;
								Temp2->Next = Temp->Next;
								Temp->Next = NULL;
								delete Temp;
								return false;
							}
							return true;
						}
					}
				}
				else
				{
					for (int i = y - 2; i >= y - RangedRange; i--)
					{
						if (Temp->Tp->y == i)
						{
							int DealDamage = RangedDamage;
							if ((Initial == 'A' && Temp->Tp->Initial == 'S') || (Initial == 'S' && Temp->Tp->Initial == 'C') || (Initial == 'C' && Temp->Tp->Initial == 'I'))
								DealDamage*=2;
							Temp->Tp->Health = Temp->Tp->Health - DealDamage;
							if (Temp->Tp->Health <= 0)
							{
								TroopNode *Temp2 = AllTroops;
								while (Temp2->Next != Temp)
									Temp2 = Temp2->Next;
								Temp2->Next = Temp->Next;
								Temp->Next = NULL;
								delete Temp;
								return false;
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
int Troop::ScanMovement(void)
{
	if (Speed > 0);
	{
		TroopNode *Temp = AllTroops;
		while (Temp->Next != NULL)
		{
			Temp = Temp->Next;
			if (Temp->Tp->x == x)
			{
				if (Player)
				{
					for (int i = y + 1; i <= y + Speed; i++)
					{
						if (Temp->Tp->y == i)
						{
							return i - 1 - y;
						}
					}
				}
				else
				{
					for (int i = y - 1; i >= y - Speed; i--)
					{
						if (Temp->Tp->y == i)
						{
							return i + 1 - y;
						}
					}
				}
			}
		}
	}
	if (Player)
		return Speed;
	else
		return -Speed;
}

char harita[10][MapLength];
void ClearMap()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < MapLength; j++)
		{
			harita[i][j] = '-';
		}
	}
}

int x = 0, y = 0;

bool yazdiriyor = false;
void yazdir()
{
	if (yazdiriyor == false)
	{
		yazdiriyor = true;
		system("cls");
		for (int i = 0; i < 10; i++)
		{
			if (i == SendRow)
				cout<<"-->";
			else
				cout<<"   ";
			
			for (int j = 5; j < MapLength - 5; j++)
			{
				cout<<harita[i][j];
			}
			cout<<endl;
		}
		
		cout<<"\n\n\n";
		cout<<"A S C I s\n";
		for (int i = 0; i < SendSoldier * 2; i++)
			cout<<' ';
		cout<<"^\n\n";
		cout<<"A: Archer, Price: "<<TroopStats[0][7]<<endl;
		cout<<"S: Spearman, Price: "<<TroopStats[1][7]<<endl;
		cout<<"C: Cavalry, Price: "<<TroopStats[2][7]<<endl;
		cout<<"I: Swordsman, Price: "<<TroopStats[3][7]<<endl;
		cout<<"s: Scout, Price: "<<TroopStats[4][7]<<endl<<endl;
		cout<<"Coins: "<<Coins[0]<<"   -   Enemy Coins: "<<Coins[1]<<endl<<endl;
		for (int i = -20; i < Progress; i++)
			cout<<">";
		cout<<"|";
		for (int i = Progress; i < 20; i++)
			cout<<"<";
		yazdiriyor = false;
	}
	else
	{
		Sleep(100);
		yazdir();
	}
}

int TroopChances[10][5];
bool ScoutProb[10] = {true, true, true, true, true, true, true, true, true, true};
void ResetChances()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			TroopChances[i][j] = 1;
		}
	}
}

int hesapla()
{
	srand(time(NULL));
	ResetChances();
	TroopNode *Temp = AllTroops;
	while (Temp->Next != NULL)
	{
		Temp = Temp->Next;
		if (Temp->Tp->Player);
		for (int i = 0; i < 4; i++)
		{
			TroopChances[Temp->Tp->x][i] *= 2;
		}
		if (Temp->Tp->Initial == 'S')
			TroopChances[Temp->Tp->x][0] *= 5;
		if (Temp->Tp->Initial == 'C')
			TroopChances[Temp->Tp->x][1] *= 5;
		if (Temp->Tp->Initial == 'I')
			TroopChances[Temp->Tp->x][2] *= 5;
		if (Temp->Tp->Initial == 'A')
			TroopChances[Temp->Tp->x][3] *= 5;
		ScoutProb[Temp->Tp->x] = false;
	}
	
	for (int i = 0; i < 10; i++)
	{
		if (ScoutProb[i])
		{
			TroopChances[i][4] *= 10;
		}
	}
	
	//Randomness
	int Rands[50];
	for (int i = 0; i < 10; i++)
	{
		Rands[0] = TroopChances[0][0];
		for (int j = 1; j <= 5; j++)
		{
			Rands[i*5 + j] = Rands[i*5 + j - 1] + TroopChances[i][j];
		}
	}
	int Chosen = rand() % Rands[50];
	if (Rands[0] <= Chosen && Rands[1] > Chosen)
		return 0;
	for (int i = 1; i < 50; i++)
		if (Rands[i] <= Chosen && Rands[i+1] > Chosen)
			return i;
}

void *bekle(void *ptr)
{
	while (true)
	{
		Sleep(1000);
		ClearMap();
		
		int WillSend = hesapla();
		string WSS = TroopNames[WillSend%5];
		int WSR = WillSend/5;
		if (TroopStats[WillSend%5][7] <= Coins[1])
		{
			Coins[1] -= TroopStats[WillSend%5][7];
			TroopNode *PointingAt = AllTroops;
			while (PointingAt->Next != NULL)
				PointingAt = PointingAt->Next;
			PointingAt->Next = new struct TroopNode;
			PointingAt->Next->Tp = new Troop(WSS, WSR, MapLength - 6, false);
			PointingAt = PointingAt->Next;
			PointingAt->Next = NULL;
		}
		
		TroopNode *Temp = AllTroops;
		while (Temp->Next != NULL)
		{
			Temp = Temp->Next;
			if (Temp->Tp->ScanMelee() == false)
			{
				if (Temp->Tp->ScanRanged() == false)
				{
					Temp->Tp->y = Temp->Tp->y + Temp->Tp->ScanMovement();
					if (Temp->Tp->y > MapLength - 5 || Temp->Tp->y <= 5)
					{
						if (Temp->Tp->Player)
							Progress++;
						else
							Progress--;
					 
						if (Progress > 20)
						{
							Endgame = true;
							win = true;
							return NULL;
						}
						else if (Progress < -2)
						{
							Endgame = true;
							win = false;
							return NULL;
						}
						
						TroopNode *Temp2 = AllTroops;
						while (Temp2->Next != Temp)
							Temp2 = Temp2->Next;
						Temp2->Next = Temp->Next;
						Temp->Next = NULL;
						delete Temp;
						Temp = Temp2;
					}
				}
			}
			if (Temp->Tp->y < MapLength)
				harita[Temp->Tp->x][Temp->Tp->y] = Temp->Tp->Initial;
		}
		
		Coins[0] += 5;
		Coins[1] += 5;
		SendingAllowed = true;
		yazdir();
	}
}

void *keyal(void *ptr)
{
	while (true)
	{
		char key = getch();
		if (key == 'w')
		{
			SendRow--;
			if (SendRow < 0)
				SendRow = 0;
		}
		else if (key == 's')
		{
			SendRow++;
			if (SendRow > 9)
				SendRow = 9;
		}
		else if (key == 'a')
		{
			SendSoldier--;
			if (SendSoldier < 0)
				SendSoldier = 0;
		}
		else if (key == 'd')
		{
			SendSoldier++;
			if (SendSoldier > 4)
				SendSoldier = 4;
		}
		else if (key == 'f')
		{
			if (SendingAllowed && TroopStats[SendSoldier][7] <= Coins[0])
			{
				TroopNode *PointingAt = AllTroops;
				while (PointingAt->Next != NULL)
					PointingAt = PointingAt->Next;
				
				Coins[0] -= TroopStats[SendSoldier][7];
				SendingAllowed = false;
				PointingAt->Next = new struct TroopNode;
				PointingAt->Next->Tp = new Troop(TroopNames[SendSoldier], SendRow, 5, true);
				PointingAt = PointingAt->Next;
				PointingAt->Next = NULL;
			}
		}
		yazdir();
		
		if (Endgame)
			break;
	}
}

int main()
{
	AllTroops->Tp = new Troop("Empty", 0, 0, false);
	AllTroops->Next = NULL;
	TroopNode *PointingAt = AllTroops;
	ClearMap();	
	pthread_t bekleyici, keyalici;
	pthread_create(&bekleyici, NULL, bekle, (void*) "test");
	pthread_create(&keyalici, NULL, keyal, (void*) "test");
	
	pthread_join(bekleyici, NULL);
	pthread_join(keyalici, NULL);
	
	system("cls");
	cout<<"KAZANDINIZ!!!!";
}
