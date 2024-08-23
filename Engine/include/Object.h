#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Log.h"

class Object : private Camera {
private:
	static int ids;
	int ID;

public:
	static int screen_width, screen_height;
	Object(SDL_Rect dest) : m_src(dest), m_dest(dest), m_screen(dest), Camera() {
		m_playerRect = nullptr;
		ID = ids;
		ids++;
		m_angle = 0;
		m_velocity = { 0,0 };
		m_isSolid = true;
		m_tex = nullptr;
		m_ren = nullptr;
	}
	Object(const Object& obj) : Camera() {
		m_dest = obj.m_dest;
		m_src = obj.m_src;
		m_screen = obj.m_screen;
		m_angle = obj.m_angle;
		m_velocity = obj.m_velocity;
		m_ren = obj.m_ren;
		m_tex = obj.m_tex;
		m_isSolid = obj.m_isSolid;
		m_playerRect = obj.m_playerRect;
	}
	Object() : Camera() {
		m_dest = { 0,0,0,0 };
		m_src = { 0,0,0,0 };
		m_screen = { 0,0,0,0 };
		m_angle = 0;
		m_velocity = { 0,0 };
		m_ren = nullptr;
		m_tex = nullptr;
		m_isSolid = false;;
		m_playerRect = nullptr;
	}
	// Move Constructor
	Object(Object&& other) noexcept
		: Camera(std::move(other)),
		ID(other.ID),
		m_src(std::move(other.m_src)),
		m_dest(std::move(other.m_dest)),
		m_screen(std::move(other.m_screen)),
		m_angle(other.m_angle),
		m_velocity(std::move(other.m_velocity)),
		m_isSolid(other.m_isSolid),
		m_tex(other.m_tex),
		m_ren(other.m_ren),
		m_playerRect(other.m_playerRect) {
		other.m_tex = nullptr;
		other.m_ren = nullptr;
		other.m_playerRect = nullptr;
	}

	// Move Assignment Operator
	Object& operator=(Object&& other) noexcept {
		if (this != &other) {
			Camera::operator=(std::move(other));  // Move the base class

			ID = other.ID;
			m_src = std::move(other.m_src);
			m_dest = std::move(other.m_dest);
			m_screen = std::move(other.m_screen);
			m_angle = other.m_angle;
			m_velocity = std::move(other.m_velocity);
			m_isSolid = other.m_isSolid;
			m_tex = other.m_tex;
			m_ren = other.m_ren;
			m_playerRect = other.m_playerRect;

			other.m_tex = nullptr;
			other.m_ren = nullptr;
			other.m_playerRect = nullptr;
		}
		return *this;
	}
	virtual ~Object() {

	}

	int GetID() { return ID; }
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual SDL_Rect* GetSrc() = 0;
	virtual SDL_Rect* GetDest() = 0;
	virtual SDL_Rect* GetScreen() = 0;
	virtual SDL_Texture* GetTex() {
		return m_tex;
	}
	inline virtual void UpdatePositionRelativeToPlayer() {
		if (m_playerRect != NULL) {
			SetCamera();
			if (CheckForChange()) {
				ChangeScreenPosFor(CalculateChange());
			}
		}
	}
	inline virtual void SetScreen(SDL_Rect newScreen) {
		m_screen = newScreen;
	}
	inline virtual void SetDest(SDL_Rect newDest) {
		m_dest = newDest;
	}
	inline virtual void SetSrc(SDL_Rect newSrc) {
		m_src = newSrc;
	}
	inline virtual void ChangeScreenPosFor(Vec2 vec2) {
		Vec2 roundedVelocity(static_cast<int>(vec2.x), static_cast<int>(vec2.y));
		m_screen.x += roundedVelocity.x;
		m_screen.y += roundedVelocity.y;
	}
	inline virtual void SetScreenPos(Vec2 vec2) {
		m_screen.x = vec2.x;
		m_screen.y = vec2.y;
	}
	inline virtual void ChangeDestPosFor(Vec2 vec2) {
		Vec2 roundedVelocity(static_cast<int>(vec2.x), static_cast<int>(vec2.y));
		m_dest.x += roundedVelocity.x;
		m_dest.y += roundedVelocity.y;
	}
	inline virtual void SetDestPos(Vec2 vec2) {
		m_dest.x = vec2.x;
		m_dest.y = vec2.y;
	}
	inline virtual void ChangeSrcPosFor(Vec2 vec2) {
		Vec2 roundedVelocity(static_cast<int>(vec2.x), static_cast<int>(vec2.y));
		m_src.x += roundedVelocity.x;
		m_src.y += roundedVelocity.y;
	}
	inline virtual void SetSrcPos(Vec2 vec2) {
		m_src.x = vec2.x;
		m_src.y = vec2.y;
	}
	inline virtual void SetSolid(bool isSolid) {
		m_isSolid = isSolid;
	}
	inline virtual Vec2 GetDestCenter() {
		return Vec2(m_dest.x + m_dest.w / 2, m_dest.y + m_dest.h / 2);
	}
	inline virtual Vec2 GetScreenCenter() {
		return Vec2(m_screen.x + m_screen.w / 2, m_screen.y + m_screen.h / 2);
	}
	inline virtual Vec2 GetDestPos() {
		return Vec2(m_dest.x, m_dest.y);
	}
	inline virtual Vec2 GetScreenPos() {
		return Vec2(m_screen.x, m_screen.y);
	}
	inline virtual bool CheckIfObjectOutOfScreen() {
		if (m_screen.x + m_screen.w < 0 || m_screen.x > screen_width)
			return true;
		if (m_screen.y + m_screen.h < 0 || m_screen.y > screen_height)
			return true;
		return false;
	}
	inline virtual Vec2 GetVelocity() {
		return m_velocity;
	}
	inline virtual void SetRenderer(SDL_Renderer* ren) {
		m_ren = ren;
	}
	inline virtual void SetTexture(SDL_Texture* tex) {
		m_tex = tex;
	}
	bool m_isSolid;
	const SDL_Rect* m_playerRect;
protected:
	void CenterObj(const SDL_Rect* playerDest) {
		m_playerRect = playerDest;
		Init(m_playerRect);
	}
	SDL_Rect m_src;
	SDL_Rect m_dest;
	SDL_Rect m_screen;
	SDL_Renderer* m_ren;
	SDL_Texture* m_tex;
	int m_angle;
	Vec2 m_velocity;
};