#include <Geode/ui/GeodeUI.hpp>
#include "EditPopup.hpp"
#include "RebeatCell.hpp"
#include "RebeatPopup.hpp"
#include <cvolton.level-id-api/include/EditorIDs.hpp>

using namespace geode::prelude;

void RebeatPopup::makeList(bool refresh) {
    if (!refresh) {
        m_cells = CCArray::create();
        //log::debug("{}", rebeats.dump());
    
        m_rebeats = 1;
        m_prevAttempts = 0;
        m_hasCompletion = false;
        m_calculateAttempts = Mod::get()->getSettingValue<bool>("calculate-attempts");
    
        for (const auto& rebeat : m_rebeatsList) {
    
            auto rebeatCell = RebeatCell::create(rebeat, this);
    
            /*RebeatCell* rebeatCell;
            if (calculateAttempts)
                rebeatCell = RebeatCell::create(rebeat, id, rebeats,);
            else 
                rebeatCell = RebeatCell::create(rebeat, id, rebeats, previousAttempts);*/
    
            m_cells->addObject(rebeatCell);
            // if (rebeats == 1 && rebeat.contains("date")) 
            // 	if (rebeat["date"].asString().unwrapOr("NA")[0] != 'f') 
            // 		rebeats++;
            // 	else 
            // 		hasCompletion = true;
            // else
            // 	rebeats++;
    
            m_rebeats++;
        }
    }

    m_listView = ListView::create(m_cells, 40, 380, 220);
    CCNode* contentLayer = static_cast<CCNode*>(m_listView->m_tableView->getChildren()->objectAtIndex(0));
    TableView* tableView = m_listView->m_tableView;

    //listView->setPrimaryCellColor(ccc3(138, 77, 46));
    //listView->setSecondaryCellColor(ccc3(138, 77, 46));
    //listView->setCellBorderColor(ccc4(0, 0, 0, 0));

    m_list = GJCommentListLayer::create(m_listView, "", ccc4(255, 255, 255, 0), 380, 220, true); //289, 153
    m_list->setPosition((m_size - m_list->getContentSize()) / 2.f);
    m_mainLayer->addChild(m_list, 2);
    m_list->setColor(ccc3(191, 114, 62));
    m_list->setOpacity(255);

    CCSprite* topBorder = m_list->getChildByType<CCSprite>(1);
    CCSprite* bottomBorder = m_list->getChildByType<CCSprite>(0);

    // topBorder->setScale(0.883f);
    // bottomBorder->setScale(0.883f);

    topBorder->setScale(1.118f);
    bottomBorder->setScale(1.118f);

    if (contentLayer->getContentSize().height >= 175) {
        m_scrollbar = Scrollbar::create(m_listView->m_tableView);
        m_scrollbar->setPosition(m_list->getPosition() + ccp(m_list->getContentSize().width + 6, 0));
        m_scrollbar->setAnchorPoint({0, 0});
        m_mainLayer->addChild(m_scrollbar, 3);
    }

    if (refresh) {
        m_rebeatCount->setString(fmt::format("Total Completions: {}", m_rebeats - 1).c_str(), "bigFont.fnt");
    }
}

void RebeatPopup::onSettings(CCObject* obj) {
    auto settings = geode::openSettingsPopup(Mod::get(), false);
    settings->setID("completionSettings"_spr);
}

bool RebeatPopup::setup() {
    this->setTitle(fmt::format("Completions for {}", m_level->m_levelName));
    auto titleWidth = m_title->getContentSize().width;
    this->m_title->limitLabelWidth(335.f, 0.8f, 0.001f);
    this->m_title->setPositionY(273.f);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSprite->setScale(0.75f);
    auto infoButton = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(RebeatPopup::onInfo));
    infoButton->setPosition({422.f, 272.f});
    m_buttonMenu->addChild(infoButton);

    
    m_rightButtonMenu = CCMenu::create();
    m_rightButtonMenu->setLayout(ColumnLayout::create()
    ->setGap(10.f)
    //->setGrowCrossAxis(true)
    ->setAxisAlignment(AxisAlignment::Center)
    //->setCrossAxisAlignment(AxisAlignment::Center)
    ->setAxisReverse(true)
    );

    //m_rightButtonMenu->setAnchorPoint({0.5, 1});
    m_rightButtonMenu->setPosition({26.f, 145.f});
    // auto layout = static_cast<AxisLayout*>(m_rightButtonMenu->getLayout());
    // layout->setAutoScale(false);
    // layout->setGap(15.f);
    // layout->setAxis
    m_mainLayer->addChild(m_rightButtonMenu);

    
    auto createSprite = CCSprite::createWithSpriteFrameName("GJ_viewLevelsBtn_001.png"); //GJ_plusBtn_001.png
    createSprite->setScale(0.75f);
    auto createButton = CCMenuItemSpriteExtra::create(createSprite, this, menu_selector(RebeatPopup::onCreate));
    createButton->setPosition({414, 215});
    m_rightButtonMenu->addChild(createButton);
    
    auto sortSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    sortSprite->setScale(0.75f);
    auto sortButton = CCMenuItemSpriteExtra::create(sortSprite, this, menu_selector(RebeatPopup::onSort));
    sortButton->setPosition({414, 175});
    m_rightButtonMenu->addChild(sortButton);

    /*auto settingsSprite = CircleButtonSprite::createWithSpriteFrameName(
        "geode.loader/settings.png", 
        1.f, 
        CircleBaseColor::DarkPurple, 
        CircleBaseSize::Small
    );
    settingsSprite->setScale(0.9);*/
    auto settingsSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    settingsSprite->setScale(0.7f);
    auto settingsButton = CCMenuItemSpriteExtra::create(settingsSprite, this, menu_selector(RebeatPopup::onSettings));
    settingsButton->setPosition({20, 22});
    m_rightButtonMenu->addChild(settingsButton);

    m_rightButtonMenu->updateLayout();

    m_id = std::to_string(EditorIDs::getID(m_level, true));
    if (m_level->m_isEditable) {
        m_id = "c" + m_id;
        m_isRobtopLevel = false;
    }
    else {
        int levelID = std::stoi(m_id);
        m_isRobtopLevel = (levelID <= 24 || (levelID > 5000 && levelID < 5005) || levelID == 3001);
    }

    m_rebeatsList = Mod::get()->getSavedValue<matjson::Value>(m_id);

    makeList(false);

    m_rebeatCount = CCLabelBMFont::create(fmt::format("Total Completions: {}", m_rebeats - 1).c_str(), "bigFont.fnt");
    m_rebeatCount->setPosition(this->m_title->getPositionX(), 20.f);
    m_rebeatCount->setScale(0.5f);
    m_mainLayer->addChild(m_rebeatCount);

    return true;
}

