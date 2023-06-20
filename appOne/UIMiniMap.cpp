#include "UIMiniMap.h"
#include "Game.h"
#include "PlayerHome.h"

UIMiniMap::UIMiniMap(class Game* game, Map* owner, bool scroll)
	:UIScreen(game)
	, mMap(owner)
	, mScroll(scroll)
	, mPositions(nullptr)
	, mWidths(nullptr)
	, mHeights(nullptr)
	, mMaxX(nullptr)
	, mMaxZ(nullptr)
	, mMinX(nullptr)
	, mMinZ(nullptr)
	, mAreaPoints(nullptr)
{
	Data = mGame->GetAllData()->miniMapData;

	switch (mGame->GetPhase())
	{
	case Game::FIRST:
		Data.mOffsetX = Data.mStage1OffsetX;
		Data.mOffsetY = Data.mStage1OffsetY;
		break;
	case Game::SECOND:
		Data.mOffsetX = Data.mStage2OffsetX;
		Data.mOffsetY = Data.mStage2OffsetY;
		break;
	case Game::THIRD:
		Data.mOffsetX = Data.mStage3OffsetX;
		break;
	default:
		break;
	}

	Create();
}

UIMiniMap::~UIMiniMap()
{
	delete[]mPositions;
	delete[]mWidths;
	delete[]mHeights;

	delete[]mMaxX;
	delete[]mMaxZ;
	delete[]mMinX;
	delete[]mMinZ;
}

void UIMiniMap::Create()
{
	auto models = mMap->GetModelCollisions();

	for (auto model : models)
	{
		Data.mNumModels++;
	}

	VECTOR** pos = new VECTOR * [Data.mNumModels];
	VECTOR** vector = new VECTOR * [Data.mNumModels];

	for (int i = 0; i < Data.mNumModels; i++)
	{
		vector[i] = new VECTOR[2];
	}

	int k = 0;

	for (int k = 0; k < Data.mNumModels; k++)
	{
		pos[k] = new VECTOR[4];
		for (int i = 0; i < 4; i++)
		{
			pos[k][i] = models[k]->GetOPosition()[i];
		}

		vector[k][0] = pos[k][1] - pos[k][0];
		vector[k][1] = pos[k][2] - pos[k][1];
	}


	int floorNum = 0;
	for (int i = 0; i < Data.mNumModels; i++)
	{
		VECTOR N = cross(vector[i][0], vector[i][1]);
		N.normalize();
		if (N.y != 0)
		{
			floorNum++;
		}
	}

	mPositions = new VECTOR2[floorNum];
	mWidths = new float[floorNum];
	mHeights = new float[floorNum];
	mMaxX = new float[floorNum];
	mMaxZ = new float[floorNum];
	mMinX = new float[floorNum];
	mMinZ = new float[floorNum];

	mAreaPoints = new VECTOR * [floorNum];
	for (int i = 0; i < floorNum; i++)
	{
		mAreaPoints[i] = new VECTOR[4];
	}

	k = 0;
	for (int i = 0; i < Data.mNumModels; i++)
	{
		VECTOR N = cross(vector[i][0], vector[i][1]);
		N.normalize();
		if (N.y != 0)
		{
			float px[4];
			float py[4];
			float pz[4];

			for (int k = 0; k < 4; k++)
			{
				px[k] = pos[i][k].x;
				py[k] = pos[i][k].y;
				pz[k] = pos[i][k].z;
			}

			mMap->Sort(px, 0, 3);
			mMap->Sort(py, 0, 3);
			mMap->Sort(pz, 0, 3);

			mPositions[k].set(px[0] * Data.m3DCoordinate2DConvertRate, pz[0] * Data.m3DCoordinate2DConvertRate);
			mWidths[k] = abs((px[3] * Data.m3DCoordinate2DConvertRate) - (px[0] * Data.m3DCoordinate2DConvertRate));
			mHeights[k] = abs((pz[3] * Data.m3DCoordinate2DConvertRate) - (pz[0] * Data.m3DCoordinate2DConvertRate));

			for (int l = 0; l < 4; l++)
			{
				mAreaPoints[k][l] = VECTOR(px[l], py[l], pz[l]);
			}

			float* pointX = new float[4];
			float* pointY = new float[4];
			float* pointZ = new float[4];

			for (int l = 0; l < 4; l++)
			{
				pointX[l] = mAreaPoints[k][l].x;
				pointY[l] = mAreaPoints[k][l].y;
				pointZ[l] = mAreaPoints[k][l].z;
			}

			mMap->Sort(pointX, 0, 3);
			mMap->Sort(pointY, 0, 3);
			mMap->Sort(pointZ, 0, 3);

			mMaxX[k] = pointX[3];
			mMaxZ[k] = pointZ[3];
			mMinX[k] = pointX[0];
			mMinZ[k] = pointZ[0];

			delete[]pointX;
			delete[]pointY;
			delete[]pointZ;

			k++;
		}
	}

	float* tmpPositionX = new float[floorNum];
	float* tmpPositionY = new float[floorNum];
	float* tmpHeights = new float[floorNum];
	float* tmpWidths = new float[floorNum];

	for (int i = 0; i < floorNum; i++)
	{
		tmpPositionX[i] = mPositions[i].x;
		tmpPositionY[i] = mPositions[i].y;
		tmpHeights[i] = mHeights[i];
		tmpWidths[i] = mWidths[i];
	}

	int left = 0;
	int right = floorNum - 1;

	mMap->Sort(tmpPositionX, left, right);
	mMap->Sort(tmpPositionY, left, right);
	mMap->Sort(tmpWidths, left, right);
	mMap->Sort(tmpHeights, left, right);

	float minWidth = tmpWidths[left];
	float maxWidth = tmpWidths[right];
	float minHeight = tmpHeights[left];
	float maxHeight = tmpHeights[right];

	float MinX = tmpPositionX[left];
	float MaxX = tmpPositionX[right] + maxWidth;
	float MinY = tmpPositionY[left];
	float MaxY = tmpPositionY[right] + maxHeight;

	Data.mMinPosX = MinX;
	Data.mMinPosY = MinY;

	Data.mMiniMapWindowWidth = MaxX - MinX;
	Data.mMiniMapWindowHeight = MaxY - MinY - 50.0f;

	delete[]tmpPositionX;
	delete[]tmpPositionY;
	delete[]tmpWidths;
	delete[]tmpHeights;

	for (int i = 0; i < Data.mNumModels; i++)
	{
		delete[] pos[i];
	}

	delete[] pos;

	for (int i = 0; i < Data.mNumModels; i++)
	{
		delete[] vector[i];
	}

	delete[] vector;

	Data.mNumModels = floorNum;

	for (int i = 0; i < Data.mNumModels; i++)
	{
		delete[] mAreaPoints[i];
	}

	delete[]mAreaPoints;

	mMap->CreateArea(mMaxX, mMaxZ, mMinX, mMinZ, floorNum);
}

