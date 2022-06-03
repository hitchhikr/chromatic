// ----------------------------------------
// Chromatic IDE Wizard template
// ----------------------------------------

// ----------------------------------------
// Declaration of variables
(defvar ReturnCode, PILL_ERROR_GHOST)	// Abort project creation process by default
(defvar PreviousButton_hWnd)
(defvar NextButton_hWnd)

// Chromatic host required to run this script
(if !(isconst CHROMATIC), (end ReturnCode))

// ----------------------------------------
// Code entry point here

// Alloc room for context structure
(if !(defvar Context, (call WALib(WAMiscAllocMem), WAContext_Size)), (end))
// Obtain it
(call WALib(WAMMGetContext), Context)

(defvar i, 0)
(call WALib(WAMiscMsgBox), 0, (tostring ARGC), MB_INFORMATION)
(while (< i, ARGC),
	(call WALib(WAMiscMsgBox), 0, ARGV(i), MB_INFORMATION)
	(inc i)
)

// Free context
(call WALib(WAMiscFreeMem), Context)
(end ReturnCode)
