#include "RebeatCell.hpp"
#include "EditPopup.hpp"
#include "RebeatPopup.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

std::string formatStat(std::string stat) {
    std::string formattedStat = "";

    for (int i = 1; i <= stat.length(); i++) {
        formattedStat = stat[stat.length() - i] + formattedStat;

        if (i % 3 == 0 && i != stat.length()) {
            formattedStat = "," + formattedStat;
        }
    }

    return formattedStat;
}

bool RebeatCell::init() {
    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
	bg->setColor({0, 0, 0});
    bg->setOpacity(25);
    bg->setPosition({5, 0});
    bg->setAnchorPoint({0, 0});
    bg->setContentSize({740, 80});
    bg->setScale(0.5f);
	addChild(bg);

    m_rebeatIndex = m_popup->m_rebeats;
    m_date = "";
    m_time = "";
    std::string dateText = "";
    GameManager* gm = GameManager::get();
    bool hasCoins = false;

    auto menu = CCMenu::create();
    menu->setPosition(getPositionX(), getPositionY());
    addChild(menu);

    if (m_rebeat.contains("date")) {
        m_date = m_rebeat["date"].asString().unwrapOr("");

        if (m_date[0] == 'f') {
            m_date.erase(0, 1);
        }

        dateText += m_date;
    }

    if (m_rebeat.contains("time")) {
        m_time = Utils::getTime(m_rebeat["time"].asString().unwrapOr(""));
        
        if (m_time != "") {
            dateText = fmt::format("{} {}", dateText, m_time);
        }
    }

    if (dateText != "") {
        m_dateLabel = CCLabelBMFont::create(dateText.c_str(), "chatFont.fnt");
        m_dateLabel->setPosition(42.5f, 12.f);
        m_dateLabel->setAnchorPoint({0, 0.5});
        m_dateLabel->setScale(0.6f);
        m_dateLabel->setOpacity(170);

        addChild(m_dateLabel);
    }

    if (m_rebeat.contains("link")) {
        m_ytLink = m_rebeat["link"].asString().unwrapOr("");

        auto ytSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
        ytSprite->setScale(0.5f);
        
        auto ytBtn = CCMenuItemSpriteExtra::create(ytSprite, this, menu_selector(RebeatCell::onVideo));
        menu->addChild(ytBtn);

        if (m_dateLabel) {
            float labelEdge = m_dateLabel->getPosition().x + m_dateLabel->getContentSize().width * m_dateLabel->getScale();
            ytBtn->setPosition({labelEdge + 10.f, 12.f});
        } 
        else {
            ytBtn->setPosition(42.5f + (ytSprite->getContentWidth() / 2), 12.f);
        }
    }

    if (m_rebeat.contains("icons")) {
        auto iconData = m_rebeat["icons"];
        auto iconTypeInt = iconData["type"].asInt().unwrapOr(0);

        if (iconTypeInt < 0 || iconTypeInt > 8) {
            iconTypeInt = 0;
        }

        m_iconType =  static_cast<IconType>(iconTypeInt);
        m_iconFrame = iconData["frame"].asInt().unwrapOr(0);
        m_iconColor = iconData["color_1"].asInt().unwrapOr(12);
        m_iconColor2 = iconData["color_2"].asInt().unwrapOr(12);
        m_glowColor = iconData["glow_color"].asInt().unwrapOr(12);
        m_hasGlow = iconData["has_glow"].asBool().unwrapOr(false);

        SimplePlayer* icon = SimplePlayer::create(1);
        icon->updatePlayerFrame(m_iconFrame, m_iconType);
        icon->setPosition({10.f, 20.f});
        icon->setScale(0.75f);

        if (m_iconType == IconType::Robot) {
            icon->m_robotSprite->setAnchorPoint({0, 0.5});
        }
        else if (m_iconType == IconType::Spider) {
            icon->m_spiderSprite->setAnchorPoint({0, 0.5});
            icon->setPosition({13.f, 20.f});
        } 
        else {
            auto iconSprite = icon->getChildByType<CCSprite*>(0);
            iconSprite->setAnchorPoint({0, 0.5});
            
            if (m_iconType == IconType::Ufo && iconSprite->getContentWidth() > 40) {
                iconSprite->setScale(0.75f);
            }
        }

        icon->setColor(gm->colorForIdx(m_iconColor));
        icon->setSecondColor(gm->colorForIdx(m_iconColor2));
        icon->m_hasGlowOutline = m_hasGlow;

        if (icon->m_hasGlowOutline) {
            icon->enableCustomGlowColor(gm->colorForIdx(m_glowColor));
        }
        else {
            icon->disableCustomGlowColor();
        }

        icon->updateColors();
        addChild(icon);
    }

    if (m_rebeat.contains("watermark")) {
        m_watermark = m_rebeat["watermark"].asString().unwrapOr("");
        auto watermarkLabel = CCLabelBMFont::create(m_watermark.c_str(), "chatFont.fnt");
        watermarkLabel->setScale(0.3f);
        watermarkLabel->setOpacity(64.f);
        watermarkLabel->setPosition({23.f, 4.f});
        addChild(watermarkLabel);
    }

    if (m_rebeat.contains("coins")) {
        auto coinData = m_rebeat["coins"];
        m_coinAmount = coinData["amount"].asInt().unwrapOr(0);

        if (m_coinAmount > 3) {
            m_coinAmount = 3;
        }

        hasCoins = (m_coinAmount > 0);

        if (hasCoins && m_coinAmount <= 3) {
            std::vector<float> xPositions;

            float coinCenter = 210.f;
            float coinOffset = 14.f;

            switch (m_coinAmount) {
                case 1: xPositions = {coinCenter}; break;
                case 2: xPositions = {coinCenter - coinOffset / 2, coinCenter + coinOffset / 2}; break;
                case 3: xPositions = {coinCenter - coinOffset, coinCenter, coinCenter + coinOffset}; break;
            }

            if (m_popup->m_id[0] != 'c') {
                int levelID = stoi(m_popup->m_id);
                m_popup->m_isRobtopLevel = (levelID <= 24 || (levelID > 5000 && levelID < 5005) || levelID == 3001);
            }

            for (int j = 0; j < m_coinAmount; j++) {
                m_coinsCollected[j]= coinData[std::to_string(j)].asBool().unwrapOr(false);
                CCSprite* coinSprite;
                CCTexture2D* coinTexture;

                if (m_popup->m_isRobtopLevel) {

                    if (m_coinsCollected[j]) {
                        coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png");
                    }
                    else {
                        coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon_gray_001.png");
                    }
                }
                else {
                    coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png");

                    if (!m_coinsCollected[j]) {
                        coinSprite->setColor({165, 165, 165});
                    }
                }

                coinSprite->setPosition({xPositions[j], 20.f});
                coinSprite->setScale(0.75f);
                addChild(coinSprite);
            }

        }
    }

    float offset = (hasCoins) ? 0 : -55;
    
    if (m_rebeat.contains("level_time")) {
        m_levelTime = m_rebeat["level_time"].asString().unwrapOr("");
        auto timeLabel = CCLabelBMFont::create(m_levelTime.c_str(), "bigFont.fnt");
        timeLabel->setAnchorPoint({0, 0.5});
        timeLabel->setPosition(256.f + offset, 30.5f);
        timeLabel->setScale(0.4f);
        timeLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f);

        auto timeSprite = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
        timeSprite->setPosition({248 + offset, 29});
        timeSprite->setScale(0.5f);

        addChild(timeLabel);
        addChild(timeSprite);
    }

    if (m_rebeat.contains("points")) {
        m_points = m_rebeat["points"].asString().unwrapOr("");

        auto pointsLabel = CCLabelBMFont::create(formatStat(m_points).c_str(), "bigFont.fnt");
        pointsLabel->setAnchorPoint({0, 0.5});
        pointsLabel->setPosition(256.f + offset, 12.5);
        pointsLabel->setScale(0.4f);
        pointsLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f);
        
        auto pointsSprite = CCSprite::createWithSpriteFrameName("GJ_pointsIcon_001.png");
        pointsSprite->setPosition({248 + offset, 11});
        pointsSprite->setScale(0.5f);

        addChild(pointsLabel);
        addChild(pointsSprite);
    }

    if (m_rebeat.contains("attempts")) {
        m_attempts = m_rebeat["attempts"].asString().unwrapOr("");

        auto attemptsLabel = CCLabelBMFont::create(formatStat(m_attempts).c_str(), "bigFont.fnt");
        attemptsLabel->setAnchorPoint({0, 0.5});
        attemptsLabel->setPosition(256 + offset, 30.5);
        attemptsLabel->setScale(0.4f);
        attemptsLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f); //90

        auto attemptsSprite = CCSprite::createWithSpriteFrameName("miniSkull_001.png");
        attemptsSprite->setPosition({248 + offset, 29});
        attemptsSprite->setScale(0.53f);

        addChild(attemptsLabel);
        addChild(attemptsSprite);
    }

    if (m_rebeat.contains("jumps")) {
        m_jumps = m_rebeat["jumps"].asString().unwrapOr("");

        auto jumpsLabel = CCLabelBMFont::create(formatStat(m_jumps).c_str(), "bigFont.fnt");
        jumpsLabel->setAnchorPoint({0, 0.5});
        jumpsLabel->setPosition(256 + offset, 12.5f);
        jumpsLabel->setScale(0.4f);
        jumpsLabel->limitLabelWidth(42.f - offset, 0.4f, 0.001f);

        auto jumpsSprite = CCSprite::create("jumps.png"_spr);
        jumpsSprite->setPosition({248 + offset, 11});
        jumpsSprite->setScale(0.5f);
    
        addChild(jumpsLabel);
        addChild(jumpsSprite);
    }
    

    if (m_rebeat.contains("name")) {
        m_name = m_rebeat["name"].asString().unwrapOr("Unnamed Completion");
    }
    else {
        m_name = fmt::format("Completion {}", m_rebeatIndex);
    }

    auto rebeatLabel = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
    rebeatLabel->setAnchorPoint({0, 0.5});
    rebeatLabel->setPosition(42.f, 28.f);
    rebeatLabel->setScale(0.55f);
    rebeatLabel->limitLabelWidth(136.f, 0.55f, 0.001f);

    addChild(rebeatLabel);

    auto trashSprite = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    trashSprite->setScale(0.71f);
    auto trashButton = CCMenuItemSpriteExtra::create(trashSprite, this, menu_selector(RebeatCell::onDelete));
    trashButton->setPosition({355.f, 20.f});
    menu->addChild(trashButton);

    auto editSprite = CCSprite::createWithSpriteFrameName("GJ_viewLevelsBtn_001.png");
    editSprite->setScale(0.65f);
    auto editButton = CCMenuItemSpriteExtra::create(editSprite, this, menu_selector(RebeatCell::onEdit));
    editButton->setPosition({317, 20.f});
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
        fmt::format("Are you sure you want to <cr>delete</c> '{}'? This <cr>cannot</c> be undone.", m_name),
        "Cancel", "Delete",
        [this](auto, bool btn2) {
            if (btn2)  {
                return m_popup->deleteCell(m_popup->m_cells->indexOfObject(this), m_rebeatIndex);
            }
        }
    );
}

void RebeatCell::onEdit(CCObject* obj) {
    auto popup = EditPopup::create(m_popup, this);
    popup->show();
    return;
}

void RebeatCell::onVideo(CCObject* obj) {
    geode::utils::web::openLinkInBrowser(m_ytLink);
}
