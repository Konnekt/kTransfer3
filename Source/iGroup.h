#ifndef __IGROUP_H__
#define __IGROUP_H__

namespace kTransfer3 {
  class iGroup: public ListWnd::Collection {
  public:
    STAMINA_OBJECT_CLASS(iGroup, iCollection);

  public:
    iGroup(UINT id, const StringRef &name = "") {
      _id = id;
      _name = name;
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

#endif /*__IGROUP_H__*/