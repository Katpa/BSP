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

	//그냥 이거 2차원 벡터로 쓸까?? 괜히 렌더 때마다 프레임 잡아 먹을 것같은데;;;
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