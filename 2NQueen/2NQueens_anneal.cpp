#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define N 1000002
#define ACC 1000000

int chessBoard[N];
int chessBoardBlack[N];
int conflicts[N];
int lineQNum[N];
int posDiaQNum[2 * N - 1];
int negDiaQNum[2 * N - 1];
int boardScale;

int GetConflict(int row, int line)
{
	int a = lineQNum[line];
	int b = posDiaQNum[boardScale - line + row - 1];
	int c = negDiaQNum[line + row];
	return a + b + c;
}

int main()
{
	FILE *fp;
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &boardScale);
	fclose(fp);
	srand((unsigned)time(NULL));

	int i;
	for (i = 0; i < boardScale; i++)
		chessBoard[i] = rand() % boardScale;
	if (boardScale % 2 == 1)
		if (chessBoard[boardScale / 2] == boardScale / 2)
			chessBoard[boardScale / 2]++;

	int line;
	for (i = 0; i < boardScale; i++)
	{
		line = chessBoard[i];
		lineQNum[line]++;
		posDiaQNum[boardScale - line + i - 1]++;
		negDiaQNum[line + i]++;
	}

	int conflictSum = 0;
	for (i = 0; i < boardScale; i++)
	{
		line = chessBoard[i];
		conflictSum += GetConflict(i, line) - 3;
	}

	int row, preMin, nextline, costS;
	int iterationTime, deltaE;
	double chance;
	bool selectSucceed;
	long startTime = clock();
	if (boardScale % 2 == 0)
	{
		for (iterationTime = 0; iterationTime < INT_MAX; iterationTime++)
		{
			row = iterationTime % boardScale;
			line = chessBoard[row];
			lineQNum[line]--;
			posDiaQNum[boardScale - line + row - 1]--;
			negDiaQNum[line + row]--;
			preMin = GetConflict(row, line);
				
			nextline = rand() % boardScale;
			costS = GetConflict(row, nextline);
			deltaE = costS - preMin;
			if (deltaE <= 0)
			{
				chessBoard[row] = nextline;
				lineQNum[nextline]++;
				posDiaQNum[boardScale - nextline + row - 1]++;
				negDiaQNum[nextline + row]++;
			}
			else 
			{
				chance = exp(-deltaE * iterationTime / (double)boardScale) * ACC;
				selectSucceed = (rand() % ACC) < ((int)chance);
				if (selectSucceed)
				{
					chessBoard[row] = nextline;
					lineQNum[nextline]++;
					posDiaQNum[boardScale - nextline + row - 1]++;
					negDiaQNum[nextline + row]++;
				}
				else
				{
					lineQNum[line]++;
					posDiaQNum[boardScale - line + row - 1]++;
					negDiaQNum[line + row]++;
					costS = preMin;
				}
			}
			
			conflictSum -= 2 * (preMin - costS);
			if (conflictSum == 0)
				break;
		}
	}
	else
	{
		for (iterationTime = 0; iterationTime < INT_MAX; iterationTime++)
		{
			row = iterationTime % boardScale;
			line = chessBoard[row];
			nextline = rand() % boardScale;
			if (nextline == row)
			{
				iterationTime--;
				continue;
			}
			lineQNum[line]--;
			posDiaQNum[boardScale - line + row - 1]--;
			negDiaQNum[line + row]--;
			preMin = GetConflict(row, line);
			
			costS = GetConflict(row, nextline);
			deltaE = costS - preMin;
			if (deltaE <= 0)
			{
				chessBoard[row] = nextline;
				lineQNum[nextline]++;
				posDiaQNum[boardScale - nextline + row - 1]++;
				negDiaQNum[nextline + row]++;
			}
			else
			{
				chance = exp(-deltaE * iterationTime / (double)boardScale) * ACC;
				selectSucceed = (rand() % ACC) < ((int)chance);
				if (selectSucceed)
				{
					chessBoard[row] = nextline;
					lineQNum[nextline]++;
					posDiaQNum[boardScale - nextline + row - 1]++;
					negDiaQNum[nextline + row]++;
				}
				else
				{
					lineQNum[line]++;
					posDiaQNum[boardScale - line + row - 1]++;
					negDiaQNum[line + row]++;
					costS = preMin;
				}
			}

			conflictSum -= 2 * (preMin - costS);
			if (conflictSum == 0)
				break;
		}
	}
	long endTime = clock();

	fp = fopen("output_simulated_annealing.txt", "w");
	for (i = 0; i < boardScale; i++)
		fprintf(fp, "%d\n", chessBoard[i]+1);
	if (boardScale % 2 == 1)
	{
		for (i = 0; i < boardScale; i++)
			chessBoardBlack[chessBoard[i]] = i;
	}
	else
	{
		for (i = 0; i < boardScale; i++)
			chessBoardBlack[i] = boardScale - chessBoard[i] - 1;
	}

	for (i = 0; i < boardScale; i++)
		fprintf(fp, "%d\n", chessBoardBlack[i]+1);
	fprintf(fp, "%f\n", (float)(endTime - startTime) / 1.67);

	return 0;
}