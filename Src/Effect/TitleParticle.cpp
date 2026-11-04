#include <DxLib.h>
#include <cmath>
#include "TitleParticle.h"

TitleParticle::TitleParticle(void)
    :
    type_(Type::Light),
    x_(0.0f),
    y_(0.0f),
    speed_(0.0f),
    swayPhase_(0.0f),
    swaySpeed_(0.0f),
    swayWidth_(0.0f),
    twinklePhase_(0.0f),
    twinkleSpeed_(0.0f),
    size_(MIN_SIZE),
    baseAlpha_(MIN_ALPHA)
{
}

void TitleParticle::Init(int screenWidth, int screenHeight)
{
    // 最初は画面全体にランダム配置する
    Reset(screenWidth,
        screenHeight,
        true
    );
}

void TitleParticle::Update(int screenWidth, int screenHeight)
{
    // ゆっくり上方向に移動
    y_ -= speed_;

    // 左右揺れの時間を進める
    swayPhase_ += swaySpeed_;

    // 明滅の時間を進める
    twinklePhase_ += twinkleSpeed_;

    // 画面の上まで行った場合
    if (y_ < -static_cast<float>(size_ * 4))
    {
        // 再び画面下から登場させる
        Reset(screenWidth,
            screenHeight,
            false
        );
    }
}

void TitleParticle::Draw(void) const
{
    // 左右にゆっくり揺らす
    const float drawX = x_ + std::sin(swayPhase_)
        * swayWidth_;

    // 明るさをゆっくり変化させる
    int alpha = baseAlpha_ + static_cast<int>(std::sin(twinklePhase_)
        * ALPHA_SWING
        );

    // 透明度の範囲を超えないようにする
    if (alpha < 0)
    {
        alpha = 0;
    }
    else if(alpha > 255)
    {
        alpha = 255;
    }

    const int drawPosX = static_cast<int>(drawX);
    const int drawPosY = static_cast<int>(y_);

    // パーティクルを半透明描画
    SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA,
        alpha
    );

    switch (type_)
    {
    case Type::Light:
    {
        // 青白い光の粒
        const unsigned int color =
            GetColor(190, 230, 255);

        DrawCircle(drawPosX,
            drawPosY,
            size_,
            color,
            true
        );
        break;
    }
    case Type::Star:
    {
        // 紫がかった星
        const unsigned int color =
            GetColor(230, 210, 255);

        DrawLine(
            drawPosX,
            drawPosY - size_ * 2,
            drawPosX,
            drawPosY + size_ * 2,
            color
        );

        DrawLine(
            drawPosX - size_ * 2,
            drawPosY,
            drawPosX + size_ * 2,
            drawPosY,
            color
        );
        break;
    }
    case Type::Prism:
    {
        // 水色のプリズム片
        const unsigned int color =
            GetColor(160, 220, 255);

        DrawTriangle(drawPosX,
            drawPosY - size_ * 2,
            drawPosX - size_,
            drawPosY,
            drawPosX + size_,
            drawPosY,
            color,
            true
        );

        DrawTriangle(drawPosX,
            drawPosY + size_ * 2,
            drawPosX - size_,
            drawPosY,
            drawPosX + size_,
            drawPosY,
            color,
            true
        );
        break;
    }
    case Type::Count:
        break;
    default:
        break;
    }

    // 通常画面に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleParticle::Reset(int screenWidth, int screenHeight, bool randomY)
{
    // 横位置
    x_ = GetRandomFloat(0.0f,
        static_cast<float>(screenWidth)
    );

    // 縦位置
    if (randomY)
    {
        // 最初は画面全体に配置
        y_ = GetRandomFloat(0.0f,
            static_cast<float>(screenHeight)
        );
    }
    else
    {
        // 画面外にでたら下から出す
        y_ = static_cast<float>(
            screenHeight + size_ * 4
            );
    }

    // 移動速度
    speed_ = GetRandomFloat(MIN_SPEED,
        MAX_SPEED
    );

    // 左右揺れ
    swayPhase_ = GetRandomFloat(0.0f,
        6.28f
    );
    swaySpeed_ = GetRandomFloat(MIN_SWAY_SPEED,
        MAX_SWAY_SPEED
    );
    swayWidth_ = GetRandomFloat(MIN_SWAY_WIDTH,
        MAX_SWAY_WIDTH
    );

    // 明滅
    twinklePhase_ = GetRandomFloat(0.0f,
        6.28f
    );
    twinkleSpeed_ = GetRandomFloat(MIN_TWINKLE_SPEED,
        MAX_TWINKLE_SPEED
    );

    // サイズ
    size_ = MIN_SIZE + GetRand(
        MAX_SIZE - MIN_SIZE
    );

    // 透明度
    baseAlpha_ = MIN_ALPHA + GetRand(
        MAX_ALPHA - MIN_ALPHA
    );

    // 種類
    type_ = static_cast<Type>(GetRand(
        static_cast<int>(Type::Count) - 1
    ));
}

float TitleParticle::GetRandomFloat(float minValue, float maxValue) const
{
    const float rate = static_cast<float>(
        GetRand(1000)) / 1000.0f;

    return minValue + (maxValue - minValue) *
        rate;
}
