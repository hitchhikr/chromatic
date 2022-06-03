/* Real name of the service */
char *ServiceName = "MyService";
char *ServiceDescription = "Service description here";

/* Startup type */
long ServiceStartFlag = SERVICE_DEMAND_START;

/* The service should start as soon as it is installed or not */
long ServiceStartRightNow = TRUE;

/*  Type of service */
long ServiceTypeFlag = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
