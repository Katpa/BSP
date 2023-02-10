#include "Framework.h"

BSPNode::BSPNode(Vector2 areaSize, Vector2 areaPos, BSPNode* parent)
	:parent(parent), type(PartitionType::NONE), room(nullptr)
{
	area = new RectCollider(areaSize);
	area->Position() = areaPos;
	area->UpdateWorld();
}

BSPNode::~BSPNode()
{
	delete area;
	
	for (BSPNode*& child : childs)
		delete child;

	childs.clear();

	if (room)
		delete room;
}

void BSPNode::Render()
{
	area->Render();
}

//---------------------------------------------------
// Code: void Partitioning()
// Desc: 주어진 구역내에서 공간을 분할해 자식 노드를 생성하는 함수
//---------------------------------------------------
void BSPNode::Partitioning()
{
	float firstRate, firstMove, secondRate, secondMove;
	int randNum = Random(0, 3);
	if (randNum == 2)
	{
		firstRate = 0.4f;
		firstMove = 0.3f;
		secondRate = 0.6f;
		secondMove = 0.2f;
	}
	else if(randNum == 1)
	{
		firstRate = 0.6f;
		firstMove = 0.2f;
		secondRate = 0.4f;
		secondMove = 0.3f;
	}
	else
	{
		firstRate = 0.5f;
		firstMove = 0.25f;
		secondRate = 0.5f;
		secondMove = 0.25f;
	}

	Vector2 size = area->Size();

	Vector2 firstNode[2];
	Vector2 secondNode[2];

	BSPNode* firstChild;
	BSPNode* secondChild;

	if (size.x > size.y)
	{
		//가로분할
		firstNode[0] = size * Vector2(firstRate, 1.0f);
		firstNode[1] = area->GlobalPosition() - Vector2(size.x * firstMove, 0);

		secondNode[0] = size * Vector2(secondRate, 1.0f);
		secondNode[1] = area->GlobalPosition() + Vector2(size.x * secondMove, 0);

		type = PartitionType::VERTICAL;
	}
	else
	{
		//세로분할
		firstNode[0] = size * Vector2(1.0f, firstRate);
		firstNode[1] = area->GlobalPosition() - Vector2(0, size.y * firstMove);

		secondNode[0] = size * Vector2(1.0f, secondRate);
		secondNode[1] = area->GlobalPosition() + Vector2(0, size.y * secondMove);

		type = PartitionType::HORIZENTAL;
	}

	firstChild = new BSPNode(firstNode[0], firstNode[1], this);
	secondChild = new BSPNode(secondNode[0], secondNode[1], this);

	childs.push_back(firstChild);
	childs.push_back(secondChild);
}

//---------------------------------------------------
// Code: void SetPath(UINT widthIndex, vector<UINT>& pathVec)
// Desc: 자식노드끼리 길에 대한 인덱스를 pathVec에 담아내는 함수
//---------------------------------------------------
void BSPNode::SetPath(UINT widthIndex, vector<UINT>& pathVec)
{
	vector<BSPNode*> leftNode;
	vector<BSPNode*> rightNode;

	leftNode = childs[0]->GetLeafChilds();
	rightNode = childs[1]->GetLeafChilds();

	float minDist = D3D10_FLOAT32_MAX;

	BSPNode* minLeft = nullptr;
	BSPNode* minRight = nullptr;

	// 방을 갖고 있는 리프 노드들 중에서 가장 거리가 가까운 두 노드를 구한다.
	// left = 좌측에 놓이는 자식노드 / right = 우측에 놓이는 자식노드
	for (BSPNode* left : leftNode)
	{
		Vector2 leftPos = left->Area()->GlobalPosition();

		for (BSPNode* right : rightNode)
		{
			Vector2 rightPos = right->Area()->GlobalPosition();

			float dist = (leftPos - rightPos).Length();
			if (dist < minDist)
			{
				minDist = dist;
				minLeft = left;
				minRight = right;
			}
		}
	}

	pathVec = minLeft->Room()->Link(*minRight->Room(), (UINT)type);
}

void BSPNode::SetRoom(UINT leftBottom, UINT rightTop, UINT worldWidth)
{
	if (room) return;

	room = new BSPRoom(leftBottom, rightTop, worldWidth);
}

//---------------------------------------------------
// Code: vector<BSPNode*> GetBottomChilds()
// Desc: 최하단 노드들을 재귀적으로 찾아 반환하는 함수
//---------------------------------------------------
vector<BSPNode*> BSPNode::GetLeafChilds()
{
	vector<BSPNode*> childNodes;

	if (room != nullptr)
	{
		childNodes.push_back(this);
	}
	else
	{
		for (BSPNode* node : childs)
		{
			vector<BSPNode*> tmp = node->GetLeafChilds();
			for (BSPNode* n : tmp)
				childNodes.push_back(n);
		}
	}

	return childNodes;
}