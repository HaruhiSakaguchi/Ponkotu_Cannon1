#include "Map.h"
#include "COLLISION_MAP.h"
#include "FILE_BUFFER.h"
#include "NAME.h"
#include "rand.h"
#include "window.h"
#include <sstream>
#include "UILog.h"


Map::Map(class Game* game)
	:Actor(game)
	, mMiniMap(nullptr)
	, mProgress(nullptr)
	, mLog(nullptr)
	, mMaxX(nullptr)
	, mMaxZ(nullptr)
	, mMinX(nullptr)
	, mMinZ(nullptr)
	, mAreaNum(0)
	, mSecond(0)
	, mMunite(0)
	, mInitItemDropFlag(false)
	, mClearFlag(false)
{
	Data = GetGame()->GetAllData()->mapData;
	Data.mStageNum = (int)(GetGame()->GetPhase() + 1);
	mStart = std::chrono::system_clock::now();
	mLog = new UILog(GetGame());
}

Map::~Map()
{
	delete GetGame()->GetCollisionMap();
	GetGame()->SetCollisionMap(nullptr);
	delete mMiniMap;
	delete mProgress;
	delete mLog;
	GetGame()->SetStage(nullptr);
	while (!mTexts.empty())
	{
		mTexts.pop_back();
	}
}

void Map::AddCollisions(MODEL_COLLISION* model)
{
	mModelCollisions.emplace_back(model);
}

void Map::RemoveCollisions(MODEL_COLLISION* model)
{
	auto iter = std::find(mModelCollisions.begin(), mModelCollisions.end(), model);
	if (iter != mModelCollisions.end())
	{
		std::iter_swap(iter, mModelCollisions.end() - 1);
		mModelCollisions.pop_back();
	}
}

void Map::CreateCollisions(const char* filename)
{
	FILE_BUFFER fb(filename);

	NAME name;
	int cntIndices = 0;
	fb.readString();
	while (!fb.end()) {
		if (fb == 'i') {
			name = fb.readString();
			int numIndices = fb.readInt();
			cntIndices++;
		}
		if (fb == 'v') {
			NAME vName = fb.readString();
			if (name != vName) {
				WARNING(1, "COLLISION_MAP::loadData()", "iの次にvが来ていない");
			}
		}
		fb.readString();
	}

	//２週目でTriangleをつくる
	fb.restart();
	int* indices;
	int numIndices;
	VECTOR* vertices;
	int numVertices;
	int idx = 0;
	int numTriangles = 0;
	while (cntIndices > 0) {
		//index
		fb.readString();
		while (fb != 'i')fb.readString();
		name = fb.readString();
		numIndices = fb.readUnsigned();
		numTriangles = numIndices / 3;
		indices = new int[numIndices];
		for (int i = 0; i < numIndices; i++)indices[i] = fb.readUnsigned();
		//vertex
		fb.readString();
		while (fb != 'v')fb.readString();
		fb.readString();
		fb.readString();
		numVertices = fb.readUnsigned();
		vertices = new VECTOR[numVertices];

		for (int i = 0; i < numVertices; i++) {
			vertices[i].x = fb.readFloat();
			vertices[i].y = fb.readFloat();
			vertices[i].z = fb.readFloat();
		}

		new MODEL_COLLISION(this, numTriangles, numVertices, indices, vertices, name);

		SAFE_DELETE_ARRAY(indices);
		SAFE_DELETE_ARRAY(vertices);

		cntIndices--;
	}
}

void Map::Draw()
{
	noStroke();
	fill(Data.mStageNumRectColor);
	rectMode(CORNER);
	rect(Data.mStageNumRectPos.x, Data.mStageNumRectPos.y, Data.mStageNumTextSize * 3.0f, Data.mStageNumTextSize);
	fill(Data.mStageNumTextColor);
	textSize(Data.mStageNumTextSize);
	text("Stage" + (let)Data.mStageNum, Data.mStageNumTextPos.x, Data.mStageNumTextPos.y);
}

void Map::CreateArea(float* maxX, float* maxZ, float* minX, float* minZ, int areaNum)
{
	//マップの範囲情報を作る
	mAreaNum = areaNum;
	mMaxX = maxX;
	mMaxZ = maxZ;
	mMinX = minX;
	mMinZ = minZ;

	float* AreaMaxX = new float[mAreaNum];
	float* AreaMinX = new float[mAreaNum];
	float* AreaMaxZ = new float[mAreaNum];
	float* AreaMinZ = new float[mAreaNum];

	for (int i = 0; i < mAreaNum; i++)
	{
		AreaMaxX[i] = mMaxX[i];
		AreaMinX[i] = mMinX[i];
		AreaMaxZ[i] = mMaxZ[i];
		AreaMinZ[i] = mMinZ[i];
	}

	//X、Z座標の最大値、最小値をソートした後、取り出す
	Sort(AreaMaxX, 0, mAreaNum - 1);
	Sort(AreaMinX, 0, mAreaNum - 1);
	Sort(AreaMaxZ, 0, mAreaNum - 1);
	Sort(AreaMinZ, 0, mAreaNum - 1);

	Data.mStageMaxX = AreaMaxX[mAreaNum - 1];
	Data.mStageMinX = AreaMinX[0];
	Data.mStageMaxZ = AreaMaxZ[mAreaNum - 1];
	Data.mStageMinZ = AreaMinZ[0];

	delete[]AreaMaxX;
	delete[]AreaMinX;
	delete[]AreaMaxZ;
	delete[]AreaMinZ;
}

