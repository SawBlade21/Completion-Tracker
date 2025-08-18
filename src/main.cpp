#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include <iostream>
#include <chrono>
#include <format>
#include "RebeatPopup.hpp"
#include "Utils.hpp"

using namespace geode::prelude;
using namespace std::chrono;

class $modify(FapGJBaseGameLayer, GJBaseGameLayer) {

    struct Fields {
        bool m_coins[3] = {false, false, false};
		bool m_coinsLastCheckpoint[3] = {false, false, false};
		std::vector<std::pair<int, int>> m_coinCoords;
		std::vector<GameObject*> m_coinObjects;
    };

	void collisionCheckObjects(PlayerObject* player, gd::vector<GameObject*>* sectionObjects, int objectCount, float dt) {
		if (m_isPracticeMode || m_isTestMode)
			return GJBaseGameLayer::collisionCheckObjects(player, sectionObjects, objectCount, dt);

		auto playerRect = player->getObjectRect();
		for (int i = 0; i < objectCount; i++) {
			auto* obj = sectionObjects->at(i);

			if (obj->m_objectType != GameObjectType::SecretCoin && obj->m_objectType != GameObjectType::UserCoin)
				continue;

			auto* effectSprite = geode::cast::typeinfo_cast<EffectGameObject*>(obj);
			if (!effectSprite)
				continue;

			auto objectRect = effectSprite->getObjectRect();
			if (!playerRect.intersectsRect(objectRect))
				continue;

			for (int i = 0; i < m_fields->m_coinObjects.size(); i++) {

				if (m_fields->m_coinObjects[i] == obj && !obj->m_isGroupDisabled) {
					m_fields->m_coins[i] = true;
				}
			}
			
		}

		GJBaseGameLayer::collisionCheckObjects(player, sectionObjects, objectCount, dt);
	}

};

class $modify(FapPlayLayer, PlayLayer) {
	bool levelHasCoins() {
		auto& coinObjects = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinObjects;
		return coinObjects.size() > 0;
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		if (!levelHasCoins())
			return;

		auto& coins = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coins;
		auto& coinsLastCheckpoint = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinsLastCheckpoint;
		for (int i = 0; i < 3; i++)
			if (m_isPlatformer) {
				if (m_gameState.m_levelTime <= 0 || !coinsLastCheckpoint[i])
					coins[i] = false;
			}
			else 
				coins[i] = false;
	}

	void removeAllCheckpoints() {
		PlayLayer::removeAllCheckpoints();

		if (!levelHasCoins())
			return;

		auto& coinsLastCheckpoint = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinsLastCheckpoint;

		for (int i = 0; i < 3; i++) {
			coinsLastCheckpoint[i] = false;
		}

	}

	void storeCheckpoint(CheckpointObject * cp) {
		PlayLayer::storeCheckpoint(cp);

		if (!levelHasCoins())
			return;

		auto& coins = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coins;
		auto& coinsLastCheckpoint = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinsLastCheckpoint;

		for (int i = 0; i < 3; i++) {
			coinsLastCheckpoint[i] = coins[i];
		}

	}

	void loadFromCheckpoint(CheckpointObject* cp) {
		PlayLayer::loadFromCheckpoint(cp);

		if (!levelHasCoins())
			return;

		auto& coins = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coins;
		auto& coinsLastCheckpoint = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinsLastCheckpoint;

		for (int i = 0; i < 3; i++) {
			coins[i] = coinsLastCheckpoint[i];
		}
	}

	void addObject(GameObject* object) {
		auto& coinObjects = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinObjects;

		PlayLayer::addObject(object);
		
        if (object->m_objectType == GameObjectType::UserCoin || object->m_objectType == GameObjectType::SecretCoin) {
			coinObjects.push_back(object);
			
			std::sort(coinObjects.begin(), coinObjects.end(), [](const auto& a, const auto& b) {
				return a->m_obPosition.x < b->m_obPosition.x || (a->m_obPosition.x == b->m_obPosition.x && a->m_obPosition.y < b->m_obPosition.y);
			});
			
		}
		
    }

