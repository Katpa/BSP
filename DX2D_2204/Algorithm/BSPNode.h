#pragma once

class BSPNode
{
public:
	BSPNode(Vector2 areaSize, Vector2 areaPos, BSPNode* parent = nullptr);
	~BSPNode();

	void Render();

	void Partitioning();
	void SetPath();

	vector<BSPNode*> GetBottomChilds();
	BSPNode* Parent() { return parent; }
	vector<BSPNode*> Childs() { return childs; }
	RectCollider* Area() { return area; }

private:
	BSPNode* parent;
	vector<BSPNode*> childs;

	RectCollider* area;
};