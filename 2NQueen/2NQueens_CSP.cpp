#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N 1000002

int chessBoard[N];
int chessBoardBlack[N];
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

int main(void)
{
	FILE *fp;
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &boardScale);
	fclose(fp);
	srand((unsigned)time(NULL));

	long startTime = clock();

	int i;
	for (i = 0; i < boardScale; i++)
		chessBoard[i] = rand() % boardScale;;
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

	int row, min, minPos, cost, preMin;
	int chance, iterationTime;
	if (boardScale % 2 == 1)
	{
		for (iterationTime = 0; iterationTime < INT_MAX; iterationTime++)
		{
			row = iterationTime % boardScale;
			line = chessBoard[row];
			preMin = GetConflict(row, line) - 3;
			chance = rand() % 2;
			if (chance == 1 && preMin == 0)
				continue;
			lineQNum[line]--;
			posDiaQNum[boardScale - line + row - 1]--;
			negDiaQNum[line + row]--;//把这一行的棋子取出再计算冲突

			min = INT_MAX;
			for (i = 0; i < boardScale; i++)
			{
				if (i == row)
					continue;
				cost = GetConflict(row, i);
				chance = rand() % 2;
				if (cost < min)
				{
					min = cost;
					minPos = i;
				}
				else if (cost == min && chance == 1)
					minPos = i;
			}

			if (min <= preMin)
			{
				chessBoard[row] = minPos;
				lineQNum[minPos]++;
				posDiaQNum[boardScale - minPos + row - 1]++;
				negDiaQNum[minPos + row]++;//放回棋子
			}
			else
			{
				lineQNum[line]++;
				posDiaQNum[boardScale - line + row - 1]++;
				negDiaQNum[line + row]++;//放回棋子
				min = preMin;
			}
			conflictSum -= 2 * (preMin - min);

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
			preMin = GetConflict(row, line) - 3;
			chance = rand() % 2;
			if (chance == 1 && preMin == 0)
				continue;
			lineQNum[line]--;
			posDiaQNum[boardScale - line + row - 1]--;
			negDiaQNum[line + row]--;

			min = INT_MAX;
			for (i = 0; i < boardScale; i++)
			{
				cost = GetConflict(row, i);
				chance = rand() % 2;
				if (cost < min)
				{
					min = cost;
					minPos = i;
				}
				else if (cost == min && chance == 1)
					minPos = i;
			}

			preMin = GetConflict(row, line);
			if (min <= preMin )
			{
				chessBoard[row] = minPos;
				lineQNum[minPos]++;
				posDiaQNum[boardScale - minPos + row - 1]++;
				negDiaQNum[minPos + row]++;
			}
			else
			{
				lineQNum[line]++;
				posDiaQNum[boardScale - line + row - 1]++;
				negDiaQNum[line + row]++;
				min = preMin;
			}
			conflictSum -= 2 * (preMin - min);

			if (conflictSum == 0)
				break;
		}
	}
	long endTime = clock();

	fp = fopen("output_csp.txt", "w");
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
	fprintf(fp, "%f\n", (float)(endTime - startTime)/1.67);

	return 0;
}