// Ŭnicode please
#pragma once

/*
irr::driver안에는 CFPSCounter가 이미 존재한다. 허나 CFPSCounter에 직접 접근하는거싱 불가능하고
어떻게 보여줄지에 대한 방법없이 값만 던지게 되어있다.
해당정보를 긁어서 그럴싸하게 보여주기 위한 목적으로 따로 만듬
*/
class FPSCounter {
public:
	FPSCounter();
	~FPSCounter();

	//! returns current fps
	irr::s32 getFPS() const;

	//! returns primitive count
	irr::u32 getPrimitive() const;

	//! returns average primitive count of last period
	irr::u32 getPrimitiveAverage() const;

	//! returns accumulated primitive count since start
	irr::u32 getPrimitiveTotal() const;

public:
	void draw();
	void drawText(int baseX, int baseY);
	void drawCaption();

public:
	int &visible;
};