	static void onModify(auto& self) {
		if (!self.setHookPriorityPre("PlayLayer::levelComplete", Priority::Last)) {
			geode::log::warn("Failed to set Completion Tracker hook priority for PlayLayer::levelComplete().");
		}
	}

	void levelComplete() {

		if (m_isPracticeMode || m_isTestMode)
			return PlayLayer::levelComplete();

		std::string id = std::to_string(EditorIDs::getID(m_level, true));
		if (m_level->m_isEditable) 
			id = "c" + id;

		
		matjson::Value rebeats = Mod::get()->getSavedValue<matjson::Value>(id);
		
		if (!rebeats.isArray())
		rebeats = matjson::Value::array();
		
		matjson::Value newRebeat;

		newRebeat["name"] = fmt::format("Completion {}", rebeats.size() + 1);
		
		std::time_t now_time_t = system_clock::to_time_t(system_clock::now());
		std::string timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&now_time_t));
		auto splitTime = Utils::splitString(timestamp, ' ');
		
		if (m_level->m_normalPercent.value() != 100) {
			timestamp = "f" + timestamp;
		}
		
		newRebeat["date"] = splitTime[0];
		newRebeat["time"] = splitTime[1];
		
		GameManager* gm = GameManager::get();
		
		matjson::Value icons;
		IconType type = gm->m_playerIconType;
		icons["type"] = static_cast<int>(type);
		icons["frame"] = Utils::getIconFrame(type);
		icons["color_1"] = gm->getPlayerColor();
		icons["color_2"] = gm->getPlayerColor2();
		icons["glow_color"] = gm->getPlayerGlowColor();
		icons["has_glow"] = gm->getPlayerGlow();
	
		newRebeat["icons"] = icons;

		auto collectedCoins = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coins;
		auto coinObjects = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinObjects;
		matjson::Value coins;

		coins["amount"] = coinObjects.size();

		for (int i = 0; i < coinObjects.size(); i++) {
			coins[std::to_string(i).c_str()] = collectedCoins[i];
		}

		newRebeat["coins"] = coins;
		
		if (m_isPlatformer) {
			auto time = std::round(m_timePlayed * 1000.0) / 1000.0;
			std::string formattedTime = "";

			int hours = static_cast<int>(time / 3600);
			time = std::fmod(time, 3600);

			int minutes = static_cast<int>(time / 60);
			time = std::fmod(time, 60);

			int seconds = static_cast<int>(time);
			int milliseconds = static_cast<int>((time - seconds) * 100);

			if (hours > 0) {
				formattedTime = fmt::format("{:02}:{:02}:{:02}.{:03}", hours, minutes, seconds, milliseconds);
			}
			else if (minutes > 0) {
				formattedTime = fmt::format("{:02}:{:02}.{:03}", minutes, seconds, milliseconds);
			}
			else {
				formattedTime = fmt::format("{}.{:03}", seconds, static_cast<int>((time - seconds) * 1000));
			}

			newRebeat["level_time"] = formattedTime;

			newRebeat["points"] = std::to_string(m_gameState.m_points);
		}
		else {
			auto attempts = std::to_string(m_level->m_attempts.value());
			newRebeat["attempts"] = attempts;

			auto jumps = std::to_string(m_jumps);
			newRebeat["jumps"] = jumps;

		}

		rebeats.push(newRebeat);
		
		Mod::get()->setSavedValue(id, rebeats);

		PlayLayer::levelComplete();
	}

};

class $modify(FapLevelInfoLayer, LevelInfoLayer) {
	void onButton(CCObject*) {
		auto popup = RebeatPopup::create(m_level);
			popup->show();
			return;
	}
	
	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;
	
		auto rebeatButton = CCMenuItemSpriteExtra::create(
			CCSprite::create("rebeat_btn_02.png"_spr),
			this,
			menu_selector(FapLevelInfoLayer::onButton)
		);
		rebeatButton->setID("rebeatBtn"_spr);

