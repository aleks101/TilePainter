#pragma once

class TimeD {
private:
	static float s_prevTime;
	static float deltaTime;
public:
	static void CalcDeltaTime();
	static float DeltaTime();
};