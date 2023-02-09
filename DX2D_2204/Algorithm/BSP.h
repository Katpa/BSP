#pragma once

/// ----------------------------------------------------------
/// 작업 순서
/// 1. 특정 조건을 만족하기 전까지 공간을 재귀적으로 나눈다.
///		1-1. 나눌 때에는 5:5 비율이 아니여야 한다.
///		1-2. 각 변의 길이를 따져 긴 쪽을 우선적으로 나눈다.
///		1-3. 나뉜 공간은 필히 직사각형의 모양이여야 한다.
///		1-4. 나누는 과정에서 나누는 순서에 맞게 이진 트리 구조로 저장한다.
/// 2. 각 공간에 지형(방)을 만든다.
/// 3. 저장한 이진 트리 구조에 따라 길(복도)를 연결한다.
/// ----------------------------------------------------------

/// ----------------------------------------------------------
/// 진행 모드 2가지로 개발
/// 1. 수동모드
///		- Q 누르면 수동모드로 진입
///			- 1 누르면 분할
///			- 2 누르면 방생성
///			- 3 누르면 경로 연결
/// 2. 자동모드
///		- W 누르면 자동모드로 진입
///			- 주기적으로 각 단계를 진행한다
/// ----------------------------------------------------------

class BSP
{
private:
	enum class Mode
	{
		NONE, MANUAL, AUTO
	};

	struct InstanceData
	{
		Matrix transform;
		Vector2 maxFrame;
		Vector2 curFrame;
		//Float4 color;
	};

	const float INTERVAL = 3.0f;

public:
	BSP(Vector2 worldSize);
	~BSP();

	void Update();
	void Render();

	void Save();

private:
	void Partitioning();
	void Generate();
	void LinkPath();

	void NoneMode();
	void ManualMode();
	void AutoMode();

private:
	Quad* gridTexture;
	vector<Vector2> poses;
	vector<InstanceData> instances;
	VertexBuffer* instanceBuffer;

	BSPNode* head;
	queue<BSPNode*> curLevelNodes;

	Mode mode = Mode::NONE;

	Vector2 worldIndex;
	float curTime;


	bool isGenerated = false;
};