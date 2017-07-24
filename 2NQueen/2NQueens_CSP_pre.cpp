#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N 1000002
#define BLACK 0
#define WHITE 1
#define absM(a, b) a>b?a-b:b-a

int chessBoard[N][2];
int conflicts[N];
int boardScale;

void GetConflict(int lineNumber, int color)
{
	int i;
	memset(conflicts, 0, sizeof(int)*boardScale);
	for (i = 0; i < boardScale; i++)
	{
		if (i == lineNumber)
			continue;
		int interval = absM(lineNumber, i);
		int posA = chessBoard[i][color] - interval;
		int posB = chessBoard[i][color] + interval;
		if (posA >= 0)
			conflicts[posA]++;
		if (posB < boardScale)
			conflicts[posB]++;
		conflicts[ chessBoard[i][color] ]++;
	}
}

int main( void )
{
	FILE *fp;
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &boardScale);
	fclose(fp);
	srand((unsigned)time(NULL));

	long startTime = clock();
	while (1)
	{
		int i;
		memset(chessBoard, 0, sizeof(chessBoard));
		for (i = 0; i < boardScale; i++)
		{
			chessBoard[i][BLACK] = rand() % (boardScale - 1);
			chessBoard[i][WHITE] = chessBoard[i][BLACK] + 1;
		}

		int conflictSum = 0;
		for (i = 0; i < boardScale; i++)
		{
			GetConflict(i, BLACK);
			conflictSum += conflicts[chessBoard[i][BLACK]];
			GetConflict(i, WHITE);
			conflictSum += conflicts[chessBoard[i][WHITE]];
		}

		int choice, min, minPos;
		int iterationTime = 0;
		int chance;
		while (iterationTime < 10000 * boardScale)
		{
			choice = iterationTime % boardScale;
			GetConflict(choice, BLACK);
			min = INT_MAX;
			for (i = 0; i < boardScale; i++)
				if (conflicts[i] <= min && chessBoard[choice][WHITE] != i)
				{
					min = conflicts[i];
					minPos = i;
				}
			chance = rand() % 2;
			if (min <= conflicts[chessBoard[choice][BLACK]])
			{
				conflictSum -= 2 * (conflicts[chessBoard[choice][BLACK]] - conflicts[minPos]);
				chessBoard[choice][BLACK] = minPos;
			}
			if (conflictSum == 0)
				break;

			choice = boardScale - (iterationTime % boardScale) - 1;
			GetConflict(choice, WHITE);
			min = INT_MAX;
			for (i = 0; i < boardScale; i++)
				if (conflicts[i] <= min && chessBoard[choice][BLACK] != i)
				{
					min = conflicts[i];
					minPos = i;
				}
			chance = rand() % 2;
			if (min < conflicts[chessBoard[choice][WHITE]] || (min == conflicts[chessBoard[choice][WHITE]] && chance == 0))
			{
				conflictSum -= 2 * (conflicts[chessBoard[choice][WHITE]] - conflicts[minPos]);
				chessBoard[choice][WHITE] = minPos;
			}
			if (conflictSum == 0)
				break;

			iterationTime++;
		}

		if (conflictSum == 0)
			break;
	}

	long endTime = clock();

    int i;
	fp = fopen("output_csp_pre.txt", "w");
	for (i = 0; i < boardScale; i++)
		fprintf(fp, "%d\n", chessBoard[i][WHITE]+1);
	for (i = 0; i < boardScale; i++)
		fprintf(fp, "%d\n", chessBoard[i][BLACK]+1);
	fprintf( fp, "%f\n", (float)(endTime - startTime));

	return 0;
}
