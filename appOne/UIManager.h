#pragma once
#include <vector>

class UIManager
{

public:

	UIManager(class Game* game) :mGame(game), mUpdatingUIs(false) {};
	~UIManager();
	void Update();
	void ProcessInput();
	void PushUI(class UIScreen* uiScreen);
	void PullUI(class UIScreen* uiScreen);

	void AddUIPSide(class UIPSideCharacterStatusBase* ui);
	void RemoveUIPSide(class UIPSideCharacterStatusBase* ui);
	void UIClear();
	const class std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	const std::vector<class UIPSideCharacterStatusBase*>& GetUIPSideStatus() { return mUIStatus; }

private:
	std::vector<class UIScreen*>mUIStack;
	std::vector<class UIPSideCharacterStatusBase*>mUIStatus;
	class Game* mGame;
	bool mUpdatingUIs;
	std::vector<class UIScreen*> mPendingUIs;

};

