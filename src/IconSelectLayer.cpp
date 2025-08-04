#include "IconSelectLayer.hpp"
#include "EditPopup.hpp"

using namespace geode::prelude;

bool IconSelectLayer::setup() {
    setTitle("Select Icon");
    gm = GameManager::get();
    m_iconCount = gm->countForType(m_gamemode);
    log::debug("iconCount: {}", m_iconCount);
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

    //log::debug("pages: {}", m_pages);
    log::debug("pageCount: {}, pages: {}", m_pageCount, m_pages.size());

    m_unlockType = m_unlockTypes[m_gamemode];

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

    if (ret->initAnchored(420, 260, "GJ_square05.png")) { //450, 245
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void IconSelectLayer::createIcon(int frame) {
    // log::debug("createIcon");
    
    auto item = GJItemIcon::create(
        m_unlockType, 
        frame, 
        gm->colorForIdx(m_popup->m_iconColor), 
        gm->colorForIdx(m_popup->m_iconColor2), 
        true, true, 
        m_popup->m_hasGlow, 
        gm->colorForIdx(m_popup->m_glowColor)
    );
    //item->setScale(0.8f);
    
    //auto icon = SimplePlayer::create(1);
    auto icon = item->getChildByType<SimplePlayer*>(0);
    icon->updatePlayerFrame(frame, m_gamemode);
    icon->setScale(0.8f);
    icon->setColor(gm->colorForIdx(m_popup->m_iconColor));
    icon->setSecondColor(gm->colorForIdx(m_popup->m_iconColor2));
    icon->m_hasGlowOutline = m_popup->m_hasGlow;
    
    //log::debug("hasGlow: {}", icon->m_hasGlowOutline);
    
    if (icon->m_hasGlowOutline) {
        //icon->m_hasCustomGlowColor = true;
        icon->enableCustomGlowColor(gm->colorForIdx(m_popup->m_glowColor));
        icon->setGlowOutline(gm->colorForIdx(m_popup->m_glowColor));
        if (m_gamemode == IconType::Robot)
            icon->m_robotSprite->m_glowSprite->setVisible(true);
        else if (m_gamemode == IconType::Spider)
            icon->m_spiderSprite->m_glowSprite->setVisible(true);
        else
            icon->m_outlineSprite->setVisible(true);

        // CCSprite* glowSprite = nullptr;

        // if (m_gamemode == IconType::Robot)
        //     glowSprite = icon->getChildByType<GJRobotSprite*>(0)->getChildByType<CCPartAnimSprite*>(0)->getChildByType<CCSprite*>(0);
        // else if (m_gamemode == IconType::Spider)
        //     glowSprite = icon->getChildByType<GJSpiderSprite*>(0)->getChildByType<CCPartAnimSprite*>(0)->getChildByType<CCSprite*>(0);
        // else
        //     glowSprite = icon->getChildByType<CCSprite*>(0)->getChildByType<CCSprite*>(0);

        // if (glowSprite)
        //     glowSprite->setVisible(true);
        
        // log::debug("glow: {}, {}", (glowSprite != nullptr), glowSprite);
    }
    else 
        icon->disableCustomGlowColor();

    auto iconBtn = CCMenuItemSpriteExtra::create(item, this, menu_selector(IconSelectLayer::onIconSelected));

    // iconBtn->setPosition({xpos, ypos});
    iconBtn->setTag(frame);
    // m_icons.push_back(iconBtn);
    m_pages[m_currentPage]->addChild(iconBtn);
    //m_iconMenu->updateLayout();
}

void IconSelectLayer::onIconSelected(CCObject* obj) {
    log::debug("iconSelected");
    m_popup->iconSelected(static_cast<CCMenuItemSpriteExtra*>(obj)->getTag());
    this->keyBackClicked();
}

std::vector<CCMenuItemSpriteExtra*> IconSelectLayer::paginate(std::vector<CCMenuItemSpriteExtra*>& array, int size, int page) {
    std::vector<CCMenuItemSpriteExtra*> result;
    if (size <= 0) return result;
    int startIndex = page * size;
    int endIndex = std::min(startIndex + size, static_cast<int>(array.size()));
    if (startIndex >= array.size()) return result;
    result.insert(result.end(), array.begin() + startIndex, array.begin() + endIndex);
    return result;
}

void IconSelectLayer::onPageLeft(CCObject* obj) {
    log::debug("pageLeft");
    m_previousPage = m_currentPage;
    m_currentPage--;
    changePage(false);
}

void IconSelectLayer::onPageRight(CCObject* obj) {
    log::debug("pageRight");
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

    //m_iconMenu->updateLayout();
    ///m_pages.push_back(page);
    //createPageIcons();
    return page;
}

void IconSelectLayer::createPageIcons() {
    //auto gm = GameManager::get();
    log::debug("createIcons for page {}", m_currentPage);
    int startIndex = m_pageSize * m_currentPage;
    if (startIndex == 0)
        startIndex++;

    for (int i = startIndex; i < m_pageSize + startIndex; i++) {
        if (i <= m_iconCount)
            createIcon(i);
    }

    m_pages[m_currentPage]->updateLayout();
}

void IconSelectLayer::onPageButton(CCObject* obj) {
    auto toggle = static_cast<CCMenuItemToggler*>(obj);
    m_previousPage = m_currentPage;
    m_currentPage = toggle->getTag();
    
    log::debug("(btn) prev: {}, curr: {}, tag: {}", m_previousPage, m_currentPage, toggle->getTag());

    if (m_previousPage == m_currentPage) {
        m_pageButtons[m_currentPage]->toggle(false);
        return;
    }

    changePage(true);
    
}

void IconSelectLayer::changePage(bool invert) {

    if (m_currentPage >= m_pageCount)
        m_currentPage =  0;

    if (m_currentPage < 0)
        m_currentPage =  m_pageCount - 1;

    if (m_pages[m_currentPage]->getChildrenCount() == 0)
        createPageIcons();
    
    m_pages[m_currentPage]->setVisible(true);
    m_pages[m_previousPage]->setVisible(false);

    m_pageButtons[m_currentPage]->toggle(!invert);
    m_pageButtons[m_previousPage]->toggle(false);

    /*for (int i = 0; i < m_pageButtons.size(); i++) {
        m_pageButtons[i]->toggle(true);
    }*/
}