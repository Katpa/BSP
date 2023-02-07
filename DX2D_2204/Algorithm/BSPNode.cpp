#include "Framework.h"

BSPNode::BSPNode(Vector2 areaSize, Vector2 areaPos, BSPNode* parent = nullptr)
	:parent(parent)
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
	int randNum = Random(0, 1);
	if (randNum)
	{
		firstRate = 0.4f;
		firstMove = 0.3f;
		secondRate = 0.6f;
		secondMove = 0.2f;
	}
	else
	{
		firstRate = 0.6f;
		firstMove = 0.2f;
		secondRate = 0.4f;
		secondMove = 0.3f;
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
	}
	else
	{
		//세로분할
		firstNode[0] = size * Vector2(1.0f, firstRate);
		firstNode[1] = area->GlobalPosition() - Vector2(0, size.x * firstMove);

		secondNode[0] = size * Vector2(1.0f, secondRate);
		secondNode[1] = area->GlobalPosition() + Vector2(0, size.x * secondMove);
	}

	firstChild = new BSPNode(firstNode[0], firstNode[1], this);
	secondChild = new BSPNode(secondNode[0], secondNode[1], this);

	childs.push_back(firstChild);
	childs.push_back(secondChild);
}

//---------------------------------------------------
// Code: void SetPath()
// Desc: 자식노드끼리 길을 만드는 함수
//---------------------------------------------------
void BSPNode::SetPath()
{
	vector<BSPNode*> leftNode;
	vector<BSPNode*> rightNode;

	leftNode = childs[0]->GetBottomChilds();
	rightNode = childs[1]->GetBottomChilds();

	float minDist = D3D10_FLOAT32_MAX;

	BSPNode* minLeft;
	BSPNode* minRight;

	// 방을 갖고 있는 리프 노드들 중에서 가장 거리가 가까운 노드들을 구한다.
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

	// 제일 가까운 노드 찾았고 여기서 어떤 식으로 이을까??
	// 복도 만드는 규칙이 뭐가 있을까
	// 제일 간단한 건 중앙끼리 연결하는 것
	// 복도 구조는 어떻게 짤래??
	// 그래 그러면 이렇게 해보자
	// 1. 일단 이 두 노드들은 자식 관계이기 때문에 어떤 분할을 당한 것이다.
	// 그러므로 그 분할의 종류에 따라 복도 시작 방향과 끝 방향이 정해지는데
	// 가로분할은 상/하향으로 세로분할은 좌/우향으로 한다.
	// 
	// 그 이전에 일단 방 구조부터 만들어야겠다.
	//
}

//---------------------------------------------------
// Code: vector<BSPNode*> GetBottomChilds()
// Desc: 최하단 노드들을 재귀적으로 찾아 반환하는 함수
//---------------------------------------------------
vector<BSPNode*> BSPNode::GetBottomChilds()
{
	vector<BSPNode*> childNodes;

	for (BSPNode* node : childs)
	{
		vector<BSPNode*> tmp;
		tmp = node->Childs();

		if (tmp.size() > 0) tmp = node->GetBottomChilds();
		
		for (BSPNode* child : tmp)
			childNodes.push_back(child);
	}

	return childNodes;
}