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

vector<UINT> BSPRoom::Link(BSPRoom& room, UINT partitionType)
{
	isReverse = false;
	vector<int> path;
	vector<UINT> output;

	DIR direction = Direction(room, partitionType);
	UINT distance = Distance(room, direction);
	
	//startPoint는 최대한 좌측 하단에 가까운 방
	UINT startIndex = 0, endIndex = 0;

	switch (direction)
	{
	case BSPRoom::DIR::UP:
		startIndex = Random((int)indexLT, (int)indexRT + 1);
		endIndex = Random((int)room.indexLT, (int)room.indexRT + 1);
		break;
	case BSPRoom::DIR::DOWN:
		startIndex = Random((int)indexLB, (int)indexRB + 1);
		endIndex = Random((int)room.indexLB, (int)room.indexRB + 1);
		break;
	case BSPRoom::DIR::RIGHT:
		startIndex = Random((int)bottom, (int)top + 1) * width + right;
		endIndex = Random((int)room.bottom, (int)room.top + 1) * width + left;
		break;
	case BSPRoom::DIR::LEFT:
		startIndex = Random((int)bottom, (int)top + 1) * width + left;
		endIndex = Random((int)room.bottom, (int)room.top + 1) * width + right;
		break;
	}

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
						for (; cur <= target; cur++)
						{
							path.push_back(DIR::UP);
						}
					}
					else
					{
						for (; cur >= target; cur--)
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
						for (; cur <= target; cur++)
						{
							path.push_back(DIR::RIGHT);
						}
					}
					else
					{
						for (; cur >= target; cur--)
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
