#pragma once

class BSPNode
{
public:
	BSPNode(Vector2 areaSize, Vector2 areaPos, BSPNode* parent = nullptr);
	~BSPNode();

	void Render();

	void Partitioning();
	void SetPath();

	BSPNode* Parent() { return parent; }
	vector<BSPNode*> Childs() { return childs; }
	vector<BSPNode*> GetBottomChilds();
	RectCollider* Area() { return area; }

private:
	BSPNode* parent;
	vector<BSPNode*> childs;

	RectCollider* area;
};