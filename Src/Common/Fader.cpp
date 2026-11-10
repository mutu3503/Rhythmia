#include <DxLib.h>
#include "../Application.h"
#include "Fader.h"

Fader::Fader(void)
	:
	state_(STATE::NONE),
	alpha_(0.0f),
	isPreEnd_(true),
	isEnd_(true)
{
}

Fader::~Fader(void)
{
}

Fader::STATE Fader::GetState(void) const
{
	return state_;
}

bool Fader::IsEnd(void) const
{
	return isEnd_;
}

void Fader::SetFade(STATE state, double fadeOutSeconds)
{
	state_ = state;
	fadeOutSeconds_ = fadeOutSeconds;

	if (state_ != STATE::NONE)
	{
		fadeStartedAt_ = std::chrono::steady_clock::now();
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::Init(void)
{
}

void Fader::Update(void)
{

	if (isEnd_)
	{
		return;
	}

	// 秒数を指定された暗転はフレーム数ではなく経過時間で進める
	if (state_ == STATE::FADE_OUT && fadeOutSeconds_ > 0.0)
	{
		const double elapsedSeconds = std::chrono::duration<double>(
			std::chrono::steady_clock::now() - fadeStartedAt_
		).count();

		if (elapsedSeconds >= fadeOutSeconds_)
		{
			alpha_ = OPAQUE_ALPHA;

			// 真っ黒なフレームを一度描いてから、切り替えを許可する
			isEnd_ = isPreEnd_;
			isPreEnd_ = true;
		}
		else
		{
			const double progress = elapsedSeconds / fadeOutSeconds_;
			alpha_ = static_cast<float>(OPAQUE_ALPHA * progress);
		}

		return;
	}

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > 255)
		{
			// フェード終了
			alpha_ = 255;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;
	case STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// フェード終了
			alpha_ = 0;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;
	default:
		return;
	}
}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}
