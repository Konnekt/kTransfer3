namespace kTransfer3 {
  class iGroup: public ListWnd::Collection {
  public:
    STAMINA_OBJECT_CLASS(iGroup, iCollection);

  public:
    iGroup(UINT id, const String name = "") {
      _id = id;
    }

    virtual UINT getID() const {
      return _id;
    }

    virtual void refresh() {
    }

    virtual ListWnd::oItem createItem(ListWnd::ListView* lv, const ListWnd::oItemCollection& collection) {
      ListWnd::oItem item = new ListWnd::ItemCollection(*this);
      return item;
    }



  private:
    UINT _id;
    String _name;
  };  
};