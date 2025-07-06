using namespace geode::prelude;

class RebeatCell;

class RebeatPopup : public geode::Popup<> {

protected:

    bool setup() override;

private:

    void onDelete(CCObject* obj);

    void onInfo(CCObject* obj);

    void makeList(bool refresh);

    void onSettings(CCObject* obj);

    void onCreate(CCObject* obj);

    void onSort(CCObject* obj);

    
    public:
    
    GJGameLevel* m_level;
    CCArray* m_cells;
    matjson::Value m_rebeatsList;
    std::string m_id;
    int m_rebeats;
    int m_prevAttempts;
    bool m_hasCompletion;
    bool m_calculateAttempts;
    bool m_isRobtopLevel;
    ListView* m_listView;
    GJCommentListLayer* m_list;
    CCLabelBMFont* m_rebeatCount;
    Scrollbar* m_scrollbar;
    CCMenu* m_rightButtonMenu;
    
    static RebeatPopup* create(GJGameLevel* level);
    
    void refreshList();
    
    void deleteCell(int index);
    
    void addCell(matjson::Value newRebeat);
    
    void updateCell(RebeatCell* cell, matjson::Value newRebeat);
};
