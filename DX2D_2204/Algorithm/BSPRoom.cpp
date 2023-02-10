#include "Framework.h"

BSPRoom::BSPRoom(UINT leftBottom, UINT rightTop, UINT worldWidth)
	:indexLB(leftBottom), indexRT(rightTop), width(worldWidth)
{
	top = indexRT / worldWidth;
	right = indexRT % worldWidth;
	bottom = indexLB / worldWidth;
	left = indexLB % worldWidth;

	indexLT = top * worldWidth + left;
	indexRB = bottom * worldWidth + right;
}

BSPRoom::~BSPRoom()
{
}

//----------------------------------------------------------------------------
// Code: vector<UINT> Link(BSPRoom& room, UINT partitionType)
// Desc: 방과 방 사이에 경로(혹은 복도)를 배치하는 함수
//----------------------------------------------------------------------------
vector<UINT> BSPRoom::Link(BSPRoom& room, UINT partitionType)
{
	isReverse = false;
	vector<int> path;
	vector<UINT> output;

	DIR direction = Direction(room, partitionType);
	UINT distance = Distance(room, direction);
	
	UINT startIndex = 0, endIndex = 0;

	SetIndeies(startIndex, endIndex, room, partitionType, direction);

	if (direction == DIR::DOWN || direction == DIR::LEFT)
	{
		if (direction == DIR::DOWN) direction = DIR::UP;
		else if (direction == DIR::LEFT) direction = DIR::RIGHT;
		UINT tmp = startIndex;
		startIndex = endIndex;
		endIndex = tmp;
		isReverse = true;
	}

	vector<DIR> pathes = Pathing(startIndex, endIndex, distance, partitionType);
	
	for (DIR dir : pathes)
	{
		int tmp = 0;
		switch (dir)
		{
		case BSPRoom::DIR::UP:
			tmp = width;
			break;
		case BSPRoom::DIR::DOWN:
			tmp = width * -1;
			break;
		case BSPRoom::DIR::RIGHT:
			tmp = 1;
			break;
		case BSPRoom::DIR::LEFT:
			tmp = -1;
			break;
		}
		if (isReverse) tmp *= -1;

		path.push_back(tmp);
	}

	if (direction == DIR::DOWN || direction == DIR::LEFT)
	{
		UINT tmp = startIndex;
		startIndex = endIndex;
		endIndex = tmp;
		isReverse = true;
	}

	UINT curPos = startIndex;
	for (int p : path)
	{
		curPos += p;
		output.push_back(curPos);
	}

	return output;
}

//----------------------------------------------------------------------------
// Code: BSPRoom::DIR Direction(BSPRoom& room, UINT partitionType)
// Desc: 출발하는 방 기준에서 대상 방이 어느 방향에 있는지 확인하는 함수
//----------------------------------------------------------------------------
BSPRoom::DIR BSPRoom::Direction(BSPRoom& room, UINT partitionType)
{
	if (partitionType == 1)
	{
		if (room.left > right)	return DIR::RIGHT;
		else					return DIR::LEFT;
	}
	else
	{
		if (room.top < bottom)	return DIR::DOWN;
		else					return DIR::UP;
	}
}

//----------------------------------------------------------------------------
// Code: UINT Distance(BSPRoom& room, DIR direction)
// Desc: 출발방과 도착방의 거리를 계산하는 함수
//----------------------------------------------------------------------------
UINT BSPRoom::Distance(BSPRoom& room, DIR direction)
{
	UINT output = 0;

	switch (direction)
	{
	case BSPRoom::DIR::UP:
		output = room.bottom - top;
		break;
	case BSPRoom::DIR::DOWN:
		output = bottom - room.top;
		break;
	case BSPRoom::DIR::RIGHT:
		output = room.left - right;
		break;
	case BSPRoom::DIR::LEFT:
		output = left - room.right;
		break;
	}
	output--;

	return output;
}

