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

namespace kTransfer3 {
  class Group {
  public:
    enum enGroup{
      grNone,
      grReceiving,
      grSending,
      grChecking,
      grOther
    };

  public:
    Group() {
      _group = grNone;
    }
    inline bool setGroup(enGroup group) {
      if (_group != grNone) {
        return false;
      }
      _group = group;
      return true;
    }
    inline enGroup getGroup() {
      return _group;
    }

  private:
    enGroup _group;
  };
};

#endif /*__GROUP_H__*/