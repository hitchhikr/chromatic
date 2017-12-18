Context = AddInContextDat.New()

Context.Load()

ComDlg = CommonDialog.New()

if ComDlg.ChooseColor(Context.hMDI, "color") then
	ChromaticLib.InsertText(Context.hCurrentChild, ChromaticLib.StringCat(ChromaticLib.StringHex32(color), "h"))
end
