# 立方数码 实验说明

## 程序说明

该试验所有程序都使用C++编写。

## Ah1

#### 类型定义

程序首先实现了一个Cub类，类中包含如下成员：

```cpp
short int numbers[3][3][3];
char lastStep;
string stepSet;
int stepNum;
int cost;
short int zeroX, zeroY, zeroZ;
```

其中,numbers为一个三维数组，代表立方体，最高维代表层数，第二维代表某一层的行数，最后一维则代表列数。这里，我特意将数组调整为`short int`来节省空间；事实上，将数组设置成`char`型可以更节省空间。

`stepSet`是一个字符串，它表示从初始状态移动到当前状态的移动方式序列，这种表示方式的好处是可以不用保存从优先队列中弹出的状态，使程序更简洁；坏处则是增加了每一个状态所费的空间。

`lastStep`唯一的作用是用来返回当前移动是否合法，之所以取这个名字是因为我曾经试想过每个状态保存其父状态到子状态的移动方式，最后再通过指针反推会初始状态得到完整的移动序列，但是最终我的实现没有采用这种方案（这种方案可能会更加节省空间，如果`stepSet`的大小相对于`numbers`较大的话）,而是采用了利用`stepSet`的方式。

`cost`表示当前状态的`f+h`之和，`zeroX`，`zeroY`，`zeroZ`则代表当前状态中0的位置，这样做的好处是节省了移动时寻找0元素的时间开销。

另外，Cub类中包含如下成员函数：

```cpp
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
```

构造函数，用来初始化一个状态。

```cpp
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
```

这个函数会返回当前状态向上移动一步之后产生的下一状态。如果这个移动不合法，那么`lastStep`的值会被置为`'E'`.其它移动函数与此类似，不再加以说明。

```cpp
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
```

这个函数用来将当前状态映射成一个字符串用于查重，可以参见之后的说明。

其他函数不一一列举，可以参见代码。

#### 算法分析

在主函数中，程序的行为如下：

首先，将第一个状态插入优先队列`cubQueue`中，之后每次从队列中弹出一个状态进行扩展，并且将扩展之后得到的子状态插入队列中。另外，当前状态被扩展完之后，需要将该状态的ID插入`statusList`中；每次从队列中弹出一个状态之后，需要检查该状态是否是目标状态并且检查改状态是否已经被扩展过，如果是的话则不再扩展该状态。这里优先队列的实现采用了STL库中的`priority_queue`，保存ID则使用了`set`。由于`set`的实现使用了红黑树，因此查询操作和插入操作的时间复杂度都是`O(log n)`；优先队列的插入操作复杂度同样也为`O(log n)`。因此，每一次状态扩展在`O(log n)`内就可以完成。另外，由于使用了`stepSet`来记录路径，因此之前的所有已经扩展过的状态都不没有必要再保存，这在搜索树不是太深的时候可以节省空间。

### Ah2

和Ah1除了`CalCost()`函数中的h1改为h2之外其它部分完全一致。

### IDAh1

和非迭代算法基本一致，只是在迭代时要注意根据迭代深度限制对状态进行截断，具体实现可以参见代码，我的实现方式和实验说明中的伪代码一致。另外，需要注意每次新一轮迭代开始时，需要对`statusList`进行初始化。

### IDAh2

和IDAh1除了`CalCost()`函数中的h1改为h2之外其它部分完全一致。

## 运行说明

我的编译环境是windows10 with visual studio 2015 community。所有程序直接运行即可。另外，在内存不足时程序会报错，对此我没有做特殊处理。

