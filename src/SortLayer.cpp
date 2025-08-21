#include "SortLayer.hpp"
#include "RebeatPopup.hpp"

bool SortLayer::setup() {
    setID("SortLayer"_spr);
    setTitle("Sort Completions");

    auto menu = CCMenu::create();
    menu->setContentSize({360, 260});
    menu->setPosition({180, 130});
    menu->setAnchorPoint({0.5f, 0.5f});
    m_mainLayer->addChild(menu);

    float xpos = -130.f;
    float ypos = 66.f;

    std::vector<std::string> labels = {"None", "Name", "Date", "Coins", "Attempts", "Jumps", "Level Time", "Points"};
    bool isPlatformer = m_popup->m_level->isPlatformer();

    for (int i = 0; i <= 7; i++) {
        auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        toggleOn->setScale(0.8f);

        auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        toggleOff->setScale(0.8f);

        auto toggle = CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(SortLayer::onToggle));
        toggle->setTag(i);
        toggle->setPosition({xpos, ypos});
        menu->addChild(toggle);
        m_toggles.push_back(toggle);

        if (i == m_popup->m_sortType) {
            toggle->toggle(true);
        }

        auto label = CCLabelBMFont::create(labels[i].c_str(), "bigFont.fnt");
        label->setPosition({xpos + 18.f, ypos});
        label->setScale(0.5f);
        label->setAnchorPoint({0.f, 0.5f});
        menu->addChild(label);

        ypos -= 48.f;

        if ((i + 1) % 4 == 0) {
            xpos += 172.f;
            ypos = 66.f;
        }

        if ((!isPlatformer && i > 5) || (isPlatformer && (i < 6 && i > 3))) {
            toggle->setEnabled(false);
            toggleOff->setColor({165, 165, 165});
            label->setColor({165, 165, 165});
        }
    }

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSprite->setScale(0.75f);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(SortLayer::onInfo));
    infoBtn->setPosition({342, 240});
    m_buttonMenu->addChild(infoBtn);

    return true;
}

SortLayer* SortLayer::create(RebeatPopup* popup) {
    SortLayer* ret = new SortLayer();

    ret->m_popup = popup;

    if (ret->initAnchored(360, 260, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void SortLayer::onToggle(CCObject* obj) {
    auto toggle = static_cast<CCMenuItemToggler*>(obj);

    if (toggle->isToggled()) {
        toggle->toggle(false);
        return;
    }

    int index = toggle->getTag();

    for (int i = 0; i < m_toggles.size(); i++) {
        if (i != index) {
            m_toggles[i]->toggle(false);
        }
    }

    m_popup->m_sortType = index;

}

void SortLayer::onClose(CCObject* obj) {
    m_popup->refreshList();
    geode::Popup<>::onClose(obj);
}

void SortLayer::onInfo(CCObject* obj) {
    FLAlertLayer::create(
        "Sorting Info",
        "Sort completions by specific data.\n<cj>Classic</c> levels can be sorted by <cy>Attempts</c> or <co>Jumps</c>.\n<cp>Platformer</c> levels can be sorted by <cy>Level Time</c> or <co>Points</c>.",
        "OK"
    )->show();
}