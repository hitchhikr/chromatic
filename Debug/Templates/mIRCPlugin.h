/* --------------- Returned values to mIRC */
#define MIRC_HALT 0
#define MIRC_CONTINUE 1
#define MIRC_FILLEDDATAPARMS 2
#define MIRC_FILLEDDATA 3
#define MIRC_MAXLEN 900

/* --------------- Dll loading structure */
typedef struct _MIRCLOADINFO
{
    DWORD mVersion;
    HWND mHwnd;
    BOOL mKeep;
} MIRCLOADINFO, *LPMIRCLOADINFO;
