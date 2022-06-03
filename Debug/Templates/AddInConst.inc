; ------------------------------------
; Chromatic AddIns constants and structures
; MASM32 version
; ------------------------------------
; Version: 2.0
; ------------------------------------
; (C) Copyright 2001-2010 Franck Charlet.
; ------------------------------------

; --- AddIns Standard invoke
LIBPROTO                        MACRO   name,argl:VARARG
                                LOCAL   @tmp_a
                                LOCAL   @tmp_b
@tmp_a                          TYPEDEF PROTO argl
@tmp_b                          TYPEDEF PTR @tmp_a
                                name    @tmp_b ?
                                ENDM

LIBINVOKE                       MACRO   Service,arglist:VARARG
                                push    ecx
                                push    edx
                                mov     edx,ChromaticLib
                                IFB     <arglist>
                                        INVOKE  [edx+CHROMATICLIB.Service]
                                ELSE
                                        INVOKE  [edx+CHROMATICLIB.Service],arglist
                                ENDIF
                                pop     edx
                                pop     ecx
                                ENDM

; --- Support constants for Windows.inc
TBSTYLE_DROPDOWN                equ     08h
TBSTYLE_EX_DRAWDDARROWS         equ     1
TBN_DROPDOWN                    equ     (TBN_FIRST - 10)

; --- Library constants
CHROMATICLIB_VERSION            equ     0106h

SPLITTER_HORZ                   equ     1
SPLITTER_VERT                   equ     2

MSG_SPLITTER_RESIZED            equ     WM_USER + 100
MSG_COLORBOX_CLICKED            equ     WM_USER + 101
MSG_SPLASH_PAINT                equ     WM_USER + 102

STATUSBAR_PARTSUNKEN            equ     2
STATUSBAR_PARTNORMAL            equ     4
STATUSBAR_PARTRAISED            equ     8
STATUSBAR_PARTOWNER             equ     16

DUMPBOX_LINELEFT                equ     WM_USER + 201
DUMPBOX_LINERIGHT               equ     WM_USER + 202
DUMPBOX_PAGELEFT                equ     WM_USER + 203
DUMPBOX_PAGERIGHT               equ     WM_USER + 204
DUMPBOX_LINEUP                  equ     WM_USER + 205
DUMPBOX_LINEDOWN                equ     WM_USER + 206
DUMPBOX_PAGEUP                  equ     WM_USER + 207
DUMPBOX_PAGEDOWN                equ     WM_USER + 208
DUMPBOX_POSITIONLEFTRIGHT       equ     WM_USER + 209
DUMPBOX_POSITIONUPDOWN          equ     WM_USER + 210
DUMPBOX_TRACKLEFTRIGHT          equ     WM_USER + 211
DUMPBOX_TRACKUPDOWN             equ     WM_USER + 212
DUMPBOX_TOP                     equ     WM_USER + 213
DUMPBOX_BOTTOM                  equ     WM_USER + 214
DUMPBOX_LEFT                    equ     WM_USER + 215
DUMPBOX_RIGHT                   equ     WM_USER + 216

CHILD_MSG_CREATED               equ     WM_USER + 217
CHILD_MSG_DESTROYED             equ     WM_USER + 218
CHILD_MSG_SETTITLE              equ     WM_USER + 219

GRIPBOX_MSG_ASKSIZE             equ     WM_USER + 220
GRIPBOX_MSG_ASKSTEPS            equ     WM_USER + 221
GRIPBOX_MSG_RESIZED             equ     WM_USER + 222
GRIPBOX_TOPLEFT                 equ     0
GRIPBOX_TOP                     equ     1
GRIPBOX_TOPRIGHT                equ     2
GRIPBOX_BOTTOMLEFT              equ     3
GRIPBOX_BOTTOM                  equ     4
GRIPBOX_BOTTOMRIGHT             equ     5
GRIPBOX_LEFT                    equ     6
GRIPBOX_RIGHT                   equ     7

GRIPBOX_STATE_RESET             equ     0
GRIPBOX_STATE_RESIZING          equ     1

DOCKINGBOX_LEFT                 equ     0
DOCKINGBOX_RIGHT                equ     1
DOCKINGBOX_TOP                  equ     2
DOCKINGBOX_BOTTOM               equ     3
DOCKINGBOX_FLOATING             equ     4

DOCKINGBOX_MSG_CLOSE            equ     WM_USER + 300
DOCKINGBOX_MSG_QUERY_STATE      equ     WM_USER + 301

; --- AddIns constants
ADDIN_FINISHED                  equ     0
ADDIN_PERSISTANT                equ     1
ADDIN_DIE                       equ     2
ADDIN_ZOMBIE                    equ     3

; --- Hooks constants
HOOK_MDICREATE                  equ     0
HOOK_MDI                        equ     1
HOOK_MDICLIENT                  equ     2
HOOK_CHILDCREATE                equ     3
HOOK_CHILD                      equ     4
HOOK_CMAXXX                     equ     5

HOOK_CONTINUE                   equ     0
HOOK_DIE                        equ     1

; --- Configuration files
INIFILE_MAIN                    equ     0
INIFILE_CURRENTUSERMENUS        equ     1
INIFILE_TOOLBARS                equ     2
INIFILE_COLORS                  equ     3
INIFILE_FTPACCOUNTS             equ     4
INIFILE_SKINS                   equ     5
INIFILE_PROJECTSTYPES           equ     6
INIFILE_LANGUAGES               equ     7
INIFILE_DATES                   equ     8
INIFILE_CGREP                   equ     9
INIFILE_FILTERS                 equ     10

; --- Directories
DIR_ROOT                        equ     0
DIR_BIN                         equ     1
DIR_INCLUDE                     equ     2
DIR_LIB                         equ     3
DIR_HELP                        equ     4
DIR_MAIN                        equ     5
DIR_WIN                         equ     6
DIR_WINSYS                      equ     7
DIR_TEMP                        equ     8
DIR_TEMPLATES                   equ     9
DIR_PROJECTS                    equ     10
DIR_MAINHELP                    equ     11
DIR_SNIPPETS                    equ     12
DIR_DATABASES                   equ     13
DIR_SKINS                       equ     14
DIR_LANGUAGES                   equ     15
DIR_DOWNLOADS                   equ     16
DIR_ADDINS                      equ     17
DIR_CONFIG                      equ     18
DIR_MENUS                       equ     19
DIR_WIZARDS                     equ     20
DIR_FILTERS                     equ     21
DIR_EXTRACODE                   equ     22
DIR_LANGSCRIPTS                 equ     23
DIR_TOOLS                       equ     24
DIR_SCRIPTS                     equ     25

; --- Misc constants
MB_INFORMATION                  equ     MB_ICONINFORMATION + MB_OK
MB_QUESTION                     equ     MB_ICONQUESTION + MB_YESNO
MB_ERRORQUESTION                equ     MB_ICONERROR + MB_YESNO
MB_ERROR                        equ     MB_ICONERROR + MB_OK
MB_QUESTIONCANCEL               equ     MB_ICONQUESTION + MB_YESNOCANCEL

NOT_CASE_SENSITIVE              equ     0
CASE_SENSITIVE                  equ     1

CONVERTDB_CONTINUE              equ     1
CONVERTDB_REQUESTABORT          equ     0
CONVERTDB_DONE                  equ     0
CONVERTDB_USERABORT             equ     1
CONVERTDB_ERROR                 equ     -1

MULTIFILESENUM_FORWARD          equ     0
MULTIFILESENUM_BACKWARD         equ     1

PANELS_MINBASE                  equ     1000

OSTYPE_WINDOWS_9X               equ     0
OSTYPE_WINDOWS_NT               equ     1
OSTYPE_WINDOWS_2K               equ     2
OSTYPE_WINDOWS_XP               equ     4
OSTYPE_WINDOWS_7                equ     8

USE_SECURITY_DESCRIPTOR         equ     1
NO_SECURITY_DESCRIPTOR          equ     0

DATE_GET_SECONDS                equ     1
DATE_NO_SECONDS                 equ     0

; --- Treeview popup menu constants
MENU_POP_NONE                   equ     0
MENU_POP_PROJECT                equ     1
MENU_POP_NEWINCLUDE             equ     2
MENU_POP_NEWLIBRARY             equ     3
MENU_POP_NEWMODULE              equ     4
MENU_POP_NEWOBJECT              equ     5
MENU_POP_NEWRESOURCE            equ     6
MENU_POP_NEWDEF                 equ     7
MENU_POP_NEWTEXT                equ     8
MENU_POP_INCLUDEDETAILS         equ     9
MENU_POP_LIBRARYDETAILS         equ     10
MENU_POP_MODULEDETAILS          equ     11
MENU_POP_OBJECTDETAILS          equ     12
MENU_POP_RESOURCEDETAILS        equ     13
MENU_POP_DEFDETAILS             equ     14
MENU_POP_TEXTDETAILS            equ     15

MENU_POP_NEWICON                equ     16
MENU_POP_NEWCURSOR              equ     17
MENU_POP_NEWBITMAP              equ     18
MENU_POP_NEWSTRINGS             equ     19
MENU_POP_NEWACCELERATORS        equ     20
MENU_POP_NEWMENUS               equ     21
MENU_POP_NEWDIALOG              equ     22
MENU_POP_NEWRAWDATA             equ     23

MENU_POP_ICONDETAILS            equ     24
MENU_POP_CURSORDETAILS          equ     25
MENU_POP_BITMAPDETAILS          equ     26
MENU_POP_STRINGDETAILS          equ     27
MENU_POP_ACCELERATORDETAILS     equ     28
MENU_POP_MENUDETAILS            equ     29
MENU_POP_DIALOGDETAILS          equ     30
MENU_POP_RAWDATADETAILS         equ     31

; --- Outbox contextual menu
MENU_OUTBOX_SAVE_ID             equ     1
MENU_OUTBOX_NEW_ID              equ     2
MENU_OUTBOX_EMPTY_ID            equ     3
MENU_OUTBOX_GOTO_ID             equ     4

; --- Debugbox contextual menu
MENU_DEBUGBOX_SAVE_ID           equ     1
MENU_DEBUGBOX_NEW_ID            equ     2
MENU_DEBUGBOX_EMPTY_ID          equ     3
MENU_DEBUGBOX_GOTO_ID           equ     4

; --- Windowsbox contextual menu
MENU_WINDOWSBOX_CLOSE_ID        equ     1
MENU_WINDOWSBOX_SAVE_ID         equ     2
MENU_WINDOWSBOX_SAVEAS_ID       equ     3
MENU_WINDOWSBOX_TOGGLE_ID       equ     4

; --- Other interface constants
ACCEL_BASE                      equ     9000
MENU_SEPARATORS_ID              equ     -1

MENU_RECENTFILES_IDBASE         equ     3000
MENU_RECENTPROJECTS_IDBASE      equ     3500
MENU_RECENTAPPLICATIONS_IDBASE  equ     3700
MENU_RECENTFILESPURGEALL_IDBASE equ     3996
MENU_RECENTPROJECTSPURGEALL_IDBASE equ  3997
MENU_RECENTFILESPURGE_IDBASE    equ     3998
MENU_RECENTPROJECTSPURGE_IDBASE equ     3999
MENU_CONTEXT_IDBASE             equ     5000
MENU_FILE_IDBASE                equ     4000
MENU_EDIT_IDBASE                equ     4500
MENU_VIEW_IDBASE                equ     5000
MENU_PROJECT_IDBASE             equ     5500
MENU_WINDOW_IDBASE              equ     6000
MENU_USER_IDBASE                equ     7000
MENU_MAXIMA_IDBASE              equ     8000
MENU_SEPS_IDBASE                equ     9999
MENU_SNIPPET_IDBASE             equ     10000
MENU_TEMPLATES_IDBASE           equ     10000
MENU_CODEMAXIMA_IDBASE          equ     11000
MENU_OUTBOX_IDBASE              equ     12000
MENU_DEBUGBOX_IDBASE            equ     12500
MENU_WINDOWSBOX_IDBASE          equ     12600
MENU_SCRIPTINSERT_IDBASE        equ     13000
SYSTRAY_MSG                     equ     14000
MENU_SYSTRAY_RESTORE            equ     14001
MENU_SYSTRAY_EXIT               equ     14002
MENU_ADDINS_IDBASE              equ     16000
MENU_POP_BASEID                 equ     4000

MENU_FILE_NEW_ID                equ     0
MENU_FILE_CLEAR_ID              equ     1
MENU_FILE_DUPLICATEFILE_ID      equ     2
MENU_FILE_OPENFILE_ID           equ     3
MENU_FILE_FILTERS_ID            equ     4
MENU_FILE_OPENWORKSPACE_ID      equ     5
MENU_FILE_INCLUDEFILE_ID        equ     6
MENU_FILE_FTPMANAGER_ID         equ     7
MENU_FILE_CLOSE_ID              equ     8
MENU_FILE_CLOSEALL_ID           equ     9
MENU_FILE_SAVE_ID               equ     10
MENU_FILE_SAVEAS_ID             equ     11
MENU_FILE_SAVEALL_ID            equ     12
MENU_FILE_SAVEWORKSPACE_ID      equ     13
MENU_FILE_REGISTERASSNIPPET_ID  equ     14
MENU_FILE_PRINTFILE_ID          equ     15
MENU_FILE_CONSOLE_RUN_ID        equ     16
MENU_FILE_CONSOLE_CHANGE_ID     equ     17
MENU_FILE_RUNEXTERNALPROGRAM_ID equ     18
MENU_FILE_SETWORKINGDIRECTORY_ID equ    19
MENU_FILE_EXIT_ID               equ     20
MENU_FILE_RELOADFILE_ID         equ     21
MENU_FILE_INCLUDEFILTERS_ID     equ     22
MENU_FILE_REGISTERASTEMPLATE_ID equ     23

