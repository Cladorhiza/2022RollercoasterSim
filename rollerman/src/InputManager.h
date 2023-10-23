#pragma once
namespace InputManager
{

	void keyboardInput(unsigned char key, int x, int y);
	void keyboardUpInput(unsigned char key, int x, int y);
	void special(int key, int x, int y);
	void specialUp(int key, int x, int y);

	const bool& getKey_LeftArrow();
	const bool& getKey_RightArrow();
	const bool& getKey_UpArrow();
	const bool& getKey_DownArrow();
	const bool& getKey_A();
	const bool& getKey_D();
	const bool& getKey_W();
	const bool& getKey_S();
	const bool& getKey_E();
	const bool& getKey_Q();
	const bool& getKey_F();
	const bool& getKey_C();
};

