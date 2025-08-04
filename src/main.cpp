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

std::string buttonSpriteName = "rebeat_btn_02.png"_spr;

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
			if (!effectSprite) // weird, but just in case
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

/*std::string getLevelID(GJGameLevel* level) {
	std::string id;

	if (level->m_isEditable)
		id = "c" + std::to_string(EditorIDs::getID(level, true));
	else
		id = std::to_string(level->m_levelID.value());

	return id;
}*/

class $modify(FapPlayLayer, PlayLayer) {
	bool levelHasCoins() {
		auto& coinObjects = static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinObjects;
		return coinObjects.size() > 0;
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		//static_cast<FapGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields->m_coinObjects.clear();
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
			
		/*if (Mod::get()->getSettingValue<bool>("confirm-completion")) {
			geode::createQuickPopup(
				"Confirm Completion",
				"Do you want to <cj>save</c> this completion?",
				"Cancel", "Save",
				[this](auto, bool btn2) {
            		if (btn2)  {
						createCompletion();
					}
				}
			);
		}
		else
			createCompletion();*/

		std::string id = std::to_string(EditorIDs::getID(m_level, true));
		if (m_level->m_isEditable) 
			id = "c" + id;

		
		matjson::Value rebeats = Mod::get()->getSavedValue<matjson::Value>(id);
		
		if (!rebeats.isArray())
		rebeats = matjson::Value::array();
		
		matjson::Value newRebeat;

		newRebeat["name"] = fmt::format("Completion {}", rebeats.size() + 1);

		/*if (m_isPlatformer)
			newRebeat["platformer"] = true;
		else newRebeat["platformer"] = false;*/
		
		std::time_t now_time_t = system_clock::to_time_t(system_clock::now());
		std::string timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&now_time_t));
		log::debug("timestamp: {}", timestamp);
		auto splitTime = Utils::splitString(timestamp, ' ');
		/*auto tm = *std::localtime(&now_time_t);
		auto date = fmt::format("{:%Y-%m-%d}", tm);
		auto time = Utils::getTime(fmt::format("{%H:%M:%S}", tm));*/

		log::debug("(main.cpp) time: {}", splitTime[1]);
		
		if (m_level->m_normalPercent.value() != 100) {
			timestamp = "f" + timestamp;
		}
		
		newRebeat["date"] = splitTime[0];
		newRebeat["time"] = splitTime[1];
		
		GameManager* gm = GameManager::get();
		// GJBaseGameLayer* bgl = GJBaseGameLayer::get();
		// GameStatsManager* gsm = GameStatsManager::sharedState();
		
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

		coins["amount"] = coinObjects.size(); //m_level->m_coins;

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

			if (hours > 0)
				formattedTime = fmt::format("{:02}:{:02}:{:02}.{:03}", hours, minutes, seconds, milliseconds);
			else if (minutes > 0)
				formattedTime = fmt::format("{:02}:{:02}.{:03}", minutes, seconds, milliseconds);
			else
				formattedTime = fmt::format("{}.{:03}", seconds, static_cast<int>((time - seconds) * 1000));

			//log::debug("time: {}, formatted: {}", time, formattedTime);

			newRebeat["level_time"] = formattedTime;


			std::string points = GameToolbox::pointsToString(m_gameState.m_points);
			newRebeat["points"] = points;

		}
		else {
			auto attempts = std::to_string(m_level->m_attempts.value());
			newRebeat["attempts"] = attempts;

			auto jumps = std::to_string(m_jumps);
			newRebeat["jumps"] = jumps;

		}

		rebeats.push(newRebeat);
		
		Mod::get()->setSavedValue(id, rebeats);
		
		// std::string iconData = fmt::format("|{},{},{},{},{}", gm->getPlayerFrame(), gm->m_playerColor.value(), gm->m_playerColor2.value(), gm->m_playerGlowColor, std::to_string(gm->m_playerGlow));
		// log::debug("icondata: {}", iconData);
		
		// rebeats.push(timestamp + iconData);
		// log::debug("push: {}", timestamp + iconData);

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
			CCSprite::create(buttonSpriteName.c_str()),
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
		
		//rebeatButton->setPosition(-185, 20);
	
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

		auto rebeatSprite = CCSprite::create(buttonSpriteName.c_str());

		auto rebeatButton = CCMenuItemSpriteExtra::create(
			CCSprite::create(buttonSpriteName.c_str()),
			this,
			menu_selector(FapEditLevelLayer::onButton)
		);
		rebeatButton->setID("rebeatBtn"_spr);

		if (!Loader::get()->isModLoaded("geode.node-ids")) {
			auto menu = CCMenu::create();
			menu->setID("rebeatMenu"_spr);
			addChild(menu);
			menu->addChild(rebeatButton);

			rebeatButton->setPosition({-254.5f, 60.f});
			rebeatSprite->setScale(0.8f);
		}
		else {
			rebeatSprite->setScale(1.3f);
			CCMenu* actionsMenu = static_cast<CCMenu*>(getChildByID("level-actions-menu"));
			actionsMenu->addChild(rebeatButton);
			actionsMenu->updateLayout();
		}
		//ebeatButton->setPosition(-185, 20);

		/*SimplePlayer* icon = SimplePlayer::create(gm->getPlayerFrame());
		icon->setPosition({23.f, 23.5f});
		icon->setScale(0.75f);
		//icon->getChildByType<CCSprite*>(0)->setAnchorPoint({0, 0.5});

		icon->setColor(gm->colorForIdx(gm->getPlayerColor()));
		icon->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
		icon->m_hasGlowOutline = gm->getPlayerGlow();

		if (icon->m_hasGlowOutline)
			icon->enableCustomGlowColor(gm->colorForIdx(gm->getPlayerGlowColor()));
		else
			icon->disableCustomGlowColor();

		icon->updateColors();
		icon->setRotation(25.f);
		rebeatButton->addChild(icon);

		// auto trophy = CCSprite::create("trophy_icon.png"_spr);
		// trophy->setScale(0.50f);
		// trophy->setPosition({30.f, 16.f});
		// rebeatButton->addChild(trophy);

		auto arrow = CCSprite::create("tracker_arrow_02.png"_spr);
		arrow->setPosition({23, 23});
		rebeatButton->addChild(arrow);*/
	
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

		auto rebeatSprite = CCSprite::create(buttonSpriteName.c_str());
		rebeatSprite->setScale(0.65f);
	
		auto rebeatButton = CCMenuItemSpriteExtra::create(
			rebeatSprite,
			this,
			menu_selector(FapPauseLayer::onButton)
		);
		rebeatButton->setID("rebeatBtn"_spr);
		//rebeatButton->setPosition(-185, 20);

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

		auto rebeatSprite = CCSprite::create(buttonSpriteName.c_str());
		rebeatSprite->setScale(0.75f);
	
		auto rebeatButton = CCMenuItemSpriteExtra::create(
			rebeatSprite,
			this,
			menu_selector(FapEndLevelLayer::onButton)
		);

		rebeatButton->setID("rebeatBtn"_spr);
		//rebeatButton->setPosition(-185, 20);
		auto btnMenu = m_mainLayer->getChildByID("button-menu");
		btnMenu->addChild(rebeatButton);
		auto level = PlayLayer::get()->m_level;
		if (level->m_isEditable && !level->isPlatformer())
			rebeatButton->setPosition({178, 115});
		else
			rebeatButton->setPosition({135, 120});

	}
};

$execute {
    geode::listenForSettingChanges("blue-button", +[](bool value) {
        if (value)
			buttonSpriteName = "rebeat_btn_02b.png"_spr;
		else
			buttonSpriteName = "rebeat_btn_02.png"_spr;
    });
};
