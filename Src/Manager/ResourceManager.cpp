#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// 推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_SOUND = Application::PATH_SOUND;

	Resource* res;

	// タイトル背景画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Title_BackGround.png");
	resourcesMap_.emplace(SRC::TITLE_BACKGROUND, res);

	// タイトルロゴ画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Title_Logo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);

	// ノーツ画像
	res = new RES(RES_T::IMG, PATH_IMG + "Notes/Nomal_Notes.png");
	resourcesMap_.emplace(SRC::NORMAL_NOTES, res);

	// ロングノーツ画像
	res = new RES(RES_T::IMG, PATH_IMG + "Notes/Rong_Notes.png");
	resourcesMap_.emplace(SRC::HOLD_NOTES, res);

	// PERFECTの判定画像
	res = new RES(RES_T::IMG, PATH_IMG + "Game/Note_judgment/Perfect_Logo.png");
	resourcesMap_.emplace(SRC::JUDGE_PERFECT, res);

	// GREATの判定画像
	res = new RES(RES_T::IMG, PATH_IMG + "Game/Note_judgment/Great_Logo.png");
	resourcesMap_.emplace(SRC::JUDGE_GREAT, res);

	// GOODの判定画像
	res = new RES(RES_T::IMG, PATH_IMG + "Game/Note_judgment/Good_Logo.png");
	resourcesMap_.emplace(SRC::JUDGE_GOOD, res);

	// MISSの判定画像
	res = new RES(RES_T::IMG, PATH_IMG + "Game/Note_judgment/Miss_Logo.png");
	resourcesMap_.emplace(SRC::JUDGE_MISS, res);

	// FINISH画像
	res = new RES(RES_T::IMG, PATH_IMG + "GameClear/Clear_Logo.png");
	resourcesMap_.emplace(SRC::CLEAR_LOGO, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	const Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;
}
