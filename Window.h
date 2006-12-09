#ifndef __EASYWIN_WINDOW_H__
#define __EASYWIN_WINDOW_H__
#include <map>
#include <string>
#include <assert.h>
#include "iEvent.h"
#include "easyWinDefines.h"

#pragma warning(push)
#pragma warning(disable: 4800)

namespace easyWin {
  class Window : public iEvent {
  public:
    typedef std::map<std::string, signals::connection> tConnections;
    typedef signal<LPARAM (HWND, UINT, WPARAM, LPARAM)> sigOnMsg;

    struct sig {
      sigOnMsg signal;
    };

    typedef std::map<HWND, sig*> tMsgProcs;
    typedef std::map<int, LPARAM> tStaticValues;

    struct sObserver {
      tConnections connections;
      sigOnMessage signal;
    };

    typedef std::map<int, sObserver*> tObservers;

  public:
    enum wndStyle {
      wsDefault = WS_OVERLAPPED,
      wsBorder = WS_BORDER,
      wsDlgFrame = WS_DLGFRAME,
      wsThickFrame = WS_THICKFRAME,
      wsVScroll = WS_VSCROLL,
      wsHScroll = WS_HSCROLL,
      wsCaption = WS_CAPTION,
      wsSysMenu = WS_SYSMENU,
      wsMaximizeBox = WS_MAXIMIZEBOX,
      wsMinimizeBox = WS_MINIMIZEBOX,
      wsOverlappedWindow = WS_OVERLAPPEDWINDOW,
      wsOverlapped = WS_OVERLAPPED,
      wsChild = WS_CHILD,
      wsDisabled = WS_DISABLED,
      wsVisible = WS_VISIBLE,
      wsMaximize = WS_MAXIMIZE,
      wsMinimize = WS_MINIMIZE
    };

    enum wndState {
      wstShow = SW_SHOW,
      wstHide = SW_HIDE,
      wstMaximize = SW_MAXIMIZE,
      wstMinimize = SW_MINIMIZE,
      wstRestore = SW_RESTORE
    };
   
    enum wndExStyle {
      wsExContextHelp = WS_EX_CONTEXTHELP,
      wsExTopMost = WS_EX_TOPMOST,
      wsExToolWindow = WS_EX_TOOLWINDOW,
      wsExClientEdge = WS_EX_CLIENTEDGE, 
      wsExAcceptFiles = WS_EX_ACCEPTFILES,
      wsExControlParent = WS_EX_CONTROLPARENT
    };

    Window(): _handle(0) {
      registerObserver(WM_SIZE, boost::bind(&Window::onSize, this, _1), 1, boost::signals::connect_position::at_back, "SizeCallback", false);
      registerObserver(WM_MOVE, boost::bind(&Window::onMove, this, _1), 1, boost::signals::connect_position::at_back, "MoveCallback", false);
      registerObserver(WM_MOUSEMOVE, boost::bind(&Window::onMouseMove, this, _1), 1, boost::signals::connect_position::at_back, "MouseMoveCallback", false);
    };

    virtual ~Window() {
      if (getHandle()) {
        destroy();
      }
    }

    inline void prepareClass(char *className = "easyWin::unknown") {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc	= (WNDPROC)&Window::_sMsgProc;
        wcex.cbClsExtra	= 0;
        wcex.cbWndExtra	= 0;
        wcex.hInstance	= instance;
        wcex.hIcon	= LoadIcon(NULL, 0);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName	= "";
        wcex.lpszClassName	= className;
        wcex.hIconSm = 0;

        RegisterClassEx(&wcex); 
    }

    virtual void create(const char *name, int style, int exStyle, int x, int y, int w, int h, HWND parent = NULL) = 0;

    inline bool show(wndState state = wstShow) {
      assert(_handle != NULL);
      return (bool)ShowWindow((HWND)_handle, state);
    }

    inline bool setText(const char *title) {
      assert(_handle != NULL);
      return (bool)SetWindowText((HWND)_handle, title);
    }

    inline const char *getText() {
      assert(_handle != NULL);
      char buff[256]={0};
      int ret = GetWindowText((HWND)_handle, buff, 255);
      return (ret) ? buff : "";
    }

    inline HANDLE getHandle() {
      assert(_handle != NULL);
      return _handle;
    }

    inline bool isHandle() {
      return (bool)_handle;
    }

