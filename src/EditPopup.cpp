#include "EditPopup.hpp"
#include "RebeatPopup.hpp"
#include "RebeatCell.hpp"
#include "Utils.hpp"
#include "ColorSelectLayer.hpp"
#include "IconSelectLayer.hpp"
#include <chrono>
#include "YoutubeLayer.hpp"

using namespace geode::prelude;
using namespace std::chrono;

bool EditPopup::setup() {
    if (m_isCreate) {
        setTitle("Create Completion");
    }
    else {
        setTitle(fmt::format("Edit {}", m_cell->m_name));
    }

    auto titleWidth = m_title->getContentSize().width;
    m_title->limitLabelWidth(320.f, 0.8f, 0.001f);

    // BGs
    auto leftBg = CCScale9Sprite::create("square02b_001.png");
    leftBg->setContentSize({132, 200});
    leftBg->setPosition({78, 150});
    leftBg->setColor({ 0,0,0});
    leftBg->setOpacity(75);
    leftBg->setID("leftBg"_spr);
    m_mainLayer->addChild(leftBg);

    auto centerBg = CCScale9Sprite::create("square02b_001.png");
    centerBg->setContentSize({132, 200});
    centerBg->setPosition({220, 150});
    centerBg->setColor({ 0,0,0});
    centerBg->setOpacity(75);
    centerBg->setID("centerBg"_spr);
    m_mainLayer->addChild(centerBg);

    auto rightBg = CCScale9Sprite::create("square02b_001.png");
    rightBg->setContentSize({132, 200});
    rightBg->setPosition({362, 150});
    rightBg->setColor({ 0,0,0});
    rightBg->setOpacity(75);
    rightBg->setID("rightBg"_spr);
    m_mainLayer->addChild(rightBg);


    // Menu
    auto popupMenu = CCMenu::create();
    popupMenu->setContentSize({440, 290});
    popupMenu->setPosition({0, 0});
    popupMenu->setAnchorPoint({0.5f, 0.5f});
    m_mainLayer->addChild(popupMenu);


    // Youtube
    auto ytSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
    ytSprite->setScale(0.75f);
    auto ytBtn = CCMenuItemSpriteExtra::create(ytSprite, this, menu_selector(EditPopup::onYoutube));
    log::debug("width: {}, {}", m_title->getContentWidth(), m_title->getContentWidth() / 2.f);
    float titleEdge = m_title->getPosition().x + m_title->getContentSize().width * m_title->getScale() / 2.f;
    ytBtn->setPosition({titleEdge + 18.f, 270.f});
    popupMenu->addChild(ytBtn);
    
    if (!m_isCreate) {
        m_ytLink = m_cell->m_ytLink;
    }


    // Info buttons
    auto mainInfoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    mainInfoSprite->setScale(0.75f);

    auto infoBtn = CCMenuItemSpriteExtra::create(mainInfoSprite, this, menu_selector(EditPopup::onInfo));
    infoBtn->setPosition({422, 270});
    popupMenu->addChild(infoBtn);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSprite->setScale(0.6f);

    auto infoInfoBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(EditPopup::onInfoInfo));
    infoInfoBtn->setPosition({133, 239});
    popupMenu->addChild(infoInfoBtn);

    auto iconInfoBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(EditPopup::onIconInfo));
    iconInfoBtn->setPosition({275, 239});
    popupMenu->addChild(iconInfoBtn);

    auto statsInfoBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(EditPopup::onStatsInfo));
    statsInfoBtn->setPosition({417, 239});
    popupMenu->addChild(statsInfoBtn);


    // Icon
    auto iconArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconArrowSprite->setScale(0.55f);
    auto leftIconArrow = CCMenuItemSpriteExtra::create(iconArrowSprite, this, menu_selector(EditPopup::onLeftIconArrow));
    leftIconArrow->setPosition({184,200});
    popupMenu->addChild(leftIconArrow);

    auto rightIconArrow = CCMenuItemSpriteExtra::create(iconArrowSprite, this, menu_selector(EditPopup::onRightIconArrow));
    rightIconArrow->setPosition({256, 200});
    rightIconArrow->setRotationY(180);
    popupMenu->addChild(rightIconArrow);

    auto resetIconSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
    resetIconSprite->setScale(0.75f);
    auto resetIconBtn = CCMenuItemSpriteExtra::create(resetIconSprite, this, menu_selector(EditPopup::onResetIcon));
    resetIconBtn->setPosition({180, 110});
    popupMenu->addChild(resetIconBtn);

    auto garageSprite = CCSprite::create("garage_btn.png"_spr);
    garageSprite->setScale(0.75f);
    auto garageBtn = CCMenuItemSpriteExtra::create(garageSprite, this, menu_selector(EditPopup::onGarage));
    garageBtn->setPosition({220, 110});
    popupMenu->addChild(garageBtn);

    auto pasteIconSprite = CCSprite::create("paste_icon_btn.png"_spr);
    pasteIconSprite->setScale(0.75f);
    auto pasteIconBtn = CCMenuItemSpriteExtra::create(pasteIconSprite, this, menu_selector(EditPopup::onPasteIcon));
    pasteIconBtn->setPosition({260, 110});
    popupMenu->addChild(pasteIconBtn);

    auto color1Sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    color1Sprite->setScale(0.6f);
    auto color1Label = CCLabelBMFont::create("1", "bigFont.fnt");
    color1Label->setScale(0.525f);
    color1Label->setPosition({24, 14});
    color1Sprite->addChild(color1Label);
    m_color1Btn = CCMenuItemSpriteExtra::create(color1Sprite, this, menu_selector(EditPopup::onColor));
    m_color1Btn->setPosition({190, 155});
    m_color1Btn->setTag(1);
    popupMenu->addChild(m_color1Btn);

    auto color2Sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    color2Sprite->setScale(0.6f);
    auto color2Label = CCLabelBMFont::create("2", "bigFont.fnt");
    color2Label->setScale(0.525f);
    color2Label->setPosition({24, 14});
    color2Sprite->addChild(color2Label);
    m_color2Btn = CCMenuItemSpriteExtra::create(color2Sprite, this, menu_selector(EditPopup::onColor));
    m_color2Btn->setPosition({220, 155});
    m_color2Btn->setTag(2);
    popupMenu->addChild(m_color2Btn);

    auto color3Sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    color3Sprite->setScale(0.6f);
    auto color3Label = CCLabelBMFont::create("3", "bigFont.fnt");
    color3Label->setScale(0.525f);
    color3Label->setPosition({24, 14});
    color3Sprite->addChild(color3Label);
    m_color3Btn = CCMenuItemSpriteExtra::create(color3Sprite, this, menu_selector(EditPopup::onColor));
    m_color3Btn->setPosition({250, 155});
    m_color3Btn->setTag(3);
    popupMenu->addChild(m_color3Btn);

    m_icon = SimplePlayer::create(1);

    if (!m_isCreate) {
        resetIconValues();
        updateColors();
    }
    else {
        updateIcon();
    }

    m_icon->setPosition({220, 200});
    m_mainLayer->addChild(m_icon);

    auto glowToggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto glowToggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    m_glowToggle = CCMenuItemToggler::create(glowToggleOff, glowToggleOn, this, menu_selector(EditPopup::onToggleGlow));
    m_glowToggle->setPosition(172, 68);
    m_glowToggle->setScale(0.6f);
    popupMenu->addChild(m_glowToggle);

    if (m_hasGlow)
        m_glowToggle->toggle(true);


    auto glowLabel = CCLabelBMFont::create("Glow", "bigFont.fnt");
    glowLabel->setPosition(205, 69);
    glowLabel->setScale(0.45f);
    popupMenu->addChild(glowLabel);


    // BG Titles
    auto infoTitle = CCLabelBMFont::create("Info", "goldFont.fnt");
    infoTitle->setPosition({78, 240});
    infoTitle->setScale(0.65f);
    m_mainLayer->addChild(infoTitle);

    auto iconTitle = CCLabelBMFont::create("Icon", "goldFont.fnt");
    iconTitle->setPosition({220, 240});
    iconTitle->setScale(0.65f);
    m_mainLayer->addChild(iconTitle);

    auto statsTitle = CCLabelBMFont::create("Stats", "goldFont.fnt");
    statsTitle->setPosition({362, 240});
    statsTitle->setScale(0.65f);
    m_mainLayer->addChild(statsTitle);


    // Bottom Menu
    auto bottomMenu = CCMenu::create();
    bottomMenu->setLayout(RowLayout::create()
        ->setGap(10.f)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setAutoScale(false)
    );
    bottomMenu->setPosition({220, 25});
    m_mainLayer->addChild(bottomMenu);

    auto cancelSprite = ButtonSprite::create("Cancel");
    cancelSprite->setScale(0.9f);
    auto cancelBtn = CCMenuItemSpriteExtra::create(cancelSprite, this, menu_selector(EditPopup::onClose));
    bottomMenu->addChild(cancelBtn);

    auto resetSprite = ButtonSprite::create("Reset");
    resetSprite->setScale(0.9f);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSprite, this, menu_selector(EditPopup::onReset));
    bottomMenu->addChild(resetBtn);

    auto saveSprite = ButtonSprite::create("Save");
    saveSprite->setScale(0.9f);
    auto saveBtn = CCMenuItemSpriteExtra::create(saveSprite, this, menu_selector(EditPopup::onSave));
    bottomMenu->addChild(saveBtn);

    bottomMenu->updateLayout();


    // Name
    auto nameLabel = CCLabelBMFont::create("Name", "bigFont.fnt");
    nameLabel->setPosition({78, 215});
    nameLabel->setScale(0.5f);
    m_mainLayer->addChild(nameLabel);

    auto resetNameSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
    resetNameSprite->setScale(0.45f);
    auto resetNameBtn = CCMenuItemSpriteExtra::create(resetNameSprite, this, menu_selector(EditPopup::onResetName));
    resetNameBtn->setPosition({42, 214});
    popupMenu->addChild(resetNameBtn);

    auto autoNameSprite = CCSprite::create("auto_btn.png"_spr);
    autoNameSprite->setScale(0.45f);
    auto autoNameBtn = CCMenuItemSpriteExtra::create(autoNameSprite, this, menu_selector(EditPopup::onAutoName));
    autoNameBtn->setPosition({114, 214});
    popupMenu->addChild(autoNameBtn);

    m_nameInput = TextInput::create(170.f, "...", "bigFont.fnt");
    m_nameInput->setScale(0.65f);

    if (!m_isCreate)
        m_nameInput->setString(m_cell->m_name);

    m_nameInput->setPosition({78, 190});
    m_mainLayer->addChild(m_nameInput);


    // Date
    // e.g. 2025-06-21 02:38:47 PM
    auto dateLabel = CCLabelBMFont::create("Date", "bigFont.fnt");
    dateLabel->setScale(0.5f);
    dateLabel->setPosition(78, 160);
    m_mainLayer->addChild(dateLabel);

    auto resetDateSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
    resetDateSprite->setScale(0.45f);
    auto resetDateBtn = CCMenuItemSpriteExtra::create(resetDateSprite, this, menu_selector(EditPopup::onResetDate));
    resetDateBtn->setPosition({42, 159});
    popupMenu->addChild(resetDateBtn);

    auto autoDateSprite = CCSprite::create("auto_btn.png"_spr);
    autoDateSprite->setScale(0.45f);
    auto autoDateBtn = CCMenuItemSpriteExtra::create(autoDateSprite, this, menu_selector(EditPopup::onAutoDate));
    autoDateBtn->setPosition({114, 159});
    popupMenu->addChild(autoDateBtn);

    std::string numbers = "0123456789";

    m_yearInput = TextInput::create(50.f, "yyyy", "bigFont.fnt");
    m_yearInput->setScale(0.65f);
    m_yearInput->setPosition(38, 135);
    m_yearInput->setCommonFilter(CommonFilter::Uint);
    m_yearInput->setMaxCharCount(4);
    m_mainLayer->addChild(m_yearInput);

    m_monthInput = TextInput::create(50.f, "mm", "bigFont.fnt");
    m_monthInput->setScale(0.65f);
    m_monthInput->setPosition(78, 135);
    m_monthInput->setCommonFilter(CommonFilter::Uint);
    m_monthInput->setMaxCharCount(2);
    m_mainLayer->addChild(m_monthInput);

    m_dayInput = TextInput::create(50.f, "dd", "bigFont.fnt");
    m_dayInput->setScale(0.65f);
    m_dayInput->setPosition(118, 135);
    m_dayInput->setCommonFilter(CommonFilter::Uint);
    m_dayInput->setMaxCharCount(2);
    m_mainLayer->addChild(m_dayInput);

    for (int i = 0; i < 2; i++) {
        auto dash = CCLabelBMFont::create("-", "bigFont.fnt");
        dash->setScale(0.3f);
        dash->setOpacity(175);
        dash->setPosition({(i % 2) ? 98.f : 58.f, 135.f});
        m_mainLayer->addChild(dash);
    }


    // Time
    auto timeLabel = CCLabelBMFont::create("Time", "bigFont.fnt");
    timeLabel->setScale(0.5f);
    timeLabel->setPosition(78, 105);
    m_mainLayer->addChild(timeLabel);

    auto resetTimeSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
    resetTimeSprite->setScale(0.45f);
    auto resetTimeBtn = CCMenuItemSpriteExtra::create(resetTimeSprite, this, menu_selector(EditPopup::onResetTime));
    resetTimeBtn->setPosition({42, 104});
    popupMenu->addChild(resetTimeBtn);

    auto autoTimeSprite = CCSprite::create("auto_btn.png"_spr);
    autoTimeSprite->setScale(0.45f);
    auto autoTimeBtn = CCMenuItemSpriteExtra::create(autoTimeSprite, this, menu_selector(EditPopup::onAutoTime));
    autoTimeBtn->setPosition({114, 104});
    popupMenu->addChild(autoTimeBtn);

    m_hourInput = TextInput::create(50.f, "hh", "bigFont.fnt");
    m_hourInput->setScale(0.65f);
    m_hourInput->setPosition(38, 80);
    m_hourInput->setCommonFilter(CommonFilter::Uint);
    m_hourInput->setMaxCharCount(4);
    m_mainLayer->addChild(m_hourInput);

    m_minuteInput = TextInput::create(50.f, "mm", "bigFont.fnt");
    m_minuteInput->setScale(0.65f);
    m_minuteInput->setPosition(78, 80);
    m_minuteInput->setCommonFilter(CommonFilter::Uint);
    m_minuteInput->setMaxCharCount(2);
    m_mainLayer->addChild(m_minuteInput);

    m_secondInput = TextInput::create(50.f, "ss", "bigFont.fnt");
    m_secondInput->setScale(0.65f);
    m_secondInput->setPosition(118, 80);
    m_secondInput->setCommonFilter(CommonFilter::Uint);
    m_secondInput->setMaxCharCount(2);
    m_mainLayer->addChild(m_secondInput);
    
    if (Mod::get()->getSettingValue<std::string>("time-format") == "12-hour") {
        auto timeBg = CCScale9Sprite::create("square02b_001.png");
        timeBg->setColor({ 0,0,0});
        timeBg->setOpacity(90);
        timeBg->setContentSize({80.f, 60.f});
        timeBg->setPosition({78, 60});
        timeBg->setScale(0.25f);
        m_mainLayer->addChild(timeBg);

        m_timeSettingText = CCLabelBMFont::create((m_isCreate) ? "AM" : m_cellTimes[3].c_str(), "bigFont.fnt");
        m_timeSettingText->setScale(0.3f);
        m_timeSettingText->setPosition({78, 60});
        m_mainLayer->addChild(m_timeSettingText);
        
        auto timeArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        timeArrowSprite->setScale(0.35f);
    
        auto timeRightArrow = CCMenuItemSpriteExtra::create(timeArrowSprite, this, menu_selector(EditPopup::onTimeArrow));
        timeRightArrow->setRotationY(180);
        timeRightArrow->setPosition({97, 60});
        popupMenu->addChild(timeRightArrow);
    
        auto timeLeftArrow = CCMenuItemSpriteExtra::create(timeArrowSprite, this, menu_selector(EditPopup::onTimeArrow));
        timeLeftArrow->setPosition({59, 60});
        popupMenu->addChild(timeLeftArrow);
    }


    if (!m_isCreate) {
        if (m_cell->m_date != "") {
            m_cellDates = Utils::splitString(m_cell->m_date, '-');
        }

        if (m_cell->m_time != "") {
            auto splitTimes = Utils::splitString(Utils::getTime(m_cell->m_time), ':');

            if (m_timeSettingText) {
                m_cellTimes[3] = Utils::isAMorPM(splitTimes[splitTimes.size() - 1]);
                splitTimes[splitTimes.size() - 1].erase(splitTimes[splitTimes.size() - 1].size() - 3);
            }
            
            for (int i = 0; i < splitTimes.size(); i++) {
                m_cellTimes[i + (3 - splitTimes.size())] = splitTimes[i];
            }
        }

        onResetDate(nullptr);
        onResetTime(nullptr);
    }

    for (int i = 0; i < 2; i++) {
        auto colon = CCLabelBMFont::create(":", "bigFont.fnt");
        colon->setScale(0.3f);
        colon->setOpacity(175);
        colon->setPosition({(i % 2) ? 98.f : 58.f, 80.f});
        m_mainLayer->addChild(colon);
    }


    // Coins
    auto coinsLabel = CCLabelBMFont::create("Coins", "bigFont.fnt");
    coinsLabel->setPosition({362, 215});
    coinsLabel->setScale(0.5f);
    m_mainLayer->addChild(coinsLabel);

    if (m_isCreate) {
        m_coinAmount = m_popup->m_level->m_coins;
    }
    else {
        m_coinAmount = m_cell->m_coinAmount;
    }

    if (m_coinAmount > 3) {
        m_coinAmount = 3;
    }

    if (m_coinAmount > 0) {
        auto resetCoinsSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
        resetCoinsSprite->setScale(0.45f);
        auto resetCoinsBtn = CCMenuItemSpriteExtra::create(resetCoinsSprite, this, menu_selector(EditPopup::onResetCoins));
        resetCoinsBtn->setPosition({326, 214});
        popupMenu->addChild(resetCoinsBtn);

        auto autoCoinsSprite = CCSprite::create("auto_btn.png"_spr);
        autoCoinsSprite->setScale(0.45f);
        auto autoCoinsBtn = CCMenuItemSpriteExtra::create(autoCoinsSprite, this, menu_selector(EditPopup::onAutoCoins));
        autoCoinsBtn->setPosition({398, 214});
        popupMenu->addChild(autoCoinsBtn);

        std::string coinSpriteName;
        std::string coinGraySpriteName;
    
        if (m_popup->m_isRobtopLevel) {
            coinSpriteName = "GJ_coinsIcon_001.png";
            coinGraySpriteName = "GJ_coinsIcon_gray_001.png";
        }
        else {
            coinSpriteName = "GJ_coinsIcon2_001.png";
            coinGraySpriteName = "GJ_coinsIcon2_gray_001.png"_spr;
        }

        std::vector<float> xPositions;
        float coinCenter = 362.f;
        float coinOffset = 24.f;

        switch (m_coinAmount) {
            case 3: xPositions = {coinCenter - coinOffset, coinCenter, coinCenter + coinOffset}; break;
            case 2: xPositions = {coinCenter - coinOffset / 2, coinCenter + coinOffset / 2}; break;
            case 1: xPositions = {coinCenter}; break;
        }
    
        auto coin1Sprite = CCSprite::createWithSpriteFrameName(coinSpriteName.c_str());
        coin1Sprite->setScale(0.85f);

        CCSprite* coin1GraySprite;
        if (m_popup->m_isRobtopLevel) {
            coin1GraySprite = CCSprite::createWithSpriteFrameName(coinGraySpriteName.c_str());
        }
        else {
            coin1GraySprite = CCSprite::create(coinGraySpriteName.c_str());
        }

        coin1GraySprite->setScale(0.85f);
    
        m_coin1Toggle = CCMenuItemToggler::create(coin1GraySprite, coin1Sprite, this, menu_selector(EditPopup::onCoin));
        m_coin1Toggle->setPosition({xPositions[0], 190});
        m_coin1Toggle->setTag(0);
    
        if (!m_isCreate) {
            m_coinsCollected[0] = m_cell->m_coinsCollected[0];
            m_coin1Toggle->toggle(m_coinsCollected[0]);
        }
    
        popupMenu->addChild(m_coin1Toggle);

        if (m_coinAmount > 1) {
            auto coin2Sprite = CCSprite::createWithSpriteFrameName(coinSpriteName.c_str());
            coin2Sprite->setScale(0.85f);

            CCSprite* coin2GraySprite;
            if (m_popup->m_isRobtopLevel) {
                coin2GraySprite = CCSprite::createWithSpriteFrameName(coinGraySpriteName.c_str());
            }
            else {
                coin2GraySprite = CCSprite::create(coinGraySpriteName.c_str());
            }

            coin2GraySprite->setScale(0.85f);
        
            m_coin2Toggle = CCMenuItemToggler::create(coin2GraySprite, coin2Sprite, this, menu_selector(EditPopup::onCoin));
            m_coin2Toggle->setPosition({xPositions[1], 190});
            m_coin2Toggle->setTag(1);
            
            if (!m_isCreate) {
                m_coinsCollected[1] = m_cell->m_coinsCollected[1];
                m_coin2Toggle->toggle(m_coinsCollected[1]);
            }
        
            popupMenu->addChild(m_coin2Toggle);

            if (m_coinAmount > 2) {
                auto coin3Sprite = CCSprite::createWithSpriteFrameName(coinSpriteName.c_str());
                coin3Sprite->setScale(0.85f);

                CCSprite* coin3GraySprite;
                if (m_popup->m_isRobtopLevel) {
                    coin3GraySprite = CCSprite::createWithSpriteFrameName(coinGraySpriteName.c_str());
                }
                else {
                    coin3GraySprite = CCSprite::create(coinGraySpriteName.c_str());
                }

                coin3GraySprite->setScale(0.85f);
            
                m_coin3Toggle = CCMenuItemToggler::create(coin3GraySprite, coin3Sprite, this, menu_selector(EditPopup::onCoin));
                m_coin3Toggle->setPosition({xPositions[2], 190});
                m_coin3Toggle->setTag(2);
                
                if (!m_isCreate) {
                    m_coinsCollected[2] = m_cell->m_coinsCollected[2];
                    m_coin3Toggle->toggle(m_coinsCollected[2]);
                }
            
                popupMenu->addChild(m_coin3Toggle);
            }
        }
    }
    else {
        auto noCoinsLabel = CCLabelBMFont::create("None", "bigFont.fnt");
        noCoinsLabel->setScale(0.45f);
        noCoinsLabel->setPosition({362, 195});
        noCoinsLabel->setOpacity(125);
        m_mainLayer->addChild(noCoinsLabel);
    }


    // Attempts & Jumps / Level Time & Points
    if (m_isCreate) {
        m_isPlatformer = m_popup->m_level->isPlatformer();
    }
    else {
        m_isPlatformer = m_cell->m_levelTime != "" || m_cell->m_points != "";
    }

    if (m_isPlatformer) {

        // Level Time
        auto levelTimeLabel = CCLabelBMFont::create("Level Time", "bigFont.fnt");
        levelTimeLabel->setScale(0.45f);
        levelTimeLabel->setPosition(362, 160);
        m_mainLayer->addChild(levelTimeLabel);

        auto resetLevelTimeSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
        resetLevelTimeSprite->setScale(0.45f);
        auto resetLevelTimeBtn = CCMenuItemSpriteExtra::create(resetLevelTimeSprite, this, menu_selector(EditPopup::onResetLevelTime));
        resetLevelTimeBtn->setPosition({310, 159});
        popupMenu->addChild(resetLevelTimeBtn);

        auto autoLevelTimeSprite = CCSprite::create("auto_btn.png"_spr);
        autoLevelTimeSprite->setScale(0.45f);
        auto autoLevelTimeBtn = CCMenuItemSpriteExtra::create(autoLevelTimeSprite, this, menu_selector(EditPopup::onAutoLevelTime));
        autoLevelTimeBtn->setPosition({414, 159});
        popupMenu->addChild(autoLevelTimeBtn);
        
        auto levelTimeMenu = CCMenu::create();
        levelTimeMenu->setLayout(RowLayout::create()
            ->setAutoScale(false)
            ->setGap(6.f)
            ->setAutoGrowAxis(false)
            ->setGrowCrossAxis(false)
        );

        levelTimeMenu->setContentSize({132, 35});
        levelTimeMenu->setPosition({362, 135});

        m_mainLayer->addChild(levelTimeMenu);

        m_levelHoursInput = TextInput::create(38.f, "hh", "bigFont.fnt");
        m_levelHoursInput->setScale(0.65f);
        m_levelHoursInput->setPosition(315.5, 135);
        m_levelHoursInput->setCommonFilter(CommonFilter::Uint);
        m_levelHoursInput->setMaxCharCount(2);
        levelTimeMenu->addChild(m_levelHoursInput);

        m_levelMinutesInput = TextInput::create(38.f, "mm", "bigFont.fnt");
        m_levelMinutesInput->setScale(0.65f);
        m_levelMinutesInput->setPosition(346.5, 135);
        m_levelMinutesInput->setCommonFilter(CommonFilter::Uint);
        m_levelMinutesInput->setMaxCharCount(2);
        levelTimeMenu->addChild(m_levelMinutesInput);

        m_levelSecondsInput = TextInput::create(38.f, "ss", "bigFont.fnt");
        m_levelSecondsInput->setScale(0.65f);
        m_levelSecondsInput->setPosition(377.5, 135);
        m_levelSecondsInput->setCommonFilter(CommonFilter::Uint);
        m_levelSecondsInput->setMaxCharCount(2);
        levelTimeMenu->addChild(m_levelSecondsInput);

        m_levelMillisecondsInput = TextInput::create(38.f, "ms", "bigFont.fnt");
        m_levelMillisecondsInput->setScale(0.65f);
        m_levelMillisecondsInput->setPosition(408.5, 135);
        m_levelMillisecondsInput->setCommonFilter(CommonFilter::Uint);
        m_levelMillisecondsInput->setMaxCharCount(3);
        levelTimeMenu->addChild(m_levelMillisecondsInput);

        levelTimeMenu->updateLayout();

        std::vector<float> colonXPositions = {331.4, 362, 392.8, 408};
        for (int i = 0; i < 3; i++) {
            auto colon = CCLabelBMFont::create((i == 2) ? "." : ":", "bigFont.fnt");
            colon->setScale(0.3f);
            colon->setOpacity(175);
            colon->setPosition({colonXPositions[i], 135.f});
            m_mainLayer->addChild(colon);
        }

        if (!m_isCreate) {
            auto cellTime = m_cell->m_levelTime;
            auto splitCellLevelTimes = Utils::splitString(cellTime, ':');

            int lastIndex = splitCellLevelTimes.size() - 1;
            auto secondsAndMilliseconds = Utils::splitString(splitCellLevelTimes[lastIndex], '.');

            splitCellLevelTimes[lastIndex] = secondsAndMilliseconds[0];
            splitCellLevelTimes.push_back(secondsAndMilliseconds[1]);

            int amount = 3 - (splitCellLevelTimes.size() - 1);
            for (int i = 0; i < splitCellLevelTimes.size(); i++) {
                m_cellLevelTimes[amount + i] = splitCellLevelTimes[i];
            }

            onResetLevelTime(nullptr);
        }

        // Points
        auto pointsLabel = CCLabelBMFont::create("Points", "bigFont.fnt");
        pointsLabel->setPosition({362, 105});
        pointsLabel->setScale(0.5f);
        m_mainLayer->addChild(pointsLabel);

        auto resetJumpsSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
        resetJumpsSprite->setScale(0.45f);
        auto resetJumpsBtn = CCMenuItemSpriteExtra::create(resetJumpsSprite, this, menu_selector(EditPopup::onResetPoints));
        resetJumpsBtn->setPosition({322, 104});
        popupMenu->addChild(resetJumpsBtn);

        auto autoPointsSprite = CCSprite::create("auto_btn.png"_spr);
        autoPointsSprite->setScale(0.45f);
        auto autoPointsBtn = CCMenuItemSpriteExtra::create(autoPointsSprite, this, menu_selector(EditPopup::onAutoPoints));
        autoPointsBtn->setPosition({402, 104});
        popupMenu->addChild(autoPointsBtn);

        m_pointsInput = TextInput::create(170.f, "...", "bigFont.fnt");
        m_pointsInput->setScale(0.65f);
        m_pointsInput->setCommonFilter(CommonFilter::Uint);
        m_pointsInput->setMaxCharCount(10);
        m_pointsInput->setMaxCharCount(10);

        if (!m_isCreate) {
            m_pointsInput->setString(m_cell->m_points);
        }

        m_pointsInput->setPosition({362, 80});
        m_mainLayer->addChild(m_pointsInput);

    } else {

        // Attempts
        auto attemptsLabel = CCLabelBMFont::create("Attempts", "bigFont.fnt");
        attemptsLabel->setPosition({362, 160});
        attemptsLabel->setScale(0.5f);
        m_mainLayer->addChild(attemptsLabel);

        auto resetAttemptsSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
        resetAttemptsSprite->setScale(0.45f);
        auto resetAttemptsBtn = CCMenuItemSpriteExtra::create(resetAttemptsSprite, this, menu_selector(EditPopup::onResetAttempts));
        resetAttemptsBtn->setPosition({310, 159});
        popupMenu->addChild(resetAttemptsBtn);

        auto autoAttemptsSprite = CCSprite::create("auto_btn.png"_spr);
        autoAttemptsSprite->setScale(0.45f);
        auto autoAttemptsBtn = CCMenuItemSpriteExtra::create(autoAttemptsSprite, this, menu_selector(EditPopup::onAutoAttempts));
        autoAttemptsBtn->setPosition({414, 159});
        popupMenu->addChild(autoAttemptsBtn);

        m_attemptsInput = TextInput::create(170.f, "...", "bigFont.fnt");
        m_attemptsInput->setScale(0.65f);

        if (!m_isCreate)
            m_attemptsInput->setString(m_cell->m_attempts);

        m_attemptsInput->setPosition({362, 135});
        m_attemptsInput->setCommonFilter(CommonFilter::Uint);
        m_attemptsInput->setMaxCharCount(10);
        m_mainLayer->addChild(m_attemptsInput);


        // Jumps
        auto jumpsLabel = CCLabelBMFont::create("Jumps", "bigFont.fnt");
        jumpsLabel->setPosition({362, 105});
        jumpsLabel->setScale(0.5f);
        m_mainLayer->addChild(jumpsLabel);

        auto resetJumpsSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
        resetJumpsSprite->setScale(0.45f);
        auto resetJumpsBtn = CCMenuItemSpriteExtra::create(resetJumpsSprite, this, menu_selector(EditPopup::onResetJumps));
        resetJumpsBtn->setPosition({326, 104});
        popupMenu->addChild(resetJumpsBtn);

        auto autoJumpsSprite = CCSprite::create("auto_btn.png"_spr);
        autoJumpsSprite->setScale(0.45f);
        auto autoJumpsBtn = CCMenuItemSpriteExtra::create(autoJumpsSprite, this, menu_selector(EditPopup::onAutoJumps));
        autoJumpsBtn->setPosition({398, 104});
        popupMenu->addChild(autoJumpsBtn);

        m_jumpsInput = TextInput::create(170.f, "...", "bigFont.fnt");
        m_jumpsInput->setScale(0.65f);

        if (!m_isCreate) {
            m_jumpsInput->setString(m_cell->m_jumps);
        }

        m_jumpsInput->setPosition({362, 80});
        m_jumpsInput->setCommonFilter(CommonFilter::Uint);
        m_jumpsInput->setMaxCharCount(10);
        m_mainLayer->addChild(m_jumpsInput);
    }


    if (Mod::get()->getSettingValue<bool>("autofill") && m_isCreate) {
        onAutoName(nullptr);
        onAutoDate(nullptr);
        onAutoTime(nullptr);
        onAutoCoins(nullptr);
        
        if (m_isPlatformer) {
            onAutoLevelTime(nullptr);
            onAutoPoints(nullptr);
        } 
        else {
            onAutoAttempts(nullptr);
            onAutoJumps(nullptr);
        }

        onPasteIcon(nullptr);
    }

    return true;
}