void UIMiniMap::Draw()
{
	rectMode(CORNER);
	noStroke();
	fill(Data.mWindowColor);
	rect(Data.mMiniMapCornerPos.x, Data.mMiniMapCornerPos.y, Data.mMiniMapWindowLength, Data.mMiniMapWindowLength);

	//マップの四角形を表示するための計算
	//ミニマップの外側にはみ出ていたら補正してミニマップウィンドウの中に収める
	for (int i = 0; i < Data.mNumModels; i++)
	{
		rectMode(CORNER);
		noStroke();
		fill(Data.mFloorColor);

		float posX = 0.0f;
		float posY = 0.0f;
		float Width = 0.0f;
		float Height = 0.0f;
		float left = mPositions[i].x + Data.mMiniMapOffsetX + Data.mOffsetX;
		float right = left + mWidths[i];
		float head = mPositions[i].y + Data.mMiniMapOffsetY + Data.mOffsetY;
		float bottom = head + mHeights[i];

		//下だけ
		if (bottom <= Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength && head < Data.mMiniMapCornerPos.y)
		{
			posY = Data.mMiniMapCornerPos.y;
			Height = bottom - posY;
		}
		//上だけ
		else if (head >= Data.mMiniMapCornerPos.y && bottom > Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength)
		{
			posY = head;
			Height = Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength - posY;
		}
		else if (head >= Data.mMiniMapCornerPos.y && bottom <= Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength)//上下両方とも範囲内
		{
			posY = head;
			Height = mHeights[i];
		}
		else if (head < Data.mMiniMapCornerPos.y && bottom > Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength)//両方ともはみ出している
		{
			posY = Data.mMiniMapCornerPos.y;
			Height = Data.mMiniMapWindowLength;
		}

		if (right <= Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength && left < Data.mMiniMapCornerPos.x)//右だけ
		{
			posX = Data.mMiniMapCornerPos.x;
			Width = right - posX;
		}
		else if (left >= Data.mMiniMapCornerPos.x && right > Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength)//左だけ
		{
			posX = left;
			Width = Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength - posX;
		}
		else if (left >= Data.mMiniMapCornerPos.x && right <= Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength)//左右両方とも範囲内
		{
			posX = left;
			Width = mWidths[i];
		}
		else if (left < Data.mMiniMapCornerPos.x && right > Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength)//両方ともはみ出している
		{
			posX = Data.mMiniMapCornerPos.x;
			Width = Data.mMiniMapWindowLength;
		}

		if ((Width > 0.0f && Height > 0.0f) && (posX >= Data.mMiniMapCornerPos.x && posY <= Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength))
		{
			rect(posX, posY, Width, Height);
		}

	}

	for (auto actor : mGame->GetActorManager()->GetCharacters())
	{
		VECTOR2 Pos(actor->GetPosition().x * Data.m3DCoordinate2DConvertRate + Data.mOffsetX + Data.mMiniMapOffsetX, actor->GetPosition().z * Data.m3DCoordinate2DConvertRate + Data.mOffsetY + Data.mMiniMapOffsetY);
		if (Pos.y <= Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength && Pos.y >= Data.mMiniMapCornerPos.y && Pos.x <= Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength && Pos.x >= Data.mMiniMapCornerPos.x)
		{
			if (actor->GetCategory() == Actor::Character)
			{
				if (Pos.y + Data.mItemSw <= Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength && Pos.y - Data.mItemSw >= Data.mMiniMapCornerPos.y && Pos.x + Data.mItemSw <= Data.mMiniMapCornerPos.x + Data.mMiniMapWindowLength && Pos.x - Data.mItemSw >= Data.mMiniMapCornerPos.x)
				{
					Arrow(Pos, actor->GetImageColor(), actor->GetRotation().y + 3.1415926f);
				}
			}
			else if (actor->GetCategory() == Actor::Item)
			{
				stroke(Data.mItemStrokeColor);
				strokeWeight(Data.mItemSw);
				fill(actor->GetImageColor());
				circle(Pos.x, Pos.y, Data.mItemCircleRadius);
			}
			else if (actor->GetCategory() == Actor::Weapon)
			{
				strokeWeight(Data.mWeaponSw);
				stroke(actor->GetImageColor());
				fill(actor->GetImageColor());
				point(Pos.x, Pos.y);
			}

		}
	}
}

