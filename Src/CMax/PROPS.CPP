#include "precomp.h"
#include "cedit.h"
#include "hkeyctrl.h"
#include "resource.h"

extern HINSTANCE hInstance;

#define IDX_BOOKMARK		0
#define IDX_COMMENT			1
#define IDX_HDIVIDERLINES	2
#define IDX_HIGHLIGHTEDLINE	3
#define IDX_KEYWORD			4
#define IDX_LMARGIN			5
#define IDX_LINENUMBER		6
#define IDX_NUMBER			7
#define IDX_OPERATOR		8
#define IDX_SCOPEKEYWORD	9
#define IDX_STRING			10
#define IDX_TAGELEMNAME		11
#define IDX_TAGENT			12
#define IDX_TAGATTRNAME		13
#define IDX_TEXT			14
#define IDX_WINDOW			15
#define IDX_TAGTEXT			16
#define IDX_VDIVIDERLINES	17
#define IDX_EXTRA1			18
#define IDX_EXTRA2			19
#define IDX_EXTRA3			20
#define IDX_BREAKPOINT 		21

#define IDX_ASPECT_COUNT	22

class CPropInfo
{
	public:
    	CPropInfo();
    	~CPropInfo();
    
    	CEdit *m_pCtrl;
    
    	typedef enum { eColorFont, eKeyboard, eTabs, eMisc } eProps;
    
    	HWND m_hWndKeyboardDlg;
    
    	//////////////////
    	// Misc Props
    	//
    	BOOL m_bSmoothScroll : 2;
    	BOOL m_bShowLeftMargin : 2;
    	BOOL m_bAllowDragDrop : 2;
    	BOOL m_bLineToolTips : 2;
    	BOOL m_bAllowColumnSel : 2;
    	BOOL m_bColorSyntax : 2;
    	BOOL m_bShowHScrollBar : 2;
    	BOOL m_bShowVScrollBar : 2;
    	BOOL m_bAllowHSplit : 2;
    	BOOL m_bAllowVSplit : 2;
    	BOOL m_bSelBounds : 2;
    	int m_nMaxUndo;
    	CM_LINENUMBERING m_cmLineNum;
    
    	////////////////////////
    	// Color/Font Props
    	//
    	int m_nPosToIndexMap[ IDX_ASPECT_COUNT ];
    	COLORREF m_crFore[ IDX_ASPECT_COUNT ];
    	COLORREF m_crBack[ IDX_ASPECT_COUNT ];
    	BYTE m_byFontStyles[ IDX_ASPECT_COUNT ];
    	LOGFONT m_lf;
    	HFONT m_hFont;
    
    	////////////////////////
    	// Keyboard Props
    	//
    	CM_HOTKEY *m_pcmAddKey;
    	WORD *m_pwAddCmd;
    	int m_nAddCount;
    	CM_HOTKEY *m_pcmRemoveKey;
    	int m_nRemoveCount;
    	void AddHotKey( CM_HOTKEY &cmHotKey, WORD wCmd );
    	void RemoveHotKey( CM_HOTKEY &cmHotKey );
    	void ResetHotKeys();
    	BOOL ShouldEatCommand() const
    	{
    	    return m_hWndKeyboardDlg ? ( SendMessage( GetDlgItem( m_hWndKeyboardDlg, IDC_HOTKEY ),
    	                                 HOTM_QUERYEATCOMMAND, 0, 0 ) != 0 ) : FALSE;
   	    }
    
    	////////////////////////
    	// Tab Props
    	//
    	CEdit::IndentStyle m_eIndentStyle;
    	int m_nTabSize;
    	BOOL m_bExpandTabs;
    	BOOL m_bNormalizeCase;
    	TCHAR m_szLang[ CM_MAX_LANGUAGE_NAME + 1 ];
    
	    void CommitChanges( eProps eWhat );
};

CPropInfo::CPropInfo()
{
	m_pcmAddKey = NULL;
	m_pwAddCmd = NULL;
	m_pcmRemoveKey = NULL;
	m_nAddCount = m_nRemoveCount = 0;
	m_hFont = NULL;
	m_hWndKeyboardDlg = NULL;
}