MENU_EDIT_UNDO_ID               equ     0
MENU_EDIT_REDO_ID               equ     1
MENU_EDIT_CUT_ID                equ     2
MENU_EDIT_COPY_ID               equ     3
MENU_EDIT_COPYFILEPATHNAME_ID   equ     4
MENU_EDIT_COPYENTIRETEXT_ID     equ     5
MENU_EDIT_PASTE_ID              equ     6
MENU_EDIT_DELETE_DELETESELECTION_ID equ 7
MENU_EDIT_DELETE_DELETELINE_ID  equ     8
MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID equ 9
MENU_EDIT_DELETE_DELETETOENDOFLINE_ID equ 10
MENU_EDIT_SELECTLINE_ID         equ     11
MENU_EDIT_SELECTALL_ID          equ     12
MENU_EDIT_SELECTPROCEDURE_ID    equ     13
MENU_EDIT_SELECTPROCANDCOLLAPSE_ID equ  14
MENU_EDIT_UNCOLLAPSEPROCEDURE_ID equ    15
MENU_EDIT_FIND_ID               equ     16
MENU_EDIT_FINDSELECTION_ID      equ     17
MENU_EDIT_FINDNEXT_ID           equ     18
MENU_EDIT_FINDPREVIOUS_ID       equ     19
MENU_EDIT_REPLACE_ID            equ     20
MENU_EDIT_PROCVARIABLESSEARCH_ID equ    21
MENU_EDIT_SEARCHINFILES_ID      equ     22
MENU_EDIT_BOOKMARKS_TOGGLE_ID   equ     23
MENU_EDIT_BOOKMARKS_FIRST_ID    equ     24
MENU_EDIT_BOOKMARKS_PREVIOUS_ID equ     25
MENU_EDIT_BOOKMARKS_NEXT_ID     equ     26
MENU_EDIT_BOOKMARKS_LAST_ID     equ     27
MENU_EDIT_BOOKMARKS_CLEARALL_ID equ     28
MENU_EDIT_GOTO_TOP_ID           equ     29
MENU_EDIT_GOTO_BOTTOM_ID        equ     30
MENU_EDIT_GOTO_LINE_ID          equ     31
MENU_EDIT_GOTO_MATCHINGBRACKET_ID equ   32
MENU_EDIT_GOTO_PREVIOUSWORD_ID  equ     33
MENU_EDIT_GOTO_NEXTWORD_ID      equ     34
MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID equ 35
MENU_EDIT_GOTO_NEXTPROCEDURE_ID equ     36
MENU_EDIT_GOTO_VARIABLEDECLARATION_ID equ 37
MENU_EDIT_GOTO_VARIABLENEXTUSE_ID equ   38
MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID equ 39
MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID equ 40
MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID equ 41
MENU_EDIT_BLOCK_INDENT_ID       equ     42
MENU_EDIT_BLOCK_UNINDENT_ID     equ     43
MENU_EDIT_BLOCK_UPPERCASE_ID    equ     44
MENU_EDIT_BLOCK_LOWERCASE_ID    equ     45
MENU_EDIT_BLOCK_COMMENT_ID      equ     46
MENU_EDIT_BLOCK_UNCOMMENT_ID    equ     47
MENU_EDIT_BLOCK_TABIFY_ID       equ     48
MENU_EDIT_BLOCK_UNTABIFY_ID     equ     49
MENU_EDIT_BLOCK_REGISTERS_ID    equ     50
MENU_EDIT_BLOCK_SAVEAS_ID       equ     51
MENU_EDIT_BLOCK_PRINT_ID        equ     52
MENU_EDIT_BLOCK_BLKCOMMENT_ID   equ     53
MENU_EDIT_BLOCK_TOGGLECASE_ID   equ     54
MENU_EDIT_BLOCK_SPACESTOTABS_ID equ     55
MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID equ 56
MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID equ    57
MENU_EDIT_PASTE_NEWWINDOW_ID    equ     58
MENU_EDIT_DELETE_DELETEPARAGRAPH_ID     equ 59
MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID     equ 60
MENU_EDIT_GOTO_NEXTPARAGRAPH_ID equ     61
MENU_EDIT_BLOCK_TRIMSPACES_ID   equ     62
MENU_EDIT_BREAKPOINTS_TOGGLE_ID equ     63
MENU_EDIT_BREAKPOINTS_FIRST_ID  equ     64
MENU_EDIT_BREAKPOINTS_PREVIOUS_ID equ   65
MENU_EDIT_BREAKPOINTS_NEXT_ID   equ     66
MENU_EDIT_BREAKPOINTS_LAST_ID   equ     67
MENU_EDIT_BREAKPOINTS_CLEARALL_ID equ   68

MENU_VIEW_STATUSBAR_ID          equ     0
MENU_VIEW_PROJECTDOCK_ID        equ     1
MENU_VIEW_WINDOWSDOCK_ID        equ     2
MENU_VIEW_OUTPUTDOCK_ID         equ     3
MENU_VIEW_DEBUGDOCK_ID          equ     4
MENU_VIEW_REGISTERSDOCK_ID      equ     5
MENU_VIEW_PROPERTIES_ID         equ     6
MENU_VIEW_USERMENUSEDITOR_ID    equ     7
MENU_VIEW_USERTOOLBARSEDITOR_ID equ     8
MENU_VIEW_FULLSCREEN_ID         equ     9

MENU_PROJECT_CREATENEWPROJECT_ID equ    0
MENU_PROJECT_OPENPROJECT_ID     equ     1
MENU_PROJECT_SAVEPROJECT_ID     equ     2
MENU_PROJECT_SAVEPROJECTAS_ID   equ     3
MENU_PROJECT_ADDINCLUDE_ID      equ     4
MENU_PROJECT_ADDLIBRARY_ID      equ     5
MENU_PROJECT_ADDMODULE_ID       equ     6
MENU_PROJECT_ADDOBJECT_ID       equ     7
MENU_PROJECT_ADDRESOURCE_ID     equ     8
MENU_PROJECT_ADDTEXT_ID         equ     9
MENU_PROJECT_ADDICON_ID         equ     10
MENU_PROJECT_ADDCURSOR_ID       equ     11
MENU_PROJECT_ADDBITMAP_ID       equ     12
MENU_PROJECT_ADDSTRINGS_ID      equ     13
MENU_PROJECT_ADDACCELERATORS_ID equ     14
MENU_PROJECT_ADDMENUS_ID        equ     15
MENU_PROJECT_ADDDIALOG_ID       equ     16
MENU_PROJECT_ADDRAWDATA_ID      equ     17
MENU_PROJECT_PROJECTPROPERTIES_ID equ   18
MENU_PROJECT_ASSEMBLEPROJECT_ID equ     19
MENU_PROJECT_RUNPROJECT_ID      equ     20
MENU_PROJECT_DEBUGPROJECT_ID    equ     21

MENU_ADDINS_MANAGER_ID          equ     0
MENU_ADDINS_FIRSTADDIN_ID       equ     1

MENU_WINDOW_CONTEXTHELP_ID      equ     0
MENU_WINDOW_MSDNHELP_ID         equ     1
MENU_WINDOW_ABOUT_ID            equ     2
MENU_WINDOW_SPLIT_ID            equ     3
MENU_WINDOW_TILEHORIZONTALLY_ID equ     4
MENU_WINDOW_TILEVERTICALLY_ID   equ     5
MENU_WINDOW_CASCADE_ID          equ     6
MENU_WINDOW_ARRANGEICONS_ID     equ     7
MENU_WINDOW_NEXTWINDOW_ID       equ     8
MENU_WINDOW_PREVIOUSWINDOW_ID   equ     9
MENU_WINDOW_DDKHELP_ID          equ     10
MENU_WINDOW_TIPOFTHEDAY_ID      equ     11

PROJECTTOOLBAR_SAVE             equ     1
PROJECTTOOLBAR_ASSEMBLE         equ     2
PROJECTTOOLBAR_RUN              equ     3
PROJECTTOOLBAR_DEBUG            equ     4

WINDOWSTOOLBAR_PREVIOUS         equ     5
WINDOWSTOOLBAR_NEXT             equ     6
WINDOWSTOOLBAR_RELOAD           equ     7

DEBUGTOOLBAR_RUN                equ     1
DEBUGTOOLBAR_PAUSE              equ     2
DEBUGTOOLBAR_STOP               equ     3
DEBUGTOOLBAR_STEPINTO           equ     4
DEBUGTOOLBAR_STEPOVER           equ     5
DEBUGTOOLBAR_STEPOUT            equ     6

STATUSTOOLBAR_SAVE              equ     1
STATUSTOOLBAR_NEW               equ     2
STATUSTOOLBAR_CLEAR             equ     3

MENU_CONTEXT_CONTEXTHELP_ID     equ     0
MENU_CONTEXT_MSDNHELP_ID        equ     1
MENU_CONTEXT_DDKHELP_ID         equ     2
MENU_CONTEXT_DIRECTXSDKHELP_ID  equ     3
MENU_CONTEXT_OPENFILEUNDERCURSOR_ID equ 4
MENU_CONTEXT_EXECFILEUNDERCURSOR_ID equ 5
MENU_CONTEXT_DECTOHEX_ID        equ     6
MENU_CONTEXT_DECTOBIN_ID        equ     7
MENU_CONTEXT_HEXTODEC_ID        equ     8
MENU_CONTEXT_HEXTOBIN_ID        equ     9
MENU_CONTEXT_BINTODEC_ID        equ     10
MENU_CONTEXT_BINTOHEX_ID        equ     11
MENU_CONTEXT_HEXTOFLT_ID        equ     12
MENU_CONTEXT_FLTTOHEX_ID        equ     13
MENU_CONTEXT_UNDO_ID            equ     14
MENU_CONTEXT_REDO_ID            equ     15
MENU_CONTEXT_CUT_ID             equ     16
MENU_CONTEXT_COPY_ID            equ     17
MENU_CONTEXT_COPYFILEPATHNAME_ID equ    18
MENU_CONTEXT_COPYENTIRETEXT_ID  equ     19
MENU_CONTEXT_PASTE_ID           equ     20
MENU_CONTEXT_PASTENEWWINDOW_ID  equ     21
MENU_CONTEXT_SELECTLINE_ID      equ     22
MENU_CONTEXT_SELECTALL_ID       equ     23
MENU_CONTEXT_SELECTPROCEDURE_ID equ     24
MENU_CONTEXT_SELECTPROCEDUREANDCOLLAPSE_ID equ 25
MENU_CONTEXT_UNCOLLAPSEPROCEDURE_ID equ 26
MENU_CONTEXT_FIND_ID            equ     27
MENU_CONTEXT_REPLACE_ID         equ     28
MENU_CONTEXT_BOOKMARKS_TOGGLE_ID equ 29
MENU_CONTEXT_BOOKMARKS_FIRST_ID equ     30
MENU_CONTEXT_BOOKMARKS_PREVIOUS_ID equ 31
MENU_CONTEXT_BOOKMARKS_NEXT_ID  equ     32
MENU_CONTEXT_BOOKMARKS_LAST_ID  equ     33
MENU_CONTEXT_BOOKMARKS_CLEARALL_ID equ 34
MENU_CONTEXT_BREAKPOINTS_TOGGLE_ID equ 35
MENU_CONTEXT_BREAKPOINTS_FIRST_ID equ   36
MENU_CONTEXT_BREAKPOINTS_PREVIOUS_ID equ 37
MENU_CONTEXT_BREAKPOINTS_NEXT_ID equ    38
MENU_CONTEXT_BREAKPOINTS_LAST_ID equ    39
MENU_CONTEXT_BREAKPOINTS_CLEARALL_ID equ 40
MENU_CONTEXT_GOTO_TOP_ID        equ     41
MENU_CONTEXT_GOTO_BOTTOM_ID     equ     42
MENU_CONTEXT_GOTO_LINE_ID       equ     43
MENU_CONTEXT_GOTO_MATCHINGBRACKET_ID equ 44
MENU_CONTEXT_GOTO_PREVIOUSWORD_ID equ 45
MENU_CONTEXT_GOTO_NEXTWORD_ID   equ     46
MENU_CONTEXT_GOTO_PREVIOUSPROCEDURE_ID equ 47
MENU_CONTEXT_GOTO_NEXTPROCEDURE_ID equ 48
MENU_CONTEXT_GOTO_PREVIOUSPARAGRAPH_ID  equ 49
MENU_CONTEXT_GOTO_NEXTPARAGRAPH_ID      equ 50
MENU_CONTEXT_GOTO_VARIABLEDECLARATION_ID equ 51
MENU_CONTEXT_GOTO_VARIABLENEXTUSE_ID equ 52
MENU_CONTEXT_GOTO_PROCEDUREPROTOTYPE_ID equ 53
MENU_CONTEXT_GOTO_PROCEDUREENTRYPOINT_ID equ 54
MENU_CONTEXT_GOTO_PROCEDURENEXTUSE_ID equ 55
MENU_CONTEXT_BLOCK_INDENT_ID    equ     56
MENU_CONTEXT_BLOCK_UNINDENT_ID  equ     57
MENU_CONTEXT_BLOCK_UPPERCASE_ID equ     58
MENU_CONTEXT_BLOCK_LOWERCASE_ID equ     59
MENU_CONTEXT_BLOCK_TOGGLECASE_ID equ    60
MENU_CONTEXT_BLOCK_COMMENT_ID   equ     61
MENU_CONTEXT_BLOCK_UNCOMMENT_ID equ     62
MENU_CONTEXT_BLOCK_TABIFY_ID    equ     63
MENU_CONTEXT_BLOCK_UNTABIFY_ID  equ     64
MENU_CONTEXT_BLOCK_SPACESTOTABS_ID equ  65
MENU_CONTEXT_BLOCK_REGISTERS_ID equ     66
MENU_CONTEXT_BLOCK_BLKCOMMENT_ID equ    67
MENU_CONTEXT_BLOCK_CUSTOMCOMMENT_ID equ 68
MENU_CONTEXT_BLOCK_TRIMSPACES_ID equ    69
MENU_CONTEXT_BLOCK_CONVERTTOINCLUDE_ID equ 70
MENU_CONTEXT_BLOCK_SAVEAS_ID    equ     71
MENU_CONTEXT_BLOCK_PRINT_ID     equ     72
MENU_CONTEXT_NEW_ID             equ     73
MENU_CONTEXT_INCLUDEFILE_ID     equ     74
MENU_CONTEXT_INCLUDEFILEASDB_ID equ     75
MENU_CONTEXT_DUPLICATEFILE_ID   equ     76
MENU_CONTEXT_RELOADFILE_ID      equ     77
MENU_CONTEXT_PROPERTIES_ID      equ     78

