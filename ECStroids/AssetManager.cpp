#include "AssetManager.h"
//#include <map>
//#include <string>
//#include "ECS/ECS.hpp"
//#include "Vector2D.hpp"
//#include "TextureManager.hpp"
//#include "EventHandler.hpp"
#include "Event.h"
#include <SDL.h>
#include "AssetManager.h"
#include "Components.h"

//Entity& AssetManager::createPlayer() {
//	auto& player(manager->addEntity());
//	SDL_ShowCursor(0);
//	player.addComponent<Text>(0, 0, "SCORE: 0", game->width/(1920/30.0), false);
//	player.addComponent<Health>(game, 10);
//	player.addComponent<PlayerHealthBar>(game);
//	player.addComponent<Transform>(game, 5, 1);
//	player.addComponent<Sprite>("media/PlayerShip.png", 50, 50);
//	player.addComponent<MouseController>();
//	player.addComponent<ShipStats>(1, 20, 8, 0);
//	player.getComponent<ShipStats>().addStats(2, 50, 60, 0);
//	player.addComponent<Collider>(1, 0.4, "p", 0);
//	player.addComponent<Cooldown>(20, 20, 120);
//	player.getComponent<Cooldown>().cooldowns[1].ready = true;
//	player.addComponent<BulletSpawner>(game, Spawn(player.getComponent<Transform>().w, player.getComponent<Transform>().h/2.0, BulletSpawner::projectile, Vector2D(1, 0), 0));
//	player.getComponent<BulletSpawner>().addSpawner(player.getComponent<Transform>().w, player.getComponent<Transform>().h/2.0, BulletSpawner::beam, 1);
//	player.addComponent<Invincibility>(60);
//	player.addComponent<FireTrail>(game);
//	player.addComponent<HealthBar>();
//	player.addGroup(Game::gPlayer);
//	return player;
//}
//
//#pragma region Projectiles
//Entity& AssetManager::createTrailProjectile(Transform t, string tag) {
//	auto& proj(manager->addEntity());
//	int x, y, xdir;
//	if (tag == "p") {
//		x = t.pos.x;
//		y = t.pos.y;
//		xdir = -1;
//	} else {
//		x = t.pos.x + t.w;
//		y = t.pos.y;
//		xdir = 1;
//	}
//	proj.addComponent<Transform>(x - t.h/3.0, y + t.h/6.0, t.h/1.5, t.h/1.5, 1);
//	proj.addComponent<DestroyOffScreen>(game);
//	proj.addComponent<Sprite>("media/Circle.png", 50, 50);
//	proj.getComponent<Sprite>().setColor(255, 0, 0);
//	proj.getComponent<Transform>().vel = Vector2D(game, xdir, 0, 1);
//	proj.addGroup(Game::gTrails);
//	return proj;
//}
//
//Entity& AssetManager::createProjectile(int x, int y, Vector2D dir, double d, int size, float s) {
//	auto& proj(manager->addEntity());
//	proj.addComponent<Transform>(x - (game->width/(1920/size))/2.0, y - (game->width/(1920/size))/2.0, size, size, 1);
//	proj.getComponent<Transform>().scaleSize(game);
//	proj.addComponent<DestroyOffScreen>(game);
//	proj.addComponent<Sprite>("media/Projectile.png", 50, 50);
//	proj.getComponent<Sprite>().setColor(0, 255, 0);
//	proj.addComponent<Collider>("playerB", d);
//	proj.getComponent<Transform>().vel = Vector2D(game, dir.x, dir.y, s);
//	return proj;
//}
//
//Entity& AssetManager::createTargetedProjectile(int x, int y, double d, int size, float s) {
//	auto& proj = createProjectile(x, y, Vector2D(0, 0), d, size, s);
//	if (!Game::manager->getGroup(Game::gPlayer).empty()) {
//		Transform target = Game::manager->getGroup(Game::gPlayer)[0]->getComponent<Transform>();
//		Transform pos = proj.getComponent<Transform>();
//		proj.getComponent<Transform>().vel = Vector2D(
//			game,
//			target.pos.x + target.w/2 - pos.pos.x - pos.w/2,
//			target.pos.y + target.h/2 - pos.pos.y - pos.h/2,
//			s
//		);
//	}
//	return proj;
//}
//
//Entity& AssetManager::createBeam(double d, int h, int dur, int del, Entity* t, int spawner) {
//	auto& beam(manager->addEntity());
//	beam.addComponent<Transform>(0, 0, 2000, h, 1);
//	beam.getComponent<Transform>().scaleSize(game);
//	beam.addComponent<Sprite>("media/Beam2.png", 2000, 50);
//	beam.addComponent<Collider>("playerBeam", d);
//	beam.getComponent<Sprite>().setColor(0, 100, 255);
//	beam.addComponent<Tie>(t, spawner);
//	beam.addComponent<BeamUpdater>(dur, del);
//	return beam;
//}
//#pragma endregion
//
//#pragma region Enemies
//Entity& AssetManager::createBasicEnemy(int y) {
//	auto& enemy(manager->addEntity());
//	enemy.addComponent<Transform>(1920, y);
//	enemy.getComponent<Transform>().scaleAll(game);
//	enemy.addComponent<Sprite>("media/EnemyShip.png", 50, 50);
//	enemy.addComponent<ShipStats>(1, 20, 4, 100);
//	enemy.addComponent<Collider>("e", 1);
//	enemy.addComponent<Cooldown>(240);
//	enemy.getComponent<Cooldown>().cooldowns[0].cooldown = 180;
//	enemy.addComponent<Health>(game, 3);
//	enemy.addComponent<Invincibility>(20);
//	enemy.addComponent<BulletSpawner>(game, Spawn(0, enemy.getComponent<Transform>().h/2.0, BulletSpawner::targetedProjectile, 0));
//	enemy.addComponent<Move>(game, 2, 0.5, Move::wave);
//	enemy.addComponent<DestroyOffScreen>(game);
//	//enemy.addComponent<FireTrail>(game);
//	enemy.addComponent<HealthBar>();
//	enemy.addGroup(Game::gEnemies);
//	return enemy;
//}
//
//Entity & AssetManager::createDroneEnemy(int y, bool up) {
//	auto& enemy(manager->addEntity());
//	enemy.addComponent<Transform>(1920, y, 40, 40, 1);
//	enemy.getComponent<Transform>().scaleAll(game);
//	enemy.addComponent<Sprite>("media/sprites/Drone.png", 50, 50);
//	enemy.addComponent<ShipStats>(1, 20, 4, 50);
//	enemy.addComponent<Collider>("e", 1);
//	enemy.addComponent<Cooldown>(120);
//	enemy.getComponent<Cooldown>().cooldowns[0].cooldown = 80;
//	enemy.addComponent<Health>(game, 1);
//	enemy.addComponent<Invincibility>(20);
//	enemy.addComponent<BulletSpawner>(game, Spawn(0, enemy.getComponent<Transform>().h/2.0, BulletSpawner::targetedProjectile, 0));
//	if (up) {
//		enemy.addComponent<Move>(game, 6, 24, -3, Move::arc);
//	} else {
//		enemy.addComponent<Move>(game, 6, 24, 3, Move::arc);
//	}
//	enemy.addComponent<DestroyOffScreen>(game);
//	enemy.addComponent<FireTrail>(game);
//	enemy.addComponent<HealthBar>();
//	enemy.addGroup(Game::gEnemies);
//	return enemy;
//}
//
//Entity& AssetManager::createSprayEnemy(int x, int y, double speed) {
//	auto& enemy(manager->addEntity());
//	enemy.addComponent<Transform>(1920, y, 100, 100, 1);
//	enemy.getComponent<Transform>().scaleAll(game);
//	enemy.addComponent<Sprite>("media/sprites/SprayBall.png", 50, 50);
//	enemy.addComponent<ShipStats>(1, 20, 12, 500);
//	enemy.addComponent<Collider>("e", 1);
//	enemy.addComponent<Cooldown>(1);
//	enemy.addComponent<Health>(game, 50);
//	enemy.addComponent<Invincibility>(20);
//	enemy.addComponent<BulletSpawner>(game, Spawn(enemy.getComponent<Transform>().w/2.0, enemy.getComponent<Transform>().h/2.0, BulletSpawner::rotatingProjectile, speed, 0));
//	enemy.addComponent<Move>(game, 2, Move::straight);
//	enemy.addComponent<SprayDetonate>(game, x, 360, speed);
//	enemy.addComponent<DestroyOffScreen>(game);
//	enemy.addComponent<FireTrail>(game);
//	enemy.addComponent<HealthBar>();
//	enemy.addGroup(Game::gEnemies);
//	return enemy;
//}
//
//Entity & AssetManager::createBossEnemy() {
//	auto& enemy(manager->addEntity());
//	enemy.addComponent<Transform>(1920, 500, 400, 400, 1);
//	enemy.getComponent<Transform>().scaleAll(game);
//	enemy.addComponent<Sprite>("media/Boss.png", 400, 400);
//	enemy.addComponent<ShipStats>(1, 40, 4, 1000);
//	enemy.getComponent<ShipStats>().addStats(2, 310, 300, 80);
//	enemy.getComponent<ShipStats>().addStats(2, 60, 300, 80);
//	enemy.addComponent<Collider>(0.8, 1, "e", 1);
//	enemy.addComponent<Cooldown>(40, 40, 3000, 3000, 3000);
//	enemy.getComponent<Cooldown>().cooldowns[0].initialCooldown = 20;
//	enemy.addComponent<Health>(game, 300);
//	enemy.addComponent<Invincibility>(20);
//	enemy.addComponent<BulletSpawner>(game, Spawn(0, 37.5*enemy.getComponent<Transform>().h/400.0, BulletSpawner::targetedProjectile, 0));
//	enemy.getComponent<BulletSpawner>().addSpawner(0, 362.5*enemy.getComponent<Transform>().h/400.0, BulletSpawner::targetedProjectile, 0);
//	enemy.getComponent<BulletSpawner>().addSpawner(enemy.getComponent<Transform>().w/2.0, enemy.getComponent<Transform>().h/2.0, BulletSpawner::beam, 1);
//	enemy.getComponent<BulletSpawner>().addSpawner(0, 37.5*enemy.getComponent<Transform>().h/400.0, BulletSpawner::beam, 2);
//	enemy.getComponent<BulletSpawner>().addSpawner(0, 362.5*enemy.getComponent<Transform>().h/400.0, BulletSpawner::beam, 2);
//	enemy.addComponent<ShootPattern>();
//	enemy.getComponent<ShootPattern>().addPattern({ 0, 1 }, 300);
//	enemy.getComponent<ShootPattern>().addPattern({ 2 }, 450);
//	enemy.getComponent<ShootPattern>().addPattern({ 3, 4 }, 300);
//	enemy.getComponent<ShootPattern>().addPattern({ -1, 0, 1, 3, 4 }, 450);
//	enemy.getComponent<ShootPattern>().addPattern({ 2 }, 450);
//	enemy.addComponent<Move>(game, 1.25, 2, Move::boss);
//	enemy.addComponent<DestroyOffScreen>(game);
//	enemy.addComponent<FireTrail>(game);
//	enemy.addComponent<HealthBar>(game);
//	enemy.addGroup(Game::gEnemies);
//	return enemy;
//}
//#pragma endregion
//
//Entity& AssetManager::createUpgrade(int x, int y, int w, int h, int type) {
//	auto& upgrade(manager->addEntity());
//	upgrade.addComponent<Transform>(x, y, w, h, 1);
//	upgrade.getComponent<Transform>().scaleAll(game);
//	upgrade.addComponent<Sprite>("media/Button.png", 50, 50);
//	upgrade.addComponent<Collider>("u");
//	upgrade.addComponent<Upgrade>(type);
//	switch (type) {
//		case Upgrade::damage:
//			upgrade.addComponent<Text>("Damage", true);
//			upgrade.getComponent<Sprite>().setColor(255, 0, 0);
//			break;
//		case Upgrade::health:
//			upgrade.addComponent<Text>("Health", true);
//			upgrade.getComponent<Sprite>().setColor(0, 255, 0);
//			break;
//		case Upgrade::bulletSize:
//			upgrade.addComponent<Text>("Size", true);
//			upgrade.getComponent<Sprite>().setColor(0, 0, 255);
//			break;
//		case Upgrade::speed:
//			upgrade.addComponent<Text>("Speed", true);
//			upgrade.getComponent<Sprite>().setColor(0, 255, 255);
//			break;
//		case Upgrade::cooldown:
//			upgrade.addComponent<Text>("Cooldown", true);
//			upgrade.getComponent<Sprite>().setColor(255, 0, 255);
//			break;
//	}
//	upgrade.addGroup(Game::gUpgrades);
//	return upgrade;
//}
//
//Entity& AssetManager::createButton(int x, int y, int w, int h) {
//	auto& button(manager->addEntity());
//	button.addComponent<Transform>(x, y, w, h, 1);
//	button.getComponent<Transform>().scaleAll(game);
//	button.addComponent<Sprite>("media/Button.png", 50, 50);
//	button.getComponent<Sprite>().setColor(255, 0, 0);
//	button.addComponent<MouseListener>("");
//	button.addComponent<DimOnHover>();
//	return button;
//}
//
//void AssetManager::createMenu() {
//	auto& menu(manager->addEntity());
//	menu.addComponent<Transform>(0, 0, 1920, 1080, 1);
//	menu.getComponent<Transform>().scaleSize(game);
//	menu.addComponent<Sprite>("media/newmainmenu.jpg", 2000, 2000);
//	menu.addGroup(Game::gMenu);
//
//	auto& button = createButton(500, 700, 200, 200);
//	button.addGroup(Game::gMenu);
//	button.getComponent<MouseListener>().tag = "easy";
//	button.addComponent<Text>("EASY", true);
//
//	auto& button2 = createButton(1200, 700, 200, 200);
//	button2.addGroup(Game::gMenu);
//	button2.getComponent<MouseListener>().tag = "hard";
//	button2.addComponent<Text>("HARD", true);
//
//	auto& button3 = createButton(0, 0, 100, 100);
//	button3.addGroup(Game::gMenu);
//	button3.getComponent<MouseListener>().tag = "quit";
//	button3.addComponent<Text>("QUIT", true);
//}
//
//void AssetManager::fade() {
//	auto& e(manager->addEntity());
//	e.addComponent<Transform>(0, 0, game->width, game->height, 1);
//	e.addComponent<FadeInOut>(game);
//	e.addGroup(Game::gFade);
//}
//
//void AssetManager::createBackground() {
//	auto& background(manager->addEntity());
//	background.addComponent<Transform>(0, 0, 1920, 1080, 1);
//	background.getComponent<Transform>().scaleSize(game);
//	background.addComponent<Sprite>("media/newbg.png", 1920, 1080);
//	background.getComponent<Sprite>().setColor(100, 100, 100);
//	background.addGroup(Game::gBackground);
//
//	//auto& starlayer(manager->addEntity());
//	//starlayer.addComponent<Transform>(0, 0, 1920, 1080, 1);
//	//starlayer.getComponent<Transform>().scaleSize(game);
//	//starlayer.addComponent<Sprite>(game, "media/starzoom.png", 1366, 768, -50);
//	//starlayer.getComponent<Sprite>().setColor(100, 100, 100);
//	//starlayer.addGroup(Game::gBackground);
//}

AssetManager::~AssetManager() {}

void AssetManager::createPlayer() {
	auto entity = _registry->create();
	std::unordered_map<unsigned int, Event::Type> map;
	map[SDL_BUTTON_LEFT] = Event::Type::collision;
	_registry->assign<MouseListener>(entity, map);
	_registry->assign<Sprite>(entity, "media/PlayerShip.png", 50, 50);
	_registry->assign<Position>(entity, 0.0f, 0.0f);
}