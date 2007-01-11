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
#include <iGroup.h>

namespace kTransfer3 {
  class Group {
  public:
    typedef std::vector<iGroup*> tGroup;

  public:
    Group {
    }

    inline bool registerGroup(iGroup* group) {
      if (isGroup(group.id)) return false;
      _group.push_back(group);
      return true;
    }

    bool isGroup(UINT id) {
      tGroup::iterator it = _group.begin();
      for(; it != _group.end(); it++) {
        if ((*it)->id == id) return true;
      }
      return false;
    }

    bool deleteGroup(UINT id) {
      tGroup::iterator it = _group.begin();
      for(; it != _group.end(); it++) {
        if ((*it)->id == id) {
          //delete (*it);
          _group.erase(it);
          return true;
        }
      }
      return false;
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