; --- Main icons
ICON_BASE                       equ     1000
ICON_ASKDIR                     equ     0
ICON_ASM                        equ     1
ICON_ASSEMBLE                   equ     2
ICON_CASCADE                    equ     3
ICON_CLOSE                      equ     4
ICON_CLOSEWIN                   equ     5
ICON_COLORS                     equ     6
ICON_COMMENT                    equ     7
ICON_COMPONENT                  equ     8
ICON_CONSOLE                    equ     9
ICON_COPY                       equ     10
ICON_CUBES                      equ     11
ICON_CUT                        equ     12
ICON_DEBUG                      equ     13
ICON_DELETE                     equ     14
ICON_DELSTATUS                  equ     15
ICON_DIRPERMS                   equ     16
ICON_DIRUP                      equ     17
ICON_DIRVIEW                    equ     18
ICON_DOWNLOAD                   equ     19
ICON_EARTH                      equ     20
ICON_EXEC                       equ     21
ICON_FILEASSOC                  equ     22
ICON_FILES                      equ     23
ICON_FIND                       equ     24
ICON_FINDNEXT                   equ     25
ICON_FINDPREV                   equ     26
ICON_FTPUSER                    equ     27
ICON_GETREG                     equ     28
ICON_HELPFILES                  equ     29
ICON_INDENT                     equ     30
ICON_INSERT                     equ     31
ICON_INTERNET                   equ     32
ICON_MAIL                       equ     33
ICON_MAINICON                   equ     34
ICON_KEYBOARD                   equ     35
ICON_MENUED                     equ     36
ICON_MSDOS                      equ     37
ICON_NEW                        equ     38
ICON_NEWDIR                     equ     39
ICON_NEWFTP                     equ     40
ICON_OPEN                       equ     41
ICON_OUTDENT                    equ     42
ICON_PASTE                      equ     43
ICON_PAUSE                      equ     44
ICON_STACK                      equ     45
ICON_PLUG                       equ     46
ICON_PRINT                      equ     47
ICON_PROPERTIES                 equ     48
ICON_REDO                       equ     49
ICON_REFRESH                    equ     50
ICON_RENAMEDIR                  equ     51
ICON_REPLACE                    equ     52
ICON_RES                        equ     53
ICON_RUNPROG                    equ     54
ICON_SAVE                       equ     55
ICON_SAVELITTLE                 equ     56
ICON_SETBOOK                    equ     57
ICON_SNAP                       equ     58
ICON_SPLIT                      equ     59
ICON_STATIC                     equ     60
ICON_STEP                       equ     61
ICON_STOP                       equ     62
ICON_STOREREG                   equ     63
ICON_TILEHORZ                   equ     64
ICON_TILEVERT                   equ     65
ICON_TOOLBARS                   equ     66
ICON_TOWIN                      equ     67
ICON_UNCOMMENT                  equ     68
ICON_UNDO                       equ     69
ICON_UPLOAD                     equ     70
ICON_USER                       equ     71
ICON_WINDOWS                    equ     72
ICON_YIN                        equ     73
ICON_LAB                        equ     74
ICON_WINDOW                     equ     75
ICON_KEY                        equ     76
ICON_KEYREPLACE                 equ     77
ICON_SERV                       equ     78
ICON_POPUP                      equ     79
ICON_ITEM                       equ     80
ICON_SEPARATOR                  equ     81
ICON_UP                         equ     82
ICON_DOWN                       equ     83
ICON_DIRECTX                    equ     84
ICON_CUSTTOOLBARS               equ     85
ICON_ADDINR                     equ     86
ICON_ADDINS                     equ     87
ICON_PANEL                      equ     88
ICON_EDITDOC                    equ     89
ICON_STAR                       equ     90
ICON_RELOAD                     equ     91
ICON_STEPBACK                   equ     92
ICON_WINDOWNORM                 equ     93
ICON_WINDOWMODIF                equ     94
ICON_FULLSCREEN                 equ     95
ICON_RUNPROJECT                 equ     96
ICON_DEBUGPROJECT               equ     97
ICON_BUILDPROJECT               equ     98
ICON_ADDACTIVE                  equ     99
ICON_ADDALLACTIVE               equ     100
ICON_ADDDIRECTORY               equ     101
ICON_PROCESS                    equ     102
ICON_PAUSELITTLE                equ     103
ICON_STOPLITTLE                 equ     104
ICON_THREAD                     equ     105
ICON_NEXTWINDOW                 equ     106
ICON_PREVWINDOW                 equ     107
ICON_FIRSTBOOK                  equ     108
ICON_PREVBOOK                   equ     109
ICON_NEXTBOOK                   equ     110
ICON_LASTBOOK                   equ     111
ICON_CLEARBOOK                  equ     112
ICON_ADDFILES                   equ     113
ICON_WINDOWLOCK                 equ     114
ICON_DIRSEARCH                  equ     115
ICON_ADDLIBRARY                 equ     116
ICON_ADDRESOURCE                equ     117
ICON_MENUDROPDOWN               equ     118
ICON_LIGHT                      equ     119
ICON_HEARTOFGOLD                equ     120
ICON_HEARTOFGOLDPLUS            equ     121
ICON_ADDFILTER                  equ     122
ICON_TOWINDOW                   equ     123
ICON_SETBREAK                   equ     124
ICON_CLEARBREAK                 equ     125
ICON_NEXTBREAK                  equ     126
ICON_PREVBREAK                  equ     127
ICON_FIRSTBREAK                 equ     128
ICON_LASTBREAK                  equ     129
ICON_STEPINTO                   equ     130
ICON_STEPOVER                   equ     131
ICON_STEPOUT                    equ     132
ICON_ADDCONST                   equ     133
ICON_STATUSBAR                  equ     134
ICON_LOWERCASE                  equ     135
ICON_UPPERCASE                  equ     136
ICON_REPLACECONST               equ     137
ICON_AUDIO                      equ     138
ICON_OBJECT                     equ     139
ICON_ADDOBJECT                  equ     140
ICON_OPENGL                     equ     141
ICON_REGEAX                     equ     142
ICON_REGEBX                     equ     143
ICON_REGECX                     equ     144
ICON_REGEDX                     equ     145
ICON_REGESI                     equ     146
ICON_REGEDI                     equ     147
ICON_REGEIP                     equ     148
ICON_REGESP                     equ     149
ICON_REGEBP                     equ     150
ICON_REGEFL                     equ     151
ICON_REGCS                      equ     152
ICON_REGDS                      equ     153
ICON_REGES                      equ     154
ICON_REGFS                      equ     155
ICON_REGGS                      equ     156
ICON_REGSS                      equ     157
ICON_REGST0                     equ     158
ICON_REGST1                     equ     159
ICON_REGST2                     equ     160
ICON_REGST3                     equ     161
ICON_REGST4                     equ     162
ICON_REGST5                     equ     163
ICON_REGST6                     equ     164
ICON_REGST7                     equ     165
ICON_REGFPUCTRL                 equ     166
ICON_REGFPUSTAT                 equ     167
ICON_REGFPUTAGS                 equ     168
ICON_REGFPUEDO                  equ     169
ICON_REGMM0                     equ     170
ICON_REGMM1                     equ     171
ICON_REGMM2                     equ     172
ICON_REGMM3                     equ     173
ICON_REGMM4                     equ     174
ICON_REGMM5                     equ     175
ICON_REGMM6                     equ     176
ICON_REGMM7                     equ     177
ICON_REGXMM0                    equ     178
ICON_REGXMM1                    equ     179
ICON_REGXMM2                    equ     180
ICON_REGXMM3                    equ     181
ICON_REGXMM4                    equ     182
ICON_REGXMM5                    equ     183
ICON_REGXMM6                    equ     184
ICON_REGXMM7                    equ     185
ICON_REGMXCSR                   equ     186

; --- Cursors
CURSOR_BASE                     equ     1950
CURSOR_SPLITH                   equ     0
CURSOR_SPLITV                   equ     1
CURSOR_DRAG                     equ     2

; --- Little icons (hLittleImageList)
ICON_SMALL_ASSEMBLE             equ     0
ICON_SMALL_SAVELITTLE           equ     1
ICON_SMALL_EXEC                 equ     2
ICON_SMALL_DEBUG                equ     3
ICON_SMALL_CLOSEWIN             equ     4
ICON_SMALL_TOWIN                equ     5
ICON_SMALL_DELSTATUS            equ     6
ICON_SMALL_RELOAD               equ     7
ICON_SMALL_BACK                 equ     8
ICON_SMALL_PAUSELITTLE          equ     9
ICON_SMALL_STOPLITTLE           equ     10

; --- Resources controls icons
ICON_RES_BASE                   equ     2500
ICON_RES_ANIMATION              equ     0
ICON_RES_BUTTON                 equ     1
ICON_RES_CALENDAR               equ     2
ICON_RES_CHECKBOX               equ     3
ICON_RES_COMBOBOX               equ     4
ICON_RES_CURSOR                 equ     5
ICON_RES_DATEPICKER             equ     6
ICON_RES_FRAME                  equ     7
ICON_RES_HOTKEY                 equ     8
ICON_RES_HSCROLLBAR             equ     9
ICON_RES_LABEL                  equ     10
ICON_RES_LISTBOX                equ     11
ICON_RES_LISTVIEW               equ     12
ICON_RES_PICTURE                equ     13
ICON_RES_PROGRESSBAR            equ     14
ICON_RES_RADIOBUTTON            equ     15
ICON_RES_SLIDER                 equ     16
ICON_RES_SPIN                   equ     17
ICON_RES_SYSTAB                 equ     18
ICON_RES_TEXT                   equ     19
ICON_RES_TEXTIP                 equ     20
ICON_RES_TREEVIEW               equ     21
ICON_RES_VSCROLLBAR             equ     22
ICON_RES_XCOMBOBOX              equ     23
ICON_RES_VERTSLIDER             equ     24
ICON_RES_TOOLBAR                equ     25
ICON_RES_STATUSBAR              equ     26
ICON_RES_RICHTEXT               equ     27
ICON_RES_PAGER                  equ     28
ICON_RES_HEADER                 equ     29
ICON_RES_REBAR                  equ     30
ICON_RES_TOOLTIP                equ     31

; --- Resources tools icons
ICON_RES_TOOL_BASE              equ     2700
ICON_RES_TOOL_LOCKCTRL          equ     0
ICON_RES_TOOL_ALIGNBOTTOM       equ     1
ICON_RES_TOOL_ALIGNLEFT         equ     2
ICON_RES_TOOL_ALIGNRIGHT        equ     3
ICON_RES_TOOL_ALIGNTOP          equ     4
ICON_RES_TOOL_CENTERHORZ        equ     5
ICON_RES_TOOL_CENTERVERT        equ     6
ICON_RES_TOOL_GRID              equ     7
ICON_RES_TOOL_SAMEHEIGHT        equ     8
ICON_RES_TOOL_SAMESIZE          equ     9
ICON_RES_TOOL_SAMEWIDTH         equ     10
ICON_RES_TOOL_SPACEACROSS       equ     11
ICON_RES_TOOL_SPACEDOWN         equ     12
ICON_RES_TOOL_ALIGNGRID         equ     13
ICON_RES_TOOL_TEST              equ     14
ICON_RES_TOOL_TOOLBAR           equ     15
ICON_RES_TOOL_PROPERTIES        equ     16

; --- Stock dialogs constants
MODALDLG_STOCK_EMPTY            equ     0
MODALDLG_STOCK_STDCLOSE         equ     1
MODALDLG_STOCK_STDOKCANCEL      equ     2
MODALDLG_STOCK_WIZARD           equ     3

WIZARD_STOCK_STDX               equ     560
WIZARD_STOCK_STDY               equ     359
WIZARD_STOCK_PICTUREX           equ     200

BUTTON_STOCK_CLOSE_ID           equ     1
BUTTON_STOCK_OK_ID              equ     1
BUTTON_STOCK_FINISH_ID          equ     1
BUTTON_STOCK_CANCEL_ID          equ     2
BUTTON_STOCK_PREVIOUS_ID        equ     3
BUTTON_STOCK_NEXT_ID            equ     4

