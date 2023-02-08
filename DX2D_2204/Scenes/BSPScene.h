#pragma once

class BSPScene :public Scene
{
public:
	BSPScene();
	~BSPScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;

private:
	BSP* bsp;
};