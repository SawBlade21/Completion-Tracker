#include <Geode/ui/GeodeUI.hpp>
#include "EditPopup.hpp"
#include "RebeatCell.hpp"
#include "RebeatPopup.hpp"
#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include "Utils.hpp"
#include "SortLayer.hpp"

using namespace geode::prelude;

void RebeatPopup::makeList(bool refresh) {
    if (!refresh) {
        m_cells = CCArray::create();
        m_rebeats = 0;
        m_prevAttempts = 0;
        m_hasCompletion = false;
    
        for (const auto& rebeat : m_rebeatsList) {       
            auto rebeatCell = RebeatCell::create(rebeat, this);
            m_cells->addObject(rebeatCell);
            m_rebeats++; 
        }
    }

    sortCells();

    m_listView = ListView::create(m_cells, 44, 380, 220);
    CCNode* contentLayer = static_cast<CCNode*>(m_listView->m_tableView->getChildren()->objectAtIndex(0));
    TableView* tableView = m_listView->m_tableView;

    m_listView->setPrimaryCellColor(ccc3(138, 77, 46));
    m_listView->setSecondaryCellColor(ccc3(138, 77, 46));
    m_listView->setCellBorderColor(ccc4(0, 0, 0, 0));

    m_list = GJCommentListLayer::create(m_listView, "", ccc4(255, 255, 255, 0), 380, 220, false); //289, 153
    m_list->setPosition((m_size - m_list->getContentSize()) / 2.f);
    m_list->setColor(ccc3(138, 77, 46));
    m_list->setOpacity(255);
    m_mainLayer->addChild(m_list, 2);


    CCSprite* topBorder = m_list->getChildByType<CCSprite>(1);
    CCSprite* bottomBorder = m_list->getChildByType<CCSprite>(0);

    topBorder->setScale(1.118f);
    bottomBorder->setScale(1.118f);

    if (contentLayer->getContentSize().height >= 175) {
        m_scrollbar = Scrollbar::create(m_listView->m_tableView);
        m_scrollbar->setPosition(m_list->getPosition() + ccp(m_list->getContentSize().width + 6, 0));
        m_scrollbar->setAnchorPoint({0, 0});
        m_mainLayer->addChild(m_scrollbar, 3);
    }

    if (refresh) {
        m_rebeatCount->setString(fmt::format("Total Completions: {}", m_rebeats).c_str(), "bigFont.fnt");
    }

    if (m_cells->count() == 0) {
        auto noneLabel = CCLabelBMFont::create("No Completions", "bigFont.fnt");
        noneLabel->setOpacity(128);
        noneLabel->setPosition({190, 120});
        m_listView->addChild(noneLabel);
    }
}

void RebeatPopup::onSettings(CCObject* obj) {
    auto settings = geode::openSettingsPopup(Mod::get(), false);
    settings->setID("completionSettings"_spr);
}

bool RebeatPopup::setup() {
    setTitle(fmt::format("Completions for {}", m_level->m_levelName));
    auto titleWidth = m_title->getContentSize().width;
    m_title->limitLabelWidth(335.f, 0.8f, 0.001f);
    m_title->setPositionY(273.f);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSprite->setScale(0.75f);
    auto infoButton = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(RebeatPopup::onInfo));
    infoButton->setPosition({464.f, 272.f});
    m_buttonMenu->addChild(infoButton);
    
    auto deleteSprite = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    deleteSprite->setScale(0.75f);
    auto deleteBtn = CCMenuItemSpriteExtra::create(deleteSprite, this, menu_selector(RebeatPopup::onDelete));
    deleteBtn->setPosition({458, 24});
    m_buttonMenu->addChild(deleteBtn);
    
    m_rightButtonMenu = CCMenu::create();
    m_rightButtonMenu->setLayout(ColumnLayout::create()
        ->setGap(10.f)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setAxisReverse(true)
    );

    m_rightButtonMenu->setPosition({26.f, 145.f});
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
;
    auto reverseSprite = CCSprite::create("sort_btn.png"_spr);
    reverseSprite->setScale(0.75f);
    auto reverseBtn = CCMenuItemSpriteExtra::create(reverseSprite, this, menu_selector(RebeatPopup::onReverse));
    m_rightButtonMenu->addChild(reverseBtn);

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

    if (!m_rebeatsList.isArray()) {
		m_rebeatsList = matjson::Value::array();
    }
    
    makeList(false);
    
    m_rebeatCount = CCLabelBMFont::create(fmt::format("Total Completions: {}", m_rebeats).c_str(), "bigFont.fnt");
    m_rebeatCount->setPosition(m_title->getPositionX(), 20.f);
    m_rebeatCount->setScale(0.5f);
    m_mainLayer->addChild(m_rebeatCount);

    return true;
}

int getRawTime(std::string formattedTime) {
    auto splitTime = Utils::splitString(formattedTime, ':');

    if (splitTime.size() == 0) {
        return 0;
    }

    auto sAndMs = Utils::splitString(splitTime[splitTime.size() - 1], '.');

    splitTime[splitTime.size() - 1] = sAndMs[0];
    splitTime.push_back(sAndMs[1]);
    std::reverse(splitTime.begin(), splitTime.end());

    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int milliseconds = 0;

    if (splitTime.size() > 0) {
        milliseconds = stoi(splitTime[0]);

        if (splitTime.size() > 1) {
            seconds = stoi(splitTime[1]) * 1000;

            if (splitTime.size() > 2) {
                minutes = stoi(splitTime[2]) * 60 * 1000;

                if (splitTime.size() > 3) {
                    hours = stoi(splitTime[3]) * 60 * 60 * 1000;
                }
            }
        }   
    }
    

    return hours + minutes + seconds + milliseconds;
    
};