void RebeatPopup::onDelete(CCObject* obj) {
    geode::createQuickPopup(
        "Warning", 
        "Are you sure you want to <cr>delete</c> this completion? This <cr>cannot</c> be undone.",
        "Cancel", "Delete",
        [this](auto, bool btn2) {
            if (btn2)  {
                log::debug("delete");
            }
        }
    );
}

void RebeatPopup::onInfo(CCObject* obj) {

    /*std::string text = "<cp>Icon</c>: Your icon on the attempt of completion.\n<cj>Date</c>: The date and time of completion.\n<cg>Coins</c>: The secret coins collected on the completion attempt.";

    text += "\n<cy>Attempts</c>: The total level attempts upon completion.\n<cy>Time</c>: The final level time upon completion.";
    text += "\n<co>Jumps</c>: The amount of jumps during the completion attempt, as shown on the endscreen.\n<co>Points</c>: The amount of points collected upon completion.";
    
    text += "\n<cr>Delete Button</c>: Deletes the completion and reloads the page. Deleting a completiong <cr>cannot</c> be undone and <cb>will affect attempt calculation for following completions</c>.";*/

    std::string text = "All completions display the <cp>Icon</c>, <cj>Date & Time</c>, and <cg>Coins</c> (if any).\nClassic mode completions display <cy>Attempts</c> and <co>Jumps</c>.\nPlatformer completions display <cy>Time</c> and <co>Points</c>.";

    auto alert = FLAlertLayer::create(
        "Completion Info",
        text,
        "Ok"
    );
    alert->show();
    // alert->setContentWidth(440);
    // alert->setContentHeight(290);

    log::debug("info");
}

RebeatPopup* RebeatPopup::create(GJGameLevel* level) {
    auto ret = new RebeatPopup();
    ret->m_level = level;
    if (ret->initAnchored(480, 290, "GJ_square02.png")) { //370, 267
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void RebeatPopup::deleteCell(int index) {
    m_cells->removeObjectAtIndex(index, true);
    auto newRebeatsList = matjson::Value::array();
    // log::debug("rebeats: {}", m_rebeatsList.dump());
    log::debug("size: {}", m_rebeatsList.size());

    for (size_t i = 0; i < m_rebeatsList.size(); i++)
        if (i != index)
            newRebeatsList.push(m_rebeatsList[i]);
            
    log::debug("size2: {}", newRebeatsList.size());

    m_rebeatsList = newRebeatsList;

    m_rebeats--;

    // log::debug("newRebeatsList: {}", newRebeatsList.dump());

    // Mod::get()->setSavedValue(m_id, newRebeatsList);

    refreshList();
}

void RebeatPopup::refreshList() {
    if (m_list)
        m_list->removeFromParentAndCleanup(false);

    if (m_scrollbar)
        m_scrollbar->removeFromParentAndCleanup(false);
    // m_listView->removeFromParentAndCleanup(true);

    makeList(true);
}

void RebeatPopup::onCreate(CCObject* obj) {
    log::debug("create");
    // FLAlertLayer::create("Cruh", "This is where she creates a completion.", "Okay!")->show();
    auto popup = EditPopup::create(this);
    popup->show();
    return;
}

void RebeatPopup::onSort(CCObject* obj) {
    log::debug("sort");
    FLAlertLayer::create("fap", "This is where she sorts a completion.", "Sure.")->show();
}

void RebeatPopup::addCell(matjson::Value newRebeat) {
    log::debug("addCell");
    auto newCell = RebeatCell::create(newRebeat, this);
    m_cells->addObject(newCell);
    m_rebeatsList.push(newRebeat);
    m_rebeats++;
    refreshList();
}

void RebeatPopup::updateCell(RebeatCell* cell, matjson::Value newRebeat) {
    log::debug("updateCell");
    auto newCell = RebeatCell::create(newRebeat, this);
    auto index = m_cells->indexOfObject(cell);
    m_cells->replaceObjectAtIndex(index, newCell, true);
    refreshList();
}

