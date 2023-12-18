class AT_LoggingLocalStorage : EDF_DbEntity
{
	//! This will be the log message
	string m_sMessage;
	//! This will be set at creating by SCR_DateTimeHelper.GetDateTimeLocal()
	private string m_sDateTime;
	//! This will be log level 
	string m_sLogLevel;
	//! This will be true if server 
	private bool m_bServer;
	//! This will be true if client 
	private bool m_bClient;
	
	//! Meta value 
	string m_sMetaOne;
	string m_sMetaTwo;
	string m_sMetaThree;
	string m_sMetaFour;
	string m_sMetaFive;
	
	void AT_LoggingLocalStorage()
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
        m_sDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		m_bServer = Replication.IsServer();
		m_bClient = Replication.IsClient();
		
		// Manually set values
		m_sLogLevel = string.Empty;
		m_sMessage = string.Empty;
	}
	
};

class AT_LoggingLocalCallback : EDF_DbOperationStatusOnlyCallback
{
	override void OnFailure(EDF_EDbOperationStatusCode statusCode, Managed context)
	{
		Print("AT_LoggingLocalCallback OnFailure_" + SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, statusCode), LogLevel.ERROR);
	}
}