#include "RebeatCell.hpp"
#include "EditPopup.hpp"
#include "RebeatPopup.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

bool RebeatCell::init() {

    m_rebeatIndex = m_popup->m_rebeats;
    log::debug("indexs: {}, {}", m_popup->m_rebeats, m_rebeatIndex);
    m_date = "NA";
    m_time = "NA";
    std::string dateText = "";
    GameManager* gm = GameManager::get();
    bool hasCoins = false;

    if (m_rebeat.contains("date")) {
        m_date = m_rebeat["date"].asString().unwrapOr("");

        if (m_date[0] == 'f') {
        m_date.erase(0, 1);
        // if (rebeats == 1)
        //     rebeatText = fmt::format("Completion", rebeats);
        }

        dateText += m_date;
        
        /*auto dateLabel = CCLabelBMFont::create(m_date.c_str(), "chatFont.fnt");
        dateLabel->setPosition(42.5f, 12.f);
        dateLabel->setAnchorPoint({0, 0.5});
        dateLabel->setScale(0.6f);
        dateLabel->setOpacity(170);

        this->addChild(dateLabel);*/
    }

    if (m_rebeat.contains("time")) {
        m_time = Utils::getTime(m_rebeat["time"].asString().unwrapOr(""));
        log::debug("(rebeatcell) time: {}", m_time);
        if (m_time != "")
            dateText = fmt::format("{} {}", dateText, m_time);
    }

    log::debug("(rbcell) date: {}, time: {}", m_date, m_time);
    log::debug("dateText: {}", dateText);
    if (dateText != "") {
        auto dateLabel = CCLabelBMFont::create(dateText.c_str(), "chatFont.fnt");
        dateLabel->setPosition(42.5f, 12.f);
        dateLabel->setAnchorPoint({0, 0.5});
        dateLabel->setScale(0.6f);
        dateLabel->setOpacity(170);

        this->addChild(dateLabel);
    }


    if (m_rebeat.contains("icons")) {
        auto iconData = m_rebeat["icons"];
        m_iconType =  static_cast<IconType>(iconData["type"].asInt().unwrapOr(0));
        m_iconFrame = iconData["frame"].asInt().unwrapOr(0);
        m_iconColor = iconData["color_1"].asInt().unwrapOr(12);
        m_iconColor2 = iconData["color_2"].asInt().unwrapOr(12);
        m_glowColor = iconData["glow_color"].asInt().unwrapOr(12);
        m_hasGlow = iconData["has_glow"].asBool().unwrapOr(false);

        SimplePlayer* icon = SimplePlayer::create(1);
        icon->updatePlayerFrame(m_iconFrame, m_iconType);
        icon->setPosition({10.f, 20.f});
        icon->setScale(0.75f);
        icon->getChildByType<CCSprite*>(0)->setAnchorPoint({0, 0.5});

        icon->setColor(gm->colorForIdx(m_iconColor));
        icon->setSecondColor(gm->colorForIdx(m_iconColor2));
        icon->m_hasGlowOutline = m_hasGlow;

        if (icon->m_hasGlowOutline)
            icon->enableCustomGlowColor(gm->colorForIdx(m_glowColor));
        else
            icon->disableCustomGlowColor();

        icon->updateColors();
        this->addChild(icon);
        }

    if (m_rebeat.contains("coins")) {
        auto coinData = m_rebeat["coins"];
        m_coinAmount = coinData["amount"].asInt().unwrapOr(0);

        hasCoins = (m_coinAmount > 0);

        if (hasCoins) {
            std::vector<float> xPositions; //(coinAmount == 3) ? {156, 170, 174} : (coinAmount == 2) ? {163, 177} : {170};

            switch (m_coinAmount) {
                case 3:
                    xPositions = {156.f, 170.f, 184.f};
                    break;
                case 2:
                    xPositions = {163.f, 177.f};
                    break;
                case 1:
                    xPositions = {170.f};
                    break;
            }

            //bool isRobtopLevel = false;
            //float xPos = 156.f;

            if (m_popup->m_id[0] != 'c') {
                int levelID = stoi(m_popup->m_id);
                m_popup->m_isRobtopLevel = (levelID <= 24 || (levelID > 5000 && levelID < 5005) || levelID == 3001);
            }

            for (int j = 0; j < m_coinAmount; j++) {
                m_coinsCollected[j]= coinData[std::to_string(j)].asBool().unwrapOr(false);
                //m_coinsCollected[j] = isCollected;
                CCSprite* coinSprite;
                CCTexture2D* coinTexture;

                if (m_popup->m_isRobtopLevel) {

                    if (m_coinsCollected[j])
                        coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png");
                    else 
                        coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_gray_001.png");
                }
                else {
                    coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png");

                    if (!m_coinsCollected[j])
                        coinSprite->setColor({165, 165, 165});
                    /*if (isCollected)
                        coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png");
                    else
                        coinSprite = CCSprite::create("GJ_coinsIcon2_gray_001.png"_spr);*/
                }

                coinSprite->setPosition({xPositions[j] + 40.f, 20.f});
                coinSprite->setScale(0.75f);
                this->addChild(coinSprite);

                //xPos += 14.f;
            }

        }
    }

    float offset = (hasCoins) ? 0 : -55;
    
    if (m_rebeat.contains("level_time")) {
        m_levelTime = m_rebeat["level_time"].asString().unwrapOr("NA");
        auto timeLabel = CCLabelBMFont::create(m_levelTime.c_str(), "bigFont.fnt");
        timeLabel->setAnchorPoint({0, 0.5});
        timeLabel->setPosition(216.f + offset + 40.f, 30.5f);
        timeLabel->setScale(0.4f);
        timeLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f); //82

        auto timeSprite = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
        timeSprite->setPosition({208 + offset + 40.f, 29});
        timeSprite->setScale(0.5f);

        this->addChild(timeLabel);
        this->addChild(timeSprite);
    }

    if (m_rebeat.contains("points")) {
        m_points = m_rebeat["points"].asString().unwrapOr("NA");
        auto pointsLabel = CCLabelBMFont::create(m_points.c_str(), "bigFont.fnt");
        pointsLabel->setAnchorPoint({0, 0.5});
        pointsLabel->setPosition(216.f + offset + 40.f, 12.5);
        pointsLabel->setScale(0.4f);
        pointsLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f);
        
        auto pointsSprite = CCSprite::createWithSpriteFrameName("GJ_pointsIcon_001.png");
        pointsSprite->setPosition({208 + offset + 40.f, 11});
        pointsSprite->setScale(0.5f);

        this->addChild(pointsLabel);
        this->addChild(pointsSprite);
    }

    if (m_rebeat.contains("attempts")) {
        m_attempts = m_rebeat["attempts"].asString().unwrapOr("NA");

        if (m_popup->m_calculateAttempts) {
            auto newAttempts = std::to_string(std::stoi(m_attempts) - m_popup->m_prevAttempts);
            m_popup->m_prevAttempts = std::stoi(m_attempts);
            m_attempts = newAttempts;
        }

        auto attemptsLabel = CCLabelBMFont::create(m_attempts.c_str(), "bigFont.fnt");
        attemptsLabel->setAnchorPoint({0, 0.5});
        attemptsLabel->setPosition(216 + offset + 40.f, 30.5);
        attemptsLabel->setScale(0.4f);
        attemptsLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f); //90

        if (m_popup->m_calculateAttempts)
            attemptsLabel->setColor(ccc3(0, 255, 0));

        auto attemptsSprite = CCSprite::create("attempts.png"_spr);
        attemptsSprite->setPosition({208 + offset + 40.f, 29});
        attemptsSprite->setScale(0.53f);

        this->addChild(attemptsLabel);
        this->addChild(attemptsSprite);
    }

    if (m_rebeat.contains("jumps")) {
        m_jumps = m_rebeat["jumps"].asString().unwrapOr("NA");
        auto jumpsLabel = CCLabelBMFont::create(m_jumps.c_str(), "bigFont.fnt");
        jumpsLabel->setAnchorPoint({0, 0.5});
        jumpsLabel->setPosition(216 + offset + 40.f, 12.5f);
        jumpsLabel->setScale(0.4f);
        jumpsLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f);

        auto jumpsSprite = CCSprite::create("jumps.png"_spr);
        jumpsSprite->setPosition({208 + offset + 40.f, 11});
        jumpsSprite->setScale(0.5f);
    
        this->addChild(jumpsLabel);
        this->addChild(jumpsSprite);
    }
    
    //std::string rebeatText;

    if (m_rebeat.contains("name")) {
        m_name = m_rebeat["name"].asString().unwrapOr("NA");
        //rebeatText = m_name;
    }
    else 
        m_name = fmt::format("Completion {}", m_rebeatIndex);

    //rebeats++;

    auto rebeatLabel = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
    rebeatLabel->setAnchorPoint({0, 0.5});
    rebeatLabel->setPosition(42.f, 28.f);
    rebeatLabel->setScale(0.55f);
    rebeatLabel->limitLabelWidth(96.f + 40.f, 0.55f, 0.001f);

    this->addChild(rebeatLabel);

    auto menu = CCMenu::create();
    menu->setPosition(this->getPositionX(), this->getPositionY());
    this->addChild(menu);

    auto trashSprite = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    trashSprite->setScale(0.71f);
    auto trashButton = CCMenuItemSpriteExtra::create(trashSprite, this, menu_selector(RebeatCell::onDelete));
    trashButton->setPosition({355.f, 20.f}); //317 // no +40.f
    //trashButton->setScale(0.65f);
    //trashButton->setAnchorPoint({1, 0.5});
    menu->addChild(trashButton);

    auto editSprite = CCSprite::createWithSpriteFrameName("GJ_viewLevelsBtn_001.png");
    editSprite->setScale(0.65f);
    auto editButton = CCMenuItemSpriteExtra::create(editSprite, this, menu_selector(RebeatCell::onEdit));
    editButton->setPosition({277.f + 40.f, 20.f}); //317
    //trashButton->setScale(0.65f);
    //trashButton->setAnchorPoint({1, 0.5});
    menu->addChild(editButton);

    return true;
}

RebeatCell* RebeatCell::create(const matjson::Value &rebeat, RebeatPopup* popup) {
    auto ret = new RebeatCell();
        ret->m_rebeat = rebeat;
        ret->m_popup = popup;

        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
}

void RebeatCell::onDelete(CCObject* obj) {
    geode::createQuickPopup(
        "Warning", 
        "Are you sure you want to <cr>delete</c> this completion? This <cr>cannot</c> be undone.",
        "Cancel", "Delete",
        [this](auto, bool btn2) {
            if (btn2)  {
                return m_popup->deleteCell(m_popup->m_cells->indexOfObject(this));
            }
        }
    );
}

void RebeatCell::onEdit(CCObject* obj) {
    auto popup = EditPopup::create(m_popup, this);
    popup->show();
    return;
}
