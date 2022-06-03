{ Standard free pascal WinHello program coming from FreePascal distribution
  adapted to Chromatic IDE by Franck Charlet.
  To compile:
  Select compiler bin directory in properties (for example: C:\pp\bin\i386-win32)
  Select compiler units directory in properties (for example: C:\pp\units\i386-win32\rtl)
  Select Free Pascal Skin
  Be sure the compiler mode of the skin is set to Free Pascal.

  It should produces a stand alone exe file called TestFreePascal.exe.

  Original comment following...

  $Id: winhello.pp,v 1.1 2000/07/13 06:30:21 michael Exp $
  Copyright (c) 1996 by Charlie Calvert
  Modifications by Florian Klaempfl

  Standard Windows API application written in Object Pascal.
  No VCL code included. This is all done on the Windows API level.
}

const
    AppName = 'WinHello';

{ Main window proc }
function WindowProc(Window:hWnd;AMessage,WParam,LParam:Longint):Longint;stdcall;export;
var
    dc:hdc;
    ps:paintstruct;
    r:rect;
begin
    WindowProc := 0;
    case AMessage of
        WM_PAINT:
        begin
            BeginPaint(Window, @ps);
            dc := ps.hdC;
            GetClientRect(Window, @r);
            DrawText(dc,'Hello world by Free Pascal',-1,@r,DT_SINGLELINE or DT_CENTER or DT_VCENTER);
            EndPaint(Window,ps);
            Exit;
        end;
        WM_DESTROY:
        begin
            PostQuitMessage(0);
            Exit;
        end;
    end;
    WindowProc := DefWindowProc(Window, AMessage, WParam, LParam);
end;

{ Register the Window Class }
function WinRegister:Boolean;
var
    WindowClass:WndClass;
begin
    WindowClass.Style := CS_HREDRAW or CS_VREDRAW;
    WindowClass.lpfnWndProc := WndProc(@WindowProc);
    WindowClass.cbClsExtra := 0;
    WindowClass.cbWndExtra := 0;
    WindowClass.hInstance := system.MainInstance;
    WindowClass.hIcon := LoadIcon(GetModuleHandle(PChar(0)), PChar(ICON_APP));
    WindowClass.hCursor := LoadCursor(0,IDC_ARROW);
    WindowClass.hbrBackground := GetStockObject(WHITE_BRUSH);
    WindowClass.lpszMenuName := nil;
    WindowClass.lpszClassName := AppName;
    WinRegister := RegisterClass(WindowClass) <> 0;
end;

{ Create the Window Class }
function WinCreate:hWnd;
var
    hWindow:hWnd;
begin
    hWindow := CreateWindow(AppName,'Hello world program',
                            WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
                            CW_USEDEFAULT,CW_USEDEFAULT,0,0,system.MainInstance,nil);
    if hWindow <> 0 then
    begin
        ShowWindow(hWindow,CMDSHOW);
        UpdateWindow(hWindow);
    end;
    WinCreate := hWindow;
end;

{ Main() }
var
    AMessage:Msg;
    hWindow:hWnd;
begin
    if not WinRegister then 
    begin
        MessageBox(0,'Register failed',nil,mb_Ok);
        Exit;
    end;
    hWindow := WinCreate;
    if longint(hWindow) = 0 then
    begin
        MessageBox(0,'WinCreate failed',nil,mb_Ok);
        Exit;
    end;
    while GetMessage(@AMessage,0,0,0) do
    begin
        TranslateMessage(AMessage);
        DispatchMessage(AMessage);
    end;
    Halt(AMessage.wParam);
end.

{
  $Log: winhello.pp,v $
  Revision 1.1  2000/07/13 06:30:21  michael
  + Initial import

  Revision 1.1  1998/12/20 22:23:54  peter
    * new name

}
