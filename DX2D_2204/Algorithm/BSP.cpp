#include "Framework.h"

BSP::BSP(Vector2 worldSize)
	:curTime(0)
{
	head = new BSPNode(worldSize, Vector2(CENTER_X, CENTER_Y));
	curLevelNodes.push(head);

	//한 칸당 10*10
	gridTexture = new Quad(L"Textures/tiles.png", Vector2(), Vector2(1.0f / 4.0f, 1.0f));
	gridTexture->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
	gridTexture->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

	//타일 텍스처 사이즈가 달라지면 변경해야합니다.
	worldIndex = worldSize * 0.1f;

	UINT gridCnt = worldIndex.x * worldIndex.y;

	float startPosX = CENTER_X - worldSize.x * 0.5f;
	float startPosY = CENTER_Y - worldSize.y * 0.5f;

	instances.resize(gridCnt);
	poses.resize(gridCnt);

	for (int i = 0; i < instances.size(); i++)
	{
		Transform transform;

		transform.Position().x = startPosX + 10.0f * (i % (int)worldIndex.x) + 5.0f;
		transform.Position().y = startPosY + 10.0f * (i / (int)worldIndex.x) + 5.0f;
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

	if (KEY_DOWN('E'))
		renderArea = !renderArea;
}

void BSP::Render()
{
	instanceBuffer->Set(1);
	gridTexture->SetRender();
	DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);

	if (!renderArea) return;

	for (int i = 0; i < renderNodes.size(); i++)
	{
		BSPNode* tmp = renderNodes.front();
		tmp->Render();
		renderNodes.pop();
		renderNodes.push(tmp);
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

	renderNodes = curLevelNodes;
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
		Vector2 roomSize = Vector2(Random(areaSize.x * 0.5f, areaSize.x * 0.8f), Random(areaSize.y * 0.5f, areaSize.y * 0.8f));

		Vector2 lb = areaPos - roomSize * 0.5f;
		Vector2 rt = areaPos + roomSize * 0.5f;

		UINT start = 0;
		UINT end = 0;

		for (int i = 0; i < instances.size(); i++)
		{
			if (poses[i].x >= lb.x && poses[i].y >= lb.y &&
				poses[i].x <= rt.x && poses[i].y <= rt.y)
			{
				instances[i].curFrame.x = 1;
				
				end = i;
				if (start == 0) start = i;
			}
		}

		tmpNode->SetRoom(start, end, (UINT)worldIndex.x);

		if(i & 1)
			curLevelNodes.push(tmpNode->Parent());
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
	while (!curLevelNodes.empty())
	{
		int levelSize = curLevelNodes.size();

		for (int i = 0; i < levelSize; i++)
		{
			BSPNode* tmpNode = curLevelNodes.front();
			curLevelNodes.pop();

			vector<UINT> pathVec;
			tmpNode->SetPath(worldIndex.x, pathVec);

			for (UINT path : pathVec)
			{
				instances[path].curFrame.x = 2;
			}

			if (i & 1)
				curLevelNodes.push(tmpNode->Parent());
		}
	}

	instanceBuffer->Update(instances.data(), instances.size());
}

//--------------------------------------------------------------------------
// Code: void NoneMode()
// Desc: 모드를 미지정한 초기 옵션 선택 상태 루프 함수
//--------------------------------------------------------------------------
void BSP::NoneMode()
{
	if (KEY_DOWN('Q'))
		mode = Mode::MANUAL;

	if (KEY_DOWN('W'))
		mode = Mode::AUTO;
}

//--------------------------------------------------------------------------
// Code: void ManualMode()
// Desc: 수동모드 루프 함수
//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
// Code: void AutoMode()
// Desc: 자동모드 루프 함수
//--------------------------------------------------------------------------
void BSP::AutoMode()
{
	curTime += DELTA;

	if (curTime < INTERVAL) return;
	curTime -= INTERVAL;

	if (autoNum < AUTO_PARTITION)
		Partitioning();
	else if (autoNum == AUTO_PARTITION)
		Generate();
	else if (autoNum == AUTO_PARTITION + 1)
		LinkPath();
	else if (autoNum == AUTO_PARTITION + 2)
		renderArea = false;
	else
		mode = Mode::NONE;

	autoNum++;
}
