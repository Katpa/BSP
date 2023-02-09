#pragma once

/// ----------------------------------------------------------
/// �۾� ����
/// 1. Ư�� ������ �����ϱ� ������ ������ ��������� ������.
///		1-1. ���� ������ 5:5 ������ �ƴϿ��� �Ѵ�.
///		1-2. �� ���� ���̸� ���� �� ���� �켱������ ������.
///		1-3. ���� ������ ���� ���簢���� ����̿��� �Ѵ�.
///		1-4. ������ �������� ������ ������ �°� ���� Ʈ�� ������ �����Ѵ�.
/// 2. �� ������ ����(��)�� �����.
/// 3. ������ ���� Ʈ�� ������ ���� ��(����)�� �����Ѵ�.
/// ----------------------------------------------------------

/// ----------------------------------------------------------
/// ���� ��� 2������ ����
/// 1. �������
///		- Q ������ �������� ����
///			- 1 ������ ����
///			- 2 ������ �����
///			- 3 ������ ��� ����
/// 2. �ڵ����
///		- W ������ �ڵ����� ����
///			- �ֱ������� �� �ܰ踦 �����Ѵ�
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