void UIMiniMap::Update()
{
	//スクロール
	if (mScroll && mGame->GetPHome())
	{
		if (mGame->GetPHome()->GetPosition().z * Data.m3DCoordinate2DConvertRate + Data.mOffsetY + Data.mMiniMapWindowHeight / 3.0f > Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength / 2.0f && Data.mMiniMapCornerPos.y + Data.mMiniMapWindowHeight / 3.0f > Data.mMinPosY + Data.mMiniMapOffsetY + Data.mOffsetY)
		{
			Data.mMiniMapOffsetY = -(mGame->GetPHome()->GetPosition().z * Data.m3DCoordinate2DConvertRate + Data.mOffsetY) + (Data.mMiniMapCornerPos.y + Data.mMiniMapWindowLength / 2.0f);
		}
	}
}

void UIMiniMap::Arrow(const VECTOR2& pos, const COLOR& color, float angle)
{
	float per = Data.m3DCoordinate2DConvertRate;
	float sw = 0.5f;
	float pai = 3.1415926f;
	float OneDeg = pai / 180.0f;
	VECTOR2 p1, p2, p3, p4;
	stroke(color);
	strokeWeight(1);

	VECTOR2 p[6];
	strokeWeight(3);
	float ang[6];
	float radius = 95.0f / per;
	ang[0] = 89.0f;
	ang[1] = 91.0f;
	ang[2] = 239.0f;
	ang[3] = 241.0f;
	ang[4] = 299.0f;
	ang[5] = 301.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 90.0f / per;
	ang[0] = 88.0f;
	ang[1] = 92.0f;
	ang[2] = 238.0f;
	ang[3] = 242.0f;
	ang[4] = 298.0f;
	ang[5] = 302.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 85.0f / per;
	ang[0] = 87.0f;
	ang[1] = 93.0f;
	ang[2] = 237.0f;
	ang[3] = 242.5f;
	ang[4] = 298.5f;
	ang[5] = 303.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 80.0f / per;
	ang[0] = 86.0f;
	ang[1] = 94.0f;
	ang[2] = 236.0f;
	ang[3] = 242.5f;
	ang[4] = 298.5f;
	ang[5] = 304.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 75.0f / per;

	ang[0] = 85.0f;
	ang[1] = 95.0f;
	ang[2] = 235.0f;
	ang[3] = 243.0f;
	ang[4] = 297.0f;
	ang[5] = 305.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 70.0f / per;

	ang[0] = 84;
	ang[1] = 96;
	ang[2] = 234;
	ang[3] = 243.5f;
	ang[4] = 296.0f;
	ang[5] = 306.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}
	radius = 65.0f / per;

	ang[0] = 83.0f;
	ang[1] = 97.0f;
	ang[2] = 233.0f;
	ang[3] = 243.5f;
	ang[4] = 295.0f;
	ang[5] = 307.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}
	radius = 60.0f / per;

	ang[0] = 81.0f;
	ang[1] = 99.0f;
	ang[2] = 231.0f;
	ang[3] = 245.50f;
	ang[4] = 294.0f;
	ang[5] = 310.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 55.0f / per;

	ang[0] = 78.0f;
	ang[1] = 102.0f;
	ang[2] = 228.0f;
	ang[3] = 246.5f;
	ang[4] = 294.0f;
	ang[5] = 312.5f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 50.0f / per;

	ang[0] = 75.0f;
	ang[1] = 105.0f;
	ang[2] = 226.0f;
	ang[3] = 248.5f;
	ang[4] = 292.0f;
	ang[5] = 314.5f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}
	radius = 45.0f / per;

	ang[0] = 72.0f;
	ang[1] = 108.0f;
	ang[2] = 222.0f;
	ang[3] = 250.5f;
	ang[4] = 290.5f;
	ang[5] = 317.5f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 40.0f / per;

	ang[0] = 68;
	ang[1] = 112;
	ang[2] = 218;
	ang[3] = 252.50f;
	ang[4] = 287.5f;
	ang[5] = 322.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}
	radius = 35.0f / per;

	ang[0] = 61.0f;
	ang[1] = 119.0f;
	ang[2] = 210.0f;
	ang[3] = 256.50f;
	ang[4] = 284.0f;
	ang[5] = 330.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}
	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 30.0f / per;

	ang[0] = 50.0f;
	ang[1] = 130.0f;
	ang[2] = 200.0f;
	ang[3] = 260.0f;
	ang[4] = 280.0f;
	ang[5] = 340.0f;

	for (int i = 0; i < 6; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 3; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 25.0f / per;

	ang[0] = 10.0f;
	ang[1] = 170.0f;
	for (int i = 0; i < 2; i++)
	{
		p[i] = VECTOR2(pos.x + cosf(angle + OneDeg * ang[i]) * radius, pos.y - sinf(angle + OneDeg * ang[i]) * radius);
	}

	for (int i = 0; i < 1; i++)
	{
		Line(p[i * 2], p[i * 2 + 1]);
	}

	radius = 100.0f / per;
	ang[0] = 90.0f;
	ang[1] = 240.0f;
	ang[2] = 300.0f;
	ang[3] = 270.0f;

	p1 = VECTOR2(pos.x + cosf(angle + OneDeg * ang[0]) * radius
		, pos.y - sinf(angle + OneDeg * ang[0]) * radius);
	p2 = VECTOR2(pos.x + cosf(angle + OneDeg * ang[1]) * radius
		, pos.y - sinf(angle + OneDeg * ang[1]) * radius);
	p3 = VECTOR2(pos.x + cosf(angle + OneDeg * ang[2]) * radius
		, pos.y - sinf(angle + OneDeg * ang[2]) * radius);

	radius = 25.0f / per;

	p4 = VECTOR2(pos.x + cosf(angle + OneDeg * ang[3]) * radius
		, pos.y - sinf(angle + OneDeg * ang[3]) * radius);


	stroke(Data.mArrowStrokeColor);
	strokeWeight(sw);
	line(p1.x, p1.y, p2.x, p2.y);
	line(p2.x, p2.y, p4.x, p4.y);
	line(p4.x, p4.y, p3.x, p3.y);
	line(p3.x, p3.y, p1.x, p1.y);

}