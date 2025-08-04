#pragma once

class RebeatPopup;

using namespace geode::prelude;

class RebeatCell : public cocos2d::CCNode {

private:

    CCLabelBMFont* m_dateLabel;

    bool init();

    void onDelete(CCObject* obj);

    void onEdit(CCObject* obj);

    void onVideo(CCObject* obj);

    void setupCell();

public:

    // TODO: make private the ones i dont end up using in EditPopup
    RebeatPopup* m_popup;
    matjson::Value m_rebeat;
    std::string m_name = "";
    std::string m_date = "";
    std::string m_time = "";
    int m_rebeatIndex;
    matjson::Value m_iconData;
    IconType m_iconType;
    int m_iconFrame;
    int m_iconColor = 12;
    int m_iconColor2 = 12;
    int m_glowColor = 12;
    bool m_hasGlow = false;
    matjson::Value m_coinData;
    int m_coinAmount;
    bool m_coinsCollected[3] = {false, false, false};
    std::string m_attempts = "";
    std::string m_jumps = "";
    std::string m_levelTime = "";
    std::string m_points = "";
    std::string m_ytLink = "";
    std::string m_watermark = "";

    static RebeatCell* create(const matjson::Value &rebeat, RebeatPopup* popup);

};
