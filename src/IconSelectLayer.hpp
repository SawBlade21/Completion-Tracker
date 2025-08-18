#pragma once

using namespace geode::prelude;

class EditPopup;

class IconSelectLayer : public geode::Popup<> {

private:

    GameManager* gm;
    EditPopup* m_popup;
    IconType m_gamemode;
    UnlockType m_unlockType;
    CCMenu* m_iconMenu;
    CCMenu* m_navButtonMenu = nullptr;
    std::vector<CCMenuItemToggler*> m_pageButtons;
    std::vector<CCMenuItemSpriteExtra*> m_icons;
    std::vector<CCMenu*> m_pages;
    int m_pageSize = 50;
    int m_iconCount = 0;
    int m_currentPage = 0;
    int m_previousPage;
    int m_pageCount = 0;

    bool setup() override;
    
    void createIcon(int frame);

    void onIconSelected(CCObject* obj);

    void onPageLeft(CCObject* obj);

    void onPageRight(CCObject* obj);

    CCMenu* createPage();

    void createPageIcons();

    void onPageButton(CCObject* obj);

    void changePage(bool invert);

public:

    static IconSelectLayer* create(EditPopup* popup);

};