    inline bool setParent(HWND parent) {
      assert(_handle != NULL);
      return (bool)SetParent((HWND)_handle, parent);
    }

    inline bool getParent(HWND parent) {
      assert(_handle != NULL);
      return (bool)GetParent((HWND)_handle);
    }

    inline HANDLE setIcon(int type, HANDLE icon = NULL) {
      assert(_handle != NULL);
      return (HANDLE)SendMessage((HWND)_handle, WM_SETICON, (WPARAM)type, (LPARAM)icon);
    }

    inline HANDLE getIcon(int type) {
      assert(_handle != NULL);
      return (HANDLE)SendMessage((HWND)_handle, WM_GETICON, (WPARAM)type, NULL);
    }

    inline bool close() {
      assert(_handle != NULL);
      return (bool)CloseWindow((HWND)_handle);
    }

    inline bool destroy() {
      assert(_handle != NULL);
      return (bool)DestroyWindow((HWND)_handle);
    }

    inline bool move(int x, int y) {
      assert(_handle != NULL);
      return (bool)SetWindowPos((HWND)_handle, 0, x, y, 0, 0, SWP_NOSIZE);
    }

    inline bool resize(int x, int y) {
      assert(_handle != NULL);
      return (bool)SetWindowPos((HWND)_handle, 0, 0, 0, x, y, SWP_NOMOVE);
    }

    inline eMsg *getMsg() {
      return &_msg;
    }

    inline void setMsg(eMsg *msg) {
      _msg = *msg;
    }

    inline void setMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
      _msg.hWnd = hWnd;
      _msg.msg = msg;
      _msg.wParam = wParam;
      _msg.lParam = lParam;
    }

    inline void setReturnCode(LRESULT code, bool returnLock = false) {
      if (!_returnLockCode) {
        _returnCode = code;
        _returnCodeSet = true;
      }
      if (!returnLock) {
        _returnLockCode = true;
      }
    }

    inline LPARAM getReturnCode() {
      return _returnCode;
    }

    inline void setStaticValue(int id, LPARAM code) {
      _staticValues[id] = code;
    }

    inline LPARAM process(eMsg *msg) {
      clear();
      setMsg(msg);

      if (_staticValues.find(msg->msg) != _staticValues.end()) {
        setReturnCode(_staticValues[msg->msg]);
      }
      notifyObservers(msg->msg); 

      return getReturnCode();
    }

    inline bool isReturnCodeSet() {
      return _returnCodeSet;
    }

    inline void clear() {
      _returnCode = NULL;
      _returnLockCode = false;
      _returnCodeSet = false;
    }

    inline bool isObserved(int id) {
      if (_observers.find(id) != _observers.end()) {
        return !_observers[id]->signal.empty();
      }
      return false;
    }

    inline void notifyObservers(int id) {
      if (!isObserved(id)) {
        return;
      }
      _observers[id]->signal(&_msg);
    }

    inline bool registerObserver(int id, fOnMessage f, int priority, signals::connect_position pos, std::string name, bool overwrite) {
      if (f.empty()) {
        return false;
      }
      if (_observers.find(id) == _observers.end()) {
        _observers[id] = new sObserver;
      }
      if (!name.size()) {
        char buff[12]={0};
        name = "unnamed.";
        name.append(itoa(_observers[id]->connections.size(), buff, 10));
      }
      if (_observers[id]->connections.find(name) != _observers[id]->connections.end()) {
        if (overwrite) {
          _observers[id]->connections[name].disconnect();
        } else {
          return false;
        }
      }
      return (_observers[id]->connections[name] = _observers[id]->signal.connect(priority, f, pos)).connected();
    }

  public:
    static HINSTANCE instance;

  private:
    static __w64 long _sMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
    __w64 long _msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  protected:
    static inline bool _isMsgProc(HWND hWnd) {
      if (_msgProcs.find(hWnd) != _msgProcs.end()) {
         return !_msgProcs[hWnd]->signal.empty();
      }
      return false;
    }

    static tMsgProcs _msgProcs;

    tObservers _observers;
    tStaticValues _staticValues;
    eMsg _msg;

    LRESULT _returnCode;
    bool _returnCodeSet;
    bool _returnLockCode;

    char *_className;
    HANDLE _handle;
  };
};

#pragma warning(pop) 

#endif /*__EASYWIN_WINDOW_H__*/