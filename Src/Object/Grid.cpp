#include <DxLib.h>
#include "Grid.h"

Grid::Grid(void)
{
}

Grid::~Grid(void)
{
}

void Grid::Init(void)
{
}

void Grid::Update(void)
{
}

void Grid::Draw(void)
{

	//// Åyó˚èKÅzç≈èâÇÃÇPñ{
	//VECTOR sPos = { 0.0f, 0.0f, 0.0f };
	//VECTOR ePos = { HLEN, 0.0f, 0.0f };
	//DrawLine3D(sPos, ePos, 0xff0000);


	for (int x = 0; x < 20; x++)
	{

		VECTOR sPos = { -HLEN, 0.0f, -500.0f + TERM * x};
		VECTOR ePos = { HLEN, 0.0f, -500.0f + TERM * x};
		DrawLine3D(sPos, ePos, 0xff0000);

	}

	for (int z = 0; z < 20; z++)
	{

		VECTOR sPos = { -500.0f + TERM * z, 0.0f, HLEN };
		VECTOR ePos = { -500.0f + TERM * z, 0.0f, -HLEN};
		DrawLine3D(sPos, ePos, 0x0000ff);

	}

}

void Grid::Release(void)
{
}