; ---- CMaxXX constants
CM_MAX_FINDREPL_TEXT            equ     100
CM_FIND_REPLACE_MRU_MAX         equ     10
CM_FIND_REPLACE_MRU_BUFF_SIZE   equ     ((CM_MAX_FINDREPL_TEXT + 1) * CM_FIND_REPLACE_MRU_MAX)
CM_MAX_CMD_STRING               equ     50
CM_MAX_CMD_DESCRIPTION          equ     100
CM_MAX_LANGUAGE_NAME            equ     30
CM_MAX_TABSIZE                  equ     100
CM_MIN_TABSIZE                  equ     2
CM_CXLEFTMARGIN                 equ     24
CML_OWNERDRAW                   equ     1
CML_NOTIFY_DEL                  equ     2

CMM_SETLANGUAGE                 equ     (WM_USER + 1600)
CMM_GETLANGUAGE                 equ     (WM_USER + 1601)
CMM_ENABLECOLORSYNTAX           equ     (WM_USER + 1610)
CMM_ISCOLORSYNTAXENABLED        equ     (WM_USER + 1620)
CMM_SETCOLORS                   equ     (WM_USER + 1630)
CMM_GETCOLORS                   equ     (WM_USER + 1640)
CMM_ENABLEWHITESPACEDISPLAY     equ     (WM_USER + 1800)
CMM_ISWHITESPACEDISPLAYENABLED  equ     (WM_USER + 1810)
CMM_ENABLETABEXPAND             equ     (WM_USER + 1811)
CMM_ISTABEXPANDENABLED          equ     (WM_USER + 1812)
CMM_ENABLESMOOTHSCROLLING       equ     (WM_USER + 1820)
CMM_SETTABSIZE                  equ     (WM_USER + 1821)
CMM_GETTABSIZE                  equ     (WM_USER + 1822)
CMM_ISSMOOTHSCROLLINGENABLED    equ     (WM_USER + 1830)
CMM_SETREADONLY                 equ     (WM_USER + 1840)
CMM_ISREADONLY                  equ     (WM_USER + 1850)
CMM_ENABLELINETOOLTIPS          equ     (WM_USER + 1860)
CMM_ISLINETOOLTIPSENABLED       equ     (WM_USER + 1870)
CMM_ENABLELEFTMARGIN            equ     (WM_USER + 1880)
CMM_ISLEFTMARGINENABLED         equ     (WM_USER + 1890)
CMM_ENABLECOLUMNSEL             equ     (WM_USER + 1891)
CMM_ISCOLUMNSELENABLED          equ     (WM_USER + 1892)
CMM_ENABLEDRAGDROP              equ     (WM_USER + 1893)
CMM_ISDRAGDROPENABLED           equ     (WM_USER + 1894)
CMM_ENABLEOVERTYPE              equ     (WM_USER + 1900)
CMM_ISOVERTYPEENABLED           equ     (WM_USER + 1910)
CMM_ENABLECASESENSITIVE         equ     (WM_USER + 1920)
CMM_ISCASESENSITIVEENABLED      equ     (WM_USER + 1930)
CMM_ENABLEPRESERVECASE          equ     (WM_USER + 1931)
CMM_ISPRESERVECASEENABLED       equ     (WM_USER + 1932)
CMM_ENABLEWHOLEWORD             equ     (WM_USER + 1940)
CMM_ISWHOLEWORDENABLED          equ     (WM_USER + 1950)
CMM_SETTOPINDEX                 equ     (WM_USER + 1960)
CMM_GETTOPINDEX                 equ     (WM_USER + 1970)
CMM_GETVISIBLELINECOUNT         equ     (WM_USER + 1980)
CMM_HITTEST                     equ     (WM_USER + 1990)
CMM_OPENFILE                    equ     (WM_USER + 2300)
CMM_SAVEFILE                    equ     (WM_USER + 2310)
CMM_INSERTFILE                  equ     (WM_USER + 2320)
CMM_INSERTTEXT                  equ     (WM_USER + 2330)
CMM_SETTEXT                     equ     (WM_USER + 2335)
CMM_REPLACETEXT                 equ     (WM_USER + 2340)
CMM_GETTEXTLENGTH               equ     (WM_USER + 2350)
CMM_GETTEXT                     equ     (WM_USER + 2360)
CMM_GETLINECOUNT                equ     (WM_USER + 2365)
CMM_GETLINE                     equ     (WM_USER + 2370)
CMM_GETLINELENGTH               equ     (WM_USER + 2380)
CMM_GETWORD                     equ     (WM_USER + 2381)
CMM_GETWORDLENGTH               equ     (WM_USER + 2382)
CMM_ADDTEXT                     equ     (WM_USER + 2390)
CMM_DELETELINE                  equ     (WM_USER + 2400)
CMM_INSERTLINE                  equ     (WM_USER + 2410)
CMM_GETSEL                      equ     (WM_USER + 2420)
CMM_GETSELFROMPOINT             equ     (WM_USER + 2425)
CMM_SETSEL                      equ     (WM_USER + 2430)
CMM_SELECTLINE                  equ     (WM_USER + 2435)
CMM_DELETESEL                   equ     (WM_USER + 2440)
CMM_REPLACESEL                  equ     (WM_USER + 2450)
CMM_ISMODIFIED                  equ     (WM_USER + 2460)
CMM_SETMODIFIED                 equ     (WM_USER + 2461)
CMM_ENABLECRLF                  equ     (WM_USER + 2470)
CMM_ISCRLFENABLED               equ     (WM_USER + 2480)
CMM_SETFONTOWNERSHIP            equ     (WM_USER + 2485)
CMM_GETFONTOWNERSHIP            equ     (WM_USER + 2486)
CMM_EXECUTECMD                  equ     (WM_USER + 2700)
CMM_SETSPLITTERPOS              equ     (WM_USER + 2900)
CMM_GETSPLITTERPOS              equ     (WM_USER + 2901)
CMM_SETAUTOINDENTMODE           equ     (WM_USER + 3100)
CMM_GETAUTOINDENTMODE           equ     (WM_USER + 3110)
CMM_CANUNDO                     equ     (WM_USER + 3300)
CMM_CANREDO                     equ     (WM_USER + 3310)
CMM_CANCUT                      equ     (WM_USER + 3320)
CMM_CANCOPY                     equ     (WM_USER + 3330)
CMM_CANPASTE                    equ     (WM_USER + 3340)
CMM_REDO                        equ     (WM_USER + 3350)
CMM_CLEARUNDOBUFFER             equ     (WM_USER + 3351)
CMM_UNDO                        equ     (WM_USER + 3360)
CMM_CUT                         equ     (WM_USER + 3370)
CMM_COPY                        equ     (WM_USER + 3380)
CMM_PASTE                       equ     (WM_USER + 3390)
CMM_SETUNDOLIMIT                equ     (WM_USER + 3400)
CMM_GETUNDOLIMIT                equ     (WM_USER + 3410)
CMM_GETVIEWCOUNT                equ     (WM_USER + 3600)
CMM_GETCURRENTVIEW              equ     (WM_USER + 3610)
CMM_SHOWSCROLLBAR               equ     (WM_USER + 3700)
CMM_HASSCROLLBAR                equ     (WM_USER + 3710)
CMM_ENABLESPLITTER              equ     (WM_USER + 3720)
CMM_ISSPLITTERENABLED           equ     (WM_USER + 3730)
CMM_ENABLEGLOBALPROPS           equ     (WM_USER + 3740)
CMM_ISGLOBALPROPSENABLED        equ     (WM_USER + 3741)
CMM_SETDLGPARENT                equ     (WM_USER + 3750)
CMM_ENABLESELBOUNDS             equ     (WM_USER + 3760)
CMM_ISSELBOUNDSENABLED          equ     (WM_USER + 3770)
CMM_SETFONTSTYLES               equ     (WM_USER + 3780)
CMM_GETFONTSTYLES               equ     (WM_USER + 3790)
CMM_ENABLEREGEXP                equ     (WM_USER + 3800)
CMM_ISREGEXPENABLED             equ     (WM_USER + 3810)
CMM_SETITEMDATA                 equ     (WM_USER + 3820)
CMM_GETITEMDATA                 equ     (WM_USER + 3830)
CMM_SETLINESTYLE                equ     (WM_USER + 3840)
CMM_GETLINESTYLE                equ     (WM_USER + 3850)
CMM_SETBOOKMARK                 equ     (WM_USER + 3860)
CMM_GETBOOKMARK                 equ     (WM_USER + 3870)
CMM_SETALLBOOKMARKS             equ     (WM_USER + 3880)
CMM_GETALLBOOKMARKS             equ     (WM_USER + 3890)
CMM_SETBREAKPOINT               equ     (WM_USER + 3900)
CMM_GETBREAKPOINT               equ     (WM_USER + 3910)
CMM_SETALLBREAKPOINTS           equ     (WM_USER + 3920)
CMM_GETALLBREAKPOINTS           equ     (WM_USER + 3930)
CMM_SETLINENUMBERING            equ     (WM_USER + 3940)
CMM_GETLINENUMBERING            equ     (WM_USER + 3950)
CMM_POSFROMCHAR                 equ     (WM_USER + 3960)
CMM_ENABLEHIDESEL               equ     (WM_USER + 3970)
CMM_ISHIDESELENABLED            equ     (WM_USER + 3980)
CMM_SETHIGHLIGHTEDLINE          equ     (WM_USER + 3990)
CMM_GETHIGHLIGHTEDLINE          equ     (WM_USER + 4000)
CMM_ENABLENORMALIZECASE         equ     (WM_USER + 4010)
CMM_ISNORMALIZECASEENABLED      equ     (WM_USER + 4020)
CMM_SETDIVIDER                  equ     (WM_USER + 4030)
CMM_GETDIVIDER                  equ     (WM_USER + 4040)
CMM_ENABLEOVERTYPECARET         equ     (WM_USER + 4050)
CMM_ISOVERTYPECARETENABLED      equ     (WM_USER + 4060)
CMM_SETFINDTEXT                 equ     (WM_USER + 4070)
CMM_GETFINDTEXT                 equ     (WM_USER + 4080)
CMM_SETREPLACETEXT              equ     (WM_USER + 4090)
CMM_GETREPLACETEXT              equ     (WM_USER + 4100)
CMM_SETIMAGELIST                equ     (WM_USER + 4110)
CMM_GETIMAGELIST                equ     (WM_USER + 4120)
CMM_SETMARGINIMAGES             equ     (WM_USER + 4130)
CMM_GETMARGINIMAGES             equ     (WM_USER + 4140)
CMM_PRINT                       equ     (WM_USER + 4150)
CMM_SETCARETPOS                 equ     (WM_USER + 4160)
CMM_VIEWCOLTOBUFFERCOL          equ     (WM_USER + 4170)
CMM_BUFFERCOLTOVIEWCOL          equ     (WM_USER + 4180)
CMM_SETBORDERSTYLE              equ     (WM_USER + 4190)
CMM_GETBORDERSTYLE              equ     (WM_USER + 4200)
CMM_SETCURRENTVIEW              equ     (WM_USER + 4210)
CMM_REPLACELINE                 equ     (WM_USER + 4220)
CMM_SETTOKENCALLBACK            equ     (WM_USER + 4230)
CMM_ENABLEALLOPENED             equ     (WM_USER + 4240)

CMN_CHANGE                      equ     100
CMN_HSCROLL                     equ     110
CMN_VSCROLL                     equ     120
CMN_SELCHANGE                   equ     130
CMN_VIEWCHANGE                  equ     140
CMN_MODIFIEDCHANGE              equ     150
CMN_SHOWPROPS                   equ     160
CMN_PROPSCHANGE                 equ     170
CMN_CREATE                      equ     180
CMN_DESTROY                     equ     190
CMN_DRAWLINE                    equ     200
CMN_DELETELINE                  equ     210
CMN_CMDFAILURE                  equ     220
CMN_REGISTEREDCMD               equ     230
CMN_KEYDOWN                     equ     240
CMN_KEYUP                       equ     250
CMN_KEYPRESS                    equ     260
CMN_MOUSEDOWN                   equ     270
CMN_MOUSEUP                     equ     280
CMN_MOUSEMOVE                   equ     290
CMN_OVERTYPECHANGE              equ     300
CMN_FINDWRAPPED                 equ     310

