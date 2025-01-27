#pragma once

// C++
#include <string>


/// <summary>
/// 2Dオブジェクトクラス
/// </summary>
class Object2D {
public:
	Object2D(const std::string& objectName, const std::string& textureName);
	~Object2D();
	void Initialize(const std::string& objectName, const std::string& textureName);
	void Update();
	void Draw();
private:


	// WVP用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	// WVPデータ
	Matrix4x4* wvpData_;


};