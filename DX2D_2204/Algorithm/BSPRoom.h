#pragma once

class BSPRoom
{
private:
	enum class DIR
	{
		UP, DOWN, RIGHT, LEFT
	};

public:
	BSPRoom(UINT leftBottom, UINT rightTop, UINT worldWidth);
	~BSPRoom();

	//partitionType -> 1 = Vertical / 2 = Horizental
	vector<UINT> Link(BSPRoom& room, UINT partitionType);

private:
	BSPRoom::DIR Direction(BSPRoom& room, UINT partitionType);
	UINT Distance(BSPRoom& room, DIR direction);

	vector<DIR> Pathing(UINT startIndex, UINT endIndex, UINT distance, UINT partitionType);
	void SetIndeies(UINT& startIndex, UINT& endIndex, BSPRoom& room, UINT partitionType, DIR direction);

public:
	UINT width;

	UINT top;
	UINT bottom;
	UINT left;
	UINT right;

	UINT indexLB;
	UINT indexRB;
	UINT indexLT;
	UINT indexRT;

	bool isReverse = false;
};