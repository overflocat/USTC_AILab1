#include <time.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <set>
#include <string>

using namespace std;

class Cub
{
private:
	short int zeroX, zeroY, zeroZ;
	void Swap(short int &a, short int &b)
	{
		short int temp = a;
		a = b;
		b = temp;
	}

	int absM(int &a, int &b)
	{
		if (a < b)
			return b - a;
		else
			return a - b;
	}

public:
	short int numbers[3][3][3];
	char lastStep;
	string stepSet;
	int stepNum;
	int cost;

	Cub(short int iNum[][3][3], char lastStep, int stepNum)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
				{
					numbers[i][j][k] = iNum[i][j][k];
					if (numbers[i][j][k] == 0)
					{
						zeroX = i;
						zeroY = j;
						zeroZ = k;
					}
				}

		this->lastStep = lastStep;
		this->stepNum = stepNum;
		stepSet = "";
	}

	Cub() { }

	Cub MoveUp(void)
	{
		Cub c = Cub(numbers, 'E', stepNum);
		if (zeroY == 0)
			return c;
		if (numbers[zeroX][zeroY - 1][zeroZ] == -1)
			return c;

		Swap(c.numbers[zeroX][zeroY - 1][zeroZ], c.numbers[zeroX][zeroY][zeroZ]);
		c.zeroY--;
		c.lastStep = 'U';
		c.stepNum++;
		c.stepSet = this->stepSet + "U";
		return c;
	}

	Cub MoveDown(void)
	{
		Cub c = Cub(numbers, 'E', stepNum);
		if (zeroY == 2)
			return c;
		if (numbers[zeroX][zeroY + 1][zeroZ] == -1)
			return c;

		Swap(c.numbers[zeroX][zeroY + 1][zeroZ], c.numbers[zeroX][zeroY][zeroZ]);
		c.zeroY++;
		c.lastStep = 'D';
		c.stepNum++;
		c.stepSet = this->stepSet + "D";
		return c;
	}

	Cub MoveLeft(void)
	{
		Cub c = Cub(numbers, 'E', stepNum);
		if (zeroZ == 0)
			return c;
		if (numbers[zeroX][zeroY][zeroZ - 1] == -1)
			return c;

		Swap(c.numbers[zeroX][zeroY][zeroZ - 1], c.numbers[zeroX][zeroY][zeroZ]);
		c.zeroZ--;
		c.lastStep = 'L';
		c.stepNum++;
		c.stepSet = this->stepSet + "L";
		return c;
	}

	Cub MoveRight(void)
	{
		Cub c = Cub(numbers, 'E', stepNum);
		if (zeroZ == 2)
			return c;
		if (numbers[zeroX][zeroY][zeroZ + 1] == -1)
			return c;

		Swap(c.numbers[zeroX][zeroY][zeroZ + 1], c.numbers[zeroX][zeroY][zeroZ]);
		c.zeroZ++;
		c.lastStep = 'R';
		c.stepNum++;
		c.stepSet = this->stepSet + "R";
		return c;
	}

	Cub MoveFront(void)
	{
		Cub c = Cub(numbers, 'E', stepNum);
		if (zeroX == 0)
			return c;
		if (numbers[zeroX - 1][zeroY][zeroZ] == -1)
			return c;

		Swap(c.numbers[zeroX - 1][zeroY][zeroZ], c.numbers[zeroX][zeroY][zeroZ]);
		c.zeroX--;
		c.lastStep = 'F';
		c.stepNum++;
		c.stepSet = this->stepSet + "F";
		return c;
	}

	Cub MoveBack(void)
	{
		Cub c = Cub(numbers, 'E', stepNum);
		if (zeroX == 2)
			return c;
		if (numbers[zeroX + 1][zeroY][zeroZ] == -1)
			return c;

		Swap(c.numbers[zeroX + 1][zeroY][zeroZ], c.numbers[zeroX][zeroY][zeroZ]);
		c.zeroX++;
		c.lastStep = 'B';
		c.stepNum++;
		c.stepSet = this->stepSet + "B";
		return c;
	}

	string GetID(void)
	{
		char status[28];
		int p = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					status[p++] = (char)numbers[i][j][k] + 'B';
		status[p] = '\0';

		return string(status);
	}

	int h1(Cub &c)
	{
		int sum = 0;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					if (c.numbers[i][j][k] != numbers[i][j][k] && numbers[i][j][k] != 0)
						sum++;

		return sum;
	}

	int h2(Cub &c)
	{
		int sum = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					for (int l = 0; l < 3; l++)
						for (int m = 0; m < 3; m++)
							for (int n = 0; n < 3; n++)
								if (c.numbers[l][m][n] == numbers[i][j][k] && numbers[i][j][k] != 0)
									sum += absM(l, i) + absM(m, j) + absM(n, k);

		return sum;
	}

	void CalCost(Cub &c)
	{
		int sum = h2(c);
		this->cost = sum + stepNum;
	}

	bool CheckBarriers(Cub &c)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					if (numbers[i][j][k] == -1 && c.numbers[i][j][k] != -1)
						return false;
		return true;
	}

	bool operator< (const Cub &c) const
	{
		if (cost > c.cost)
			return true;
		else
			return false;
	}

	bool operator== (const Cub &c) const
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					if (c.numbers[i][j][k] != numbers[i][j][k])
						return false;

		return true;
	}
};

