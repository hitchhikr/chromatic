Context = AddInContextDat.New()

Context.Load()

ComDlg = CommonDialog.New()

if ComDlg.ChooseColor(Context.hMDI, "color") then
    ChromaticLib.InsertText(Context.hCurrentChild, color)
end
