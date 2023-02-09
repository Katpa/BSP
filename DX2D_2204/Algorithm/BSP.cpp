#include "Framework.h"

BSP::BSP(Vector2 worldSize)
	:curTime(0)
{
	head = new BSPNode(worldSize, Vector2(CENTER_X, CENTER_Y));
	curLevelNodes.push(head);

	//여기서 타일갯수 정해주고 인스턴스 리사이즈해준다.
	//한 칸당 10*10
	gridTexture = new Quad(L"Textures/tiles.png", Vector2(), Vector2(1.0f / 4.0f, 1.0f));
	gridTexture->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
	gridTexture->SetPixelShader(L"Shaders/PixelInstancing.hlsl");
	Vector2 amount = worldSize * 0.1f;
	UINT gridCnt = amount.x * amount.y;

	float startPosX = CENTER_X - worldSize.x * 0.5f;
	float startPosY = CENTER_Y - worldSize.y * 0.5f;

	instances.resize(gridCnt);
	poses.resize(gridCnt);

	for (int i = 0; i < instances.size(); i++)
	{
		Transform transform;

		transform.Position().x = startPosX + 10.0f * (i % (int)amount.x) + 5.0f;
		transform.Position().y = startPosY + 10.0f * (i / (int)amount.x) + 5.0f;
		poses[i] = transform.Position();

		transform.UpdateWorld();

		instances[i].transform = XMMatrixTranspose(transform.GetWorld());

		instances[i].maxFrame = { 4, 1 };
		instances[i].curFrame.x = 0;
		instances[i].curFrame.y = 0;
	}

	instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), instances.size());
}

BSP::~BSP()
{
	delete head;
	delete instanceBuffer;
	delete gridTexture;
}

void BSP::Update()
{
	switch (mode)
	{
	case BSP::Mode::NONE:
		NoneMode();
		break;
	case BSP::Mode::MANUAL:
		ManualMode();
		break;
	case BSP::Mode::AUTO:
		AutoMode();
		break;
	}
}

void BSP::Render()
{
	instanceBuffer->Set(1);
	gridTexture->SetRender();
	DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);

	for (int i = 0; i < curLevelNodes.size(); i++)
	{
		BSPNode* tmp = curLevelNodes.front();
		tmp->Render();
		curLevelNodes.pop();
		curLevelNodes.push(tmp);
	}
}

//--------------------------------------------------------------------------
// Code: void Partitioning()
// Desc: 공간을 나누는 함수
//--------------------------------------------------------------------------
void BSP::Partitioning()
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

//--------------------------------------------------------------------------
// Code: void Generate()
// Desc: 최종으로 나뉜 영역들 안에 각 방 1개 씩 생성하는 함수
//--------------------------------------------------------------------------
void BSP::Generate()
{
	if (isGenerated) return;

	int levelSize = curLevelNodes.size();

	for (int i = 0; i < levelSize; i++)
	{
		BSPNode* tmpNode = curLevelNodes.front();
		curLevelNodes.pop();

		Vector2 areaSize = tmpNode->Area()->Size();
		Vector2 areaPos = tmpNode->Area()->GlobalPosition();
		Vector2 roomSize = Vector2(Random(areaSize.x * 0.6f, areaSize.x * 0.8f), Random(areaSize.y * 0.6f, areaSize.y * 0.8f));

		Vector2 lb = areaPos - roomSize * 0.5f;
		Vector2 rt = areaPos + roomSize * 0.5f;

		for (int i = 0; i < instances.size(); i++)
		{
			if (poses[i].x >= lb.x && poses[i].y >= lb.y &&
				poses[i].x <= rt.x && poses[i].y <= rt.y)
			{
				instances[i].curFrame.x = 1;
			}
		}

		curLevelNodes.push(tmpNode);
	}

	instanceBuffer->Update(instances.data(), instances.size());

	isGenerated = true;
}

//--------------------------------------------------------------------------
// Code: void LinkPath()
// Desc: 복도를 넣는 함수
//--------------------------------------------------------------------------
void BSP::LinkPath()
{
	return;

	int levelSize = curLevelNodes.size();

	for (int i = 0; i < levelSize; i++)
	{
		BSPNode* tmpNode = curLevelNodes.front();
		curLevelNodes.pop();

		tmpNode->SetPath();

		if (i & 1)
			curLevelNodes.push(tmpNode->Parent());
	}

	instanceBuffer->Update(instances.data(), instances.size());
}

void BSP::NoneMode()
{
	if (KEY_DOWN('Q'))
		mode = Mode::MANUAL;

	if (KEY_DOWN('W'))
		mode = Mode::AUTO;
}

void BSP::ManualMode()
{
	if (KEY_DOWN('1'))
		Partitioning();
	else if (KEY_DOWN('2'))
		Generate();
	else if (KEY_DOWN('3'))
	{
		LinkPath();
		mode = Mode::NONE;
	}
}

void BSP::AutoMode()
{
	curTime += DELTA;

	if (curTime < INTERVAL) return;
	curTime -= INTERVAL;


}
