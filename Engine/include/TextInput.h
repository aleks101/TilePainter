#pragma once

#include "Button.h"

class TextInput : public Button
{
public:
	TextInput(){}
	TextInput(SDL_Renderer* ren, Vec2 pos, TTF_Font* _font, std::string _text, SDL_Color _color1, SDL_Color _color2, SDL_Event* ev);
	TextInput(const TextInput& obj);
	~TextInput();
	TextInput& operator=(const TextInput& obj);
	void SetNoChangeText(int indexTo) override {
		LOGln("TextInput class does not allow this function!");
	}
	void SetNoChangeText(std::string newText) override {
		LOGln("TextInput class does not allow this function!");
	}
	void StartInput();
	void StopInput();
	void Update();
	void Input();
	void AcceptNumbers(bool yesOrno);
	void AcceptNegativeNumbers(bool yesOrno);
	int GetNum();
	bool HasInputEnded();

	void ChangeText(std::string _text) override;
	void ChangeText(int num) override;
private:
	bool IsOver();
	bool m_clicked;
	bool m_inputAllowed;
	bool m_negAllowed;
	bool m_onlyNumbers;
	std::string m_inputText;
};

