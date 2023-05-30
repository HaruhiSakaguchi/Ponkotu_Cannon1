#pragma once
#include "UIScreen.h"
class UIPSideCharacterStatusBase :
	public UIScreen
{
public:

	UIPSideCharacterStatusBase(class PSideCharacterActor* owner);
	virtual  ~UIPSideCharacterStatusBase();
	class PSideCharacterActor* GetOwner() { return mOwner; }
	const VECTOR2& GetPosition() { return mPosition; }
	void SetPosition(const VECTOR2& pos) { mPosition = pos; }
	const VECTOR2& GetOffset() { return mOffset; }
	void SetOffset(const VECTOR2& pos) { mOffset = pos; }
protected:
	PSideCharacterActor* mOwner;
	VECTOR2 mPosition;
	VECTOR2 mOffset;
	Button* mLvUpButton;
};

