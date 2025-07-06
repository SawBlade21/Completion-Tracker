#pragma once

class RebeatCell;

class RebeatPopup;

using namespace geode::prelude;

class EditPopup : public geode::Popup<> {

    protected:

        bool setup() override;

    private:

        bool m_isPlatformer;
        bool m_isAutocomplete;
        TextInput* m_nameInput;

        TextInput* m_timeInput;
        TextInput* m_yearInput;
        TextInput* m_monthInput;
        TextInput* m_dayInput;
        std::vector<std::string> m_cellDates = {"", "", ""};

        TextInput* m_hourInput;
        TextInput* m_minuteInput;
        TextInput* m_secondInput;
        std::vector<std::string> m_cellTimes = {"", "", "", ""};
        CCLabelBMFont* m_timeSettingText;

        CCMenuItemToggler* m_coin1Toggle;
        CCMenuItemToggler* m_coin2Toggle;
        CCMenuItemToggler* m_coin3Toggle;

        TextInput* m_attemptsInput;
        TextInput* m_jumpsInput;
        TextInput* m_pointsInput;

        TextInput* m_levelHoursInput;
        TextInput* m_levelMinutesInput;
        TextInput* m_levelSecondsInput;
        TextInput* m_levelMillisecondsInput;
        std::vector<std::string> m_cellLevelTimes = {"", "", "", ""};

        SimplePlayer* m_icon;
        CCMenuItemSpriteExtra* m_color1Btn;
        CCMenuItemSpriteExtra* m_color2Btn;
        CCMenuItemSpriteExtra* m_color3Btn;
        CCMenuItemToggler* m_glowToggle;
        int m_iconFrames[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

        int m_coinAmount = 0;
        bool m_coinsCollected[3] = {false, false, false};
        
        
        void onSave(CCObject* obj);
        
        void onDelete(CCObject* obj);
        
        void onReset(CCObject* obj);
        
        void onResetName(CCObject* obj);
        
        void onResetDate(CCObject* obj);
        
        void onResetTime(CCObject* obj);

        void onResetCoins(CCObject* obj);

        void onResetAttempts(CCObject* obj);

        void onResetJumps(CCObject* obj);

        void onResetLevelTime(CCObject* obj);

        void onResetPoints(CCObject* obj);
        
        void onAutoName(CCObject* obj);
        
        void onAutoDate(CCObject* obj);
        
        void onAutoTime(CCObject* obj);

        void onAutoCoins(CCObject* obj);

        void onAutoAttempts(CCObject* obj);

        void onAutoJumps(CCObject* obj);

        void onAutoLevelTime(CCObject* obj);

        void onAutoPoints(CCObject* obj);
        
        void onResetIcon(CCObject* obj);
        
        void resetIconValues();
        
        void updateIcon();
        
        void onLeftIconArrow(CCObject* obj);
        
        void onRightIconArrow(CCObject* obj);
        
        void onGarage(CCObject* obj);
        
        void onPasteIcon(CCObject* obj);
        
        void onColor1(CCObject* obj);
        
        void onColor2(CCObject* obj);
        
        void onColor3(CCObject* obj);
        
        void openColorSelect();
        
        void onToggleGlow(CCObject* obj);
        
        void updateColors();

        void onInfo(CCObject* obj);

        void onInfoInfo(CCObject* obj);

        void onIconInfo(CCObject* obj);

        void onStatsInfo(CCObject* obj);

        void onCoin(CCObject* obj);

        void onTimeArrow(CCObject* obj);

        matjson::Value dataToJson(std::string name, std::string date, std::string time, int iconType, int frame, int color1, int color2, int glowColor, bool hasGlow, int coinAmount, bool coinsCollected[3], std::string stat1, std::string stat2, bool isPlatformer);
        
        
    public:
        
        IconType m_iconType = IconType::Cube;
        int m_iconFrame = 0;
        int m_iconColor = 12;
        int m_iconColor2 = 12;
        int m_glowColor = 12;
        bool m_hasGlow = false;
        RebeatPopup* m_popup;
        RebeatCell* m_cell;
        bool m_isCreate;
        
        static EditPopup* create(RebeatPopup* popup);
        
        static EditPopup* create(RebeatPopup* popup, RebeatCell* cell);

        void colorSelected(const cocos2d::ccColor3B& color, int colorIndex, int colorID);

        void iconSelected(int frame);
    };