CMD_FIRST                       equ     100
CMD_WORDUPPERCASE               equ     (CMD_FIRST + 0)
CMD_WORDTRANSPOSE               equ     (CMD_FIRST + 1)
CMD_WORDRIGHTEXTEND             equ     (CMD_FIRST + 2)
CMD_WORDRIGHT                   equ     (CMD_FIRST + 3)
CMD_WORDENDRIGHT                equ     (CMD_FIRST + 4)
CMD_WORDENDRIGHTEXTEND          equ     (CMD_FIRST + 5)
CMD_WORDLOWERCASE               equ     (CMD_FIRST + 6)
CMD_WORDLEFTEXTEND              equ     (CMD_FIRST + 7)
CMD_WORDLEFT                    equ     (CMD_FIRST + 8)
CMD_WORDENDLEFT                 equ     (CMD_FIRST + 9)
CMD_WORDENDLEFTEXTEND           equ     (CMD_FIRST + 10)
CMD_WORDDELETETOSTART           equ     (CMD_FIRST + 11)
CMD_WORDDELETETOEND             equ     (CMD_FIRST + 12)
CMD_WORDCAPITALIZE              equ     (CMD_FIRST + 13)
CMD_WINDOWSTART                 equ     (CMD_FIRST + 14)
CMD_WINDOWSCROLLUP              equ     (CMD_FIRST + 15)
CMD_WINDOWSCROLLTOTOP           equ     (CMD_FIRST + 16)
CMD_WINDOWSCROLLTOCENTER        equ     (CMD_FIRST + 17)
CMD_WINDOWSCROLLTOBOTTOM        equ     (CMD_FIRST + 18)
CMD_WINDOWSCROLLRIGHT           equ     (CMD_FIRST + 19)
CMD_WINDOWSCROLLLEFT            equ     (CMD_FIRST + 20)
CMD_WINDOWSCROLLDOWN            equ     (CMD_FIRST + 21)
CMD_WINDOWRIGHTEDGE             equ     (CMD_FIRST + 22)
CMD_WINDOWLEFTEDGE              equ     (CMD_FIRST + 23)
CMD_WINDOWEND                   equ     (CMD_FIRST + 24)
CMD_UPPERCASESELECTION          equ     (CMD_FIRST + 25)
CMD_UNTABIFYSELECTION           equ     (CMD_FIRST + 26)
CMD_UNINDENTSELECTION           equ     (CMD_FIRST + 27)
CMD_UNDOCHANGES                 equ     (CMD_FIRST + 28)
CMD_UNDO                        equ     (CMD_FIRST + 29)
CMD_TABIFYSELECTION             equ     (CMD_FIRST + 30)
CMD_SENTENCERIGHT               equ     (CMD_FIRST + 31)
CMD_SENTENCELEFT                equ     (CMD_FIRST + 32)
CMD_SENTENCECUT                 equ     (CMD_FIRST + 33)
CMD_SELECTSWAPANCHOR            equ     (CMD_FIRST + 34)
CMD_SELECTPARA                  equ     (CMD_FIRST + 35)
CMD_SELECTLINE                  equ     (CMD_FIRST + 36)
CMD_SELECTALL                   equ     (CMD_FIRST + 37)
CMD_REDOCHANGES                 equ     (CMD_FIRST + 38)
CMD_REDO                        equ     (CMD_FIRST + 39)
CMD_PASTE                       equ     (CMD_FIRST + 40)
CMD_PARAUP                      equ     (CMD_FIRST + 41)
CMD_PARADOWN                    equ     (CMD_FIRST + 42)
CMD_PAGEUPEXTEND                equ     (CMD_FIRST + 43)
CMD_PAGEUP                      equ     (CMD_FIRST + 44)
CMD_PAGEDOWNEXTEND              equ     (CMD_FIRST + 45)
CMD_PAGEDOWN                    equ     (CMD_FIRST + 46)
CMD_LOWERCASESELECTION          equ     (CMD_FIRST + 47)
CMD_LINEUPEXTEND                equ     (CMD_FIRST + 48)
CMD_LINEUP                      equ     (CMD_FIRST + 49)
CMD_LINETRANSPOSE               equ     (CMD_FIRST + 50)
CMD_LINESTART                   equ     (CMD_FIRST + 51)
CMD_LINEOPENBELOW               equ     (CMD_FIRST + 52)
CMD_LINEOPENABOVE               equ     (CMD_FIRST + 53)
CMD_LINEENDEXTEND               equ     (CMD_FIRST + 54)
CMD_LINEEND                     equ     (CMD_FIRST + 55)
CMD_LINEDOWNEXTEND              equ     (CMD_FIRST + 56)
CMD_LINEDOWN                    equ     (CMD_FIRST + 57)
CMD_LINEDELETETOSTART           equ     (CMD_FIRST + 58)
CMD_LINEDELETETOEND             equ     (CMD_FIRST + 59)
CMD_LINEDELETE                  equ     (CMD_FIRST + 60)
CMD_LINECUT                     equ     (CMD_FIRST + 61)
CMD_INDENTTOPREV                equ     (CMD_FIRST + 62)
CMD_INDENTSELECTION             equ     (CMD_FIRST + 63)
CMD_HOMEEXTEND                  equ     (CMD_FIRST + 64)
CMD_HOME                        equ     (CMD_FIRST + 65)
CMD_GOTOMATCHBRACE              equ     (CMD_FIRST + 66)
CMD_GOTOINDENTATION             equ     (CMD_FIRST + 67)
CMD_GOTOLINE                    equ     (CMD_FIRST + 68)
CMD_FINDREPLACE                 equ     (CMD_FIRST + 69)
CMD_REPLACE                     equ     (CMD_FIRST + 70)
CMD_REPLACEALLINBUFFER          equ     (CMD_FIRST + 71)
CMD_REPLACEALLINSELECTION       equ     (CMD_FIRST + 72)
CMD_FINDPREVWORD                equ     (CMD_FIRST + 73)
CMD_FINDPREV                    equ     (CMD_FIRST + 74)
CMD_FINDNEXTWORD                equ     (CMD_FIRST + 75)
CMD_FINDNEXT                    equ     (CMD_FIRST + 76)
CMD_FINDMARKALL                 equ     (CMD_FIRST + 77)
CMD_FIND                        equ     (CMD_FIRST + 78)
CMD_SETFINDTEXT                 equ     (CMD_FIRST + 79)
CMD_SETREPLACETEXT              equ     (CMD_FIRST + 80)
CMD_TOGGLEPRESERVECASE          equ     (CMD_FIRST + 81)
CMD_TOGGLEWHOLEWORD             equ     (CMD_FIRST + 82)
CMD_TOGGLECASESENSITIVE         equ     (CMD_FIRST + 83)
CMD_END                         equ     (CMD_FIRST + 84)
CMD_TOGGLEWHITESPACEDISPLAY     equ     (CMD_FIRST + 85)
CMD_TOGGLEOVERTYPE              equ     (CMD_FIRST + 86)
CMD_SETREPEATCOUNT              equ     (CMD_FIRST + 87)
CMD_DOCUMENTSTARTEXTEND         equ     (CMD_FIRST + 88)
CMD_DOCUMENTSTART               equ     (CMD_FIRST + 89)
CMD_DOCUMENTENDEXTEND           equ     (CMD_FIRST + 90)
CMD_DOCUMENTEND                 equ     (CMD_FIRST + 91)
CMD_DELETEHORIZONTALSPACE       equ     (CMD_FIRST + 92)
CMD_DELETEBLANKLINES            equ     (CMD_FIRST + 93)
CMD_DELETEBACK                  equ     (CMD_FIRST + 94)
CMD_DELETE                      equ     (CMD_FIRST + 95)
CMD_CUTSELECTION                equ     (CMD_FIRST + 96)
CMD_CUT                         equ     (CMD_FIRST + 97)
CMD_COPY                        equ     (CMD_FIRST + 98)
CMD_CHARTRANSPOSE               equ     (CMD_FIRST + 99)
CMD_CHARRIGHTEXTEND             equ     (CMD_FIRST + 100)
CMD_CHARRIGHT                   equ     (CMD_FIRST + 101)
CMD_CHARLEFTEXTEND              equ     (CMD_FIRST + 102)
CMD_CHARLEFT                    equ     (CMD_FIRST + 103)
CMD_BOOKMARKTOGGLE              equ     (CMD_FIRST + 104)
CMD_BOOKMARKPREV                equ     (CMD_FIRST + 105)
CMD_BOOKMARKNEXT                equ     (CMD_FIRST + 106)
CMD_BOOKMARKCLEARALL            equ     (CMD_FIRST + 107)
CMD_BOOKMARKJUMPTOFIRST         equ     (CMD_FIRST + 108)
CMD_BOOKMARKJUMPTOLAST          equ     (CMD_FIRST + 109)
CMD_BREAKPOINTTOGGLE            equ     (CMD_FIRST + 110)
CMD_BREAKPOINTPREV              equ     (CMD_FIRST + 111)
CMD_BREAKPOINTNEXT              equ     (CMD_FIRST + 112)
CMD_BREAKPOINTCLEARALL          equ     (CMD_FIRST + 113)
CMD_BREAKPOINTJUMPTOFIRST       equ     (CMD_FIRST + 114)
CMD_BREAKPOINTJUMPTOLAST        equ     (CMD_FIRST + 115)
CMD_APPENDNEXTCUT               equ     (CMD_FIRST + 116)
CMD_INSERTCHAR                  equ     (CMD_FIRST + 117)
CMD_NEWLINE                     equ     (CMD_FIRST + 118)
CMD_PROPERTIES                  equ     (CMD_FIRST + 119)
CMD_BEGINUNDO                   equ     (CMD_FIRST + 120)
CMD_ENDUNDO                     equ     (CMD_FIRST + 121)
CMD_RESERVED3                   equ     (CMD_FIRST + 122)
CMD_TOGGLEREGEXP                equ     (CMD_FIRST + 123)
CMD_CLEARSELECTION              equ     (CMD_FIRST + 124)
CMD_REGEXPON                    equ     (CMD_FIRST + 125)
CMD_REGEXPOFF                   equ     (CMD_FIRST + 126)
CMD_WHOLEWORDON                 equ     (CMD_FIRST + 127)
CMD_WHOLEWORDOFF                equ     (CMD_FIRST + 128)
CMD_PRESERVECASEON              equ     (CMD_FIRST + 129)
CMD_PRESERVECASEOFF             equ     (CMD_FIRST + 130)
CMD_CASESENSITIVEON             equ     (CMD_FIRST + 131)
CMD_CASESENSITIVEOFF            equ     (CMD_FIRST + 132)
CMD_WHITESPACEDISPLAYON         equ     (CMD_FIRST + 133)
CMD_WHITESPACEDISPLAYOFF        equ     (CMD_FIRST + 134)
CMD_OVERTYPEON                  equ     (CMD_FIRST + 135)
CMD_OVERTYPEOFF                 equ     (CMD_FIRST + 136)
CMD_ALLOPENED                   equ     (CMD_FIRST + 137)
CMD_LAST                        equ     (CMD_FIRST + 138)

CMD_USER_BASE                   equ     (CMD_FIRST + 900)

CMDERR_FAILURE                  equ     1
CMDERR_INPUT                    equ     2
CMDERR_SELECTION                equ     3
CMDERR_NOTFOUND                 equ     4
CMDERR_EMPTYBUF                 equ     5
CMDERR_READONLY                 equ     6

CMLS_PROCEDURAL                 equ     0
CMLS_SGML                       equ     1

CM_FONT_NORMAL                  equ     0
CM_FONT_BOLD                    equ     1
CM_FONT_ITALIC                  equ     2
CM_FONT_BOLDITALIC              equ     3
CM_FONT_UNDERLINE               equ     4

CM_INDENT_OFF                   equ     0
CM_INDENT_SCOPE                 equ     1
CM_INDENT_PREVLINE              equ     2

CM_PRINT_PROMPTDLG              equ     0x0
CM_PRINT_DEFAULTPRN             equ     0x1
CM_PRINT_HDC                    equ     0x2
CM_PRINT_RICHFONTS              equ     0x4
CM_PRINT_COLOR                  equ     0x8
CM_PRINT_PAGENUMS               equ     0x10
CM_PRINT_DATETIME               equ     0x20
CM_PRINT_BORDERTHIN             equ     0x40
CM_PRINT_BORDERTHICK            equ     0x80
CM_PRINT_BORDERDOUBLE           equ     0x100
CM_PRINT_SELECTION              equ     0x200

CM_BORDER_NONE                  equ     0
CM_BORDER_THIN                  equ     1
CM_BORDER_CLIENT                equ     2
CM_BORDER_STATIC                equ     4
CM_BORDER_MODAL                 equ     8
CM_BORDER_CORRAL                equ     (CM_BORDER_MODAL Or CM_BORDER_CLIENT)

CM_KEY_SHIFT                    equ     0x1
CM_KEY_CTRL                     equ     0x2
CM_KEY_ALT                      equ     0x4

CM_BTN_LEFT                     equ     0x1
CM_BTN_RIGHT                    equ     0x2
CM_BTN_MIDDLE                   equ     0x4

CM_BINARY                       equ     2
CM_OCTAL                        equ     8
CM_DECIMAL                      equ     10
CM_HEXADECIMAL                  equ     16

CME_SUCCESS                     equ     1
CME_FAILURE                     equ     0
CME_BADARGUMENT                 equ     -1

CM_NOWHERE                      equ     0
CM_HSPLITTER                    equ     1
CM_VSPLITTER                    equ     2
CM_HVSPLITTER                   equ     3
CM_EDITSPACE                    equ     4
CM_HSCROLLBAR                   equ     5
CM_VSCROLLBAR                   equ     6
CM_SIZEBOX                      equ     7
CM_LEFTMARGIN                   equ     8

; ---- AddIns Structs
ADDINCONTEXT                    STRUCT
Version                         dd      ?
NbrChilds                       dd      ?
ActiveProject                   dd      ?
hInstance                       dd      ?
OSType                          dd      ?
FullScreenMode                  dd      ?
SysTrayMode                     dd      ?
hMDI                            dd      ?
hClient                         dd      ?
hAccelerators                   dd      ?
hProjectTreeView                dd      ?
hWindowsList                    dd      ?
hStatusBar                      dd      ?
hOutputBox                      dd      ?
hDebugBox                       dd      ?
hOutputSysTab                   dd      ?
hCurrentChild                   dd      ?
hCurrentCodeMax                 dd      ?
hRebar                          dd      ?
hEmptyImageList                 dd      ?
hMainImageList                  dd      ?
hLittleImageList                dd      ?
hControlsImageList              dd      ?
hDialogEdImageList              dd      ?
MDIhWndProc                     dd      ?
ChildCreateProc                 dd      ?
ChildhWndProc                   dd      ?
CodeMaxhWndProc                 dd      ?
hProjectToolbar                 dd      ?
hProjectPager                   dd      ?
hWindowsToolbar                 dd      ?
hWindowsPager                   dd      ?
hOutputBoxToolbar               dd      ?
hOutputBoxPager                 dd      ?
ColdStart                       dd      ?
ADDINCONTEXT                    ENDS

MINICHILDDAT                    STRUCT
hChildCodeMax                   dd      ?
FileName                        dd      ?
Language                        dd      ?
ReadOnly                        dd      ?
FileLoaded                      dd      ?
ModifFlag                       dd      ?
MINICHILDDAT                    ENDS