void EditPopup::resetIconValues() {
    if (m_isCreate) {
        m_iconType = IconType::Cube;
        m_iconFrame = 0;
        m_iconColor = 12;
        m_iconColor2 = 12;
        m_glowColor = 12;
        m_hasGlow = false;
    } else {
        m_iconType = m_cell->m_iconType;
        m_iconFrame = m_cell->m_iconFrame;
        m_iconColor = m_cell->m_iconColor;
        m_iconColor2 = m_cell->m_iconColor2;
        m_glowColor = m_cell->m_glowColor;
        m_hasGlow = m_cell->m_hasGlow;
    }

    int iconTypeInt = static_cast<int>(m_iconType);
    for (int i = 0; i < 9; i++) {
        m_iconFrames[i] = (i == iconTypeInt) ? m_iconFrame : 0;
    }

    updateIcon();

    if (m_glowToggle) {
        m_glowToggle->toggle(m_hasGlow);
    }
}

void EditPopup::updateIcon() {
    auto gm = GameManager::get();
    m_icon->updatePlayerFrame(m_iconFrame, m_iconType);
    m_icon->setColor(gm->colorForIdx(m_iconColor));
    m_icon->setSecondColor(gm->colorForIdx(m_iconColor2));
    m_icon->m_hasGlowOutline = m_hasGlow;

    if (m_icon->m_hasGlowOutline) {
        m_icon->enableCustomGlowColor(gm->colorForIdx(m_glowColor));
    }
    else {
        m_icon->disableCustomGlowColor();
    }

    m_icon->updateColors();
}

