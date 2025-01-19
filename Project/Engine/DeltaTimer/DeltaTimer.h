#pragma once

// Windows
#include <windows.h>

// C++
#include <cstdint>

class DeltaTimer {
public:
	DeltaTimer();
	~DeltaTimer();
	void Reset();
	void Update();
	double GetDeltaTime() const;
private:
	// デルタタイム
	double deltaTime_;
	int64_t lastTime_;
	double frequency_;
};