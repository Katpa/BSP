#include "Framework.h"
#include "BSPScene.h"

BSPScene::BSPScene()
{
	Vector2 tmpWorldMapSize;
	bsp = new BSP(tmpWorldMapSize);
}

BSPScene::~BSPScene()
{
	delete bsp;
}

void BSPScene::Update()
{
	bsp->Generate();
}

void BSPScene::Render()
{
}
