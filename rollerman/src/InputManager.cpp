#include "InputManager.h"
#include "freeglut_std.h"
#include <iostream>
namespace InputManager {

	bool key_LeftArrow = false;
	bool key_RightArrow = false;
	bool key_UpArrow = false;
	bool key_DownArrow = false;
	bool key_A = false;
	bool key_D = false;
	bool key_W = false;
	bool key_S = false;
	bool key_E = false;
	bool key_Q = false;
	bool key_F = false;
	bool key_C = false;

	const bool& getKey_LeftArrow() {	return	key_LeftArrow;}
	const bool& getKey_RightArrow() {	return	key_RightArrow; }
	const bool& getKey_UpArrow() {		return	key_UpArrow; }
	const bool& getKey_DownArrow() {	return	key_DownArrow; }
	const bool& getKey_A() {			return	key_A; }
	const bool& getKey_D() {			return	key_D; }
	const bool& getKey_W() {			return	key_W; }
	const bool& getKey_S() {			return	key_S; }
	const bool& getKey_E() {			return	key_E; }
	const bool& getKey_Q() {			return	key_Q; }
	const bool& getKey_F() {			return  key_F; }
	const bool& getKey_C() {			return  key_C; }

	void keyboardInput(unsigned char key, int x, int y) {
		switch (key)
		{
		case 101:
			key_E = true;
			break;
		case 113:
			key_Q = true;
			break;
		case 97:
			key_A = true;
			break;
		case 100:
			key_D = true;
			break;
		case 119:
			key_W = true;
			break;
		case 115:
			key_S = true;
			break;
		case 102:
			key_F = true;
			break;		
		case 99:
			key_C = true;
			break;
		}
	}

	void keyboardUpInput(unsigned char key, int x, int y) {

		switch (key)
		{
		case 101:
			key_E = false;
			break;
		case 113:
			key_Q = false;
			break; 
		case 97:
			key_A = false;
			break;
		case 100:
			key_D = false;
			break;
		case 119:
			key_W = false;
			break;
		case 115:
			key_S = false;
			break;
		case 102:
			key_F = false;
			break;
		case 99:
			key_C = false;
			break;
		}
	}

	void special(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_LEFT:
			key_LeftArrow = true;
			break;
		case GLUT_KEY_RIGHT:
			key_RightArrow = true;
			break;
		case GLUT_KEY_UP:
			key_UpArrow = true;
			break;
		case GLUT_KEY_DOWN:
			key_DownArrow = true;
			break;
		}
	}

	void specialUp(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_LEFT:
			key_LeftArrow = false;
			break;
		case GLUT_KEY_RIGHT:
			key_RightArrow = false;
			break;
		case GLUT_KEY_UP:
			key_UpArrow = false;
			break;
		case GLUT_KEY_DOWN:
			key_DownArrow = false;
			break;
		}
	}
}