void EditPopup::onGarage(CCObject* obj) {
    IconSelectLayer::create(this)->show();
}

void EditPopup::onPasteIcon(CCObject* obj) {
    GameManager* gm = GameManager::get();
    m_iconType = gm->m_playerIconType;
    m_iconFrame = Utils::getIconFrame(m_iconType);
    m_iconColor = gm->getPlayerColor();
    m_iconColor2 = gm->getPlayerColor2();
	m_glowColor = gm->getPlayerGlowColor();
	m_hasGlow = gm->getPlayerGlow();

    for (int i = 0; i < 9; i++) {
        m_iconFrames[i] = Utils::getIconFrame(static_cast<IconType>(i));
    }
    
    updateIcon();
    updateColors();

    if (m_glowToggle) {
        m_glowToggle->toggle(m_hasGlow);
    }
}

void EditPopup::onResetIcon(CCObject* obj) {
    resetIconValues();
    updateIcon();
    updateColors();
}

void EditPopup::onRightIconArrow(CCObject*) {
    int type = static_cast<int>(m_iconType);
    type++;

    if (type > 8) {
        type = 0;
    }

    m_iconType = static_cast<IconType>(type);
    m_iconFrame = m_iconFrames[type];
    updateIcon();
}

void EditPopup::onLeftIconArrow(CCObject*) {
    int type = static_cast<int>(m_iconType);
    type--;
    
    if (type < 0) {
        type = 8;
    }

    m_iconType = static_cast<IconType>(type);
    m_iconFrame = m_iconFrames[type];
    updateIcon();
}

