#include "precomp.h"
#include "cedit.h"

CLanguage **CEdit::g_pLanguages = NULL;
int CEdit::g_nLanguageCount = 0;

CME_CODE CEdit::RegisterLanguage( LPCTSTR pszName, CM_LANGUAGE *pLang )
{
	if ( !pLang )
	{
		return CME_BADARGUMENT;
	}

	// make sure the same language is not registered twice
	BOOL bDupe = FALSE;
	for ( int i = 0; i < g_nLanguageCount; i++ )
	{
		if ( _tcsicmp( g_pLanguages[ i ]->m_szName, pszName ) == 0 )
		{
			bDupe = TRUE;
			break;
		} 
	} 
	
	if ( bDupe )
	{
		delete g_pLanguages[ i ];
		g_pLanguages[ i ] = new CLanguage( pszName, pLang );
		
		// 'flash' the controls that have this language
		for ( int nWnd = 0; nWnd < g_nhWndCount; nWnd++ )
		{
			HWND hWnd = CEdit::g_phWnds[ nWnd ];
			TCHAR szLang[ CM_MAX_LANGUAGE_NAME ];
			CM_GetLanguage( hWnd, szLang );
			if ( _tcsicmp( szLang, pszName ) == 0 )
			{
				// language in use -- re-establish the new language
				CM_SetLanguage( hWnd, NULL );
				CM_SetLanguage( hWnd, pszName );
			}
		}
	}
	else
	{
		g_pLanguages = g_pLanguages ? ( CLanguage ** ) realloc( g_pLanguages, ( g_nLanguageCount + 1 ) * sizeof( CLanguage * ) ) :
		                              ( CLanguage ** ) malloc( sizeof( CLanguage * ) );
		g_pLanguages[ g_nLanguageCount++ ] = new CLanguage( pszName, pLang );
	}
	
	return CME_SUCCESS;
}

BOOL CEdit::UnregisterLanguage( LPCTSTR pszName )
{
	BOOL bFound = FALSE;
	if ( g_pLanguages )
	{
		// find the language
		for ( int nLanguage = 0; nLanguage < g_nLanguageCount; nLanguage++ )
		{
			if ( _tcsicmp( g_pLanguages[ nLanguage ]->m_szName, pszName ) == 0 )
			{
				bFound = TRUE;
				break;
			}
		}

		if ( bFound )
		{
			delete g_pLanguages[ nLanguage ];
			// fixup array indexes
			if ( g_nLanguageCount == 1 )
			{
				free( g_pLanguages );
				g_pLanguages = NULL;
			}
			else
			{
				// shift array down
				if ( nLanguage < g_nLanguageCount - 1 )
				{
					memmove( g_pLanguages + nLanguage, 
					         g_pLanguages + nLanguage + 1, 
							 ( g_nLanguageCount - nLanguage - 1 ) * sizeof( CLanguage * ) );
				}
			}
			g_nLanguageCount--;
		}
	}

	return bFound;
}

CME_CODE CEdit::UnregisterAllLanguages()
{
	int nCount = CEdit::g_nLanguageCount;
	for ( int i = 0; i < nCount; i++ )
	{
		UnregisterLanguage( CEdit::g_pLanguages[ 0 ]->m_szName );
	}
	return ( CEdit::g_nLanguageCount == 0 ) ? CME_SUCCESS : CME_FAILURE;
}

int CEdit::GetLanguageDef( LPCTSTR pszName, CM_LANGUAGE *pLang )
{
	int cbLang = 0;
	for ( int i = 0; i < g_nLanguageCount; i++ )
	{
		CLanguage *pLangFound = g_pLanguages[ i ];
		if ( _tcsicmp( pLangFound->m_szName, pszName ) == 0 )
		{
			cbLang = pLangFound->GetSize();
			if ( pLang )
			{
				memcpy( pLang, pLangFound->m_pLang, cbLang );
			}
			break;
		} 
	}

	return cbLang;
}

