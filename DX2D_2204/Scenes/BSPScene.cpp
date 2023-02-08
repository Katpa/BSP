#include "Framework.h"
#include "BSPScene.h"

BSPScene::BSPScene()
{
	Vector2 tmpWorldMapSize = { CENTER_X, CENTER_Y };
	bsp = new BSP(tmpWorldMapSize);
}

BSPScene::~BSPScene()
{
	delete bsp;
}

void BSPScene::Update()
{
	bsp->Update();
}

void BSPScene::Render()
{
	bsp->Render();
}
