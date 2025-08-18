#include "YoutubeLayer.hpp"
#include "EditPopup.hpp"

bool YoutubeLayer::setup() {
    setTitle("Set Youtube Video");

    auto menu = CCMenu::create();
    menu->setContentSize({260, 150});
    menu->setPosition({0, 0});
    menu->setAnchorPoint({0.5f, 0.5f});
    m_mainLayer->addChild(menu);

    auto bottomMenu = CCMenu::create();
    bottomMenu->setLayout(RowLayout::create()
        ->setGap(15.f)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setAutoScale(false)
    );
    bottomMenu->setPosition({130, 25});
    m_mainLayer->addChild(bottomMenu);

    auto openSprite = ButtonSprite::create("Open");
    openSprite->setScale(0.9f);
    auto openBtn = CCMenuItemSpriteExtra::create(openSprite, this, menu_selector(YoutubeLayer::onOpen));
    bottomMenu->addChild(openBtn);

    auto saveSprite = ButtonSprite::create("Save");
    saveSprite->setScale(0.9f);
    auto saveBtn = CCMenuItemSpriteExtra::create(saveSprite, this, menu_selector(YoutubeLayer::onSave));
    bottomMenu->addChild(saveBtn);

    bottomMenu->updateLayout();

    auto ytText = CCLabelBMFont::create("Link", "bigFont.fnt");
    ytText->setScale(0.65f);
    ytText->setPosition(130, 100);
    menu->addChild(ytText);

    m_linkInput = TextInput::create(220.f, "...", "chatFont.fnt");
    m_linkInput->setScale(1.f);
    m_linkInput->setPosition(130, 68);
    m_linkInput->setCommonFilter(CommonFilter::Any);

    if (m_link != "") m_linkInput->setString(m_link);

    menu->addChild(m_linkInput);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSprite->setScale(0.75f);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSprite, this, menu_selector(YoutubeLayer::onInfo));
    infoBtn->setPosition(245, 133);
    menu->addChild(infoBtn);

    return true;
}

YoutubeLayer* YoutubeLayer::create(EditPopup* popup) {
    YoutubeLayer* ret = new YoutubeLayer();
    ret->m_popup = popup;
    ret->m_link = popup->m_ytLink;

    if (ret->initAnchored(260, 150, "GJ_square05.png")) { //450, 245
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void YoutubeLayer::onOpen(CCObject* obj) {
    if (m_linkInput->getString() != "") {
        geode::utils::web::openLinkInBrowser(m_linkInput->getString());
    } 
    else {
        FLAlertLayer::create(
            "Error",
            "Please enter a link.",
            "OK"
        )->show();
    }
}

void YoutubeLayer::onSave(CCObject* obj) {
    m_popup->m_ytLink = m_linkInput->getString();
    keyBackClicked();
}

void YoutubeLayer::onInfo(CCObject* obj) {
    FLAlertLayer::create(
        "Video Info",
        "Input the full URL of the Youtube video.\nThe link will open when clicking the Youtube button in the completion.\nTo remove a video from the completion, leave the link empty.",
        "OK"
    )->show();
}