CLanguage::CLanguage( LPCTSTR pszName, CM_LANGUAGE *pLang )
{
	//////////////////////////////////
	// Must make complete copy of language details
	//////////////////////////////////
	_tcscpy( m_szName, pszName );

	int nLen = ( pLang->pszKeywords ? _tcslen( pLang->pszKeywords ) + 1 : 0 ) +
	           ( pLang->pszOperators ? _tcslen( pLang->pszOperators ) + 1 : 0 ) +
	           ( pLang->pszSingleLineComments ? _tcslen( pLang->pszSingleLineComments ) + 1 : 0 ) +
	           ( pLang->pszMultiLineComments1 ? _tcslen( pLang->pszMultiLineComments1 ) + 1 : 0 ) +
	           ( pLang->pszMultiLineComments2 ? _tcslen( pLang->pszMultiLineComments2 ) + 1 : 0 ) +
	           ( pLang->pszScopeKeywords1 ? _tcslen( pLang->pszScopeKeywords1 ) + 1 : 0 ) +
	           ( pLang->pszScopeKeywords2 ? _tcslen( pLang->pszScopeKeywords2 ) + 1 : 0 ) +
	           ( pLang->pszStringDelims ? _tcslen( pLang->pszStringDelims ) + 1 : 0 ) +
	           ( pLang->pszTagElementNames ? _tcslen( pLang->pszTagElementNames ) + 1 : 0 ) +
	           ( pLang->pszTagAttributeNames ? _tcslen( pLang->pszTagAttributeNames ) + 1 : 0 ) +
	           ( pLang->pszTagEntities ? _tcslen( pLang->pszTagEntities ) + 1 : 0 ) +
	           ( pLang->pszExtra1 ? _tcslen( pLang->pszExtra1 ) + 1 : 0 ) +
	           ( pLang->pszExtra2 ? _tcslen( pLang->pszExtra2 ) + 1 : 0 ) +
	           ( pLang->pszExtra3 ? _tcslen( pLang->pszExtra3 ) + 1 : 0 );

	int cbLang = sizeof( CM_LANGUAGE ) +			    // the language structure
	             sizeof( DWORD ) +				        // size of language struct
	             sizeof( TCHAR ) * ( nLen + 1 );		// the lang buffer. +1 in case nLen = 0
	m_pLang = ( CM_LANGUAGE * ) new BYTE[ cbLang ];
	ZeroMemory( m_pLang, cbLang );

	*( ( LPDWORD ) ( m_pLang + 1 ) ) = cbLang;
	m_pLang->bIsCaseSensitive = pLang->bIsCaseSensitive;
	m_pLang->chEscape = pLang->chEscape;
	m_pLang->chTerminator = pLang->chTerminator;
	m_pLang->dwStyle = pLang->dwStyle;
	m_pLang->dwHighLightNbrs = pLang->dwHighLightNbrs;

	LPTSTR pszOut = ( LPTSTR ) ( ( LPBYTE ) ( m_pLang + 1 ) + sizeof( DWORD ) );

	if ( pLang->pszKeywords )
	{
		_tcscpy( pszOut, pLang->pszKeywords );
		m_pLang->pszKeywords = pszOut;
		pszOut += _tcslen( pLang->pszKeywords ) + 1;
	}

	if ( pLang->pszOperators )
	{
		_tcscpy( pszOut, pLang->pszOperators );
		m_pLang->pszOperators = pszOut;
		pszOut += _tcslen( pLang->pszOperators ) + 1;
	}

	if ( pLang->pszSingleLineComments )
	{
		_tcscpy( pszOut, pLang->pszSingleLineComments );
		m_pLang->pszSingleLineComments = pszOut;
		pszOut += _tcslen( pLang->pszSingleLineComments ) + 1;
	}

	if ( pLang->pszMultiLineComments1 )
	{
		_tcscpy( pszOut, pLang->pszMultiLineComments1 );
		m_pLang->pszMultiLineComments1 = pszOut;
		pszOut += _tcslen( pLang->pszMultiLineComments1 ) + 1;
		_tcscpy( pszOut, pLang->pszMultiLineComments2 );
		m_pLang->pszMultiLineComments2 = pszOut;
		pszOut += _tcslen( pLang->pszMultiLineComments2 ) + 1;
	}

	if ( pLang->pszScopeKeywords1 )
	{
		_tcscpy( pszOut, pLang->pszScopeKeywords1 );
		m_pLang->pszScopeKeywords1 = pszOut;
		pszOut += _tcslen( pLang->pszScopeKeywords1 ) + 1;
		_tcscpy( pszOut, pLang->pszScopeKeywords2 );
		m_pLang->pszScopeKeywords2 = pszOut;
		pszOut += _tcslen( pLang->pszScopeKeywords2 ) + 1;
	}

	if ( pLang->pszStringDelims )
	{
		_tcscpy( pszOut, pLang->pszStringDelims );
		m_pLang->pszStringDelims = pszOut;
		pszOut += _tcslen( pLang->pszStringDelims ) + 1;
	}

	if ( pLang->pszTagElementNames )
	{
		_tcscpy( pszOut, pLang->pszTagElementNames );
		m_pLang->pszTagElementNames = pszOut;
		pszOut += _tcslen( pLang->pszTagElementNames ) + 1;
	}

	if ( pLang->pszTagAttributeNames )
	{
		_tcscpy( pszOut, pLang->pszTagAttributeNames );
		m_pLang->pszTagAttributeNames = pszOut;
		pszOut += _tcslen( pLang->pszTagAttributeNames ) + 1;
	}

	if ( pLang->pszTagEntities )
	{
		_tcscpy( pszOut, pLang->pszTagEntities );
		m_pLang->pszTagEntities = pszOut;
		pszOut += _tcslen( pLang->pszTagEntities ) + 1;
	}

	if ( pLang->pszExtra1 )
	{
		_tcscpy( pszOut, pLang->pszExtra1 );
		m_pLang->pszExtra1 = pszOut;
		pszOut += _tcslen( pLang->pszExtra1 ) + 1;
	}

	if ( pLang->pszExtra2 )
	{
		_tcscpy( pszOut, pLang->pszExtra2 );
		m_pLang->pszExtra2 = pszOut;
		pszOut += _tcslen( pLang->pszExtra2 ) + 1;
	}

	if ( pLang->pszExtra3 )
	{
		_tcscpy( pszOut, pLang->pszExtra3 );
		m_pLang->pszExtra3 = pszOut;
		pszOut += _tcslen( pLang->pszExtra3 ) + 1;
	}

}

int CLanguage::GetSize() const
{
	return ( int ) *( ( LPDWORD ) ( m_pLang + 1 ) );
}

CLanguage::~CLanguage()
{
	delete m_pLang;
}