SPLITTERDAT                     STRUCT
hClient                         dd      ?
hparent                         dd      ?
Orientation                     dd      ?
Sizing                          dd      ?
MinLeftTop                      dd      ?
MaxRightBottom                  dd      ?
SplitBarSize                    dd      ?
StartupPos                      dd      ?
OldXPos                         dd      ?
OldYPos                         dd      ?
OldWndProc                      dd      ?
StartDragX                      dd      ?
StartDragY                      dd      ?
DisplacmentX                    dd      ?
DisplacmentY                    dd      ?
SPLITTERDAT                     ENDS

GRIPBOXDAT                      STRUCT
TopLeftBox                      dd      ?
TopBox                          dd      ?
TopRightBox                     dd      ?
BottomLeftBox                   dd      ?
BottomBox                       dd      ?
BottomRightBox                  dd      ?
LeftBox                         dd      ?
RightBox                        dd      ?
GRIPBOXDAT                      ENDS

BACKDC                          STRUCT
hWnd                            dd      ?
Color                           dd      ?
hDC                             dd      ?
Bitmap                          dd      ?
OldObject                       dd      ?
BACKDC                          ENDS

; ---- Library served functions
CHROMATICLIB                    STRUCT
; Standard functions
LIBPROTO                        CreateSplashDialog,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateModalDialog,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateNonModalDialog,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateSplitter,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateMDIDialog,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateMDIChildDialog,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateToolBar,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateImageList,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateSysTab,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateTrackBar,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateUpDown,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateProgressBar,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateStatusBar,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateColorBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateGripBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateLabel,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreatePictureBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateFrame,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateButton,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateIPBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateRichTextBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateHexBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateCheckBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateRebar,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreatePager,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateClient,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateRadioButton,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateListView,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateTreeView,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateComboBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateListBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateScrollBar,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateDumpBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateStockModalDialog,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateTextBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateDockingBox,:DWORD,:DWORD
LIBPROTO                        DialogSetIcon,:DWORD,:DWORD
LIBPROTO                        DialogGetXUnit
LIBPROTO                        DialogGetYUnit
LIBPROTO                        DialogXPixelToUnit,:DWORD,:DWORD
LIBPROTO                        DialogYPixelToUnit,:DWORD,:DWORD
LIBPROTO                        DialogXUnitToPixel,:DWORD,:DWORD
LIBPROTO                        DialogYUnitToPixel,:DWORD,:DWORD
LIBPROTO                        ToolBarAddButton,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarGetButton,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarGetButtonXSize,:DWORD
LIBPROTO                        ToolBarGetButtonYSize,:DWORD
LIBPROTO                        ToolBarGetButtonsCount,:DWORD
LIBPROTO                        ToolBarGetRowsCount,:DWORD
LIBPROTO                        ToolBarGetImagelist,:DWORD
LIBPROTO                        ToolBarAddSeparator,:DWORD,:DWORD
LIBPROTO                        ToolBarGetNotifiedToolTip,:DWORD
LIBPROTO                        ToolBarGetNotifiedDropDownItem,:DWORD
LIBPROTO                        ToolBarDisplayToolTip,:DWORD,:DWORD
LIBPROTO                        ToolBarResize,:DWORD
LIBPROTO                        ToolBarSetButtonPressed,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarSetButtonChecked,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarSetButtonEnabled,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarIsButtonPressed,:DWORD,:DWORD
LIBPROTO                        ToolBarIsButtonChecked,:DWORD,:DWORD
LIBPROTO                        ToolBarDisplayPopupMenu,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarGetXSize,:DWORD
LIBPROTO                        ToolBarGetYSize,:DWORD
LIBPROTO                        ToolBarGetButtonXPosition,:DWORD,:DWORD
LIBPROTO                        ToolBarGetXYSize,:DWORD,:DWORD
LIBPROTO                        ToolBarGetRealPos,:DWORD,:DWORD
LIBPROTO                        ToolBarGetButtonXYPos,:DWORD,:DWORD,:DWORD
LIBPROTO                        ToolBarGetButtonIndexXSize,:DWORD,:DWORD
LIBPROTO                        ToolBarGetXPadding,:DWORD
LIBPROTO                        ToolBarGetYPadding,:DWORD
LIBPROTO                        ToolBarGetNotifiedHotItem,:DWORD
LIBPROTO                        ToolBarRemoveButton,:DWORD,:DWORD
LIBPROTO                        ToolBarSetButtonBitmap,:DWORD,:DWORD,:DWORD
LIBPROTO                        ImageListDestroy,:DWORD
LIBPROTO                        ImageListAddIcon,:DWORD,:DWORD
LIBPROTO                        ImageListGetIconsCount,:DWORD
LIBPROTO                        ImageListGetIcon,:DWORD,:DWORD
LIBPROTO                        SysTabAddItem,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        SysTabGetCurrentItem,:DWORD
LIBPROTO                        SysTabGetImagelist,:DWORD
LIBPROTO                        SysTabSetCurrentItem,:DWORD,:DWORD
LIBPROTO                        SysTabSetFocusItem,:DWORD,:DWORD
LIBPROTO                        SysTabHighLightItem,:DWORD,:DWORD,:DWORD
LIBPROTO                        SysTabItemsCount,:DWORD
LIBPROTO                        SysTabGetRowsCount,:DWORD
LIBPROTO                        SysTabSetSeparators,:DWORD,:DWORD
LIBPROTO                        SysTabSetItemsSize,:DWORD,:DWORD,:DWORD
LIBPROTO                        SysTabRemoveItem,:DWORD,:DWORD
LIBPROTO                        SysTabSetMinWidth,:DWORD,:DWORD
LIBPROTO                        SysTabSetPadding,:DWORD,:DWORD,:DWORD
LIBPROTO                        TrackBarSetRange,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        TrackBarSetPos,:DWORD,:DWORD
LIBPROTO                        TrackBarGetPos,:DWORD
LIBPROTO                        TrackBarSetTicks,:DWORD,:DWORD
LIBPROTO                        TrackBarGetToolTips,:DWORD
LIBPROTO                        ProgressBarSetRange,:DWORD,:DWORD,:DWORD
LIBPROTO                        ProgressBarSetRelPos,:DWORD,:DWORD
LIBPROTO                        StatusBarSetParts,:DWORD,:DWORD,:DWORD
LIBPROTO                        StatusBarSetText,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StatusBarGetYSize,:DWORD
LIBPROTO                        ColorBoxGetColor,:DWORD
LIBPROTO                        ColorBoxSetColor,:DWORD,:DWORD
LIBPROTO                        GripBoxGetColor,:DWORD
LIBPROTO                        GripBoxSetColor,:DWORD,:DWORD
LIBPROTO                        GripBoxClose,:DWORD
LIBPROTO                        GripBoxVisible,:DWORD,:DWORD
LIBPROTO                        GripBoxResize,:DWORD,:DWORD
LIBPROTO                        LabelGetImage,:DWORD,:DWORD
LIBPROTO                        PictureBoxChangeIcon,:DWORD,:DWORD
LIBPROTO                        PictureBoxChangeImage,:DWORD,:DWORD
LIBPROTO                        FrameRefreshToolbar,:DWORD
LIBPROTO                        ButtonSetIcon,:DWORD,:DWORD,:DWORD
LIBPROTO                        ButtonGetImage,:DWORD,:DWORD
LIBPROTO                        ButtonGetState,:DWORD
LIBPROTO                        IPBoxClearIP,:DWORD
LIBPROTO                        IPBoxGetIP,:DWORD
LIBPROTO                        IPBoxSetIP,:DWORD,:DWORD
LIBPROTO                        IPBoxSetFocus,:DWORD,:DWORD
LIBPROTO                        IPBoxSetRange,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        IPBoxIsBlank,:DWORD
LIBPROTO                        TextBoxGetLen,:DWORD
LIBPROTO                        TextBoxSetMaxLen,:DWORD,:DWORD
LIBPROTO                        TextBoxGetMaxLen,:DWORD
LIBPROTO                        TextBoxGetTextHandle,:DWORD
LIBPROTO                        TextBoxAddText,:DWORD,:DWORD
LIBPROTO                        TextBoxSelText,:DWORD,:DWORD
LIBPROTO                        TextBoxRemoveSel,:DWORD
LIBPROTO                        TextBoxSetCaretPos,:DWORD,:DWORD
LIBPROTO                        TextBoxLinesCount,:DWORD
LIBPROTO                        TextBoxGetMarginWidth,:DWORD
LIBPROTO                        TextBoxGetFirstVisibleLine,:DWORD
LIBPROTO                        TextBoxIsModified,:DWORD
LIBPROTO                        TextBoxGetCurrentOptions,:DWORD
LIBPROTO                        TextBoxGetPasswordChar,:DWORD
LIBPROTO                        TextBoxSetPasswordChar,:DWORD,:DWORD
LIBPROTO                        TextBoxGetSelection,:DWORD
LIBPROTO                        TextBoxGetScrollPos,:DWORD
LIBPROTO                        TextBoxGetBreakProcAddress,:DWORD
LIBPROTO                        TextBoxCanPaste,:DWORD
LIBPROTO                        TextBoxCanUndo,:DWORD
LIBPROTO                        CheckBoxGetState,:DWORD
LIBPROTO                        CheckBoxSetState,:DWORD,:DWORD
LIBPROTO                        RebarAddBand,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        RebarRemoveBand,:DWORD,:DWORD
LIBPROTO                        RebarGetXSize,:DWORD
LIBPROTO                        RebarGetYSize,:DWORD
LIBPROTO                        RebarResize,:DWORD
LIBPROTO                        RebarSetBackColor,:DWORD,:DWORD
LIBPROTO                        RebarSetTextColor,:DWORD,:DWORD
LIBPROTO                        RebarBandVisible,:DWORD,:DWORD,:DWORD
LIBPROTO                        RebarGetBandsCount,:DWORD
LIBPROTO                        RebarGetHeight,:DWORD
LIBPROTO                        RebarGetBackGroundColor,:DWORD
LIBPROTO                        RebarGetRowsCount,:DWORD
LIBPROTO                        RebarGetToolTips,:DWORD
LIBPROTO                        RebarGetTextColor,:DWORD
LIBPROTO                        RebarGetPalette,:DWORD
LIBPROTO                        RebarGetImageList,:DWORD
LIBPROTO                        PagerDisplaySetSize,:DWORD,:DWORD
LIBPROTO                        PagerMaximizeX,:DWORD,:DWORD
LIBPROTO                        PagerMaximizeY,:DWORD,:DWORD
LIBPROTO                        PagerGetNotifiedChild,:DWORD
LIBPROTO                        PagerSetBackColor,:DWORD,:DWORD
LIBPROTO                        PagerGetBackColor,:DWORD
LIBPROTO                        PagerGetBorderSize,:DWORD
LIBPROTO                        PagerGetButtonSize,:DWORD
LIBPROTO                        PagerGetScrollPos,:DWORD
LIBPROTO                        PagerGetOrientation,:DWORD
LIBPROTO                        PagerSetOrientation,:DWORD,:DWORD
LIBPROTO                        PagerChangeOrientation,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ClientResize,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ClientGetActiveChild,:DWORD
LIBPROTO                        ClientSetNextChild,:DWORD
LIBPROTO                        ClientSetPreviousChild,:DWORD
LIBPROTO                        ClientTileHorizontal,:DWORD
LIBPROTO                        ClientTileVertical,:DWORD
LIBPROTO                        ClientTileCascade,:DWORD
LIBPROTO                        ClientTileArrangeIconic,:DWORD
LIBPROTO                        RadioButtonGetState,:DWORD
LIBPROTO                        RadioButtonSetState,:DWORD,:DWORD
LIBPROTO                        ListViewAddCol,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewSetColWidth,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewGetColWidth,:DWORD,:DWORD
LIBPROTO                        ListViewSetScroll,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewClear,:DWORD
LIBPROTO                        ListViewDetachImageList,:DWORD
LIBPROTO                        ListViewAddItem,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewDeleteItem,:DWORD,:DWORD
LIBPROTO                        ListViewSetSubItem,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewSetSubItemImage,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewGetSubItemImage,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewEditValidated,:DWORD
LIBPROTO                        ListViewGetEditResult,:DWORD
LIBPROTO                        ListViewItemCount,:DWORD
LIBPROTO                        ListViewSelItemCount,:DWORD
LIBPROTO                        ListViewMultiSelFirstItem,:DWORD
LIBPROTO                        ListViewGetItemSelState,:DWORD,:DWORD
LIBPROTO                        ListViewSetEditModeOn,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewSetItemSel,:DWORD,:DWORD
LIBPROTO                        ListViewSetItemHighlight,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewSetItemFocus,:DWORD,:DWORD
LIBPROTO                        ListViewGetItemUnderCursor,:DWORD
LIBPROTO                        ListViewGetFirstFocusItem,:DWORD
LIBPROTO                        ListViewGetSelItem,:DWORD,:DWORD
LIBPROTO                        ListViewGetSelItemText,:DWORD,:DWORD
LIBPROTO                        ListViewSetSelItemText,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewCheckBoxItemDoubleClick,:DWORD
LIBPROTO                        ListViewGetItemText,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewGetItemIcon,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewGetNotifiedColumnIndex,:DWORD
LIBPROTO                        ListViewSort,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewReOrder,:DWORD
LIBPROTO                        ListViewGetHeaderLabel,:DWORD,:DWORD
LIBPROTO                        ListViewGetHeaderPosition,:DWORD,:DWORD
LIBPROTO                        ListViewSetHeaderPosition,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewSetBackColor,:DWORD,:DWORD
LIBPROTO                        ListViewGetBackColor,:DWORD
LIBPROTO                        ListViewGetTextBackColor,:DWORD
LIBPROTO                        ListViewGetTextColor,:DWORD
LIBPROTO                        ListViewGetToolTips,:DWORD
LIBPROTO                        ListViewGetTopIndex,:DWORD
LIBPROTO                        ListViewGetCountPerPage,:DWORD
LIBPROTO                        ListViewGetEditControl,:DWORD
LIBPROTO                        ListViewGetExStyle,:DWORD
LIBPROTO                        ListViewGetSysHeader,:DWORD
LIBPROTO                        ListViewGetHotCursor,:DWORD
LIBPROTO                        ListViewGetImageList,:DWORD,:DWORD
LIBPROTO                        ListViewSetItemCheckbox,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewGetItemCheckbox,:DWORD,:DWORD
LIBPROTO                        ListViewIsCheckboxNotify,:DWORD,:DWORD
LIBPROTO                        ListViewFindItem,:DWORD,:DWORD
LIBPROTO                        ListViewFindSubItem,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewBeginDrag,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListViewMoveDrag,:DWORD
LIBPROTO                        ListViewEndDrag,:DWORD
LIBPROTO                        ListViewPasteAutoEdit,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewAddItem,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewSetToolTips,:DWORD,:DWORD
LIBPROTO                        TreeViewGetToolTips,:DWORD
LIBPROTO                        TreeViewSetIndent,:DWORD,:DWORD
LIBPROTO                        TreeViewRemoveItem,:DWORD,:DWORD
LIBPROTO                        TreeViewSetBackColor,:DWORD,:DWORD
LIBPROTO                        TreeViewGetExpandingState,:DWORD
LIBPROTO                        TreeViewGetExpandingItem,:DWORD
LIBPROTO                        TreeViewGetDeletedItem,:DWORD
LIBPROTO                        TreeViewSetItemIcon,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewSetItemExpandedState,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewGetItemExpandedState,:DWORD,:DWORD
LIBPROTO                        TreeViewSetSingleExpandState,:DWORD,:DWORD
LIBPROTO                        TreeViewGetSelectedItem,:DWORD
LIBPROTO                        TreeViewGetItemFromPos,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewSetSelectedItem,:DWORD,:DWORD
LIBPROTO                        TreeViewGetItemText,:DWORD,:DWORD
LIBPROTO                        TreeViewSetItemText,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewGetItemParent,:DWORD,:DWORD
LIBPROTO                        TreeViewGetRoot,:DWORD
LIBPROTO                        TreeViewGetItemCount,:DWORD
LIBPROTO                        TreeViewGetVisibleCount,:DWORD
LIBPROTO                        TreeViewGetFirstItemChild,:DWORD,:DWORD
LIBPROTO                        TreeViewGetNextItem,:DWORD,:DWORD
LIBPROTO                        TreeViewGetPreviousItem,:DWORD,:DWORD
LIBPROTO                        TreeViewSearchItemText,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewSearchChildPartialText,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewGetChildItemsCount,:DWORD,:DWORD
LIBPROTO                        TreeViewGetSelChildItemPos,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewGetItemLevel,:DWORD,:DWORD
LIBPROTO                        TreeViewGetBackColor,:DWORD
LIBPROTO                        TreeViewGetEditControl,:DWORD
LIBPROTO                        TreeViewGetImageList,:DWORD,:DWORD
LIBPROTO                        TreeViewGetIndent,:DWORD
LIBPROTO                        TreeViewGetInsertMarkColor,:DWORD
LIBPROTO                        TreeViewGetLineColor,:DWORD
LIBPROTO                        TreeViewGetTextColor,:DWORD
LIBPROTO                        TreeViewGetItemHeight,:DWORD
LIBPROTO                        TreeViewGetScrollTime,:DWORD
LIBPROTO                        TreeViewGetItemTextRect,:DWORD,:DWORD,:DWORD
LIBPROTO                        TreeViewGetItemRect,:DWORD,:DWORD,:DWORD
LIBPROTO                        ComboBoxAddItem,:DWORD,:DWORD,:DWORD
LIBPROTO                        ComboBoxDeleteItem,:DWORD,:DWORD
LIBPROTO                        ComboBoxReset,:DWORD
LIBPROTO                        ComboBoxSetMaxLen,:DWORD,:DWORD
LIBPROTO                        ComboBoxCount,:DWORD
LIBPROTO                        ComboBoxGetIndex,:DWORD
LIBPROTO                        ComboBoxSetIndex,:DWORD,:DWORD
LIBPROTO                        ComboBoxGetItem,:DWORD,:DWORD
LIBPROTO                        ComboBoxGetCurrentItem,:DWORD
LIBPROTO                        ComboBoxItemExist,:DWORD,:DWORD
LIBPROTO                        ComboBoxGetItemData,:DWORD,:DWORD
LIBPROTO                        ComboBoxSaveInIniFile,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ComboBoxFillFromIniFile,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListBoxAddItem,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListBoxReset,:DWORD
LIBPROTO                        ListBoxSetHorzScrollWidth,:DWORD,:DWORD
LIBPROTO                        ListBoxSetColumnsWidth,:DWORD,:DWORD
LIBPROTO                        ListBoxCount,:DWORD
LIBPROTO                        ListBoxGetTopIndex,:DWORD
LIBPROTO                        ListBoxSetTopIndex,:DWORD,:DWORD
LIBPROTO                        ListBoxSetItemData,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListBoxGetItemData,:DWORD,:DWORD
LIBPROTO                        ListBoxIsItemSelected,:DWORD,:DWORD
LIBPROTO                        ListBoxGetSelItemIndex,:DWORD
LIBPROTO                        ListBoxSetIndex,:DWORD,:DWORD
LIBPROTO                        ListBoxGetCurrentItem,:DWORD
LIBPROTO                        ListBoxGetItem,:DWORD,:DWORD
LIBPROTO                        ListBoxDeleteItem,:DWORD,:DWORD
LIBPROTO                        ListBoxReplaceItem,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListBoxReplaceSelItem,:DWORD,:DWORD
LIBPROTO                        ListBoxProcessDrag,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ListBoxSelItemDown,:DWORD
LIBPROTO                        ListBoxSelItemUp,:DWORD
LIBPROTO                        ListBoxSelItemRemove,:DWORD
LIBPROTO                        ListBoxItemExist,:DWORD,:DWORD
LIBPROTO                        ListBoxItemUnderCursor,:DWORD
LIBPROTO                        ScrollBarGetMinRange,:DWORD,:DWORD
LIBPROTO                        ScrollBarGetMaxRange,:DWORD,:DWORD
LIBPROTO                        ScrollBarGetPageRange,:DWORD,:DWORD
LIBPROTO                        ScrollBarSetPageRange,:DWORD,:DWORD,:DWORD
LIBPROTO                        ScrollBarShowHide,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ScrollBarSetMinMaxRange,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        DumpBoxResize,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        DumpBoxScrollDown,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        DumpBoxScrollUp,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        DumpBoxGetVisibleLines,:DWORD,:DWORD,:DWORD
LIBPROTO                        DumpBoxGetVisibleColumns,:DWORD,:DWORD,:DWORD
LIBPROTO                        DockingBoxGetState,:DWORD
LIBPROTO                        DockingBoxRemove,:DWORD
LIBPROTO                        DockingBoxShow,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        DockingBoxWasVisible,:DWORD
LIBPROTO                        MenuGetString,:DWORD,:DWORD
LIBPROTO                        MenuEnable,:DWORD,:DWORD,:DWORD
LIBPROTO                        MenuSetDefaultItem,:DWORD,:DWORD
LIBPROTO                        MenuSetItemState,:DWORD,:DWORD,:DWORD
LIBPROTO                        MenuSetItemType,:DWORD,:DWORD,:DWORD
LIBPROTO                        MenuSetItemText,:DWORD,:DWORD,:DWORD
LIBPROTO                        MenuAddSeparator,:DWORD
LIBPROTO                        MenuAddString,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ScreenWidth
LIBPROTO                        ScreenHeight
LIBPROTO                        ScreenRectToClient,:DWORD,:DWORD
LIBPROTO                        CursorSetWait
LIBPROTO                        CursorSetNo
LIBPROTO                        CursorSetCross
LIBPROTO                        CursorSetNormal
LIBPROTO                        CursorDisable
LIBPROTO                        CursorEnable,:DWORD
LIBPROTO                        IniReadKey,:DWORD,:DWORD,:DWORD
LIBPROTO                        IniReadBoolKey,:DWORD,:DWORD,:DWORD
LIBPROTO                        IniWriteSection,:DWORD,:DWORD,:DWORD
LIBPROTO                        IniDeleteKey,:DWORD,:DWORD,:DWORD
LIBPROTO                        IniWriteKey,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        MathGenerateRandomNumber,:DWORD
LIBPROTO                        DateGetNow,:DWORD
LIBPROTO                        DateGetYear
LIBPROTO                        DateGetMonth
LIBPROTO                        DateGetDay
LIBPROTO                        DateGetHour
LIBPROTO                        DateGetMinutes
LIBPROTO                        DateGetSeconds
LIBPROTO                        DateGetFormat,:DWORD
LIBPROTO                        DateGetTimeFormat,:DWORD
LIBPROTO                        StringIsLabel,:DWORD,:DWORD
LIBPROTO                        StringIsHexaDecimal,:DWORD
LIBPROTO                        StringIsDecimal,:DWORD,:DWORD
LIBPROTO                        StringIsLabelChar,:DWORD
LIBPROTO                        StringIsDigitChar,:DWORD
LIBPROTO                        StringIsHexaChar,:DWORD
LIBPROTO                        StringIsAlphaChar,:DWORD
LIBPROTO                        StringIsBlankChar,:DWORD
LIBPROTO                        StringIsOpenBracketChar,:DWORD
LIBPROTO                        StringIsUnderLineChar,:DWORD
LIBPROTO                        StringIsDotChar,:DWORD
LIBPROTO                        StringIsCommaChar,:DWORD
LIBPROTO                        StringIsQuoteChar,:DWORD
LIBPROTO                        StringIsArobasChar,:DWORD
LIBPROTO                        StringIsSpaceChar,:DWORD
LIBPROTO                        StringIsEOL,:DWORD
LIBPROTO                        StringNumberComplement,:DWORD,:DWORD
LIBPROTO                        StringHexNumberComplement,:DWORD,:DWORD
LIBPROTO                        StringOctNumberComplement,:DWORD,:DWORD
LIBPROTO                        StringBinNumberComplement,:DWORD,:DWORD
LIBPROTO                        StringCount,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringReplace,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringHex32,:DWORD
LIBPROTO                        StringHex16,:DWORD
LIBPROTO                        StringHex8,:DWORD
LIBPROTO                        StringSplit,:DWORD,:DWORD
LIBPROTO                        StringGetSplitUBound,:DWORD
LIBPROTO                        StringGetSplitElement,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringReleaseSplit,:DWORD
LIBPROTO                        StringJoin,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringFree,:DWORD
LIBPROTO                        StringLen,:DWORD
LIBPROTO                        StringCat,:DWORD,:DWORD
LIBPROTO                        StringDecToString,:DWORD
LIBPROTO                        StringStringToDec,:DWORD
LIBPROTO                        StringStringToHex,:DWORD
LIBPROTO                        StringCountLines,:DWORD,:DWORD
LIBPROTO                        StringGetLinesOffsets,:DWORD
LIBPROTO                        StringFilterNonPrintableChars,:DWORD,:DWORD
LIBPROTO                        StringLeft,:DWORD,:DWORD
LIBPROTO                        StringRight,:DWORD,:DWORD
LIBPROTO                        StringTrim,:DWORD
LIBPROTO                        StringLTrim,:DWORD
LIBPROTO                        StringRTrim,:DWORD
LIBPROTO                        StringInstr,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringMid,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringUCase,:DWORD
LIBPROTO                        StringLCase,:DWORD
LIBPROTO                        StringReverse,:DWORD
LIBPROTO                        StringSpace,:DWORD
LIBPROTO                        StringString,:DWORD,:DWORD
LIBPROTO                        StringIsNumeric,:DWORD
LIBPROTO                        StringFillHexBytes,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringFillHexWords,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringFillHexDWords,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        StringFileNameToLabel,:DWORD
LIBPROTO                        FileGetWriteTime,:DWORD,:DWORD
LIBPROTO                        FileGetCreationTime,:DWORD,:DWORD
LIBPROTO                        FileGetAccessedTime,:DWORD,:DWORD
LIBPROTO                        FileGetExtension,:DWORD
LIBPROTO                        FileRemoveExtension,:DWORD
LIBPROTO                        FileReplaceExtension,:DWORD,:DWORD
LIBPROTO                        FileGetDirectory,:DWORD
LIBPROTO                        FileGetFileName,:DWORD
LIBPROTO                        FileOpenR,:DWORD
LIBPROTO                        FileOpenW,:DWORD,:DWORD
LIBPROTO                        FileOpenWAppend,:DWORD
LIBPROTO                        FileClose,:DWORD
LIBPROTO                        FileCreateEmpty,:DWORD,:DWORD
LIBPROTO                        FileWriteBuffer,:DWORD,:DWORD,:DWORD
LIBPROTO                        FileWriteLine,:DWORD,:DWORD
LIBPROTO                        FileIsDirectory,:DWORD
LIBPROTO                        FileGetSize,:DWORD
LIBPROTO                        FileExist,:DWORD
LIBPROTO                        FileIsReadOnly,:DWORD
LIBPROTO                        FileIsUnix,:DWORD
LIBPROTO                        FileLoadIntoMem,:DWORD,:DWORD
LIBPROTO                        FileSaveFromMem,:DWORD,:DWORD,:DWORD
LIBPROTO                        FileDir,:DWORD,:DWORD
LIBPROTO                        RegistryUpdateKey,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        RegistryDeleteKey,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        RegistryGetKeyValue,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIFillWindow,:DWORD,:DWORD
LIBPROTO                        GDIFillWindowFromBrush,:DWORD,:DWORD
LIBPROTO                        GDIFillWindowFromBrushAndhDC,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDICreateColorBrush,:DWORD
LIBPROTO                        GDIObtainFont,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIDrawHorzSep,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIDrawVertSep,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIDrawLine,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIDrawLineXOR,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIDrawPixel,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIWriteText,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIWriteClippedText,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIGetTextWidth,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIGetTextHeight,:DWORD,:DWORD,:DWORD
LIBPROTO                        GDIGetFontWidth,:DWORD,:DWORD
LIBPROTO                        GDIGetFontHeight,:DWORD,:DWORD
LIBPROTO                        GDICreateBackDC,:DWORD
LIBPROTO                        GDIDestroyBackDC,:DWORD
LIBPROTO                        GDIBlitBackDC,:DWORD,:DWORD
LIBPROTO                        GDIColorCalcLuminosity,:DWORD,:DWORD
LIBPROTO                        GDIGetSerif
LIBPROTO                        GDIGetSerif10
LIBPROTO                        GDIGetCourierNew9
LIBPROTO                        ControlNextHook,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ControlSendMessage,:DWORD,:DWORD,:DWORD
LIBPROTO                        ControlClientTop,:DWORD
LIBPROTO                        ControlClientLeft,:DWORD
LIBPROTO                        ControlClientWidth,:DWORD
LIBPROTO                        ControlClientHeight,:DWORD
LIBPROTO                        ControlBringToTop,:DWORD
LIBPROTO                        ControlBringToBottom,:DWORD
LIBPROTO                        ControlSetTopMost,:DWORD
LIBPROTO                        ControlRemTopMost,:DWORD
LIBPROTO                        ControlRefresh,:DWORD
LIBPROTO                        ControlRefreshLocal,:DWORD
LIBPROTO                        ControlRepaint,:DWORD
LIBPROTO                        ControlResize,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ControlFreeze,:DWORD,:DWORD
LIBPROTO                        ControlBound,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ControlEnable,:DWORD,:DWORD
LIBPROTO                        ControlVisible,:DWORD,:DWORD
LIBPROTO                        ControlIsVisible,:DWORD
LIBPROTO                        ControlIsEnabled,:DWORD
LIBPROTO                        ControlGetText,:DWORD
LIBPROTO                        ControlGetTextLen,:DWORD
LIBPROTO                        ControlSetText,:DWORD,:DWORD
LIBPROTO                        ControlGetNotifiedMsg,:DWORD
LIBPROTO                        ControlGetNotifiedhWnd,:DWORD
LIBPROTO                        ControlGetNotifiedID,:DWORD
LIBPROTO                        ControlTop,:DWORD
LIBPROTO                        ControlLeft,:DWORD
LIBPROTO                        ControlWidth,:DWORD
LIBPROTO                        ControlHeight,:DWORD
LIBPROTO                        ControlGetHwndFromPoint
LIBPROTO                        ControlGetNotifiedCommand,:DWORD
LIBPROTO                        ControlGetXMousePos,:DWORD
LIBPROTO                        ControlGetYMousePos,:DWORD
LIBPROTO                        ControlGetFont,:DWORD
LIBPROTO                        ControlGetIcon,:DWORD,:DWORD
LIBPROTO                        ControlGetHotKey,:DWORD
LIBPROTO                        ControlSetFont,:DWORD,:DWORD
LIBPROTO                        ControlSetFontNoRedraw,:DWORD,:DWORD
LIBPROTO                        ControlIsWindowChild,:DWORD
LIBPROTO                        ControlGetClassName,:DWORD
LIBPROTO                        ControlSetClassCursor,:DWORD,:DWORD
LIBPROTO                        ControlGetWindowState,:DWORD
LIBPROTO                        ControlClose,:DWORD
LIBPROTO                        ComDlgChooseColor,:DWORD,:DWORD
LIBPROTO                        ComDlgGetOpenFileName,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ComDlgGetSaveFileName,:DWORD,:DWORD,:DWORD
LIBPROTO                        ComDlgChooseFont,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        ComDlgBrowseForFolder,:DWORD,:DWORD
LIBPROTO                        ComDlgParseMultiFilesSelection,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        WinsockInit,:DWORD,:DWORD
LIBPROTO                        WinsockUnInit
LIBPROTO                        SocketCreate
LIBPROTO                        SocketDestroy,:DWORD,:DWORD
LIBPROTO                        SocketConnect,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        SocketGetPort,:DWORD,:DWORD
LIBPROTO                        SocketListen,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        SocketAcceptConnection,:DWORD,:DWORD
LIBPROTO                        SocketSendASCIIDatasCR,:DWORD,:DWORD
LIBPROTO                        SocketSendUnknownDatas,:DWORD,:DWORD,:DWORD
LIBPROTO                        SocketGetRevcLen,:DWORD
LIBPROTO                        SocketReceiveASCIIDatas,:DWORD
LIBPROTO                        SocketReceiveUnknownDatas,:DWORD,:DWORD,:DWORD
LIBPROTO                        FTPInitiatePort,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        FTPGetEntryDate,:DWORD
LIBPROTO                        FTPGetEntryFileName,:DWORD
LIBPROTO                        FTPGetEntryFileAttributes,:DWORD
LIBPROTO                        FTPGetEntryFileSize,:DWORD
LIBPROTO                        FTPIsEntryDir,:DWORD
LIBPROTO                        MiscClipBoardCopyText,:DWORD
LIBPROTO                        MiscClipBoardPasteText
LIBPROTO                        MiscClipBoardIsEmpty
LIBPROTO                        MiscShellAddFileToRecents,:DWORD
LIBPROTO                        MiscWaitEvents,:DWORD,:DWORD,:DWORD
LIBPROTO                        MiscMsgBox,:DWORD,:DWORD,:DWORD
LIBPROTO                        MiscSendBroadCastMsg,:DWORD
LIBPROTO                        MiscDoEvents,:DWORD,:DWORD,:DWORD
LIBPROTO                        MiscObtainGUID,:DWORD
LIBPROTO                        MiscAllocMem,:DWORD
LIBPROTO                        MiscFreeMem,:DWORD
LIBPROTO                        MiscCopyMemLoop,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        MiscCopyMemWithOffsets,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        MiscGetOSClass
LIBPROTO                        MiscObtainExtendedFunctions   ; Reserved for future extensions
LIBPROTO                        GetWindowLangGUID,:DWORD
LIBPROTO                        GetWindowLangInclude,:DWORD
LIBPROTO                        GetWindowLangDoubleSlash,:DWORD
LIBPROTO                        GetWindowLangComment,:DWORD
LIBPROTO                        GetWindowLangMultiComment,:DWORD
LIBPROTO                        GetWindowLangMultiCommentEnd,:DWORD
LIBPROTO                        GetWindowLangProc,:DWORD
LIBPROTO                        GetWindowLangEndp,:DWORD
LIBPROTO                        GetWindowLangProcInterlace,:DWORD
LIBPROTO                        GetWindowLangProcOrphan,:DWORD
LIBPROTO                        GetWindowLangProcOpenBlock,:DWORD
LIBPROTO                        GetWindowLangProcCloseBlock,:DWORD
LIBPROTO                        GetWindowLangFileName,:DWORD
LIBPROTO                        GetWindowLangIncludeName,:DWORD
LIBPROTO                        GetWindowLangIncludeNameFoot,:DWORD
LIBPROTO                        DecodeRelativePaths,:DWORD
LIBPROTO                        RemoveRelativePaths,:DWORD
LIBPROTO                        EncodeRelativePaths,:DWORD
LIBPROTO                        RunUserCommand,:DWORD
LIBPROTO                        RunCommandScript,:DWORD,:DWORD
LIBPROTO                        RunDOSCmd,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        RunDOSCmdCapture,:DWORD,:DWORD,:DWORD
LIBPROTO                        DisplayHelpFile,:DWORD,:DWORD
LIBPROTO                        GetWindowPath
LIBPROTO                        IsFileWorkSpace,:DWORD
LIBPROTO                        IsFileProject,:DWORD
LIBPROTO                        GetCurrentChildState
LIBPROTO                        GotoFileLine,:DWORD,:DWORD,:DWORD
LIBPROTO                        FocusOnFile,:DWORD,:DWORD
LIBPROTO                        GetFileNameFromTreeViewItem,:DWORD
LIBPROTO                        GetLabelFromTreeViewItem,:DWORD
LIBPROTO                        OpenUnknownFile,:DWORD,:DWORD
LIBPROTO                        HexToDec,:DWORD
LIBPROTO                        DecToHex,:DWORD
LIBPROTO                        DecToBin,:DWORD
LIBPROTO                        HexToBin,:DWORD
LIBPROTO                        BinToDec,:DWORD
LIBPROTO                        BinToHex,:DWORD
LIBPROTO                        HexToFlt,:DWORD
LIBPROTO                        FltToHex,:DWORD
LIBPROTO                        GetProcedureList,:DWORD
LIBPROTO                        GetLineFromFile,:DWORD,:DWORD
LIBPROTO                        CreateNewWindow
LIBPROTO                        ClearWindow,:DWORD
LIBPROTO                        ReloadWindow,:DWORD
LIBPROTO                        CloseWindow,:DWORD
LIBPROTO                        CloseAllWindows
LIBPROTO                        SaveAllWindows
LIBPROTO                        PrintWindow,:DWORD,:DWORD
LIBPROTO                        SaveWindow,:DWORD
LIBPROTO                        Undo,:DWORD
LIBPROTO                        Redo,:DWORD
LIBPROTO                        Cut,:DWORD
LIBPROTO                        Copy,:DWORD
LIBPROTO                        CopyFilePathName,:DWORD
LIBPROTO                        CopyEntireText,:DWORD
LIBPROTO                        Paste,:DWORD
LIBPROTO                        DeleteSel,:DWORD
LIBPROTO                        DeleteLine,:DWORD
LIBPROTO                        DeleteToStart,:DWORD
LIBPROTO                        DeleteToEnd,:DWORD
LIBPROTO                        SelectLine,:DWORD
LIBPROTO                        SelectAll,:DWORD
LIBPROTO                        SelectProc,:DWORD
LIBPROTO                        FindNext,:DWORD
LIBPROTO                        FindPrevious,:DWORD
LIBPROTO                        FindSelection,:DWORD
LIBPROTO                        GotoTop,:DWORD
LIBPROTO                        GotoBottom,:DWORD
LIBPROTO                        GotoLine,:DWORD,:DWORD
LIBPROTO                        GotoMatchingBracket,:DWORD
LIBPROTO                        GotoPrevWord,:DWORD,:DWORD
LIBPROTO                        GotoNextWord,:DWORD,:DWORD
LIBPROTO                        GotoPrevLetter,:DWORD,:DWORD
LIBPROTO                        GotoNextLetter,:DWORD,:DWORD
LIBPROTO                        GotoPrevLine,:DWORD,:DWORD
LIBPROTO                        GotoNextLine,:DWORD,:DWORD
LIBPROTO                        GotoPrevPage,:DWORD,:DWORD
LIBPROTO                        GotoNextPage,:DWORD,:DWORD
LIBPROTO                        GetMiniChildStruct,:DWORD,:DWORD
LIBPROTO                        GetIniFilePathName,:DWORD
LIBPROTO                        GetQuickDirectory,:DWORD
LIBPROTO                        GetContext,:DWORD
LIBPROTO                        GetcurrentLine,:DWORD
LIBPROTO                        SelectCurrentWord,:DWORD
LIBPROTO                        GetCurrentWord,:DWORD
LIBPROTO                        InsertText,:DWORD,:DWORD
LIBPROTO                        InsertLine,:DWORD,:DWORD
LIBPROTO                        InsertLines,:DWORD,:DWORD
LIBPROTO                        SetText,:DWORD,:DWORD
LIBPROTO                        GetCaretRealXCoordinate,:DWORD
LIBPROTO                        GetCaretRealYCoordinate,:DWORD
LIBPROTO                        SetFocusInsideWindow,:DWORD
LIBPROTO                        RestoreMDIFromSysTray
LIBPROTO                        InstallHookProc,:DWORD,:DWORD
LIBPROTO                        UnInstallHookProc,:DWORD,:DWORD,:DWORD
LIBPROTO                        GetCurrentLineNumber,:DWORD
LIBPROTO                        GetCurrentColNumber,:DWORD
LIBPROTO                        GetCurrentLineText,:DWORD
LIBPROTO                        GetLineText,:DWORD,:DWORD
LIBPROTO                        WriteToOutputBox,:DWORD
LIBPROTO                        EnumSourceWindows,:DWORD
LIBPROTO                        AddInKillZombie,:DWORD
LIBPROTO                        SpAddInIsAvailable,:DWORD
LIBPROTO                        MiscInputBox,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
LIBPROTO                        IsChildReadOnly,:DWORD
LIBPROTO                        SetChildReadOnly,:DWORD,:DWORD
LIBPROTO                        CreateCustomChildDialog,:DWORD,:DWORD,:DWORD
LIBPROTO                        CreateCodeMax,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
CHROMATICLIB                    ENDS