void defaultSort(CCArrayExt<RebeatCell*> cellsExt) {
    std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
        return a->m_rebeatIndex < b->m_rebeatIndex;
    });
};

void RebeatPopup::sortCells() {
    auto cellsExt = CCArrayExt<RebeatCell*>(m_cells);

    switch(m_sortType) {
        case 0: // default
            defaultSort(cellsExt);
            break;

        case 1: // name
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                return a->m_name < b->m_name;
            });
            break;

        case 2: // date
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                auto timeA = Utils::convertDate(fmt::format("{} {}", a->m_date, Utils::convertTime(a->m_time)));
                auto timeB = Utils::convertDate(fmt::format("{} {}", b->m_date, Utils::convertTime(b->m_time)));

                return timeA < timeB;
            });
            break;

        case 3: // coins
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                int coinsA = 0;
                int coinsB = 0;

                for (int i = 0; i < 3; i++) {
                    if (a->m_coinsCollected[i]) {
                        coinsA++;
                    }

                    if (b->m_coinsCollected[i]) {
                        coinsB++;
                    }
                }

                return coinsA < coinsB;

            });
            break;

        case 4: // attempts
            if (m_level->isPlatformer()) {
                defaultSort(cellsExt);
                break;
            }
            
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                return numFromString<int>(a->m_attempts).unwrapOr(0) < numFromString<int>(b->m_attempts).unwrapOr(0);
            });
            break;

        case 5: // jumps
            if (m_level->isPlatformer()) {
                defaultSort(cellsExt);
                break;
            }
            
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                return numFromString<int>(a->m_jumps).unwrapOr(0) < numFromString<int>(b->m_jumps).unwrapOr(0);
            });
            break;

        case 6: // level time
            if (!m_level->isPlatformer()) {
                defaultSort(cellsExt);
                break;
            }
            
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                int timeA = getRawTime(a->m_levelTime);
                int timeB = getRawTime(b->m_levelTime);

                return timeA < timeB;
            });
            break;
            
        case 7: // points
            if (!m_level->isPlatformer()) {
                defaultSort(cellsExt);
                break;
            }
            
            std::sort(cellsExt.begin(), cellsExt.end(), [](const auto& a, const auto& b) {
                return numFromString<int>(a->m_points).unwrapOr(0) < numFromString<int>(b->m_points).unwrapOr(0);
            });
            break;
    }

    if (m_isReverse) {
        std::reverse(cellsExt.begin(), cellsExt.end());
    }
}

void RebeatPopup::onDelete(CCObject* obj) {
    geode::createQuickPopup(
        "Warning", 
        fmt::format("Are you sure you want to <cr>delete ALL</c> completions for '{}'? This <cr>cannot</c> be undone.", m_level->m_levelName),
        "Cancel", "Delete",
        [this](auto, bool btn2) {
            if (btn2)  {
                m_cells->removeAllObjects();
                m_rebeatsList = matjson::Value::array();
                refreshList();
            }
        }
    );
}

void RebeatPopup::onInfo(CCObject* obj) {
    auto alert = FLAlertLayer::create(
        nullptr,
        "Completion Info",
        "<cj>Classic mode</c> completions display <cy>Attempts</c> and <co>Jumps</c>.\n<cp>Platformer</c> completions display <cy>Time</c> and <co>Points</c>.\n<co>(edited)</c> label: The completion's data has been modified (excluding the <cr>Name</c> and <cp>Icon</c>).\n<cg>(custom)</c> label: The completion was created manually and not upon completing the level.",
        "Ok",
        nullptr,
        400
    );
    alert->show();
}

RebeatPopup* RebeatPopup::create(GJGameLevel* level) {
    auto ret = new RebeatPopup();
    ret->m_level = level;

    if (ret->initAnchored(480, 290, "GJ_square01.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}


void RebeatPopup::refreshList() {
    if (m_list) {
        m_list->removeFromParentAndCleanup(false);
    }
    
    if (m_scrollbar) {
        m_scrollbar->removeFromParentAndCleanup(false);
    }
    
    makeList(true);
}

void RebeatPopup::onCreate(CCObject* obj) {
    auto popup = EditPopup::create(this, nullptr);
    popup->show();
    return;
}

void RebeatPopup::deleteCell(int index, int cellNum) {
    m_cells->removeObjectAtIndex(index, true);
    m_rebeats--;

    auto newRebeatsList = matjson::Value::array();

    for (size_t i = 0; i < m_rebeatsList.size(); i++)
        if (i != cellNum) {
            newRebeatsList.push(m_rebeatsList[i]);
        }

    m_rebeatsList = newRebeatsList;

    refreshList();
}

void RebeatPopup::onSort(CCObject* obj) {
    SortLayer::create(this)->show();
}

void RebeatPopup::addCell(matjson::Value newRebeat) {
    auto newCell = RebeatCell::create(newRebeat, this);

    newCell->m_rebeatIndex = m_rebeats;
    m_rebeats++;

    m_cells->addObject(newCell);
    m_rebeatsList.push(newRebeat);

    refreshList();
}

void RebeatPopup::updateCell(RebeatCell* cell, matjson::Value newRebeat) {
    auto newCell = RebeatCell::create(newRebeat, this);
    newCell->m_rebeatIndex = cell->m_rebeatIndex;

    auto index = m_cells->indexOfObject(cell);
    m_cells->replaceObjectAtIndex(index, newCell, true);

    m_rebeatsList[cell->m_rebeatIndex] = newRebeat;

    refreshList();
}

void RebeatPopup::onReverse(CCObject* obj) {
    m_isReverse = !m_isReverse;
    refreshList();
}

void RebeatPopup::onClose(CCObject* obj) {
    Mod::get()->setSavedValue(m_id, m_rebeatsList);
    geode::Popup<>::onClose(obj);
}