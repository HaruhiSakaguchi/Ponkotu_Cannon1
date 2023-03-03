#include "UIHelpBase.h"

UIHelpBase::UIHelpBase(class Game* game)
	:UIScreen(game)
	, mBackImg(-1)
{
	AddButton(
		"~",
		[this]() {
			CloseMe();
		}
		, 2
			, "ƒwƒ‹ƒv‚ğ•Â‚¶‚Ü‚·"
			, VECTOR2(width / 2 - 50.0f, -100.0f - 34.5f)
			);
}

void UIHelpBase::draw()
{
	rectMode(CORNER);
	if (mBackImg != -1)
	{
		image(mBackImg, 0.0f, 0.0f);
	}
	rectMode(CENTER);
}
