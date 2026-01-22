#include"SceneGame.h"
#include "ShotBoss.h"

void ShotBoss::UpdateShot(void)
{
	Shot::UpdateShot();
	dir_ = { sceneGame_->GetPlayerPos().x - pos_.x,sceneGame_->GetPlayerPos().y - pos_.y };
	dir_ = { static_cast<float>(dir_.x / hypot(dir_.x,dir_.y)),static_cast<float>(dir_.y / hypot(dir_.x,dir_.y)) };
}
