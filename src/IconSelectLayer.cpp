#include "IconSelectLayer.hpp"
#include "EditPopup.hpp"

using namespace geode::prelude;

bool IconSelectLayer::setup() {
    setTitle("Select Icon");
    gm = GameManager::get();
    m_iconCount = gm->countForType(m_gamemode);
    
    if (m_iconCount <= 0) {
        return true;
    }
    
    m_unlockType = gm->iconTypeToUnlockType(m_gamemode);
    m_pageCount = static_cast<int>(std::ceil(static_cast<float>(m_iconCount) / static_cast<float>(m_pageSize)));

    m_navButtonMenu = CCMenu::create();
    m_navButtonMenu->setPosition({210, 20});
    m_navButtonMenu->setLayout(RowLayout::create()
        ->setAutoScale(false)
    );
    m_mainLayer->addChild(m_navButtonMenu);

    auto toggleOnSprite = CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png");
    auto toggleOffSprite = CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png");

    for (int i = 0; i < m_pageCount; i++) {
        auto page = createPage();
        m_pages.push_back(page);
        m_mainLayer->addChild(page);

        auto pageBtn = CCMenuItemToggler::create(toggleOffSprite, toggleOnSprite, this, menu_selector(IconSelectLayer::onPageButton));
        pageBtn->setTag(i);

        m_pageButtons.push_back(pageBtn);
        m_navButtonMenu->addChild(pageBtn);
    }

    m_pageButtons[m_currentPage]->toggle(true);
    m_navButtonMenu->updateLayout();

    if (m_pageCount > 1) {
        auto rightArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        rightArrowSprite->setRotationY(180.f);

        auto rightArrowBtn = CCMenuItemSpriteExtra::create(rightArrowSprite, this, menu_selector(IconSelectLayer::onPageRight));
        rightArrowBtn->setPosition(440, 125);
        m_buttonMenu->addChild(rightArrowBtn);
    
        auto leftArrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        auto leftArrowBtn = CCMenuItemSpriteExtra::create(leftArrowSprite, this, menu_selector(IconSelectLayer::onPageLeft));
        leftArrowBtn->setPosition(-20, 125);
        m_buttonMenu->addChild(leftArrowBtn);
    }

    createPageIcons();
    return true;
}

IconSelectLayer* IconSelectLayer::create(EditPopup* popup) {
    IconSelectLayer* ret = new IconSelectLayer();
    ret->m_popup = popup;
    ret->m_gamemode = popup->m_iconType;

    if (ret->initAnchored(420, 260, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void IconSelectLayer::createIcon(int frame) {
    
    auto item = GJItemIcon::create(
        m_unlockType, 
        frame, 
        gm->colorForIdx(m_popup->m_iconColor), 
        gm->colorForIdx(m_popup->m_iconColor2), 
        true, true, 
        m_popup->m_hasGlow, 
        gm->colorForIdx(m_popup->m_glowColor)
    );

    auto icon = item->getChildByType<SimplePlayer*>(0);
    icon->updatePlayerFrame(frame, m_gamemode);
    icon->setScale(0.8f);
    icon->setColor(gm->colorForIdx(m_popup->m_iconColor));
    icon->setSecondColor(gm->colorForIdx(m_popup->m_iconColor2));
    icon->m_hasGlowOutline = m_popup->m_hasGlow;
    
    if (icon->m_hasGlowOutline) {
        icon->enableCustomGlowColor(gm->colorForIdx(m_popup->m_glowColor));
        icon->setGlowOutline(gm->colorForIdx(m_popup->m_glowColor));

        if (m_gamemode == IconType::Robot) {
            icon->m_robotSprite->m_glowSprite->setVisible(true);
        }
        else if (m_gamemode == IconType::Spider) {
            icon->m_spiderSprite->m_glowSprite->setVisible(true);
        }
        else {
            icon->m_outlineSprite->setVisible(true);
        }
    }
    else {
        icon->disableCustomGlowColor();
    }

    auto iconBtn = CCMenuItemSpriteExtra::create(item, this, menu_selector(IconSelectLayer::onIconSelected));

    iconBtn->setTag(frame);
    m_pages[m_currentPage]->addChild(iconBtn);
}

void IconSelectLayer::onIconSelected(CCObject* obj) {
    m_popup->iconSelected(static_cast<CCMenuItemSpriteExtra*>(obj)->getTag());
    this->keyBackClicked();
}

void IconSelectLayer::onPageLeft(CCObject* obj) {
    m_previousPage = m_currentPage;
    m_currentPage--;
    changePage(false);
}

void IconSelectLayer::onPageRight(CCObject* obj) {
    m_previousPage = m_currentPage;
    m_currentPage++;
    changePage(false);
}

CCMenu* IconSelectLayer::createPage() {
    auto page = CCMenu::create();
    page->setAnchorPoint({0.f, 1.f});
    page->setPosition({24, 220});
    
    page->setLayout(RowLayout::create()
        ->setGap(8.f)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setAutoGrowAxis(std::nullopt)
    );

    page->setContentSize({400, 180});
    return page;
}

void IconSelectLayer::createPageIcons() {
    int startIndex = m_pageSize * m_currentPage;

    if (startIndex == 0) startIndex++;

    for (int i = startIndex; i < m_pageSize + startIndex; i++) {
        if (i <= m_iconCount) {
            createIcon(i);
        }
    }

    m_pages[m_currentPage]->updateLayout();
}

void IconSelectLayer::onPageButton(CCObject* obj) {
    auto toggle = static_cast<CCMenuItemToggler*>(obj);
    m_previousPage = m_currentPage;
    m_currentPage = toggle->getTag();

    if (m_previousPage == m_currentPage) {
        m_pageButtons[m_currentPage]->toggle(false);
        return;
    }

    changePage(true);
}

void IconSelectLayer::changePage(bool invert) {

    if (m_currentPage >= m_pageCount) m_currentPage =  0;

    if (m_currentPage < 0) m_currentPage =  m_pageCount - 1;

    if (m_pages[m_currentPage]->getChildrenCount() == 0) createPageIcons();
    
    m_pages[m_currentPage]->setVisible(true);
    m_pages[m_previousPage]->setVisible(false);

    m_pageButtons[m_currentPage]->toggle(!invert);
    m_pageButtons[m_previousPage]->toggle(false);
}