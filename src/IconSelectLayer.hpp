#pragma once

using namespace geode::prelude;

class EditPopup;

class IconSelectLayer : public geode::Popup<> {

private:

    EditPopup* m_popup;
    IconType m_gamemode;
    UnlockType m_unlockType;
    CCMenu* m_iconMenu;
    std::vector<CCMenuItemSpriteExtra*> m_icons;
    int m_pageSize = 50;
    int m_iconCount = 0;
    int m_currentPage = 0;
    int m_previousPage;
    int m_pageCount = 0;
    std::vector<CCMenu*> m_pages;
    GameManager* gm;
    // CCMenuItemToggler* m_currPageBtn = nullptr;
    // CCMenuItemToggler* m_prevPageBtn = nullptr;
    CCMenu* m_navButtonMenu = nullptr;
    std::vector<CCMenuItemToggler*> m_pageButtons;

    std::map<IconType, UnlockType> m_unlockTypes {
        {IconType::Cube, UnlockType::Cube},
        {IconType::Ship, UnlockType::Ship},
        {IconType::Ball, UnlockType::Ball},
        {IconType::Ufo, UnlockType::Bird},
        {IconType::Wave, UnlockType::Dart},
        {IconType::Robot, UnlockType::Robot},
        {IconType::Spider, UnlockType::Spider},
        {IconType::Swing, UnlockType::Swing},
        {IconType::Jetpack, UnlockType::Jetpack},
    };

    /*std::map<IconType, int> m_spacings {
        {IconType::Cube, 25},
        {IconType::Ship, 35},
        {IconType::Ball, 25},
        {IconType::Ufo, 35},
        {IconType::Wave, 12},
        {IconType::Robot, UnlockType::Robot},
        {IconType::Spider, UnlockType::Spider},
        {IconType::Swing, UnlockType::Swing},
        {IconType::Jetpack, UnlockType::Jetpack},
    }*/

    bool setup() override;
    
    void createIcon(int frame);

    void onIconSelected(CCObject* obj);

    std::vector<CCMenuItemSpriteExtra*> paginate(std::vector<CCMenuItemSpriteExtra*>& array, int size, int page);

    void onPageLeft(CCObject* obj);

    void onPageRight(CCObject* obj);

    CCMenu* createPage();

    void createPageIcons();

    void onPageButton(CCObject* obj);

    void changePage(bool invert);

public:

    static IconSelectLayer* create(EditPopup* popup);

};
