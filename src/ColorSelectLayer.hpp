// Credit to Icon Gradients by Zilko for the ColorSelectLayer code (Used with permission from Zilko)

using namespace geode::prelude;

class EditPopup;

class ColorSelectLayer : public geode::Popup<> {

private:

    EditPopup* m_popup;
    int m_colorIndex;

    bool setup() override;

    void createButton(int color, const cocos2d::CCPoint& pos);

    void onColor(CCObject* sender);

public:

    static ColorSelectLayer* create(EditPopup* popup, const int colorIndex);

};
