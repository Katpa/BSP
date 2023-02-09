#pragma once

class BSPNode
{
private:
	enum class PartitionType
	{
		NONE = 0, VERTICAL = 1, HORIZENTAL = 2
	};

public:
	BSPNode(Vector2 areaSize, Vector2 areaPos, BSPNode* parent = nullptr);
	~BSPNode();

	void Render();

	void Partitioning();
	void SetPath(UINT widthIndex, vector<UINT>& pathVec);

	vector<BSPNode*> GetLeafChilds();
	BSPNode* Parent() { return parent; }
	vector<BSPNode*> Childs() { return childs; }
	RectCollider* Area() { return area; }

	UINT& Start() { return startIndex; }
	UINT& End() { return endIndex; }

private:
	BSPNode* parent;
	vector<BSPNode*> childs;

	RectCollider* area;
	PartitionType type;

	UINT startIndex = 0, endIndex = 0;
};