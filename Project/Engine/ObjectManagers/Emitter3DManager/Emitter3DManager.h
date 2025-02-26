#pragma once

#include <vector>
#include <string>
#include <memory>

#include "3D/Emitter3D/Emitter3D.h"

/// <summary>
/// 3Dエミッターマネージャ
/// </summary>
class Emitter3DManager {
public:
	Emitter3DManager();
	~Emitter3DManager();

	void Update();

	// エミッター作成
	std::string CreateEmitter(const std::string& emitterName, const Vector3& position);

	void Remove(const std::string& emitterName);
	Emitter3D* Find(const std::string& emitterName);

private:
	// エミッターコンテナ
	std::vector<std::unique_ptr<Emitter3D>> emitters3D_;
};