void EditPopup::onColor(CCObject* obj) {
    ColorSelectLayer::create(this, static_cast<CCMenuItemSpriteExtra*>(obj)->getTag())->show();
}

void EditPopup::onToggleGlow(CCObject* obj) {
    m_hasGlow = !m_hasGlow;
    updateIcon();
}

void EditPopup::colorSelected(const cocos2d::ccColor3B& color, int colorIndex, int colorID) {
    CCSprite* colorSprite = nullptr;
    switch(colorIndex) {
        case 1:
            colorSprite = static_cast<CCSprite*>(m_color1Btn->getNormalImage());
            m_iconColor = colorID;
            break;
        case 2:
            colorSprite = static_cast<CCSprite*>(m_color2Btn->getNormalImage());
            m_iconColor2 = colorID;
            break;
        case 3:
            colorSprite = static_cast<CCSprite*>(m_color3Btn->getNormalImage());
            m_glowColor = colorID;
            break;
    }

    colorSprite->setColor(color);
    updateIcon();
}

void EditPopup::updateColors() {
    auto gm = GameManager::get();
    static_cast<CCSprite*>(m_color1Btn->getNormalImage())->setColor(gm->colorForIdx(m_iconColor));
    static_cast<CCSprite*>(m_color2Btn->getNormalImage())->setColor(gm->colorForIdx(m_iconColor2));
    static_cast<CCSprite*>(m_color3Btn->getNormalImage())->setColor(gm->colorForIdx(m_glowColor));
}