void Map::UpdateActor()
{
	std::ostringstream oss;

	if (GetGame()->GetScene() == Game::EPlay)
	{
		auto end = std::chrono::system_clock::now();
		auto dur = end - mStart;
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		float time = msec / 1000.0f;

		mMunite = (int)time / 60;
		mSecond = (int)time - 60 * mMunite;
	}

	const char* tenMunite = "";
	const char* tenSecond = "";
	if (mSecond < 10)
	{
		tenSecond = "0";
	}
	if (mMunite < 10)
	{
		tenMunite = "0";
	}
	oss << tenMunite << mMunite << ":" << tenSecond << mSecond;

	mTimeText = oss.str();

	if (!mInitItemDropFlag)
	{
		mInitItemDropFlag = true;
		GetLog()->AddText("Mapを生成。");
		GetLog()->AddText("Cannonが出現。");
		for (int i = 0; i < 3; i++)
		{
			DropItems(VECTOR(((float)i - 1.0f) * 2.0f, 5.0f, -2.0f));
		}
	}

	if (GetClearCnt() <= 0 && !mClearFlag)
	{
		mClearFlag = true;
		GetLog()->AddText("すべての敵を倒した！！");
	}

	if (GetClearCnt() - GetGame()->GetEnemies().size() > 0 && GetGame()->GetEnemies().size() != Data.mStageCharacterCapa)
	{
		VECTOR pos = VECTOR(random(GetStageMinX(), GetStageMaxX()), random(4.0f, 7.5f), random(GetStageMinZ(), GetStageMaxZ()));
		if (GetGame()->GetPhase() == Game::FIRST)
		{
			if (PositionOnMap(pos, GetGame()->GetAllData()->tamaData.mRadius) && !PositionOnMapArea0(pos, GetGame()->GetAllData()->tamaData.mRadius))
			{
				Tama* tama = new Tama(GetGame());
				tama->SetPosition(pos);
				GetLog()->AddText("Tamaが出現。");
			}
		}
		else if (GetGame()->GetPhase() == Game::SECOND)
		{
			if (PositionOnMap(pos, GetGame()->GetAllData()->satelliteData.mHeight) && !PositionOnMapArea0(pos, GetGame()->GetAllData()->satelliteData.mHeight))
			{
				Satellite* satellite = new Satellite(GetGame(), pos);
				CharacterActor::SEGMENT* seg = new CharacterActor::SEGMENT(satellite);
				satellite->SetSeg(seg);
				if (satellite->GetId() == 0)
				{
					GetGame()->GetStage()->GetLog()->AddText("SatelliteAが出現。");
				}
				else
				{
					GetGame()->GetStage()->GetLog()->AddText("SatelliteBが出現。");
				}
			}
		}
		else if (GetGame()->GetPhase() == Game::THIRD)
		{
			int num = random();
			if (num % 4 == 0 || num % 4 == 2 || num % 4 == 3)
			{
				if (PositionOnMap(pos, GetGame()->GetAllData()->tamaData.mRadius) && !PositionOnMapArea0(pos, GetGame()->GetAllData()->tamaData.mRadius))
				{
					Tama* tama = new Tama(GetGame());
					tama->SetPosition(pos);
					GetLog()->AddText("Tamaが出現。");
				}
			}
			else
			{
				if (PositionOnMap(pos, GetGame()->GetAllData()->satelliteData.mHeight) && !PositionOnMapArea0(pos, GetGame()->GetAllData()->satelliteData.mHeight))
				{
					Satellite* satellite = new Satellite(GetGame(), pos);
					CharacterActor::SEGMENT* seg = new CharacterActor::SEGMENT(satellite);
					satellite->SetSeg(seg);
					if (satellite->GetId() == 0)
					{
						GetGame()->GetStage()->GetLog()->AddText("SatelliteAが出現。");
					}
					else
					{
						GetGame()->GetStage()->GetLog()->AddText("SatelliteBが出現。");
					}
				}
			}
		}
	}

}

class UILog* Map::GetLog() { return mLog; }

void Map::AddText(std::string string)
{
	std::ostringstream oss;
	oss << mTimeText.c_str() << " " << string.c_str();
	mTexts.emplace_back(oss.str());
	if (mTexts.size() > (int)(GetGame()->GetAllData()->logData.mHeight / GetGame()->GetAllData()->logData.mTextSize))
	{
		RemoveText(0);
	}
}

void Map::RemoveText(int num)
{
	//num番目のナンバーを取り除く
	auto iter = std::find(mTexts.begin(), mTexts.end(), mTexts[num]);
	if (iter != mTexts.end())
	{
		mTexts.erase(iter);
	}
}