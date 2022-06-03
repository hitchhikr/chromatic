// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2010 Franck Charlet.
// All rights reserved.
//
// This file is copyright (C) bjarke viksoe
// (modified/debugged version by Franck Charlet)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// -----------------------------------------------------------------------
// WADock.h: Implementation of the docking windows class
// -----------------------------------------------------------------------

#ifndef _WADOCK_H_
#define _WADOCK_H_

#include <atlbase.h>
#include "../sys/include/atlapp.h"
extern HINSTANCE ApphInstance;
extern CAppModule _Module;
#include <atlwin.h>
#include "../sys/include/atlframe.h"
#include "../sys/include/atlctrls.h"
#include "../sys/include/atldlgs.h"
#include "../sys/include/atlctrlw.h"

// Dock positions
#define DOCKINGBOX_LEFT 0
#define DOCKINGBOX_RIGHT 1
#define DOCKINGBOX_TOP 2
#define DOCKINGBOX_BOTTOM 3
#define DOCKINGBOX_FLOATING 4

#define DOCKINGBOX_HIDDEN 6
#define DOCKINGBOX_LASTKNOWN 7

// Docking position helpers
inline bool IsDockedVertically(short Side)
{
    return (Side == DOCKINGBOX_LEFT) || (Side == DOCKINGBOX_RIGHT);
};
inline bool IsDocked(short Side)
{
    return (Side == DOCKINGBOX_TOP) ||
           (Side == DOCKINGBOX_BOTTOM) ||
           (Side == DOCKINGBOX_LEFT) ||
           (Side == DOCKINGBOX_RIGHT);
};

#define DOCK_INFO_CHILD 0x1000

// Minimum size of docking pane
#define MIN_DOCKPANE_SIZE 12
// Default width/height of docking pane
#define DEFAULT_DOCKPANE_SIZE 94
// Default size of floating window rectangle
#define DEFAULT_FLOAT_SIZE_X 120
#define DEFAULT_FLOAT_SIZE_Y 120
// The splitter size in pixels
#define DEFAULT_SPLITTER_SIZE 4

#define DOCKINGBOX_MSG_CLOSE WM_USER + 300
#define DOCKINGBOX_MSG_QUERY_STATE WM_USER + 301
#define WM_DOCK_CHANGELAYOUT WM_USER + 302
#define DOCKINGBOX_MSG_CLOSE_INTERNAL WM_USER + 303
#define WM_DOCK_QUERYRECT WM_USER + 350
#define WM_DOCK_QUERYTRACK WM_USER + 351
#define WM_DOCK_UNDOCK WM_USER + 352
#define WM_DOCK_UNFLOAT WM_USER + 353
#define WM_DOCK_DOCK WM_USER + 354
#define WM_DOCK_FLOATING WM_USER + 355
#define WM_DOCK_UPDATELAYOUT WM_USER + 356
#define WM_DOCK_REPOSITIONWINDOW WM_USER + 357
#define WM_DOCK_SETSPLITTER WM_USER + 358

class CDockingPaneChildWindow;
class CFloatingWindow;

struct DOCKCONTEXT
{
    HWND hwndDocked;   // The docked pane
    HWND hwndFloated;  // The floating pane
    HWND hwndChild;    // The view window
    HWND hwndOrigPrnt; // The original parent window
    short Side;        // Dock state
    short LastSide;    // Last dock state
    RECT rcWindow;     // Preferred window size
    SIZE sizeFloat;    // Last window size (floating)
    SIZE sizeMax;      // Used to correct a bug during windows creation
    HWND hwndRoot;     // Main dock window
};

typedef CSimpleValArray <DOCKCONTEXT *> CDockMap;

struct TRACKINFO
{
    HWND hWnd;
    DOCKCONTEXT* pCtx;
    POINT ptPos;
    POINT ptStart;
    RECT rc;
    short Side;
};

///////////////////////////////////////////////////////
// CSplitterBar

template<class T>
class CSplitterBar
{
public:
    LONG m_cxySplitter;
    bool m_bTracking;
    bool m_bDragging;
    static HCURSOR s_hVertCursor;
    static HCURSOR s_hHorizCursor;

    CDCHandle m_dc;
    POINT m_ptStartDragPoint;
    POINT m_ptEndDragPoint;
    POINT m_ptDeltaDragPoint;
    RECT m_rcTracker;
    SIZE m_sizeTracker;
    RECT m_rcTrackerBounds;   

    CSplitterBar() : m_bTracking(false), m_bDragging(false)
    {
        if( s_hVertCursor == NULL )
        {
            ::EnterCriticalSection(&_Module.m_csStaticDataInit);
            s_hVertCursor = ::LoadCursor(NULL, IDC_SIZENS);
            s_hHorizCursor = ::LoadCursor(NULL, IDC_SIZEWE);
            ::LeaveCriticalSection(&_Module.m_csStaticDataInit);
        }
        m_sizeTracker.cx = ::GetSystemMetrics(SM_CXFRAME);
        m_sizeTracker.cy = ::GetSystemMetrics(SM_CYFRAME);
        m_cxySplitter = DEFAULT_SPLITTER_SIZE;
    }

    LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        T *pT = static_cast <T *>(this);
        DWORD dwPos = ::GetMessagePos();
        POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
        pT->ScreenToClient(&ptPos);
        if( ::PtInRect(&pT->m_rcSplitter, ptPos) ) return 1;
        bHandled = FALSE;
        return 0;
    }

    void DrawGhostBar()
    {
        RECT rect = m_rcTracker;
        if(!::IsRectEmpty(&rect))
        {
            // Invert the brush pattern (looks just like frame window sizing)
            CBrush brush = CDCHandle::GetHalftoneBrush();
            if(brush.m_hBrush != NULL)
            {
                CBrushHandle brushOld = m_dc.SelectBrush(brush);
                m_dc.PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
                m_dc.SelectBrush(brushOld);
            }
        }
    }

    void DrawDragBar()
    {
        RECT rect = m_rcTracker;
        if(!::IsRectEmpty(&rect))
        {
            // Invert the brush pattern (looks just like frame window sizing)
            CBrush brush = CDCHandle::GetHalftoneBrush();
            if(brush.m_hBrush != NULL)
            {
                CBrushHandle brushOld = m_dc.SelectBrush(brush);
                m_dc.PatBlt(rect.left + m_sizeTracker.cx, rect.top, rect.right - rect.left - (m_sizeTracker.cx * 2), m_sizeTracker.cy, PATINVERT);
                m_dc.PatBlt(rect.left, rect.bottom - m_sizeTracker.cy, rect.right - rect.left, m_sizeTracker.cy, PATINVERT);
                m_dc.PatBlt(rect.left, rect.top, m_sizeTracker.cx, rect.bottom - rect.top - m_sizeTracker.cy, PATINVERT);
                m_dc.PatBlt(rect.right - m_sizeTracker.cx, rect.top, m_sizeTracker.cx, rect.bottom - rect.top - m_sizeTracker.cy, PATINVERT);
                m_dc.SelectBrush(brushOld);
            }
        }
    }

    void DrawSplitterBar(CDCHandle dc, bool bVertical, RECT &rect)
    {
        if(::IsRectEmpty(&rect)) return;
        dc.FillRect(&rect, ::GetSysColorBrush(COLOR_3DFACE));
    }

    bool PtInSplitter(POINT &pt, short Side, RECT &rcSplitter)
    {
        if(!IsDocked(Side)) return false;
        if(m_bTracking) return false;
        if(!::PtInRect(&rcSplitter, pt)) return false;
        return true;
    }

    // Track loop
    bool Track(bool bDragging)
    {
        T *pT = static_cast<T *>(this);
        StartTracking(bDragging);
        // Get messages until capture lost or cancelled/accepted
        while(::GetCapture() == pT->m_hWnd)
        {
            MSG msg;
            if(!::GetMessage(&msg, NULL, 0, 0))
            {
                ::PostQuitMessage(msg.wParam);
                break;
            }
            switch(msg.message)
            {
                case WM_LBUTTONUP:
                    if(m_bDragging) pT->OnEndDrag(); else pT->OnEndResize();
                    CancelTracking();
                    return true;
                case WM_MOUSEMOVE:
                    if(m_bDragging) pT->OnMove(msg.pt); else pT->OnStretch(msg.pt);
                    break;
                case WM_KEYUP:
                    if(m_bDragging) pT->OnKey((int) msg.wParam, false);
                    break;
                case WM_KEYDOWN:
                    if(m_bDragging) pT->OnKey((int) msg.wParam, true);
                    if(msg.wParam == VK_ESCAPE)
                    {
                        CancelTracking();
                        return false;
                    }
                    break;
                case WM_SYSKEYDOWN:
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                    CancelTracking();
                    return false;
                default:
                    // Just dispatch rest of the messages
                    ::DispatchMessage(&msg);
                    break;
            }
        }
        CancelTracking();
        return false;
    }

    void StartTracking(bool bDragging)
    {
        T *pT = static_cast <T *>(this);
        // Capture window
        m_bTracking = true;
        pT->SetCapture();
        // Make sure no updates are pending
        pT->RedrawWindow(NULL, NULL, RDW_ALLCHILDREN|RDW_UPDATENOW);
        // Lock Window update while dragging over desktop
        HWND hWnd = ::GetDesktopWindow();
        m_dc = ::GetDCEx(hWnd, NULL, ::LockWindowUpdate(hWnd) ? DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE : DCX_WINDOW | DCX_CACHE);
        // Draw the initial focus rect
        m_bDragging = bDragging;
        if( m_bDragging ) DrawDragBar(); else DrawGhostBar();
        return;
    }

    void CancelTracking()
    {
        if(!m_bTracking) return;
        // Erase the focus rect
        if(m_bDragging) DrawDragBar(); else DrawGhostBar();
        // Let window updates free
        ::LockWindowUpdate(NULL);
        HWND hWnd = ::GetDesktopWindow();
        if(!m_dc.IsNull()) ::ReleaseDC(hWnd, m_dc.Detach());
        // Release the capture
        ::ReleaseCapture();
        m_bTracking = false;
    }

    // Overridables
    void OnEndDrag() { };
    void OnEndResize() { };
    void OnKey(int nCode, bool bDown) { };
    void OnMove(POINT &pt) { };
    void OnStretch(POINT &pt) { };
};

