#pragma once

class TitleParticle
{
public:

	TitleParticle(void);	// コンストラクタ

	// 初期化
	void Init(int screenWidth,
		int screenHeight
	);

	// 更新
	void Update(int screenWidth,
		int screenHeight
	);

	// 描画
	void Draw(void) const;

private:

	// パーティクルの種類
	enum class Type
	{
		Light,	// 光の粒
		Star,	// 星
		Prism,	// プリズム片
		Count
	};

	Type type_;

	// 上昇速度
	static constexpr float MIN_SPEED = 0.3f;
	static constexpr float MAX_SPEED = 0.9f;

	// 左右に揺れる幅
	static constexpr float MIN_SWAY_WIDTH = 1.0f;
	static constexpr float MAX_SWAY_WIDTH = 6.0f;

	// 左右揺れの速度
	static constexpr float MIN_SWAY_SPEED = 0.01f;
	static constexpr float MAX_SWAY_SPEED = 0.03f;

	// 明滅速度
	static constexpr float MIN_TWINKLE_SPEED = 0.02f;
	static constexpr float MAX_TWINKLE_SPEED = 0.05f;

	// パーティクルサイズ
	static constexpr int MIN_SIZE = 1;
	static constexpr int MAX_SIZE = 3;

	// 基本透明度
	static constexpr int MIN_ALPHA = 100;
	static constexpr int MAX_ALPHA = 190;

	// 明滅による透明度変化
	static constexpr int ALPHA_SWING = 35;

	// 位置
	float x_;
	float y_;

	// 上昇速度
	float speed_;

	// 左右揺れ
	float swayPhase_;
	float swaySpeed_;
	float swayWidth_;

	// 明滅
	float twinklePhase_;
	float twinkleSpeed_;

	// サイズ
	int size_;

	// 基本透明度
	int baseAlpha_;

	// パーティクルを再配置
	void Reset(int screenWidth,
		int screenHeight,
		bool randomY
	);

	// 指定範囲のランダムな小数を取得
	float GetRandomFloat(float minValue,
		float maxValue) const;
};