CPropInfo::~CPropInfo()
{
	if ( m_pcmAddKey )
	{
		free( m_pcmAddKey );
	}
	if ( m_pwAddCmd )
	{
		free( m_pwAddCmd );
	}
	if ( m_pcmRemoveKey )
	{
		free( m_pcmRemoveKey );
	}
	if ( m_hFont )
	{
		DeleteObject( m_hFont );
	}
}

void CPropInfo::AddHotKey( CM_HOTKEY &cmHotKey, WORD wCmd )
{
	BOOL bAppend = TRUE;

	CEdit::NormalizeHotKey( cmHotKey );

	// check to see if already have 'add' record for this hotkey
	// and modify it if so
	for ( int i = 0; i < m_nAddCount; i++ )
	{
		if ( m_pcmAddKey[ i ] == cmHotKey )
		{
			bAppend = FALSE;
			m_pwAddCmd[ i ] = wCmd;
			break;
		}
	}

	// check to see if have a 'remove' record for this hotkey
	// and remove if so
	for ( i = 0; i < m_nRemoveCount; i++ )
	{
		if ( m_pcmRemoveKey[ i ] == cmHotKey )
		{
			m_pcmRemoveKey[ i ].nVirtKey1 = 0;  // don't actually remove the element -- just wipe it
			break;
		}
	}

	if ( bAppend )
	{
		m_pcmAddKey = m_pcmAddKey ? ( CM_HOTKEY * ) realloc( m_pcmAddKey, sizeof( CM_HOTKEY ) * ( m_nAddCount + 1 ) ) :
		                            ( CM_HOTKEY * ) malloc( sizeof( CM_HOTKEY ) );
		m_pwAddCmd = m_pwAddCmd ? ( WORD * ) realloc( m_pwAddCmd, sizeof( WORD ) * ( m_nAddCount + 1 ) ) :
		                          ( WORD * ) malloc( sizeof( WORD ) );
		m_pcmAddKey[ m_nAddCount ] = cmHotKey;
		m_pwAddCmd[ m_nAddCount++ ] = wCmd;
	}
}

void CPropInfo::RemoveHotKey( CM_HOTKEY &cmHotKey )
{
	BOOL bAppend = TRUE;
	CEdit::NormalizeHotKey( cmHotKey );

	// check to see if already have a 'remove' record for this hotkey
	// and ignore this one if so
	for ( int i = 0; i < m_nRemoveCount; i++ )
	{
		if ( m_pcmRemoveKey[ i ] == cmHotKey )
		{
			bAppend = FALSE;
			break;
		}
	}

	// check to see if have an 'add' record for this hotkey
	// and remove if so
	for ( i = 0; i < m_nAddCount; i++ )
	{
		if ( m_pcmAddKey[ i ] == cmHotKey )
		{
			m_pcmAddKey[ i ].nVirtKey1 = 0;  // don't actually add the element -- just wipe it
			m_pwAddCmd[ i ] = 0;  // don't actually add the element -- just wipe it
			break;
		}
	}

	if ( bAppend )
	{
		m_pcmRemoveKey = m_pcmRemoveKey ? ( CM_HOTKEY * ) realloc( m_pcmRemoveKey, sizeof( CM_HOTKEY ) * ( m_nRemoveCount + 1 ) ) : 
		                                  ( CM_HOTKEY * ) malloc( sizeof( CM_HOTKEY ) );
		m_pcmRemoveKey[ m_nRemoveCount++ ] = cmHotKey;
	}
}

