#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	camera_ = new Camera();
	camera_->Initialize(1280, 720);

	dxCommon_ = DirectXCommon::GetInstance();
	
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureManager_ = TextureManager::GetInstance();

	light_ = Light::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	playerModel_.reset(Model::CreateModelFromObj("resource/player", "player.obj"));
	weaponModel_.reset(Model::CreateModelFromObj("resource/hammer", "hammer.obj"));

	modelFighterBody_.reset(Model::CreateModelFromObj("resource/float_Body", "float_Body.obj"));
	modelFighterHead_.reset(Model::CreateModelFromObj("resource/float_Head", "float_Head.obj"));
	modelFighterL_arm_.reset(Model::CreateModelFromObj("resource/float_L_arm", "float_L_arm.obj"));
	modelFighterR_arm_.reset(Model::CreateModelFromObj("resource/float_R_arm", "float_R_arm.obj"));

	goalModel_.reset(Model::CreateModelFromObj("resource/player", "player.obj"));

	groundModel_.reset(Model::CreateModelFromObj("resource/ground", "ground.obj"));

	skydomeModel_.reset(Model::CreateModelFromObj("resource/skydome", "skydome.obj"));
	skydomeModel_->SetLighting(Lighting::checkLighting);

	collisionManager_ = std::make_unique<CollisionManager>();

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();

	std::vector<Model*> playerModels = { playerModel_.get(),weaponModel_.get()};

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	
	AddEnemy({ 0.0f,0.5f,72.0f });
	AddEnemy({ 5.0f,0.5f,80.0f });
	AddEnemy({ -5.0f,0.5f,85.0f });
	AddEnemy({ 10.0f,0.5f,90.0f });
	AddEnemy({ -10.0f,0.5f,95.0f });

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get(),{0.0f,1.0f,100.0f});

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());


	for (int i = 0; i < 10; i++)
	{
		ground_[i] = std::make_unique<Ground>();
	}

	ground_[0]->Initialize(groundModel_.get(), { 0.0f,0.0f,0.0f });
	ground_[1]->Initialize(groundModel_.get(), { 0.0f,0.0f,68.0f });
	ground_[2]->Initialize(groundModel_.get(), { -15.0f,0.0f,68.0f });
	ground_[3]->Initialize(groundModel_.get(), { 15.0f,0.0f,68.0f });
	ground_[4]->Initialize(groundModel_.get(), { -15.0f,0.0f,83.0f });
	ground_[5]->Initialize(groundModel_.get(), { 15.0f,0.0f,83.0f });
	ground_[6]->Initialize(groundModel_.get(), { 0.0f,0.0f,83.0f });
	ground_[7]->Initialize(groundModel_.get(), { -15.0f,0.0f,98.0f });
	ground_[8]->Initialize(groundModel_.get(), { 15.0f,0.0f,98.0f });
	ground_[9]->Initialize(groundModel_.get(), { 0.0f,0.0f,98.0f });

	moveGround_ = std::make_unique<MoveGround>();

	moveGround_->Initialize(groundModel_.get(), { 0.0f,0.0f,30.0f });

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&viewProjection_);
}

void GamePlayScene::Update()
{
	player_->Update();

	
	/*enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDeathAnimationEnd()) {
			enemy.reset();
			return true;
		}
		return false;
		});*/

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}

	goal_->Update();

	skydome_->Updata();

	for (int i = 0; i < 10; i++)
	{
		ground_[i]->Update();
	}

	moveGround_->Update();

	/*lockOn_->Update(enemies_,viewProjection_);*/

	viewProjection_.UpdateMatrix();
	
	followCamera_->Update();
	viewProjection_ = followCamera_->GetViewProjection();

	collisionManager_->ClearColliders();
	collisionManager_->AddCollider(player_.get());

	if (player_->GetWeapon()->GetIsAttack())
	{
		collisionManager_->AddCollider(player_->GetWeapon());
	}

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}

	for (int i = 0; i < 10; i++)
	{
		collisionManager_->AddCollider(ground_[i].get());
	}

	collisionManager_->AddCollider(moveGround_.get());
	collisionManager_->AddCollider(goal_.get());
	collisionManager_->CheckAllCollision();
}

void GamePlayScene::Draw()
{
	player_->Draw(viewProjection_);

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	goal_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	for (int i = 0; i < 10; i++)
	{
		ground_[i]->Draw(viewProjection_);
	}

	moveGround_->Draw(viewProjection_);

	lockOn_->Draw();
}

void GamePlayScene::AddEnemy(const Vector3& position) 
{
	std::vector<Model*> enemyModels = { modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
		modelFighterR_arm_.get() };

	Enemy* enemy = new Enemy();

	enemy->Initialize(enemyModels);
	enemy->SetPosition(position);

	enemies_.push_back(std::unique_ptr<Enemy>(enemy));
}