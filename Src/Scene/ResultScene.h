#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
};