void EditPopup::iconSelected(int frame) {
    m_iconFrame = frame;
    m_iconFrames[static_cast<int>(m_iconType)] = m_iconFrame;
    updateIcon();
}

void EditPopup::onResetName(CCObject* obj) {
    if (m_isCreate) {
        m_nameInput->setString("");
    }
    else {
        m_nameInput->setString(m_cell->m_name);
    }
}

void EditPopup::onResetDate(CCObject* obj) {
    log::debug("resetDate");
    if (m_isCreate) {
        m_yearInput->setString("");
        m_monthInput->setString("");
        m_dayInput->setString("");
    } else {
        m_yearInput->setString(m_cellDates[0]);
        m_monthInput->setString(m_cellDates[1]);
        m_dayInput->setString(m_cellDates[2]);
    }
}

void EditPopup::onResetTime(CCObject* obj) {
    if (m_isCreate) {
        m_hourInput->setString("");
        m_minuteInput->setString("");
        m_secondInput->setString("");
        if (m_timeSettingText) {
            m_timeSettingText->setString("AM");
        }
    }
    else {
        m_hourInput->setString(m_cellTimes[0]);
        m_minuteInput->setString(m_cellTimes[1]);
        m_secondInput->setString(m_cellTimes[2]);
        if (m_timeSettingText) {
            m_timeSettingText->setString(m_cellTimes[3].c_str());
        }
    }
}

