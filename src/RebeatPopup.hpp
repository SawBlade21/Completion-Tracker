using namespace geode::prelude;

class RebeatCell;

class RebeatPopup : public geode::Popup<> {

protected:

    bool setup() override;

    void onClose(CCObject* obj) override;

private:

    void onDelete(CCObject* obj);

    void onInfo(CCObject* obj);

    void makeList(bool refresh);

    void onSettings(CCObject* obj);

    void onCreate(CCObject* obj);

    void onSort(CCObject* obj);

    void onReverse(CCObject* obj);

    
public:
    
    int m_rebeats = 0;
    int m_prevAttempts;
    int m_sortType = 0;
    bool m_hasCompletion;
    bool m_isRobtopLevel;
    bool m_isReverse = false;
    GJGameLevel* m_level;
    CCArray* m_cells;
    matjson::Value m_rebeatsList;
    std::string m_id;
    ListView* m_listView;
    GJCommentListLayer* m_list;
    Scrollbar* m_scrollbar;
    CCLabelBMFont* m_rebeatCount;
    CCMenu* m_rightButtonMenu;
    
    static RebeatPopup* create(GJGameLevel* level);
    
    void refreshList();
    
    void deleteCell(int index, int cellNum);
    
    void addCell(matjson::Value newRebeat);
    
    void updateCell(RebeatCell* cell, matjson::Value newRebeat);

    void sortCells();
};
