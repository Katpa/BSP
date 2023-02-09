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

vector<int> BSPRoom::Link(BSPRoom& room, UINT partitionType)
{
	isReverse = false;
	vector<int> output;

	DIR direction = Direction(room, partitionType);
	UINT distance = Distance(room, direction);
	
	//startPoint는 최대한 좌측 하단에 가까운 방
	UINT startIndex, endIndex;

	if (direction == DIR::DOWN || direction == DIR::LEFT)
	{
		UINT tmp = startIndex;
		startIndex = endIndex;
		endIndex = tmp;
		isReverse = true;
	}

	vector<DIR> pathes = Pathing(startIndex, endIndex, distance, partitionType);
	
	for (DIR dir : pathes)
	{
		int tmp;
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

		output.push_back(tmp);
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
		UINT middlePoint;
	}
	else
	{
		//직선형 복도
		if (partitionType == 1)
		{
			for (int i = startIndex + 1; i < endIndex; i++)
				path.push_back(DIR::RIGHT);
		}
		else
		{
			for (int i = startIndex + width; i < endIndex; i += width)
				path.push_back(DIR::UP);
		}
	}

	return path;
}