//----------------------------------------------------------------------------
// Code: vector<BSPRoom::DIR> Pathing(UINT startIndex, UINT endIndex, UINT distance, UINT partitionType)
// Desc: 출발방에서 도착방으로 가는 경로를 지정하는 함수
//----------------------------------------------------------------------------
vector<BSPRoom::DIR> BSPRoom::Pathing(UINT startIndex, UINT endIndex, UINT distance, UINT partitionType)
{
	vector<DIR> path;

	if (distance & 1 && distance != 1)
	{
		//꺾임이 있는 복도
		UINT middlePoint = distance >> 1;

		if (partitionType == 1)
		{
			for (int i = 0; i < distance; i++)
			{
				if(i < middlePoint)
					path.push_back(DIR::RIGHT);
				else if(i > middlePoint)
					path.push_back(DIR::RIGHT);
				else
				{
					int cur = startIndex / width;
					int target = endIndex / width;

					path.push_back(DIR::RIGHT);

					if (cur < target)
					{
						for (; cur < target; cur++)
						{
							path.push_back(DIR::UP);
						}
					}
					else
					{
						for (; cur > target; cur--)
						{
							path.push_back(DIR::DOWN);
						}
					}
					
				}
			}
		}
		else
		{
			middlePoint = distance >> 1;

			for (int i = 0; i < distance; i++)
			{
				if (i < middlePoint)
					path.push_back(DIR::UP);
				else if (i > middlePoint)
					path.push_back(DIR::UP);
				else
				{
					int cur = startIndex % width;
					int target = endIndex % width;

					path.push_back(DIR::UP);

					if (cur < target)
					{
						for (; cur < target; cur++)
						{
							path.push_back(DIR::RIGHT);
						}
					}
					else
					{
						for (; cur > target; cur--)
						{
							path.push_back(DIR::LEFT);
						}
					}

				}
			}
		}
	}
	else
	{
		//직선형 복도
		if (partitionType == 1)
		{
			for (int i = 0; i < distance; i++)
				path.push_back(DIR::RIGHT);
		}
		else
		{
			for (int i = 0; i < distance; i ++)
				path.push_back(DIR::UP);
		}
	}

	return path;
}

//----------------------------------------------------------------------------
// Code: void SetIndeies(UINT& startIndex, UINT& endIndex, BSPRoom& room, UINT partitionType, DIR direction)
// Desc: 출발지점과 도착지점을 중복되는 범위 내에서 임의의 지점들을 결정하는 함수
//----------------------------------------------------------------------------
void BSPRoom::SetIndeies(UINT& startIndex, UINT& endIndex, BSPRoom& room, UINT partitionType, DIR direction)
{
	int start[2];
	int end[2];
	int minNum = 0, maxNum = 0;

	if (partitionType == 1)
	{
		start[0] = bottom;
		start[1] = top;
		end[0] = room.bottom;
		end[1] = room.top;
	}
	else
	{
		start[0] = left;
		start[1] = right;
		end[0] = room.left;
		end[1] = room.right;
	}

	if (start[0] < end[0])
		minNum = end[0];
	else
		minNum = start[0];

	if (start[1] < end[1])
		maxNum = start[1];
	else
		maxNum = end[1];

	startIndex = Random(minNum, maxNum + 1);
	endIndex = Random(minNum, maxNum + 1);

	switch (direction)
	{
	case BSPRoom::DIR::UP:
		startIndex += top * width;
		endIndex += room.bottom * width;
		break;
	case BSPRoom::DIR::DOWN:
		startIndex += bottom * width;
		endIndex += room.top * width;
		break;
	case BSPRoom::DIR::RIGHT:
		startIndex = startIndex * width + right;
		endIndex = endIndex * width + left;
		break;
	case BSPRoom::DIR::LEFT:
		startIndex = startIndex * width + left;
		endIndex = endIndex * width + right;
		break;
	}
}