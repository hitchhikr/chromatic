#include "precomp.h"
#include "editdlgs.h"
#include "cedit.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Breakpoints
//
//

void CEdit::BreakpointToggle()
{
	if ( m_Buffer.GetLineCount() )
	{
		int nLine = m_Selection.GetEndRow();
		m_Buffer.ToggleBreakpoint( nLine );
		DamageAllViews( nLine, nLine );
	}
}

void CEdit::BreakpointPrev()
{
	if ( m_Buffer.GetLineCount() )
	{
		int nLine = GetAdjacentBreakpoint( m_Selection.GetEndRow(), TRUE );
		if ( nLine != -1 )
		{
			GoToLine( nLine, !m_pActiveView->LineIsVisible( nLine ) );
		}
	}
}

void CEdit::BreakpointNext()
{
	if ( m_Buffer.GetLineCount() )
	{
		int nLine = GetAdjacentBreakpoint( m_Selection.GetEndRow(), FALSE );
		if ( nLine != -1 )
		{
			GoToLine( nLine, !m_pActiveView->LineIsVisible( nLine ) );
		}
	}
}

void CEdit::BreakpointClearAll()
{
	int nLineCount = m_Buffer.GetLineCount();
	for ( int nLine = 0; nLine < nLineCount; nLine++ )
	{
		if ( m_Buffer.HasBreakpoint( nLine ) )
		{
			m_Buffer.RemoveBreakpoint( nLine );
		}
	}

	Repaint( FALSE );
}

void CEdit::BreakpointJumpToFirst()
{
	if ( m_Buffer.GetLineCount() )
	{
		int nLine = GetAdjacentBreakpoint( -1, FALSE );
		if ( nLine != -1 )
		{
			GoToLine( nLine, !m_pActiveView->LineIsVisible( nLine ) );
		}
	}
}

void CEdit::BreakpointJumpToLast()
{
	int nLineCount = m_Buffer.GetLineCount();
	if ( nLineCount )
	{
		int nLine = GetAdjacentBreakpoint( nLineCount, TRUE );
		if ( nLine != -1 )
		{
			GoToLine( nLine, !m_pActiveView->LineIsVisible( nLine ) );
		}
	}
}

int CEdit::GetAdjacentBreakpoint( int nLine, BOOL bPrev ) const
{
	int nStartLine = nLine;
	int nLineCount = m_Buffer.GetLineCount();
	
	if ( bPrev )
	{
		for ( --nLine; nLine >= 0; nLine-- )
		{
			if ( m_Buffer.HasBreakpoint( nLine ) )
			{
				return nLine;
			}
		}
		for ( nLine = nLineCount - 1; nLine >= nStartLine; nLine-- )
		{
			if ( m_Buffer.HasBreakpoint( nLine ) )
			{
				return nLine;
			}
		}
	}
	else
	{
		for ( ++nLine; nLine < nLineCount; nLine++ )
		{
			if ( m_Buffer.HasBreakpoint( nLine ) )
			{
				return nLine;
			}
		}
		for ( nLine = 0; nLine <= nStartLine; nLine++ )
		{
			if ( m_Buffer.HasBreakpoint( nLine ) )
			{
				return nLine;
			}
		}
	}
	return -1;
}