		if (!Loader::get()->isModLoaded("geode.node-ids")) {
			auto menu = CCMenu::create();
			menu->setID("rebeatMenu"_spr);
			addChild(menu);
			menu->addChild(rebeatButton);

			rebeatButton->setPosition({-254.5f, -36.f});
		}
		else {
			CCMenu* actionsMenu = static_cast<CCMenu*>(getChildByID("left-side-menu"));
			actionsMenu->addChild(rebeatButton);
			actionsMenu->updateLayout();
		}
	
		return true;
	}
};

class $modify(FapEditLevelLayer, EditLevelLayer) {
	void onButton(CCObject*) {
		auto popup = RebeatPopup::create(m_level);
			popup->show();
			return;
	}
	
	bool init(GJGameLevel* level) {
		if (!EditLevelLayer::init(level)) return false;

		auto rebeatSprite = CCSprite::create("rebeat_btn_02.png"_spr);
		// rebeatSprite->setScale(0.7f);

		auto rebeatButton = CCMenuItemSpriteExtra::create(
			rebeatSprite,
			this,
			menu_selector(FapEditLevelLayer::onButton)
		);
		rebeatButton->setID("rebeatBtn"_spr);

		if (Loader::get()->isModLoaded("geode.node-ids")) {
			rebeatSprite->setScale(1.115f);
			CCMenu* actionsMenu = static_cast<CCMenu*>(getChildByID("level-actions-menu"));
			actionsMenu->addChild(rebeatButton);
			actionsMenu->updateLayout();
		}
		else {
			auto menu = CCMenu::create();
			menu->setID("rebeatMenu"_spr);
			addChild(menu);
			menu->addChild(rebeatButton);

			rebeatButton->setPosition({-254.5f, 60.f});
			rebeatSprite->setScale(0.8f);
		}
	
		return true;
	}
};

class $modify(FapPauseLayer, PauseLayer) {
	void onButton(CCObject*) {
		auto popup = RebeatPopup::create(PlayLayer::get()->m_level);
			popup->show();
			return;
	}

	void customSetup() {
		PauseLayer::customSetup();

		auto rebeatSprite = CCSprite::create("rebeat_btn_02.png"_spr);
		rebeatSprite->setScale(0.65f);
	
		auto rebeatButton = CCMenuItemSpriteExtra::create(
			rebeatSprite,
			this,
			menu_selector(FapPauseLayer::onButton)
		);
		rebeatButton->setID("rebeatBtn"_spr);

		if (!Loader::get()->isModLoaded("geode.node-ids")) {
            CCMenu* menu = CCMenu::create();
            menu->setID("rebeatBtn"_spr);
            addChild(menu);
            rebeatButton->setPosition({248, -122});
            menu->addChild(rebeatButton);
        }
		else {
			auto menu = getChildByID("right-button-menu");
			menu->addChild(rebeatButton);
			menu->updateLayout();
		}

	}
};

class $modify(FapEndLevelLayer, EndLevelLayer) {
	void onButton(CCObject*) {
		auto popup = RebeatPopup::create(PlayLayer::get()->m_level);
		popup->show();
		return;
	}

	void customSetup() {
		EndLevelLayer::customSetup();

		if (!Mod::get()->getSettingValue<bool>("endscreen-button")) 
			return;

		auto rebeatSprite = CCSprite::create("rebeat_btn_02.png"_spr);
		rebeatSprite->setScale(0.75f);
	
		auto rebeatButton = CCMenuItemSpriteExtra::create(
			rebeatSprite,
			this,
			menu_selector(FapEndLevelLayer::onButton)
		);

		rebeatButton->setID("rebeatBtn"_spr);
		auto btnMenu = m_mainLayer->getChildByID("button-menu");
		btnMenu->addChild(rebeatButton);
		auto level = PlayLayer::get()->m_level;
		
		if (level->m_isEditable && !level->isPlatformer())
			rebeatButton->setPosition({178, 115});
		else
			rebeatButton->setPosition({135, 120});

	}
};
