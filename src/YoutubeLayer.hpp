#pragma once

using namespace geode::prelude;

class EditPopup;

class YoutubeLayer : public geode::Popup<> {

private:

    EditPopup* m_popup;
    std::string m_link = "";
    TextInput* m_linkInput;

    bool setup() override;

    void onOpen(CCObject* obj);

    void onSave(CCObject* obj);

    void onInfo(CCObject* obj);
    
public:

    static YoutubeLayer* create(EditPopup* popup);

};