void CPropInfo::ResetHotKeys()
{
	// remove all pending changes
	if ( m_pcmAddKey )
	{
		free( m_pcmAddKey );
		m_pcmAddKey = NULL;
	}
	if ( m_pwAddCmd )
	{
		free( m_pwAddCmd );
		m_pwAddCmd = NULL;
	}
	if ( m_pcmRemoveKey )
	{
		free( m_pcmRemoveKey );
		m_pcmRemoveKey = NULL;
	}
	m_nAddCount = m_nRemoveCount = 0;

	// Now, add 'remove' records for all existing hotkeys
	for ( int i = 0; i < CEdit::g_nHotKeyCount; i++ )
	{
		CHotKey *pHotKey = CEdit::g_pHotKeys + i;
		if ( !CEdit::IsRegisteredCommand( pHotKey->wCmd ) )
        {
			RemoveHotKey( pHotKey->cmHotKey ); 
        }
	}

	// Now, add 'add' records for all default hotkeys
	for ( i = 0; i < CEdit::g_nDefHotKeyCount; i++ )
	{
		_defhotkeyrec *pHotKey = &CEdit::g_DefHotKeys[ i ];
		CM_HOTKEY cmHotKey = { pHotKey->fsModifiers1, pHotKey->nVirtKey1,
		                       pHotKey->fsModifiers2, pHotKey->nVirtKey2 };
		AddHotKey( cmHotKey, pHotKey->wCmd );
	}
}

