#include "TransitionFade.h"
#include "graphic.h"
#include "window.h"

TransitionFade::TransitionFade(class Game* game)
	:Transition(game)
	, mColor(0, 0, 0)
	, mTime(0.0f) 
{
	create();
}

void TransitionFade::create() {
	colorMode(RGB, 255);
	mColor = COLOR(0, 0, 0);
	mTime = 1.7f;
}

void TransitionFade::inTriggerProc() {
	rectMode(CORNER);
	colorMode(RGB, 255);
	noStroke();
	mColor.a = 255;
	fill(mColor);
	rect(0, 0, width, height);
}

bool TransitionFade::inProc() {
	rectMode(CORNER);
	colorMode(RGB, 255);
	noStroke();
	fill(mColor);
	rect(0, 0, width, height);
	mColor.a -= 255 / mTime * delta;
	if (mColor.a < 0.0f) {
		return true;
	}
	return false;
}

void TransitionFade::outTriggerProc() {
	mColor.a = 0.0f;
}

bool TransitionFade::outProc() {
	rectMode(CORNER);
	colorMode(RGB, 255);
	noStroke();
	fill(mColor);
	rect(0, 0, width, height);
	mColor.a += 255 / mTime * delta;
	if (mColor.a >= 255) {
		return true;
	}
	return false;
}