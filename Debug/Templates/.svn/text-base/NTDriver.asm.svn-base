; --------------- Code section
                        .code

TEXTW			        DeviceName, <\Device\MyDriver>
TEXTW			        DosDeviceName, <\DosDevices\MyDriver>
; DRV_TYPE must be >32767 <65536
DRV_TYPE		        equ	40000
; DRV_IOCTL_INDEX must be >2048 <4095
DRV_IOCTL_INDEX		    equ	3000

			; Put your devices services here
			; Eg.	dd	offset MyDeviceServiceRoutine
			; These services can be called externally with DeviceIoControl()
DIOCServices  		    dd	0

			; Calc number of services
MAXSERVICE		        equ	(($ - offset DIOCServices) - 1)

; --------------- Device services
; ... put them here ...


; ---------------
DRV_MASK		        equ	(DRV_TYPE SHL 16) OR (FILE_ANY_ACCESS SHL 14) OR (DRV_IOCTL_INDEX SHL 2) OR METHOD_BUFFERED
DeviceNameUnicode	    UNICODE_STRING <LDeviceName * 2, LDeviceName * 2, sDeviceName>
SymLinkNameUnicode	    UNICODE_STRING <LDosDeviceName * 2, LDosDeviceName * 2, sDosDeviceName>

; --------------- Driver's unload
UnloadDriver		    proc	DriverObject
            			invoke	IoDeleteSymbolicLink,offset SymLinkNameUnicode
            			; Delete the object
            			mov	    eax,DriverObject
            			invoke	IoDeleteDevice,(DRIVER_OBJECT ptr [eax])._PDEVICE_OBJECT
            			ret
UnloadDriver		    endp

; --------------- Driver's dispatch for DeviceIoControl 
Dispatch		        proc	uses ecx edx esi edi DeviceObject:dword,pIRP:dword
            			mov	    ecx,pIRP
            			mov	    edi,[ecx + _IRP.PCurrentIrpStackLocation]
            			mov	    esi,[ecx + _IRP.SystemBuffer]
            			mov	    eax,[edi + IO_STACK_LOCATION.DeviceIoControl.IoControlCode]
            			push	ecx			; Save IRP
            			sub	    eax,DRV_MASK
             			jb	    BadRequest
             			cmp	    eax,MAXSERVICE
             			ja	    BadRequest
            			call	[DIOCServices + eax]
            			jmp	    QuitDispatch
BadRequest:	        	; Report invalid service request
            			mov	    eax,STATUS_INVALID_DEVICE_REQUEST
            			xor	    edx,edx
QuitDispatch:		    pop	    ecx		; Restore IRP
            			push	eax		; Save status
            			mov	    dword ptr [ecx + _IRP.IoStatus.Information],0
            			mov	    [ecx + _IRP.IoStatus.Status],eax
            			invoke	IoCompleteRequest,ecx,IO_NO_INCREMENT
            			pop	    eax		; Return operation status
            			ret
Dispatch		        endp

; --------------- Driver's dispatch point for Create/Close (IRP_MJ_CREATE/IRP_MJ_CLOSE)
StandardDispatch	    proc	uses ebx DeviceObject:dword,pIRP:dword
            			mov	    ebx,pIRP
            			; STATUS_SUCCESSFUL
            			xor	    eax,eax
            			mov	    [ebx + _IRP.IoStatus.Information],eax
            			mov	    [ebx + _IRP.IoStatus.Status],eax
            			push	eax
            			invoke	IoCompleteRequest,ebx,IO_NO_INCREMENT
            			pop	    eax
            			ret
StandardDispatch	    endp

; --------------- Driver's entry point
DriverEntry		        proc	uses ebx esi edi,DriverObject:dword,RegPath:dword
             			xor	    eax,eax
             			mov	    esi,DriverObject
             			lea	    edi,DeviceNameUnicode
ExtensionSize		    equ	    eax
DevCharacteristic	    equ	    eax
Exclusive		        equ	    eax
            			; Use stack to obtain device object
            			push	eax
            			invoke	IoCreateDevice,esi,ExtensionSize,edi,DRV_TYPE,DevCharacteristic,Exclusive,esp
            			test	eax,eax
            			pop	    ebx
            			jl	    CreateDeviceFailed
            			lea	    eax,StandardDispatch
             			; Register the driver's function
             			mov	    [esi + DRIVER_OBJECT.PDRIVER_UNLOAD],offset UnloadDriver
             			mov	    [esi + DRIVER_OBJECT.PDISPATCH_IRP_MJ_CREATE],eax
            			mov	    [esi + DRIVER_OBJECT.PDISPATCH_IRP_MJ_CLOSE],eax
            			mov	    [esi + DRIVER_OBJECT.PDISPATCH_IRP_MJ_DEVICE_CONTROL],offset Dispatch
            			invoke	IoCreateSymbolicLink,addr SymLinkNameUnicode,edi
            			test	eax,eax
            			jge	    SymLinkCreated
            			push	eax
            			invoke	IoDeleteDevice,ebx
            			pop	    eax
            			ret
SymLinkCreated:		    xor	    eax,eax		; STATUS_SUCCESS
CreateDeviceFailed:	    ret			        ; from now on INIT, reloc and other discardable sections don't exist anymore
DriverEntry		        endp