void CPropInfo::CommitChanges( eProps eWhat )
{
	for ( int nWnd = 0; nWnd < CEdit::g_nhWndCount; nWnd++ )
	{
		HWND hWnd = m_pCtrl->m_bGlobalProps ? CEdit::g_phWnds[ nWnd ] : m_pCtrl->m_hWnd;
		CEdit *pCtrl = ( CEdit * ) GetWindowLong( hWnd, 0 );

		pCtrl->Repaint( FALSE );

		if ( eWhat == eMisc )
		{
			if ( m_bSmoothScroll != pCtrl->m_bSmoothScroll )
			{
				SendMessage( hWnd, CMM_ENABLESMOOTHSCROLLING, m_bSmoothScroll, 0 );
			}
			if ( m_bShowLeftMargin != pCtrl->m_bShowLeftMargin )
			{
				SendMessage( hWnd, CMM_ENABLELEFTMARGIN, m_bShowLeftMargin, 0 );
			}
			if ( m_bAllowDragDrop != pCtrl->m_bAllowDragDrop )
			{
				SendMessage( hWnd, CMM_ENABLEDRAGDROP, m_bAllowDragDrop, 0 );
			}
			if ( m_bLineToolTips != pCtrl->m_bLineToolTips )
			{
				SendMessage( hWnd, CMM_ENABLELINETOOLTIPS, m_bLineToolTips, 0 );
			}
			if ( m_bAllowColumnSel != pCtrl->m_bAllowColumnSel )
			{
				SendMessage( hWnd, CMM_ENABLECOLUMNSEL, m_bAllowColumnSel, 0 );
			}
			if ( m_bColorSyntax != pCtrl->m_bColorSyntax )
			{
				SendMessage( hWnd, CMM_ENABLECOLORSYNTAX, m_bColorSyntax, 0 );
			}
			if ( m_bShowHScrollBar != pCtrl->ShowHScrollBar() )
			{
				SendMessage( hWnd, CMM_SHOWSCROLLBAR, TRUE, m_bShowHScrollBar );
			}
			if ( m_bShowVScrollBar != pCtrl->ShowVScrollBar() )
			{
				SendMessage( hWnd, CMM_SHOWSCROLLBAR, FALSE, m_bShowVScrollBar );
			}
			if ( m_bAllowHSplit != pCtrl->m_bAllowHSplit )
			{
				SendMessage( hWnd, CMM_ENABLESPLITTER, TRUE, m_bAllowHSplit );
			}
			if ( m_bAllowVSplit != pCtrl->m_bAllowVSplit )
			{
				SendMessage( hWnd, CMM_ENABLESPLITTER, FALSE, m_bAllowVSplit );
			}
			if ( m_bSelBounds != pCtrl->BoundSelection() )
			{
				SendMessage( hWnd, CMM_ENABLESELBOUNDS, m_bSelBounds, 0 );
			}
			if ( m_nMaxUndo != pCtrl->m_Buffer.GetMaxUndo() )
			{
				SendMessage( hWnd, CMM_SETUNDOLIMIT, m_nMaxUndo, 0 );
			}
			SendMessage( hWnd, CMM_SETLINENUMBERING, 0, ( LPARAM ) &m_cmLineNum );
		}
		else if ( eWhat == eColorFont )
		{
			CM_COLORS Colors;
			ZeroMemory( &Colors, sizeof( Colors ) );
			Colors.crWindow = m_crFore[ IDX_WINDOW ];
			Colors.crLeftMargin = m_crFore[ IDX_LMARGIN ];
			Colors.crBookmark = m_crFore[ IDX_BOOKMARK ];
			Colors.crBookmarkBk = m_crBack[ IDX_BOOKMARK ];
			Colors.crBreakpoint = m_crFore[ IDX_BREAKPOINT ];
			Colors.crBreakpointBk = m_crBack[ IDX_BREAKPOINT ];
			Colors.crText = m_crFore[ IDX_TEXT ];
			Colors.crTextBk = m_crBack[ IDX_TEXT ];
			Colors.crNumber = m_crFore[ IDX_NUMBER ];
			Colors.crNumberBk = m_crBack[ IDX_NUMBER ];
			Colors.crKeyword = m_crFore[ IDX_KEYWORD ];
			Colors.crKeywordBk = m_crBack[ IDX_KEYWORD ];
			Colors.crOperator = m_crFore[ IDX_OPERATOR ];
			Colors.crOperatorBk = m_crBack[ IDX_OPERATOR ];
			Colors.crScopeKeyword = m_crFore[ IDX_SCOPEKEYWORD ];
			Colors.crScopeKeywordBk = m_crBack[ IDX_SCOPEKEYWORD ];
			Colors.crComment = m_crFore[ IDX_COMMENT ];
			Colors.crCommentBk = m_crBack[ IDX_COMMENT ];
			Colors.crString = m_crFore[ IDX_STRING ];
			Colors.crStringBk = m_crBack[ IDX_STRING ];
			Colors.crTagText = m_crFore[ IDX_TAGTEXT ];
			Colors.crTagTextBk = m_crBack[ IDX_TAGTEXT ];
			Colors.crTagEntity = m_crFore[ IDX_TAGENT ];
			Colors.crTagEntityBk = m_crBack[ IDX_TAGENT ];
			Colors.crTagElementName = m_crFore[ IDX_TAGELEMNAME ];
			Colors.crTagElementNameBk = m_crBack[ IDX_TAGELEMNAME ];
			Colors.crTagAttributeName = m_crFore[ IDX_TAGATTRNAME ];
			Colors.crTagAttributeNameBk = m_crBack[ IDX_TAGATTRNAME ];
			Colors.crLineNumber = m_crFore[ IDX_LINENUMBER ];
			Colors.crLineNumberBk = m_crBack[ IDX_LINENUMBER ];
			Colors.crHDividerLines = m_crFore[ IDX_HDIVIDERLINES ];
			Colors.crVDividerLines = m_crFore[ IDX_VDIVIDERLINES ];
			Colors.crHighlightedLine = m_crFore[ IDX_HIGHLIGHTEDLINE ];

			Colors.crExtra1 = m_crFore[ IDX_EXTRA1 ];
			Colors.crExtra2 = m_crFore[ IDX_EXTRA2 ];
			Colors.crExtra3 = m_crFore[ IDX_EXTRA3 ];

			SendMessage( hWnd, CMM_SETCOLORS, 0, ( LPARAM ) &Colors );

			SendMessage( hWnd, WM_SETFONT, ( WPARAM ) CreateFontIndirect( &m_lf ), MAKELPARAM( TRUE, 0 ) );
			pCtrl->m_bOwnFont = TRUE;

			CM_FONTSTYLES FontStyles;
			ZeroMemory( &FontStyles, sizeof( FontStyles ) );
			FontStyles.byText = m_byFontStyles[ IDX_TEXT ];
			FontStyles.byNumber = m_byFontStyles[ IDX_NUMBER ];
			FontStyles.byKeyword = m_byFontStyles[ IDX_KEYWORD ];
			FontStyles.byOperator = m_byFontStyles[ IDX_OPERATOR ];
			FontStyles.byScopeKeyword = m_byFontStyles[ IDX_SCOPEKEYWORD ];
			FontStyles.byComment = m_byFontStyles[ IDX_COMMENT ];
			FontStyles.byString = m_byFontStyles[ IDX_STRING ];
			FontStyles.byTagText = m_byFontStyles[ IDX_TAGTEXT ];
			FontStyles.byTagEntity = m_byFontStyles[ IDX_TAGENT ];
			FontStyles.byTagElementName = m_byFontStyles[ IDX_TAGELEMNAME ];
			FontStyles.byTagAttributeName = m_byFontStyles[ IDX_TAGATTRNAME ];
			FontStyles.byLineNumber = m_byFontStyles[ IDX_LINENUMBER ];

			SendMessage( hWnd, CMM_SETFONTSTYLES, 0, ( LPARAM ) &FontStyles );
		}
		else if ( eWhat == eKeyboard )
		{
			if ( m_pcmAddKey )
			{
				for ( int i = 0; i < m_nAddCount; i++ )
				{
					CM_HOTKEY cmHotKey = m_pcmAddKey[ i ];
					if ( cmHotKey.nVirtKey1 )
					{
						CEdit::RegisterHotKey( cmHotKey, m_pwAddCmd[ i ] );
					}
				}
				free( m_pcmAddKey );
				free( m_pwAddCmd );
				m_pcmAddKey = NULL;
				m_pwAddCmd = NULL;
				m_nAddCount = 0;
			}

			if ( m_pcmRemoveKey )
			{
				for ( int i = 0; i < m_nRemoveCount; i++ )
				{
					CM_HOTKEY cmHotKey = m_pcmRemoveKey[ i ];
					if ( cmHotKey.nVirtKey1 )
					{
						CEdit::UnregisterHotKey( cmHotKey );
					}
				}
				free( m_pcmRemoveKey );
				m_pcmRemoveKey = NULL;
				m_nRemoveCount = 0;
			}
		}
		else if ( eWhat == eTabs )
		{
			if ( m_eIndentStyle != pCtrl->m_eIndentStyle )
			{
				SendMessage( hWnd, CMM_SETAUTOINDENTMODE, m_eIndentStyle, 0 );
			}

			if ( m_nTabSize != pCtrl->m_Buffer.GetTabSize() )
			{
				SendMessage( hWnd, CMM_SETTABSIZE, m_nTabSize, 0 );
			}

			if ( m_bExpandTabs != pCtrl->m_bExpandTabs )
			{
				SendMessage( hWnd, CMM_ENABLETABEXPAND, m_bExpandTabs, 0 );
			}

			if ( m_bNormalizeCase != pCtrl->m_Buffer.NormalizeCase() )
			{
				SendMessage( hWnd, CMM_ENABLENORMALIZECASE, m_bNormalizeCase, 0 );
			}

			// language setting only applies to the current window
			if ( ( pCtrl == m_pCtrl ) && ( _tcsicmp( m_szLang, pCtrl->m_szLang ) != 0 ) )
			{
				SendMessage( hWnd, CMM_SETLANGUAGE, 0, ( LPARAM ) m_szLang );
			}
		}

		// tell the parent that props changed
		pCtrl->NotifyParent( CMN_PROPSCHANGE );

		if ( !m_pCtrl->m_bGlobalProps )
		{
			break;
		}
	}
}

void OnPropChanged( HWND hWndPropPage )
{
	SendMessage( GetParent( hWndPropPage ), PSM_CHANGED, ( WPARAM ) hWndPropPage, 0 );
}

#define BIT_NODEFAULT 0x1
#define BIT_NOBKCOLOR 0x2
#define BIT_NOFONTSTYLE	0x4

static int g_nLastPage = 0;

#define UM_UPDATE_FONT WM_USER + 100
#define UM_ITEM_CHANGED WM_USER + 101

void CEdit::Properties()
{
	// the tabsize may change -- make sure the caret is updated
	CSaveSelection save( this );
	NotifyParent( CMN_SHOWPROPS );
}