int main()
{
	short int numbers[3][3][3];
	FILE *fp;

	fp = fopen("input.txt", "r");
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				fscanf(fp, "%hd", &numbers[i][j][k]);
	Cub cBegin = Cub(numbers, 'S', 0);
	fclose(fp);

	fp = fopen("target.txt", "r");
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				fscanf(fp, "%hd", &numbers[i][j][k]);
	Cub cEnd = Cub(numbers, 'S', 0);
	fclose(fp);

	if (!cBegin.CheckBarriers(cEnd))
		return -1;

	cBegin.CalCost(cEnd);

	priority_queue<Cub> cubQueue;
	set<string> statusList;
	Cub tempC1, tempC2;
	string statS;

	cubQueue.push(cBegin);
	long startTime = clock();

	while (true)
	{
		tempC1 = cubQueue.top();
		cubQueue.pop();

		if (tempC1 == cEnd)
			break;

		statS = tempC1.GetID();
		if (statusList.find(statS) != statusList.end())
			continue;

		tempC2 = tempC1.MoveFront();
		if (tempC2.lastStep != 'E')
		{
			tempC2.CalCost(cEnd);
			cubQueue.push(tempC2);
		}

		tempC2 = tempC1.MoveBack();
		if (tempC2.lastStep != 'E')
		{
			tempC2.CalCost(cEnd);
			cubQueue.push(tempC2);
		}

		tempC2 = tempC1.MoveUp();
		if (tempC2.lastStep != 'E')
		{
			tempC2.CalCost(cEnd);
			cubQueue.push(tempC2);
		}

		tempC2 = tempC1.MoveDown();
		if (tempC2.lastStep != 'E')
		{
			tempC2.CalCost(cEnd);
			cubQueue.push(tempC2);
		}

		tempC2 = tempC1.MoveLeft();
		if (tempC2.lastStep != 'E')
		{
			tempC2.CalCost(cEnd);
			cubQueue.push(tempC2);
		}

		tempC2 = tempC1.MoveRight();
		if (tempC2.lastStep != 'E')
		{
			tempC2.CalCost(cEnd);
			cubQueue.push(tempC2);
		}

		statusList.insert(statS);
	}

	long endTime = clock();

	fp = fopen("output_Ah2.txt", "w");
	fprintf(fp, "%f\n", (float)(endTime - startTime) / 1.67);//huaji
	fprintf(fp, "%s\n", tempC1.stepSet.c_str());

	return 0;
}