template<class T> HCURSOR CSplitterBar<T>::s_hVertCursor = NULL;
template<class T> HCURSOR CSplitterBar<T>::s_hHorizCursor = NULL;

///////////////////////////////////////////////////////
// CFloatingWindow
typedef CWinTraits<WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU, WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE> CFloatWinTraits;

template< class T, class TBase = CWindow, class TWinTraits = CFloatWinTraits >
          class ATL_NO_VTABLE CFloatingWindowImpl :
		  public CWindowImpl<T, TBase, TWinTraits>,
#if _MSC_VER >= 1400
          public CSplitterBar<CFloatingWindowImpl<T>>
#else
          public CSplitterBar<CFloatingWindowImpl>
#endif
{

public:
    DECLARE_WND_CLASS_EX(NULL, CS_DBLCLKS, NULL)

    typedef CFloatingWindowImpl< T , TBase, TWinTraits > thisClass;
    BEGIN_MSG_MAP(thisClass)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
        MESSAGE_HANDLER(WM_MENUCHAR, OnMenuChar)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnMsgForward)
        MESSAGE_HANDLER(WM_DOCK_UPDATELAYOUT, OnSize)
        MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
        MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnLeftButtonDown)
        MESSAGE_HANDLER(WM_NCRBUTTONDOWN, OnRightButtonDown)
        MESSAGE_HANDLER(WM_NCLBUTTONDBLCLK, OnButtonDblClick)
    END_MSG_MAP()

    DOCKCONTEXT *m_pCtx;

    CFloatingWindowImpl(DOCKCONTEXT *pCtx) : m_pCtx(pCtx)
    {
    }

    HWND Create(HWND hWndParent, RECT &rcPos, LPCTSTR szWindowName = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0, UINT nID = 0, LPVOID lpCreateParam = NULL)
    {
        return CWindowImpl< T, TBase, TWinTraits >::Create(hWndParent, rcPos, szWindowName, dwStyle, dwExStyle, nID, lpCreateParam);
    }

    // Message handlers
    LRESULT OnNcActivate(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
    {
        return DefWindowProc(uMsg, IsWindowEnabled(), lParam);
    }

    LRESULT OnLeftButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
    {
        SetFocus();
        if(wParam == HTCAPTION)
        {
            // Get cursor point and start a tracking look
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            m_ptStartDragPoint = m_ptEndDragPoint = pt;
            GetWindowRect(&m_rcTracker);
            // Enter tracking loop
            bool res = Track(true);
            if(res)
            {
                // Determine if we landed over a docking pane or just moved around...
                TRACKINFO ti = { m_hWnd, m_pCtx, m_ptEndDragPoint.x, m_ptEndDragPoint.y, m_ptStartDragPoint.x, m_ptStartDragPoint.y };
                ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_QUERYTRACK, 0, (LPARAM) &ti);
                if( ti.Side == DOCKINGBOX_FLOATING )
                {
                    MoveWindow(&ti.rc, TRUE);
                }
                else
                {
                    ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_UNFLOAT, 0, (LPARAM) m_pCtx);
                    ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_DOCK, ti.Side, (LPARAM) m_pCtx);
                }
                return 0;
            }
            return 1;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnRightButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &bHandled)
    {
        // We don't particular like the right - mousebutton. We need to
        // disable the system menu because it causes problems...
        if( wParam == HTCAPTION ) return 1;
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnButtonDblClick(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &bHandled)
    {
        // Don't like double-clicks either. We need to disable
        // maximize on double-click for floating windows as well.
        if( wParam == HTCAPTION ) return 1;
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
    {
        return 1; // handled, no background painting needed
    }

    LRESULT OnMenuChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
    {
        // Forward a failed menu-key strike to main window.
        // Perhaps it will show the menu for this key.
        LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
        if(HIWORD(lRes) == MNC_IGNORE)
        {
            CWindow wndRoot = m_pCtx->hwndRoot;
            return ::SendMessage(wndRoot.GetTopLevelParent(), uMsg, wParam, lParam);
        }
        return lRes;
    }

    LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &bHandled)
    {
        switch(wParam & 0xFFF0)
        {
            case SC_CLOSE:
                // Use post message so it comes async
                ::SendMessage(m_pCtx->hwndChild, DOCKINGBOX_MSG_CLOSE, WM_DOCK_UNFLOAT, (LPARAM) m_pCtx);
                ::PostMessage(m_pCtx->hwndRoot, DOCKINGBOX_MSG_CLOSE_INTERNAL, WM_DOCK_UNFLOAT, (LPARAM) m_pCtx);
                return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
    {
        T *pT = static_cast <T *>(this);
        pT->UpdateLayout();
        return 0;
    }

    LRESULT OnMsgForward(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
    {
        // Forward to view
        return ::SendMessage(m_pCtx->hwndChild, uMsg, wParam, lParam);
    }

    // Track methods
    void OnMove(POINT &pt)
    {
        TRACKINFO ti = { m_hWnd, m_pCtx, pt.x, pt.y, m_ptStartDragPoint.x, m_ptStartDragPoint.y };
        ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_QUERYTRACK, 0, (LPARAM) &ti);
        if(!::EqualRect(&m_rcTracker, &ti.rc))
        {
            DrawDragBar();
            m_rcTracker = ti.rc;
            DrawDragBar();
        }
        m_ptEndDragPoint = pt;
    }

    // Overridables
    void UpdateLayout()
    {
        RECT rc;
        GetWindowRect(&rc);
        m_pCtx->sizeFloat.cx = rc.right - rc.left;
        m_pCtx->sizeFloat.cy = rc.bottom - rc.top;
        GetClientRect(&rc);
        ::SetWindowPos(m_pCtx->hwndChild, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
    }
};

class CFloatingWindow: public CFloatingWindowImpl<CFloatingWindow>
{
    public:
        DECLARE_WND_CLASS_EX(_T("WAFloatWindowClass"), CS_DBLCLKS, NULL)

        CFloatingWindow(DOCKCONTEXT* ctx) : CFloatingWindowImpl<CFloatingWindow>(ctx)
        {
        }
};

///////////////////////////////////////////////////////
// CDockingPaneChildWindow

template< class T, class TBase = CWindow, class TWinTraits = CControlWinTraits >
          class ATL_NO_VTABLE CDockingPaneChildWindowImpl : 
          public CWindowImpl<T, TBase, TWinTraits>,
#if _MSC_VER >= 1400
          public CSplitterBar<CDockingPaneChildWindowImpl<T>>
#else
          public CSplitterBar<CDockingPaneChildWindowImpl>
#endif
{
    public:
        DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, NULL)

        typedef CDockingPaneChildWindowImpl <T , TBase, TWinTraits> thisClass;
        BEGIN_MSG_MAP(thisClass)
            MESSAGE_HANDLER(WM_PAINT, OnPaint)
            MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
            MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
            MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
            MESSAGE_HANDLER(WM_LBUTTONDOWN, OnButtonDown)
            MESSAGE_HANDLER(WM_LBUTTONUP, OnButtonUp)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
            MESSAGE_HANDLER(WM_DOCK_UPDATELAYOUT, OnSize)
            MESSAGE_HANDLER(WM_DOCK_SETSPLITTER, OnSetSplitter)
        END_MSG_MAP()

        DOCKCONTEXT* m_pCtx;
        RECT m_rcChild;
        RECT m_rcSplitter;
        RECT m_rcGripper;
        RECT m_rcCloseButton;
        int m_cxyGripper;
        int m_cxyCloseButton;
        bool m_fCloseCapture;
        bool m_fCloseDown;

        CDockingPaneChildWindowImpl(DOCKCONTEXT *pCtx) : m_pCtx(pCtx), m_fCloseDown(false), m_fCloseCapture(false)
        {
            ::SetRectEmpty(&m_rcChild);
            ::SetRectEmpty(&m_rcGripper);
            ::SetRectEmpty(&m_rcSplitter);
            ::SetRectEmpty(&m_rcCloseButton);
            m_cxyGripper = 11;
            m_cxyCloseButton = 11;
        }

        LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            return 1; // handled, no background painting needed
        }

        LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            T *pT = static_cast<T *>(this);
            pT->UpdateLayout();
            return 0;
        }

        LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            T *pT = static_cast<T *>(this);
            CPaintDC dc(m_hWnd);
            RECT rc;
            GetClientRect(&rc);
            dc.IntersectClipRect(&rc);
            dc.FillRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));
            short Side = m_pCtx->Side;
            bool bVertical = IsDockedVertically(Side);
            if( m_cxySplitter > 0 ) pT->DrawSplitterBar((HDC) dc, bVertical, m_rcSplitter);
            pT->DrawGripperBar((HDC) dc, Side, m_rcGripper, m_rcCloseButton, m_fCloseDown);
            return 0;
        }

        LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
        {
            POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if( m_fCloseCapture ) {
                // Mouse capture because we're in a close-button loop
                bool fCloseDown = ::PtInRect(&m_rcCloseButton, ptPos) == TRUE;
                if( m_fCloseDown != fCloseDown ) {
                    m_fCloseDown = fCloseDown;
                    InvalidateRect(&m_rcCloseButton);
                }
            } else if( PtInSplitter(ptPos, m_pCtx->Side, m_rcSplitter) ) {
                bool bVertical = IsDockedVertically(m_pCtx->Side);
                ::SetCursor( bVertical ? s_hVertCursor : s_hHorizCursor);
                bHandled = FALSE;
            }
            return 0;
        }

        LRESULT OnButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if( ::PtInRect(&m_rcCloseButton, ptPos) )
            {
                // Clicked on close button? Start a mouse capture look because you
                // can still move the mouse away from the button...
                m_fCloseCapture = m_fCloseDown = true;
                InvalidateRect(&m_rcCloseButton);
                SetCapture();
            }
            else if( PtInSplitter(ptPos, m_pCtx->Side, m_rcSplitter) )
            {
                // Clicked on splitter. Start dragging then...
                ::ClientToScreen(m_hWnd, &ptPos);
                m_ptStartDragPoint = m_ptEndDragPoint = m_ptDeltaDragPoint = ptPos;
                //
                RECT rcWin;
                GetWindowRect(&rcWin);
                m_rcTracker = m_rcSplitter;
                ::OffsetRect(&m_rcTracker, rcWin.left, rcWin.top);
                //
                ::GetWindowRect(::GetParent(m_hWnd), &m_rcTrackerBounds);
                ::InflateRect(&m_rcTrackerBounds, -MIN_DOCKPANE_SIZE, -MIN_DOCKPANE_SIZE);
                // Enter tracking loop
                bool res = Track(false);
                if(res)
                {
                    if(IsDockedVertically(m_pCtx->Side))
                    {
                        m_pCtx->rcWindow.bottom += m_ptEndDragPoint.y - m_ptStartDragPoint.y;
                    }
                    else
                    {
                        m_pCtx->rcWindow.right += (m_ptEndDragPoint.x - m_ptStartDragPoint.x);
                    }
                    ::SendMessage(GetParent(), WM_DOCK_UPDATELAYOUT, 0, 0);
                }
            }
            else if(::PtInRect(&m_rcGripper, ptPos))
            {
                // Clicked on gripper? Allow user to move/drag the window out...
                ::ClientToScreen(m_hWnd, &ptPos);
                m_ptStartDragPoint = m_ptEndDragPoint = m_ptDeltaDragPoint = ptPos;
                GetWindowRect(&m_rcTracker);
                // Enter tracking loop
                bool res = Track(true);
                if(res)
                {
                    // Done dragging! Let's determine where the window went...
                    TRACKINFO ti = { m_hWnd, m_pCtx, m_ptEndDragPoint.x, m_ptEndDragPoint.y, m_ptStartDragPoint.x, m_ptStartDragPoint.y };
                    ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_QUERYTRACK, 0, (LPARAM)&ti);
                    if( ti.Side == m_pCtx->Side )
                    {
                        // Dragged window to same side. Move the window to top position...
                        RECT rc;
                        GetWindowRect(&rc);
                        if( !::PtInRect(&rc, m_ptEndDragPoint) ) ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_REPOSITIONWINDOW, 0, (LPARAM) m_pCtx);
                    }
                    else
                    {
                        // Move the window out in the open or to another pane...
                        ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_UNDOCK, 0, (LPARAM) m_pCtx);
                        if( ti.Side == DOCKINGBOX_FLOATING ) m_pCtx->rcWindow = ti.rc;
                        ::SendMessage(m_pCtx->hwndRoot, ti.Side == DOCKINGBOX_FLOATING ? WM_DOCK_FLOATING : WM_DOCK_DOCK, ti.Side, (LPARAM) m_pCtx);
                    }
                }
            }
            return 0;
        }

        LRESULT OnButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            if(m_fCloseCapture)
            {
                // We're in a mouse capture loop. That means we're trying to figure
                // out if the close button is being hit...
                m_fCloseCapture = m_fCloseDown = false;
                ::ReleaseCapture();
                InvalidateRect(&m_rcCloseButton);
                POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                if(::PtInRect(&m_rcCloseButton, ptPos))
                {
                    ::PostMessage(m_pCtx->hwndRoot, DOCKINGBOX_MSG_CLOSE_INTERNAL, WM_DOCK_UNDOCK, (LPARAM) m_pCtx);
                }
                if(::IsWindow(m_pCtx->hwndChild))
                {
                    POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                    if(::PtInRect(&m_rcCloseButton, ptPos))
                    {
                        SendMessage(m_pCtx->hwndChild, (UINT) DOCKINGBOX_MSG_CLOSE, 0, (LPARAM) 0);
                    }
                }
            }
            return 0;
        }

        // Track methods
        void OnMove(POINT &pt)
        {
            TRACKINFO ti = { m_hWnd, m_pCtx, pt.x, pt.y, m_ptStartDragPoint.x, m_ptStartDragPoint.y };
            ::SendMessage(m_pCtx->hwndRoot, WM_DOCK_QUERYTRACK, 0, (LPARAM) &ti);
            if(!::EqualRect(&m_rcTracker, &ti.rc))
            {
                DrawDragBar();
                m_rcTracker = ti.rc;
                DrawDragBar();
            }
            m_ptDeltaDragPoint = pt;
        }

        void OnStretch(POINT &pt)
        {
            DrawGhostBar();
            if(IsDockedVertically(m_pCtx->Side))
            {
                int nOffset = pt.y - m_ptDeltaDragPoint.y;
                if( m_rcTracker.top + nOffset <= m_rcTrackerBounds.top ) nOffset = m_rcTrackerBounds.top - m_rcTracker.top;
                if( m_rcTracker.bottom + nOffset >= m_rcTrackerBounds.bottom ) nOffset = m_rcTrackerBounds.bottom - m_rcTracker.bottom;
                ::OffsetRect(&m_rcTracker, 0, nOffset);
                m_ptDeltaDragPoint.y += nOffset;
            }
            else
            {
                int nOffset = pt.x - m_ptDeltaDragPoint.x;
                if( m_rcTracker.left + nOffset <= m_rcTrackerBounds.left ) nOffset = m_rcTrackerBounds.left - m_rcTracker.left;
                if( m_rcTracker.right + nOffset >= m_rcTrackerBounds.right ) nOffset = m_rcTrackerBounds.right - m_rcTracker.right;
                ::OffsetRect(&m_rcTracker, nOffset, 0);
                m_ptDeltaDragPoint.x += nOffset;
            }
            DrawGhostBar();
        }

        void OnEndResize() { m_ptEndDragPoint = m_ptDeltaDragPoint; }

        void OnEndDrag() { m_ptEndDragPoint = m_ptDeltaDragPoint; }

        LRESULT OnSetSplitter(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            m_cxySplitter = wParam;
            return 0;
        }

        // Overridables
        void UpdateLayout()
        {
            const int CHILD_GAP = 2;
            bool bVertical = IsDockedVertically(m_pCtx->Side);

            // Reposition splitter and gripper bars
            RECT rect;
            RECT rectBound;
            RECT rectParent;
            GetClientRect(&rect);
            GetWindowRect(&rectBound);
            ::GetWindowRect(m_pCtx->hwndRoot, &rectParent);
            if(bVertical)
            {
                int nGap = 0;
                nGap = CHILD_GAP + m_cxyCloseButton;
                m_rcCloseButton.left = rect.right - nGap;
                m_rcCloseButton.top = rect.top + 1 + (m_cxyGripper - m_cxyCloseButton) / 2;
                m_rcCloseButton.right = m_rcCloseButton.left + m_cxyCloseButton;
                m_rcCloseButton.bottom = m_rcCloseButton.top + m_cxyCloseButton;
                ::SetRect(&m_rcGripper, rect.left, rect.top, rect.right-nGap, rect.top + m_cxyGripper);
                ::SetRect(&m_rcSplitter, rect.left, rect.bottom - m_cxySplitter, rect.right, rect.bottom);
                rect.top += m_cxyGripper;
                rect.bottom -= m_cxySplitter;
            }
            else
            {
                int nGap = 0;
                nGap = CHILD_GAP + m_cxyCloseButton;
                m_rcCloseButton.left = rect.left + 1;
                m_rcCloseButton.top = rect.top + 2 + (m_cxyGripper - m_cxyCloseButton) / 2;
                m_rcCloseButton.right = m_rcCloseButton.left + m_cxyCloseButton;
                m_rcCloseButton.bottom = m_rcCloseButton.top + m_cxyCloseButton;
                ::SetRect(&m_rcGripper, rect.left, rect.top + nGap, rect.left + m_cxyGripper, rect.bottom);
                ::SetRect(&m_rcSplitter, rect.right - m_cxySplitter, rect.top, rect.right, rect.bottom);
                rect.left += m_cxyGripper;
                rect.right -= m_cxySplitter;
            }
            // Calculate the client window area
            ::InflateRect(&rect, -CHILD_GAP, -CHILD_GAP);
            m_rcChild = rect;
            ::SetWindowPos(m_pCtx->hwndChild, NULL, m_rcChild.left, m_rcChild.top, m_rcChild.right - m_rcChild.left, m_rcChild.bottom - m_rcChild.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }

        void DrawGripperBar(CDCHandle dc, short Side, const RECT &rcBar, RECT &rcCloseButton, bool bCloseDown)
        {
            if(::IsRectEmpty(&rcBar)) return;
            const int INSET = 3;
            const int LINE_GAP = 4;
            bool bVertical = IsDockedVertically(Side);
            RECT rcLine;
            if(bVertical)
            {
                ::SetRect(&rcLine, rcBar.left+INSET, rcBar.top+3, rcBar.right-INSET, rcBar.top+6);
            }
            else
            {
                ::SetRect(&rcLine, rcBar.left+3, rcBar.top+INSET, rcBar.left+6, rcBar.bottom-INSET);
            }
            dc.Draw3dRect(&rcLine, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
    
            ::OffsetRect(&rcLine, bVertical ? 0 : LINE_GAP, bVertical ? LINE_GAP : 0);
            dc.Draw3dRect(&rcLine, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
            if(!::IsRectEmpty(&rcCloseButton))
            {
                UINT uFlags = DFCS_CAPTIONCLOSE;
                if(bCloseDown) uFlags |= DFCS_PUSHED;
                dc.DrawFrameControl(&rcCloseButton, DFC_CAPTION, uFlags);
            }
        }
};

class CDockingPaneChildWindow : public CDockingPaneChildWindowImpl<CDockingPaneChildWindow>
{
    public:
        DECLARE_WND_CLASS_EX(_T("WADockWindowChildClass"), CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, NULL)

        CDockingPaneChildWindow(DOCKCONTEXT* ctx) : CDockingPaneChildWindowImpl<CDockingPaneChildWindow>(ctx) 
        { 
        }
};

///////////////////////////////////////////////////////
// CDockingPaneWindow

template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits >
          class ATL_NO_VTABLE CDockingPaneWindowImpl : public CWindowImpl< T, TBase, TWinTraits >,
#if _MSC_VER >= 1400
          public CSplitterBar<CDockingPaneWindowImpl<T>>
#else
          public CSplitterBar<CDockingPaneWindowImpl>
#endif
{

    public:
        DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, COLOR_WINDOW)

        typedef CDockingPaneWindowImpl< T , TBase, TWinTraits > thisClass;
        BEGIN_MSG_MAP(thisClass)
            MESSAGE_HANDLER(WM_PAINT, OnPaint)
            MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
            MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
            MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
            MESSAGE_HANDLER(WM_LBUTTONDOWN, OnButtonDown)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
            MESSAGE_HANDLER(WM_DOCK_UPDATELAYOUT, OnRecalcSpace)
        END_MSG_MAP()

        CDockMap m_map;       // Panels docking in this pane
                              // Pointers are owned by the parent window
        int m_cy;             // Current size of panel
        int m_cyOld;          // Last size when panel was visible
        short m_Side;         // The side the panel is shown the
        RECT m_rcSplitter;    // Rectangle of the splitter

        CDockingPaneWindowImpl() : m_cy(0), m_cyOld(DEFAULT_DOCKPANE_SIZE)
        {
            ::SetRectEmpty(&m_rcSplitter);
        }

        // Implementation

        void DockWindow(DOCKCONTEXT *ctx, int RequestedSize)
        {
            if(m_map.GetSize() == 0)
            {
                if(RequestedSize) m_cy = RequestedSize;
                else m_cy = m_cyOld;
                ShowWindow(SW_SHOWNOACTIVATE);
            }
            ctx->Side = m_Side;
            ctx->LastSide = m_Side;
            ::SetParent(ctx->hwndDocked, m_hWnd);
            ::SetParent(ctx->hwndChild, ctx->hwndDocked);
            ::ShowWindow(ctx->hwndDocked, SW_SHOWNOACTIVATE);
            m_map.Add(ctx);

            bool bVertical = IsDockedVertically(m_Side);
            RECT RootRect;
            ::GetWindowRect(m_hWnd, &RootRect);
            if(::IsRectEmpty(&RootRect))
            {
                RootRect = ctx->rcWindow;
                if(m_map.GetSize() > 1)
                {
                    RootRect.left = 0;
                    RootRect.top = 0;
                    RootRect.right = ctx->sizeMax.cx;
                    RootRect.bottom = ctx->sizeMax.cy;
                }
            }
            int nActualHeight = (bVertical ? RootRect.bottom - RootRect.top : RootRect.right - RootRect.left);
            if(nActualHeight < 10) nActualHeight = 30;
            nActualHeight /= m_map.GetSize();
            for(int i = 0; i < m_map.GetSize(); i++)
            {
                if(bVertical)
                {
                    m_map[i]->rcWindow.top = 0;
                    m_map[i]->rcWindow.bottom = nActualHeight;
                }
                else
                {
                    m_map[i]->rcWindow.left = 0;
                    m_map[i]->rcWindow.right = nActualHeight;
                }
            }
            PostMessage(WM_DOCK_UPDATELAYOUT);
        }

        void UnDockWindow(DOCKCONTEXT *ctx)
        {
			int i;

			for(i = 0; i < m_map.GetSize(); i++)
            {
                if(m_map[i] == ctx)
                {
                    m_map.RemoveAt(i);
                    ::ShowWindow(ctx->hwndDocked, SW_HIDE);
                    break;
                }
            }
            if(m_map.GetSize() == 0)
            {
                m_cyOld = m_cy; // remember pane size to until next time
                m_cy = 0;
                ShowWindow(SW_HIDE);
            }
            else
            {
                SendMessage(WM_DOCK_UPDATELAYOUT);
            }
        }

        void RepositionWindow(DOCKCONTEXT *ctx, int iPos)
        {
			int i;

			if(iPos >= m_map.GetSize()) return;
            for(i = 0; i < m_map.GetSize(); i++)
            {
                if(m_map[i] == ctx)
                {
                    DOCKCONTEXT *pCtx = m_map[iPos];
                    m_map.SetAtIndex(iPos, ctx);
                    m_map.SetAtIndex(i, pCtx);
                    break;
                }
            }
            for(i = 0; i < (m_map.GetSize() - 1); i++)
            {
                ::SendMessage(m_map[i]->hwndDocked, WM_DOCK_SETSPLITTER, DEFAULT_SPLITTER_SIZE, 0L);
            }
            // The last panel does not have a splitter
            ::SendMessage(m_map[i]->hwndDocked, WM_DOCK_SETSPLITTER, 0, 0L);
            SendMessage(WM_DOCK_UPDATELAYOUT);
            Invalidate();
        }

        // Message handlers
        LRESULT OnRecalcSpace(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
        {
            if(m_map.GetSize() == 0) return 0;
            for(int i = 0; i < m_map.GetSize(); i++)
            {
                ::SendMessage(m_map[i]->hwndDocked, WM_DOCK_UPDATELAYOUT, 0, 0);
            }
            OnSize(0, 0, 0, bHandled);
            return 0;
        }

        LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            CPaintDC dc(m_hWnd);
            // Draw splitter along the pane side
            T *pT = static_cast <T *> (this);
            pT->DrawSplitterBar((HDC) dc, !IsDockedVertically(m_Side), m_rcSplitter);
            return 0;
        }

        LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            return 1; // handled, no background painting needed
        }

        LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
        {
            bHandled = FALSE;
            if( m_map.GetSize() == 0 ) return 1;
            T *pT = static_cast <T *> (this);
            pT->UpdateLayout();
            HDWP hdwp = BeginDeferWindowPos(m_map.GetSize());
            for(int i = 0; i < m_map.GetSize(); i++)
            {
                RECT &rc = m_map[i]->rcWindow;
                ::DeferWindowPos(hdwp, m_map[i]->hwndDocked, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            EndDeferWindowPos(hdwp);
            return 1;
        }

        LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
        {
            POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if(PtInSplitter(ptPos, m_Side, m_rcSplitter))
            {
                bool bVertical = !IsDockedVertically(m_Side);
                ::SetCursor( bVertical ? s_hVertCursor : s_hHorizCursor);
                bHandled = FALSE;
            }
            return 0;
        }

        LRESULT OnButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if(PtInSplitter(ptPos, m_Side, m_rcSplitter))
            {
                // Click on the spiltter? Determine how har we can drag the pane and
                // start tracking...
                ::ClientToScreen(m_hWnd, &ptPos);
                m_ptStartDragPoint = m_ptEndDragPoint = m_ptDeltaDragPoint = ptPos;
                RECT rcWin;
                GetWindowRect(&rcWin);
                m_rcTracker = m_rcSplitter;
                ::OffsetRect(&m_rcTracker, rcWin.left, rcWin.top);
                ::GetWindowRect(GetParent(), &m_rcTrackerBounds);
                RECT rcLimit;
                ::SendMessage(GetParent(), WM_DOCK_QUERYRECT, DOCK_INFO_CHILD, (LPARAM)&rcLimit);
                switch(m_Side)
                {
                    case DOCKINGBOX_LEFT:   m_rcTrackerBounds.right = rcLimit.right; break;
                    case DOCKINGBOX_RIGHT:  m_rcTrackerBounds.left = rcLimit.left; break;
                    case DOCKINGBOX_TOP:    m_rcTrackerBounds.bottom = rcLimit.bottom; break;
                    case DOCKINGBOX_BOTTOM: m_rcTrackerBounds.top = rcLimit.top; break;
                }
                // The actual dragging area is slightly smaller than the client area
                ::InflateRect(&m_rcTrackerBounds, -MIN_DOCKPANE_SIZE, -MIN_DOCKPANE_SIZE);
                // Enter tracking loop
                bool res = Track(false);
                if(res)
                {
                    int nOffset = ( !IsDockedVertically(m_Side) ? (m_ptEndDragPoint.y - m_ptStartDragPoint.y) : (m_ptEndDragPoint.x - m_ptStartDragPoint.x) );
                    if(m_Side == DOCKINGBOX_RIGHT || m_Side == DOCKINGBOX_BOTTOM) nOffset = -nOffset;
                    m_cy += nOffset;
                    ::SendMessage(GetParent(), WM_DOCK_UPDATELAYOUT, 0,0);
                }
            }
            return 0;
        }

        // Track methods
        void OnStretch(POINT &pt)
        {
            DrawGhostBar();
            if(!IsDockedVertically(m_Side))
            {
                int nOffset = pt.y - m_ptDeltaDragPoint.y;
                if( m_rcTracker.top + nOffset <= m_rcTrackerBounds.top ) nOffset = m_rcTrackerBounds.top - m_rcTracker.top;
                if( m_rcTracker.bottom + nOffset >= m_rcTrackerBounds.bottom ) nOffset = m_rcTrackerBounds.bottom - m_rcTracker.bottom;
                ::OffsetRect(&m_rcTracker, 0, nOffset);
                m_ptDeltaDragPoint.y += nOffset;
            }
            else
            {
                int nOffset = pt.x - m_ptDeltaDragPoint.x;
                if( m_rcTracker.left + nOffset <= m_rcTrackerBounds.left ) nOffset = m_rcTrackerBounds.left - m_rcTracker.left;
                if( m_rcTracker.right + nOffset >= m_rcTrackerBounds.right ) nOffset = m_rcTrackerBounds.right - m_rcTracker.right;
                ::OffsetRect(&m_rcTracker, nOffset, 0);
                m_ptDeltaDragPoint.x += nOffset;
            }
            DrawGhostBar();
        }

        void OnEndResize()
        {
            m_ptEndDragPoint = m_ptDeltaDragPoint;
        }

        // Overridables
        void UpdateLayout()
        {
            int i;

			if(m_map.GetSize() == 0) return;
            if(!IsWindowVisible()) return;

            int nPanes = m_map.GetSize();
            bool bVertical = IsDockedVertically(m_Side);

            // Place side splitter for this docking area
            RECT rect; 
            GetClientRect(&rect);
            switch(m_Side)
            {
                case DOCKINGBOX_LEFT:
                    ::SetRect(&m_rcSplitter, rect.right - m_cxySplitter, rect.top, rect.right, rect.bottom);
                    rect.right -= m_cxySplitter;
                    break;
                case DOCKINGBOX_TOP:
                    ::SetRect(&m_rcSplitter, rect.left, rect.bottom - m_cxySplitter, rect.right, rect.bottom);
                    rect.bottom -= m_cxySplitter;
                    break;
                case DOCKINGBOX_RIGHT:
                    ::SetRect(&m_rcSplitter, rect.left, rect.top, rect.left + m_cxySplitter, rect.bottom);
                    rect.left += m_cxySplitter;
                    break;
                case DOCKINGBOX_BOTTOM:
                    ::SetRect(&m_rcSplitter, rect.left, rect.top, rect.right, rect.top + m_cxySplitter);
                    rect.top += m_cxySplitter;
                    break;
            }
    
            // Place splitters in each child panel (except in the last one)
            for(i = 0; i < (nPanes - 1); i++)
            {
                ::SendMessage(m_map[i]->hwndDocked, WM_DOCK_SETSPLITTER, DEFAULT_SPLITTER_SIZE, 0L);
            }
            // The last panel does not have a splitter
            ::SendMessage(m_map[i]->hwndDocked, WM_DOCK_SETSPLITTER, 0, 0L);

            // Get actual height of all panels
            int nActualHeight = 0;
            for(i = 0; i < nPanes; i++)
            {
                const RECT &rc = m_map[i]->rcWindow;
                int iPaneHeight = (bVertical ? rc.bottom - rc.top : rc.right - rc.left);
                if( iPaneHeight < 10 ) iPaneHeight = 30;
                nActualHeight += iPaneHeight;
            }

            // Get height of docking area
            int nTop, nHeight;
            //int max_top = 0;
            if(bVertical)
            {
                nTop = rect.top;
                nHeight = rect.bottom - rect.top;
            }
            else
            {
                nTop = rect.left;
                nHeight = rect.right - rect.left;
            }

            // Distribute the difference among panels
            int max_panes = nPanes - 1;
            for(i = 0; i < nPanes; i++)
            {
                const RECT &rc = m_map[i]->rcWindow;
                int nSize = (bVertical ? rc.bottom - rc.top : rc.right - rc.left);
                int max_pos = (nHeight - nTop - (max_panes * (MIN_DOCKPANE_SIZE + DEFAULT_SPLITTER_SIZE)));
                if(nSize >= max_pos) nSize = max_pos;
                if(nSize < (MIN_DOCKPANE_SIZE + DEFAULT_SPLITTER_SIZE)) nSize = (MIN_DOCKPANE_SIZE + DEFAULT_SPLITTER_SIZE);
                if(bVertical)
                {
                    ::SetRect(&m_map[i]->rcWindow, rect.left, nTop, rect.right, nTop + nSize);
                }
                else
                {
                    ::SetRect(&m_map[i]->rcWindow, nTop, rect.top, nTop + nSize, rect.bottom);
                }
                nTop += nSize;
                max_panes--;
            }
            // Stretch the last window to the size of the docking window
            RECT &toto = m_map[nPanes - 1]->rcWindow;
            (bVertical ? m_map[nPanes - 1]->rcWindow.bottom : m_map[nPanes - 1]->rcWindow.right ) = nHeight;
        }
};

class CDockingPaneWindow : public CDockingPaneWindowImpl<CDockingPaneWindow>
{
    public:
        DECLARE_WND_CLASS_EX(_T("WADockPanelClass"),
                             CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_WINDOW)
};

///////////////////////////////////////////////////////
// CDockingWindow

template< class T, class TPaneWindow = CDockingPaneWindow,
          class TDockWindow = CDockingPaneChildWindow,
          class TFloatWindow = CFloatingWindow,
          class TBase = CWindow, class TWinTraits = CControlWinTraits >
          class ATL_NO_VTABLE CDockingWindowImpl : 
          public CWindowImpl< T, TBase, TWinTraits >
{

    public:
        DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, NULL)

        typedef CDockingWindowImpl <T , TBase, TWinTraits> thisClass;
        BEGIN_MSG_MAP(thisClass)
            MESSAGE_HANDLER(WM_CREATE, OnCreate)
            MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
            MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
            MESSAGE_HANDLER(WM_DOCK_QUERYRECT, OnQueryRect)
            MESSAGE_HANDLER(WM_DOCK_QUERYTRACK, OnQueryTrack)
            MESSAGE_HANDLER(WM_DOCK_DOCK, OnDock)
            MESSAGE_HANDLER(WM_DOCK_FLOATING, OnFloat)
            MESSAGE_HANDLER(WM_DOCK_UNDOCK, OnUnDock)
            MESSAGE_HANDLER(WM_DOCK_UNFLOAT, OnUnFloat)
            MESSAGE_HANDLER(WM_DOCK_UPDATELAYOUT, OnSize)
            MESSAGE_HANDLER(WM_DOCK_REPOSITIONWINDOW, OnRepositionWindow)
            MESSAGE_HANDLER(DOCKINGBOX_MSG_CLOSE_INTERNAL, OnClientClose)
        END_MSG_MAP()

        TPaneWindow m_panes[4];  // The 4 panel windows (one for each side)
        CDockMap m_map;          // The master map of dockable windows
        HWND m_hwndClient;       // The client window contained in the center
        SIZE m_sizeBorder;       // Size of window borders

        // Operations
        void SetClient(HWND hWnd)
        {
            m_hwndClient = hWnd;
        }

        BOOL IsDockPane(HWND hWnd) const
        {
            for(int i = 0; i < m_map.GetSize(); i++)
                if(m_map[i]->hwndChild == hWnd) return TRUE;
            return FALSE;
        }

        void AddWindow(HWND hWnd, int MaxSizeX, int MaxSizeY,
                       int DefaultSizeX = DEFAULT_FLOAT_SIZE_X,
                       int DefaultSizeY = DEFAULT_FLOAT_SIZE_Y)
        {
            // Create docking context
            DOCKCONTEXT* ctx;
            ATLTRY(ctx = new DOCKCONTEXT);
            if(ctx == NULL) return;
            ctx->Side = DOCKINGBOX_HIDDEN;
            ctx->LastSide = DOCKINGBOX_LEFT;
            ctx->hwndChild = hWnd;
            ctx->hwndRoot = m_hWnd;
            ::SetRect(&ctx->rcWindow, 0, 0, MIN_DOCKPANE_SIZE, MIN_DOCKPANE_SIZE);
            ctx->sizeFloat.cx = DefaultSizeX;
            ctx->sizeFloat.cy = DefaultSizeY;
            ctx->sizeMax.cx = MaxSizeX;
            ctx->sizeMax.cy = MaxSizeY;
            ctx->hwndOrigPrnt = ::GetParent(hWnd);

            // Create docking child
            TDockWindow* wndDock;
            ATLTRY(wndDock = new TDockWindow(ctx));
            if(wndDock == NULL) return;
            wndDock->Create(m_hWnd, rcDefault, NULL);
            ctx->hwndDocked = *wndDock;

            // Create floating child
            TFloatWindow *wndFloat;
            TCHAR szCaption[128];    // max text length is 127 for floating caption
            ::GetWindowText(hWnd, szCaption, sizeof(szCaption)/sizeof(TCHAR));
            ATLTRY(wndFloat = new TFloatWindow(ctx));
            if(wndFloat == NULL) return;
            wndFloat->Create(m_hWnd, rcDefault, szCaption);
            ctx->hwndFloated = *wndFloat;

            ::SetParent(ctx->hwndChild, ctx->hwndDocked);

            // Add Context to master list
            m_map.Add(ctx);
        }

        BOOL RemoveWindow(HWND hWnd)
        {
            DOCKCONTEXT* pCtx = _GetContext(hWnd);
            if(pCtx == NULL) return FALSE;
            // Hide and destroy panel
            HideWindow(hWnd);
            // Remove context from map in case of repaint
            m_map.Remove(pCtx);
            ::SetParent(pCtx->hwndChild, pCtx->hwndOrigPrnt);
            if(::IsWindow(pCtx->hwndDocked)) ::DestroyWindow(pCtx->hwndDocked);
            if(::IsWindow(pCtx->hwndFloated)) ::DestroyWindow(pCtx->hwndFloated);
            // Destroy context. Allocated in AddWindow()...
            delete pCtx;
            return TRUE;
        }

        BOOL DockWindow(HWND hWnd, short Side, int iRequestedSize = 0)
        {
            T* pT = static_cast<T*>(this);
            DOCKCONTEXT* pCtx = _GetContext(hWnd);
            if(pCtx == NULL) return FALSE;
            ::ShowWindow(pCtx->hwndChild, SW_SHOWNOACTIVATE);
            if(Side == DOCKINGBOX_LASTKNOWN) Side = pCtx->LastSide;
            if(!IsDocked(Side)) return FALSE;
            return pT->_DockWindow(pCtx, Side, iRequestedSize);
        }

        BOOL FloatWindow(HWND hWnd, RECT &rc)
        {
            T *pT = static_cast <T *>(this);
            DOCKCONTEXT *pCtx = _GetContext(hWnd);
            if(pCtx == NULL) return FALSE;
            ::ShowWindow(pCtx->hwndChild, SW_SHOWNOACTIVATE);
            pCtx->rcWindow = rc;
            return pT->_FloatWindow(pCtx);
        }

        void SetWindow(HWND hWnd, short Side, RECT &rc, int iRequestedSize)
        {
            T *pT = static_cast <T *> (this);
            if(Side == DOCKINGBOX_FLOATING) pT->FloatWindow(hWnd, rc);
            else pT->DockWindow(hWnd, Side, iRequestedSize);
        }

        BOOL HideWindow(HWND hWnd)
        {
            T *pT = static_cast <T *> (this);
            DOCKCONTEXT *pCtx = _GetContext(hWnd);
            if(pCtx == NULL) return FALSE;
            ::ShowWindow(pCtx->hwndChild, SW_HIDE);
            if(pCtx->Side == DOCKINGBOX_FLOATING) return pT->_UnFloatWindow(pCtx);
            else if(IsDocked(pCtx->Side)) return pT->_UnDockWindow(pCtx);
            return TRUE;
        }

        int GetWindowState(HWND hWnd, RECT *rect = NULL) const
        {
            int DockState;
            const DOCKCONTEXT *pCtx = _GetContext(hWnd);
            if(pCtx == NULL) return(0);
            DockState = pCtx->Side;
            // Don't report hidden state
            if(DockState == DOCKINGBOX_HIDDEN) DockState = pCtx->LastSide;
            if(rect != NULL)
            {
                ::GetWindowRect(::GetParent(hWnd), rect);
                if(DockState != DOCKINGBOX_FLOATING)
                {
                    rect->right += DEFAULT_SPLITTER_SIZE;
                    rect->bottom += DEFAULT_SPLITTER_SIZE;
                }
            }
            return(DockState);
        }

        // Message handlers
        LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            for(short i = 0; i < 4; i++)
            {
                m_panes[i].m_Side = i;
                m_panes[i].Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE);
            }
            m_sizeBorder.cx = ::GetSystemMetrics(SM_CXEDGE);
            m_sizeBorder.cy = ::GetSystemMetrics(SM_CYEDGE);
            return 0;
        }

        LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            for(int i = 0; i < m_map.GetSize(); i++)
            {
                if(::IsWindow(m_map[i]->hwndDocked)) ::DestroyWindow(m_map[i]->hwndDocked);
                if(::IsWindow(m_map[i]->hwndFloated)) ::DestroyWindow(m_map[i]->hwndFloated);
                delete m_map[i];
            }
            m_map.RemoveAll();
            return 0;
        }

        LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            return 1; // handled, no background painting needed
        }

        LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
        {
            T *pT = static_cast <T *> (this);
            pT->UpdateLayout();
            return 0;
        }

        // Custom defined messages
        LRESULT OnQueryRect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
        {
            // wParam: Side
            // lParam: LPRECT
            LPRECT prc = (LPRECT) lParam;
            switch((short) wParam)
            {
                case DOCKINGBOX_LEFT:
                case DOCKINGBOX_TOP:
                case DOCKINGBOX_RIGHT:
                case DOCKINGBOX_BOTTOM:
                    ::GetWindowRect(m_panes[wParam], prc);
                    break;
                case DOCK_INFO_CHILD:
                    ::GetWindowRect(m_hwndClient, prc);
                    break;
                default:
                    break;
            }
            return 0;
        }

        LRESULT OnQueryTrack(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            // lParam: TRACKINFO*
            TRACKINFO *pTI = (TRACKINFO *) lParam;
            POINT &pt = pTI->ptPos;
            RECT rc;

            // But by default, we're floating in the skies
            ::GetWindowRect(pTI->hWnd, &rc);
            ::SetRect(&pTI->rc, rc.left, rc.top, rc.left + pTI->pCtx->sizeFloat.cx, rc.top + pTI->pCtx->sizeFloat.cy);
            ::OffsetRect(&pTI->rc, pt.x - pTI->ptStart.x, pt.y - pTI->ptStart.y);
            pTI->Side = DOCKINGBOX_FLOATING;

            // Pressing CTRL key gives default floating
            //if( ::GetKeyState(VK_CONTROL) < 0 ) return 0;
            // Are we perhaps hovering over the tracked window?
            ::GetWindowRect(pTI->hWnd, &rc);
            if(::PtInRect(&rc, pt))
            {
                pTI->rc = rc;
                ::OffsetRect(&pTI->rc, pt.x - pTI->ptStart.x, pt.y - pTI->ptStart.y);
                pTI->Side = pTI->pCtx->Side;
                return 0;
            }
            // Or is the point inside one of the other docking areas?
            for(short i = 0; i < 4; i++)
            {
                if(m_panes[i].m_cy == 0)
                {
                    // Simulate docking areas that are currently invisible
                    ::GetWindowRect(m_hWnd, &rc);
                    switch(m_panes[i].m_Side)
                    {
                        case DOCKINGBOX_LEFT:   rc.right = rc.left + DEFAULT_DOCKPANE_SIZE; break;
                        case DOCKINGBOX_RIGHT:  rc.left = rc.right - DEFAULT_DOCKPANE_SIZE; break;
                        case DOCKINGBOX_TOP:    rc.bottom = rc.top + DEFAULT_DOCKPANE_SIZE; break;
                        case DOCKINGBOX_BOTTOM: rc.top = rc.bottom - DEFAULT_DOCKPANE_SIZE; break;
                    }
                }
                else
                {
                    ::GetWindowRect(m_panes[i], &rc);
                }
                if(::PtInRect(&rc, pt))
                {
                    pTI->Side = i;
                    pTI->rc = rc;
                    return 0;
                }
            }
            return 0;
        }

        LRESULT OnDock(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
        {
            T *pT = static_cast <T *> (this);
            const DOCKCONTEXT *pCtx = (DOCKCONTEXT *) lParam;
            pT->_DockWindow((DOCKCONTEXT *) lParam, (short) wParam, 0);
            return 0;
        }

        LRESULT OnUnDock(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            T *pT = static_cast <T *> (this);
            pT->_UnDockWindow((DOCKCONTEXT *) lParam);
            return 0;
        }

        LRESULT OnFloat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            T *pT = static_cast <T *> (this);
            pT->_FloatWindow((DOCKCONTEXT *) lParam);
            return 0;
        }

        LRESULT OnUnFloat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
        {
            T *pT = static_cast <T *> (this);
            pT->_UnFloatWindow((DOCKCONTEXT *) lParam);
            return 0;
        }

        LRESULT OnClientClose(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
        {
            DOCKCONTEXT *pCtx = (DOCKCONTEXT *) lParam;
            // Make sure to bring focus back to this app!
            ::SetForegroundWindow(m_hWnd);
            SetFocus();
            // Hide or destroy view
            HWND hWnd = pCtx->hwndChild;
            // Commit suicide
            RemoveWindow(hWnd);
            return 0;
        }

        LRESULT OnRepositionWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
        {
            T *pT = static_cast <T *> (this);
            pT->_RepositionWindow((DOCKCONTEXT *) lParam, (int) wParam);
            return 0;
        }

        // Overridables
        void UpdateLayout()
        {
            RECT rect;
            GetClientRect(&rect);
            RECT rcClient = rect;

            if(m_panes[DOCKINGBOX_TOP].m_cy > 0)
            {
                ::SetWindowPos(m_panes[DOCKINGBOX_TOP], NULL,
                               rect.left, rect.top,
                               rect.right - rect.left,
                               m_panes[DOCKINGBOX_TOP].m_cy,
                               SWP_NOZORDER | SWP_NOACTIVATE);
                rcClient.top += m_panes[DOCKINGBOX_TOP].m_cy;
            }
            if(m_panes[DOCKINGBOX_LEFT].m_cy > 0)
            {
                ::SetWindowPos(m_panes[DOCKINGBOX_LEFT],
                                       NULL, rect.left,
                                       rcClient.top,
                                       m_panes[DOCKINGBOX_LEFT].m_cy,
                                       rect.bottom - rcClient.top,
                                       SWP_NOZORDER | SWP_NOACTIVATE);
                rcClient.left += m_panes[DOCKINGBOX_LEFT].m_cy;
            }
            if(m_panes[DOCKINGBOX_RIGHT].m_cy > 0)
            {
                int left = max((int) rect.right - m_panes[DOCKINGBOX_RIGHT].m_cy, (int) rcClient.left);
                ::SetWindowPos(m_panes[DOCKINGBOX_RIGHT], NULL,
                                       left, rcClient.top,
                                       m_panes[DOCKINGBOX_RIGHT].m_cy,
                                       rect.bottom - rcClient.top,
                                       SWP_NOZORDER | SWP_NOACTIVATE);
                rcClient.right -= m_panes[DOCKINGBOX_RIGHT].m_cy;
            }
            if(m_panes[DOCKINGBOX_BOTTOM].m_cy > 0)
            {
                int top = max((int) rcClient.bottom - m_panes[DOCKINGBOX_BOTTOM].m_cy, (int) rcClient.top);
                ::SetWindowPos(m_panes[DOCKINGBOX_BOTTOM], NULL, rcClient.left,
                               top, rcClient.right - rcClient.left,
                               m_panes[DOCKINGBOX_BOTTOM].m_cy,
                               SWP_NOZORDER | SWP_NOACTIVATE);
                rcClient.bottom -= m_panes[DOCKINGBOX_BOTTOM].m_cy;
            }

            if(::IsWindow(m_hwndClient))
            {
                // Adjust borders around docking panes
                DWORD dwExtStyle = (DWORD)::GetWindowLong(m_hwndClient, GWL_EXSTYLE);
                bool bClientEdge = ((dwExtStyle & WS_EX_CLIENTEDGE) != 0);
                if(bClientEdge)
                {
                    if(m_panes[DOCKINGBOX_TOP].m_cy > 0) rcClient.top -= m_sizeBorder.cy;
                    if(m_panes[DOCKINGBOX_LEFT].m_cy > 0) rcClient.left -= m_sizeBorder.cx;
                    if(m_panes[DOCKINGBOX_RIGHT].m_cy > 0) rcClient.right += m_sizeBorder.cx;
                    if(m_panes[DOCKINGBOX_BOTTOM].m_cy > 0) rcClient.bottom += m_sizeBorder.cy;
                }
                // Map client rectangle to windows's coord system
                ::MapWindowPoints(m_hWnd, ::GetParent(m_hwndClient), (LPPOINT)&rcClient, sizeof(rcClient)/sizeof(POINT));
                ::SetWindowPos(m_hwndClient, NULL,
                               rcClient.left, rcClient.top,
                               rcClient.right - rcClient.left,
                               rcClient.bottom - rcClient.top,
                               SWP_NOZORDER);
            }
        }

        BOOL _DockWindow(DOCKCONTEXT *ctx, short Side, int iRequestedSize)
        {
            if(!IsDocked(Side)) return FALSE;
            bool bVertical = IsDockedVertically(Side);
            // Make up a new panel size

            //if(CutSize) {
            //} else {
            //  if( iRequestedSize < 10 ) iRequestedSize = 0;
            //  if( iRequestedSize <= 0 ) {
            //      RECT rc;
            //      ::GetClientRect(m_panes[Side], &rc);
            //      iRequestedSize = bVertical ? rc.bottom - rc.top : rc.right - rc.left;
            //      if( m_panes[Side].m_map.GetSize() > 0 ) iRequestedSize /= m_panes[Side].m_map.GetSize();
            //  }
            //}
            // Set the new size of the pane (subject to readjustment)
            ::SetRectEmpty(&ctx->rcWindow);
            (bVertical ? ctx->rcWindow.bottom : ctx->rcWindow.right) = iRequestedSize;
            // Signal that we would like to retain this size after
            // first being laid out...
            // Dock
            m_panes[Side].DockWindow(ctx, iRequestedSize);
            PostMessage(WM_DOCK_UPDATELAYOUT);
            return TRUE;
        }

        BOOL _FloatWindow(DOCKCONTEXT *ctx)
        {
            if(ctx->Side != DOCKINGBOX_HIDDEN) return FALSE;
            ctx->Side = DOCKINGBOX_FLOATING;
            ::SetParent(ctx->hwndChild, ctx->hwndFloated);
            ::SetWindowPos(ctx->hwndFloated, NULL, ctx->rcWindow.left,
                           ctx->rcWindow.top, ctx->rcWindow.right - ctx->rcWindow.left,
                           ctx->rcWindow.bottom - ctx->rcWindow.top,
                           SWP_NOZORDER);
            ::SendMessage(ctx->hwndFloated, WM_DOCK_UPDATELAYOUT, 0, 0);
            ::ShowWindow(ctx->hwndFloated, SW_SHOWNORMAL);
            return TRUE;
        }

        BOOL _UnDockWindow(DOCKCONTEXT *ctx)
        {
            if(!IsDocked(ctx->Side)) return FALSE;
            m_panes[ctx->Side].UnDockWindow(ctx);
            ctx->Side = DOCKINGBOX_HIDDEN;
            SendMessage(WM_DOCK_UPDATELAYOUT);
            return TRUE;
        }

        BOOL _UnFloatWindow(DOCKCONTEXT *ctx)
        {
            if(ctx->Side != DOCKINGBOX_FLOATING) return FALSE;
            ::ShowWindow(ctx->hwndFloated, SW_HIDE);
            ctx->Side = DOCKINGBOX_HIDDEN;
            return TRUE;
        }

        BOOL _RepositionWindow(DOCKCONTEXT *ctx, int iPos)
        {
            if(!IsDocked(ctx->Side)) return FALSE;
            m_panes[ctx->Side].RepositionWindow(ctx,iPos);
            return TRUE;
        }

        DOCKCONTEXT *_GetContext(HWND hWnd) const
        {
            for(int i = 0; i < m_map.GetSize(); i++)
            {
                if(m_map[i]->hwndChild == hWnd) return m_map[i];
            }
            return NULL;
        }
};

class CDockingWindow : public CDockingWindowImpl<CDockingWindow>
{
    public:
        DECLARE_WND_CLASS_EX(_T("WADockWindowClass"),
                             CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                             NULL)
};

#endif // _WADOCK_H_