void EditPopup::onResetAttempts(CCObject* obj) {
    if (m_isCreate) {
        m_attemptsInput->setString("");
    }
    else {
        m_attemptsInput->setString(m_cell->m_attempts);
    }
}

void EditPopup::onResetJumps(CCObject* obj) {
    if (m_isCreate) {
        m_jumpsInput->setString("");
    }
    else {
        m_jumpsInput->setString(m_cell->m_jumps);
    }
}

void EditPopup::onResetCoins(CCObject* obj) {
    if (m_coin1Toggle) {
        m_coinsCollected[0] = (m_isCreate) ? false : m_cell->m_coinsCollected[0];
        m_coin1Toggle->toggle(m_coinsCollected[0]);
    }

    if (m_coin2Toggle) {
        m_coinsCollected[1] = (m_isCreate) ? false : m_cell->m_coinsCollected[1];
        m_coin2Toggle->toggle(m_coinsCollected[1]);
    }

    if (m_coin3Toggle) {
        m_coinsCollected[2] = (m_isCreate) ? false : m_cell->m_coinsCollected[2];
        m_coin3Toggle->toggle(m_coinsCollected[2]);
    }

}

void EditPopup::onResetLevelTime(CCObject* obj) {
    if (m_isCreate) {
        m_levelHoursInput->setString("");
        m_levelMinutesInput->setString("");
        m_levelSecondsInput->setString("");
        m_levelMillisecondsInput->setString("");
    } else {
        m_levelHoursInput->setString(m_cellLevelTimes[0]);
        m_levelMinutesInput->setString(m_cellLevelTimes[1]);
        m_levelSecondsInput->setString(m_cellLevelTimes[2]);
        m_levelMillisecondsInput->setString(m_cellLevelTimes[3]);
    }
}

