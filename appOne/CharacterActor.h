#pragma once
#include "Actor.h"
#include "MATRIX.h"
#include "Particle.h"
#include <string>
#include "CapsuleComponent.h"

class CharacterActor :public Actor
{
public:
	CharacterActor(class Game* game);
	virtual ~CharacterActor();
	void Update()override;
	virtual void Damage(int damage = 1) {};
	const VECTOR& GetCapsulOffset() { return mCapsulOffset; }
	void SetCapsulOffset(const VECTOR& offset) { mCapsulOffset = offset; }

	//フィールドとの当たり判定用データ
	float GetRadius() const { return mRadius; }
	float GetHeight() const { return mHeight; }
	void SetRadius(float rad) { mRadius = rad; }
	void SetHeight(float heigh) { mHeight = heigh; }

	virtual void SetJumpFlag(int flag) { mJumpFlag = flag; }
	virtual int GetJumpFlag() { return mJumpFlag; }
	virtual void SetJumpVel(float vel) { mJumpVel = vel; }
	virtual float GetJumpVel() { return mJumpVel; }
	virtual void SetAdvSpeed(float speed) { mAdvSpeed = speed; }
	virtual float GetAdvSpeed() { return mAdvSpeed; }
	virtual float GetGravity() { return mGravity; }
	virtual void SetGravity(float grav) { mGravity = grav; }

	float GetOffsetY() { return mOffsetY; }
	void SetHp(int hp) { mHp = hp; }
	int GetHp() { return mHp; }
	int GetMaxHp() { return mMaxHp; }
	void SetMaxHp(int hp) { mMaxHp = hp; }
	float GetSlant() const { return mSlant; }

	//モデル回転関数
	//xyCompolete 通常、y軸の回転が完了すれば、x軸の回転が終わっていなくても完了させる。それに関して、x軸も条件に含めるかを判別するフラッグ
	int rotate(VECTOR* angle, const VECTOR& dir, float rotSpeed,bool xyComplete = false);

	float GetDamageInterval() { return mDamageInterval; }
	void SetDamageInterval(float interval) { mDamageInterval = interval; }
	const COLOR& GetDamageColor() { return mDamageColor; }

	//mDamageInterval > 0の時に呼び出す。Damageに何か付け足したいときのもの。
	virtual void DamageOption() {};
	//mPosition.y < -45なら呼び出す。
	virtual void FallOption() {};

	//表示する際の点の色
	const COLOR& GetImageColor() { return mImageColor; }
	void SetImageColor(const COLOR& color) { mImageColor = color; }

	std::string GetName() { return mName; }
	void SetName(const char* name) { mName = name; }
	const int GetLevel() { return mLevel; }
	void SetLevel(int level) { mLevel = level; }
	int GetMaxLevel() { return mMaxLevel; }
	bool GetOnMapFlag() { return mOnMapFlag; }
	int GetInitMaxHp() { return mInitMaxHp; }
	void SetInitMaxHp(int hp) { mInitMaxHp = hp; }

	int GetDamage() { return mDamage; }
	int GetRDamage() { return mRDamage; }
	void SetDamage(int damage) { mDamage = damage; }
	void SetRDamage(int damage) { mRDamage = damage; }

	//メッシュの名前からパーティクルをスポーン
	
	void SpawnParticle(class Game*game,const VECTOR& pos, const char* name,int num = 1, float maxLifeSpan = 1.0f, Particle::MeshType type = Particle::MeshType::EBatch,float spawnRadius = 0.0f);
	
	//当たり判定コンポーネント
	class CapsuleComponent* GetCapsule() { return mCapsule; }

	enum class CharactersTag
	{
		ECannon,
		ESatellite,
		ETama,
		EBarricade,
		EPHome,
		EEHome,
		EWaepon,
		EOthers
	};

	enum class CharactersTag GetTag() { return mTag; }
	void SetTag(enum class CharactersTag tag) { mTag = tag; }

private:
	VECTOR mCapsulOffset;
	float mHeight;
	float mRadius;
	float mSlant;
	int  mJumpFlag;
	float mJumpVel;
	float mGravity;
	float mAdvSpeed;
	float mDamageInterval;
	float mOffsetY;
	int mLevel;
	int mMaxLevel;
	//与えるダメージ
	int mDamage;
	//受けるダメージ
	int mRDamage;
	COLOR mDamageColor;
	//ミニマップ用カラー　アクターごとに持たせる
	COLOR mImageColor;
	//マップエリア内にいるか
	bool mOnMapFlag;
	int mHp;
	int mMaxHp;
	int mInitMaxHp;
	std::string mName;
	enum class CharactersTag mTag;
protected:
	int mDeadSound;
	class CapsuleComponent* mCapsule;
};