#pragma once

class BSPScene :public Scene
{
public:
	BSPScene();
	~BSPScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;

private:
	BSP* bsp;
};