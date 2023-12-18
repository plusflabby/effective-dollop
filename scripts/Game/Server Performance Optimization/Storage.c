class SPO_Storage : EDF_DbEntity
{
	int m_iPlayerCount;
	int m_iMemoryUsage;
	string m_sSizeOfDatabase;
	float m_fNetworkPingAverageOfPlayers;
	float m_fNetworkPingMinimumOfPlayers;
	float m_fNetworkPingMaximumOfPlayers;
	float m_fFpsServer;
	float m_fFpsClientAverage;
	float m_fFpsClientMinimum;
	float m_fFpsClientMaximum;
	int m_iAiCount;
	float m_fScriptAverageCompleteInMiliSeconds;
	string m_sDateTime;
	
	void generate(
		int playerCount,
		int memoryUsage,
		string sizeOfDatabase,
		float networkPingAverageOfPlayers,
		float networkPingMinimumOfPlayers,
		float networkPingMaximumOfPlayers,
		float fpsServer,
		float fpsClientAverage,
		float fpsClientMinimum,
		float fpsClientMaximum,
		int aiCount,
		float scriptAverageCompleteInMiliSeconds,
		string dateTime
	)
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		m_iPlayerCount = playerCount;
		m_iMemoryUsage = memoryUsage;
		m_sSizeOfDatabase = sizeOfDatabase;
		m_fNetworkPingAverageOfPlayers = networkPingAverageOfPlayers;
		m_fNetworkPingMinimumOfPlayers = networkPingMinimumOfPlayers;
		m_fNetworkPingMaximumOfPlayers = networkPingMaximumOfPlayers;
		m_fFpsServer = fpsServer;
		m_fFpsClientAverage = fpsClientAverage;
		m_fFpsClientMinimum = fpsClientMinimum;
		m_fFpsClientMaximum = fpsClientMaximum;
		m_iAiCount = aiCount;
		m_fScriptAverageCompleteInMiliSeconds = scriptAverageCompleteInMiliSeconds;
		m_sDateTime = dateTime;
	}
}