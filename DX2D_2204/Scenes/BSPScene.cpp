#include "Framework.h"
#include "BSPScene.h"

BSPScene::BSPScene()
{
	Vector2 tmpWorldMapSize = { CENTER_X, CENTER_Y + 200 };
	bsp = new BSP(tmpWorldMapSize);

	Texture* tex = Texture::Add(L"Textures/Guide.png");
	Vector2 size = tex->GetSize();
	guideBoard = new Quad(size, Vector2(size.x * 0.5f + 10, WIN_HEIGHT - size.y * 0.5f - 10));
	guideBoard->SetTexture(tex);
	guideBoard->UpdateWorld();
}

BSPScene::~BSPScene()
{
	delete bsp;
	delete guideBoard;
}

void BSPScene::Update()
{
	bsp->Update();
}

void BSPScene::Render()
{
	bsp->Render();
	guideBoard->Render();
}