void EditPopup::onResetPoints(CCObject* obj) {
    if (m_isCreate) {
        m_pointsInput->setString("");
    }
    else {
        m_pointsInput->setString(m_cell->m_points);
    }
}


void EditPopup::onAutoName(CCObject* obj) {
    if (m_isCreate) {
        m_nameInput->setString(fmt::format("Completion {}", std::to_string(m_popup->m_rebeats + 1)));
    }
}

void EditPopup::onAutoDate(CCObject* obj) {
    auto now = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    auto splitDate = Utils::splitString(std::format("{:%Y-%m-%d}", now), '-');
    m_yearInput->setString(splitDate[0]);
    m_monthInput->setString(splitDate[1]);
    m_dayInput->setString(splitDate[2]);
}

void EditPopup::onAutoTime(CCObject* obj) {
    std::time_t now_time_t = system_clock::to_time_t(system_clock::now());
    auto local_tm = std::localtime(&now_time_t);
    std::vector<std::string> timestamp;

    if (m_timeSettingText) {
        timestamp = Utils::splitString(fmt::format("{:%I:%M:%S %p}", *local_tm), ':');
        std::string ampm = Utils::isAMorPM(timestamp[2]);

        timestamp[2].erase(timestamp[2].size() - 3);
        m_timeSettingText->setString(ampm.c_str());
    }
    else {
        timestamp = Utils::splitString(fmt::format("{:%H:%M:%S}", *local_tm), ':');
    }

    m_hourInput->setString(timestamp[0]);
    m_minuteInput->setString(timestamp[1]);
    m_secondInput->setString(timestamp[2]);

}

void EditPopup::onAutoCoins(CCObject* obj) {
    if (m_coin1Toggle) {
        m_coinsCollected[0] = static_cast<bool>(m_popup->m_level->m_firstCoinVerified.value());
        m_coin1Toggle->toggle(m_coinsCollected[0]);
    }

    if (m_coin2Toggle) {
       m_coinsCollected[1] = static_cast<bool>(m_popup->m_level->m_secondCoinVerified.value());
        m_coin2Toggle->toggle(m_coinsCollected[1]);
    }

    if (m_coin3Toggle) {
        m_coinsCollected[2] = static_cast<bool>(m_popup->m_level->m_thirdCoinVerified.value());
        m_coin3Toggle->toggle(m_coinsCollected[2]);
    }
}

void EditPopup::onAutoAttempts(CCObject* obj) {
    m_attemptsInput->setString(std::to_string(m_popup->m_level->m_attempts.value()));
}

void EditPopup::onAutoJumps(CCObject* obj) {
    m_jumpsInput->setString(std::to_string(m_popup->m_level->m_jumps.value()));
}

void EditPopup::onAutoLevelTime(CCObject* obj) {
    int time = m_popup->m_level->m_bestTime;
    std::string formattedTime;

    int hours = time / 3600000;
    int minutes = time / 60000 % 60;
    int seconds = time / 1000 % 60;
    int ms = time % 1000;

    m_levelHoursInput->setString(hours > 0 ? std::to_string(hours) : "");
    m_levelMinutesInput->setString(minutes > 0 ? std::to_string(minutes) : "");
    m_levelSecondsInput->setString(std::to_string(seconds));
    m_levelMillisecondsInput->setString(std::to_string(ms));
}

void EditPopup::onAutoPoints(CCObject* obj) {
    m_pointsInput->setString(std::to_string(m_popup->m_level->m_bestPoints));
}

void EditPopup::onInfo(CCObject* obj) {
    FLAlertLayer::create(
        nullptr,
        fmt::format("{} Completion Info", (m_isCreate) ? "Create" : "Edit").c_str(),
        "This is where you can set the completion's information.\nThe <cg>Reset</c> buttons reset the field to its original value.\nThe <co>Auto</c> buttons autofill the value. Enable the Autocomplete option in the <cy>Mod Settings</c> to automatically apply this to all fields. All fields left empty will be autofilled.",
        "OK",
        nullptr,
        380
    )->show();
}

void EditPopup::onInfoInfo(CCObject* obj) {
    FLAlertLayer::create(
        "Info Help",
        "<cj>Name</c>: The completion title.\n<co>Date</c>: The day of the completion in the format <cp>YYYY-MM-DD</c>.\n<cg>Time</c>: The time of the completion in the format <cp>HH:MM:SS</c> using either 12-hour or 24-hour time. The time format can be changed in the <cy>Mod Settings</c>.",
        "OK"
    )->show();
}

void EditPopup::onIconInfo(CCObject* obj) {
    FLAlertLayer::create(
        "Icon Help",
        "Sets the icon displayed in the completion.\nThe <cy>Paste</c> button sets the icons to your current icon set.",
        "OK"
    )->show();
}

void EditPopup::onStatsInfo(CCObject* obj) {
    std::string text = "<cj>Coins</c>: Toggle the coins to set which have been collected.";

    if (m_isPlatformer) {
        text += "\n<co>Level Time</c>: The amount of in-game time taken to complete the level, as shown on the endscreen.\n<cg>Points</c>: The amount of Points collected in the level. Leave blank if the level does not contain points.";
    }
    else {
        text += "\n<co>Attempts</c>: The total amount of attempts the completion took.\n<cg>Jumps</c>: The number of jumps during the completion session, as shown on the endscreen.";
    }

    FLAlertLayer::create(
        "Stats Help",
        text,
        "OK"
    )->show();
}

void EditPopup::onCoin(CCObject* obj) {
    auto toggle = static_cast<CCMenuItemToggler*>(obj);
    int index = toggle->getTag();
    m_coinsCollected[index] = !m_coinsCollected[index];
}

void EditPopup::onTimeArrow(CCObject* obj) {
    m_timeSettingText->setString((std::string_view(m_timeSettingText->getString()) == "AM") ? "PM" : "AM");
}

