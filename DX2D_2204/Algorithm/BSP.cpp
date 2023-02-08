#include "Framework.h"

BSP::BSP(Vector2 worldSize)
{
	head = new BSPNode(worldSize, Vector2(CENTER_X, CENTER_Y));
	curLevelNodes.push(head);
}

BSP::~BSP()
{
	for (Quad*& room : rooms)
		delete room;

	rooms.clear();

	delete head;
}

void BSP::Update()
{

}

void BSP::Render()
{
	for (Quad* room : rooms)
		room->Render();

	//�׳� �̰� 2���� ���ͷ� ����?? ���� ���� ������ ������ ��� ���� �Ͱ�����;;;
	for (int i = 0; i < curLevelNodes.size(); i++)
	{
		BSPNode* tmp = curLevelNodes.front();
		tmp->Render();
		curLevelNodes.pop();
		curLevelNodes.push(tmp);
	}
}

void BSP::Generate()
{
	int levelSize = curLevelNodes.size();

	for (int i = 0; i < levelSize; i++)
	{
		BSPNode* tmpNode = curLevelNodes.front();
		curLevelNodes.pop();

		tmpNode->Partitioning();

		for (BSPNode* child : tmpNode->Childs())
			curLevelNodes.push(child);
	}
}