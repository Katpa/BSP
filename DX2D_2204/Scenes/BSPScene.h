#pragma once

class BSPScene :public Scene
{
public:
	BSPScene();
	~BSPScene();

	virtual void Update() override;
	virtual void Render() override;

private:
	BSP* bsp;
	Quad* guideBoard;
};