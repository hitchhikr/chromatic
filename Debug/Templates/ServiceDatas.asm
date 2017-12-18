                        .data

			            ; Real name of the service
ServiceName		        db	"MyService",0
ServiceDescription	    db	"Service description here",0

            			; Startup type
ServiceStartFlag	    dd	SERVICE_DEMAND_START

; The service should start as soon as it is installed or not
ServiceStartRightNow	dd	TRUE

			            ; Type of service
ServiceTypeFlag		    dd	SERVICE_WIN32_OWN_PROCESS or SERVICE_INTERACTIVE_PROCESS