EditPopup* EditPopup::create(RebeatPopup* popup, RebeatCell* cell) {
    auto ret = new EditPopup();
    ret->m_popup = popup;
    ret->m_cell = cell;
    ret->m_isCreate = (cell == nullptr);

    if (ret->initAnchored(440, 290)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void EditPopup::onSave(CCObject* obj) {
    std::string name = (m_nameInput->getString() != "") ? m_nameInput->getString() : "Unnamed Completion";
    std::string year = (m_yearInput->getString() != "") ? m_yearInput->getString() : "????";
    std::string month = (m_monthInput->getString() != "") ? m_monthInput->getString() : "??";
    std::string day = (m_dayInput->getString() != "") ? m_dayInput->getString() : "??";
    std::string date = fmt::format("{}-{}-{}", year, month, day);

    std::string hour = (m_hourInput->getString() != "") ? m_hourInput->getString() : "00";
    if (hour.length() < 2) {
        hour = "0" + hour;
    }

    std::string minute = (m_minuteInput->getString() != "") ? m_minuteInput->getString() : "00";
    if (minute.length() < 2) {
        minute = "0" + minute;
    }

    std::string second = (m_secondInput->getString() != "") ? m_secondInput->getString() : "00";
    if (second.length() < 2) {
        second = "0" + second;
    }

    std::string time;
    if (m_timeSettingText) {
        time = fmt::format("{}:{}:{} {}", hour, minute, second, m_timeSettingText->getString());
    }
    else {
        time = fmt::format("{}:{}:{}", hour, minute, second);
    }

    std::string stat1;
    std::string stat2;

    std::string levelMilliseconds = "";
    std::string levelSeconds = "";
    std::string levelMinutes = "";
    std::string levelHours = "";

    if (m_isPlatformer) {
        std::string levelTime;
        levelMilliseconds = (m_levelMillisecondsInput->getString() != "") ? m_levelMillisecondsInput->getString() : "0";
        levelSeconds = (m_levelSecondsInput->getString() != "") ? m_levelSecondsInput->getString() : "0";
        levelTime = fmt::format("{}.{}", levelSeconds, levelMilliseconds);

        levelMinutes = m_levelMinutesInput->getString();

        if (levelMinutes != "") {
            levelTime = fmt::format("{}:{}", levelMinutes, levelTime);
            levelHours = m_levelHoursInput->getString();

            if (levelHours != "") {
                levelTime = fmt::format("{}:{}", levelHours, levelTime);
            }
        }

        stat1 = levelTime;

        stat2 = (m_pointsInput->getString() != "") ? m_pointsInput->getString() : "0";
    } 
    else {
        stat1 = (m_attemptsInput->getString() != "") ? m_attemptsInput->getString() : "0";
        stat2 = (m_jumpsInput->getString() != "") ? m_jumpsInput->getString() : "0";
    }

    if (!m_isCreate) {
        m_isEdited = (year != m_cellDates[0] || month != m_cellDates[1] || day != m_cellDates[2] || hour != m_cellTimes[0] || minute != m_cellTimes[1] || second != m_cellTimes[2] || m_coinsCollected[0] != m_cell->m_coinsCollected[0] || m_coinsCollected[1] != m_cell->m_coinsCollected[1] || m_coinsCollected[2] != m_cell->m_coinsCollected[2] || ((m_isPlatformer) ? (levelMilliseconds != m_cellLevelTimes[3] || levelSeconds != m_cellLevelTimes[2] || levelMinutes != m_cellLevelTimes[1] || levelHours != m_cellLevelTimes[0] || stat2 != m_cell->m_points) : (stat1 != m_cell->m_attempts || stat2 != m_cell->m_jumps)));
    }

    matjson::Value newRebeat = dataToJson(name, date, time, static_cast<int>(m_iconType), m_iconFrame, m_iconColor, m_iconColor2, m_glowColor, m_hasGlow, m_coinAmount, m_coinsCollected, stat1, stat2, m_isPlatformer, m_ytLink);

    if (m_isCreate) {
        m_popup->addCell(newRebeat);
    }
    else {
        m_popup->updateCell(m_cell, newRebeat);
    }

    keyBackClicked();
}

matjson::Value EditPopup::dataToJson(std::string name, std::string date, std::string time, int iconType, int frame, int color1, int color2, int glowColor, bool hasGlow, int coinAmount, bool coinsCollected[3], std::string stat1, std::string stat2, bool isPlatformer, std::string ytLink) {
    matjson::Value newRebeat;
    newRebeat["name"] = name;
    newRebeat["date"] = date;
    newRebeat["time"] = time;

    matjson::Value icons;
    icons["type"] = iconType;
    icons["frame"] = frame;
    icons["color_1"] = color1;
    icons["color_2"] = color2;
    icons["glow_color"] = glowColor;
    icons["has_glow"] = hasGlow;
    newRebeat["icons"] = icons;

    matjson::Value coins;
    coins["amount"] = coinAmount;

    for (int i = 0; i < coinAmount; i++) {
        coins[std::to_string(i).c_str()] = coinsCollected[i];
    }

    newRebeat["coins"] = coins;

    if (isPlatformer) {
        newRebeat["level_time"] = stat1;
        newRebeat["points"] = stat2;
    } 
    else {
        newRebeat["attempts"] = stat1;
        newRebeat["jumps"] = stat2;
    }

    if (ytLink != "") {
        newRebeat["link"] = m_ytLink;
    }

    std::string watermark = "";
    
    if (m_isCreate)
        watermark = "(custom)";
    else {
        if (m_cell->m_watermark == "" && m_isEdited) {
            watermark = "(edited)";
        }
        else {
            watermark = m_cell->m_watermark;
        }
    }

    newRebeat["watermark"] =  watermark;

    return newRebeat;
}

void EditPopup::onDelete(CCObject* obj) {
    keyBackClicked();
}

void EditPopup::onReset(CCObject* obj) {
    geode::createQuickPopup(
        "Warning", 
        "Are you sure you want to <cr>reset</c> all values? This <cr>cannot</c> be undone.",
        "Cancel", "Reset",
        [this](auto, bool btn2) {
            if (btn2)  {
                onResetName(nullptr);
                onResetDate(nullptr);
                onResetTime(nullptr);

                resetIconValues();
                updateIcon();
                updateColors();

                onResetCoins(nullptr);
                if (m_isPlatformer) {
                    onResetLevelTime(nullptr);
                    onResetPoints(nullptr);
                }
                else {
                    onResetAttempts(nullptr);
                    onResetJumps(nullptr);
                }
            }
        }
    );
}

void EditPopup::onYoutube(CCObject* obj) {
    YoutubeLayer::create(this)->show();
}

void EditPopup::onCancel(CCObject* obj) {
    geode::createQuickPopup(
        "Warning", 
        "Are you sure you want to <cr>cancel</c> creating this completion? Any changes will be <cr>lost</c>.",
        "Cancel", "Delete",
        [this](auto, bool btn2) {
            if (btn2)  {
                keyBackClicked();
            }
        }
    );
}

void EditPopup::onClose(CCObject* obj) {
    geode::createQuickPopup(
        "Warning", 
        fmt::format("Are you sure you want to <cr>cancel</c> {} this completion? All changes will be <cr>lost</c>.", (m_isCreate) ? "creating" : "editing"),
        "Return", "Confirm",
        [this, obj](auto, bool btn2) {
            if (btn2)  {
                geode::Popup<>::onClose(obj);
            }
        }
    );
}
