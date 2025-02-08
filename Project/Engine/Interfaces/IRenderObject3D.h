#pragma once

/// <summary>
/// 3D描画用の抽象インターフェース
/// </summary>
class IRenderObject3D {
public:
	virtual ~IRenderObject3D() = default;
	virtual void AssignShape() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};