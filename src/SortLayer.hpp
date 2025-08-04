using namespace geode::prelude;

class RebeatPopup;

class SortLayer : public geode::Popup<> {

protected:

    bool setup() override;

    void onClose(CCObject* obj) override;

private:

    RebeatPopup* m_popup;
    int m_selectedSort = 0;
    std::vector<CCMenuItemToggler*> m_toggles;

    void onToggle(CCObject* obj);

    void onInfo(CCObject* obj);

public:
    
    static SortLayer* create(RebeatPopup* popup);
   
};
