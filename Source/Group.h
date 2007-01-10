/**
  *  Transfer class - Group
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __GROUP_H__
#define __GROUP_H__

#include <vector>

namespace kTransfer3 {
  class Group {
 /* public:
    struct sGroup {
      UINT id;
      String name;
      ListWnd::oItem item; // grupa
    };
    typedef std::vector<sGroup> tGroup;

  public:
    Group {
    }

    inline bool registerGroup(sGroup &group) {
      if (isGroup(group.id)) return false;
    }

    bool isGroup(UINT id) {
      tGroup::iterator it = _group.begin();
      for(; it != _group.end(); it++) {
        if ((*it).id == id) return true;
      }
      return false;
    }

    bool deleteGroup(UINT id) {

    }

  private:
    ListWnd::oItem _addGroupToLv(const ListWnd::oItem &groupEntry) {

    }

    ListWnd::oItem _getGroupFromLv(UINT id) {

    }
  private:
    tGroup _group;
  
  */
  };
  
};

#endif /*__GROUP_H__*/