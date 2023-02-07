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
// Desc: �־��� ���������� ������ ������ �ڽ� ��带 �����ϴ� �Լ�
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
		//���κ���
		firstNode[0] = size * Vector2(firstRate, 1.0f);
		firstNode[1] = area->GlobalPosition() - Vector2(size.x * firstMove, 0);

		secondNode[0] = size * Vector2(secondRate, 1.0f);
		secondNode[1] = area->GlobalPosition() + Vector2(size.x * secondMove, 0);
	}
	else
	{
		//���κ���
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
// Desc: �ڽĳ�峢�� ���� ����� �Լ�
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

	// ���� ���� �ִ� ���� ���� �߿��� ���� �Ÿ��� ����� ������ ���Ѵ�.
	// left = ������ ���̴� �ڽĳ�� / right = ������ ���̴� �ڽĳ��
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

	// ���� ����� ��� ã�Ұ� ���⼭ � ������ ������??
	// ���� ����� ��Ģ�� ���� ������
	// ���� ������ �� �߾ӳ��� �����ϴ� ��
	// ���� ������ ��� ©��??
	// �׷� �׷��� �̷��� �غ���
	// 1. �ϴ� �� �� ������ �ڽ� �����̱� ������ � ������ ���� ���̴�.
	// �׷��Ƿ� �� ������ ������ ���� ���� ���� ����� �� ������ �������µ�
	// ���κ����� ��/�������� ���κ����� ��/�������� �Ѵ�.
	// 
	// �� ������ �ϴ� �� �������� �����߰ڴ�.
	//
}

//---------------------------------------------------
// Code: vector<BSPNode*> GetBottomChilds()
// Desc: ���ϴ� ������ ��������� ã�� ��ȯ�ϴ� �Լ�
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