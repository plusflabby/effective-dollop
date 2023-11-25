class Monitoring
{
	// Player Count
	protected int playerCount;
	// CPU Usage
	protected float cpuUsage;
	// Memory Usage
	protected int memoryUsage;
	// File Size and IO Operations
	protected string sizeOfDatabase;
	protected int readOperations;
	protected int writeOperations;
	// Network Latency
	protected float networkPingAverageOfPlayers;
	protected float networkPingMinimumOfPlayers;
	protected float networkPingMaximumOfPlayers;
	// Frame Rate (FPS)
	protected float fpsServer;
	protected float fpsClientAverage;
	protected float fpsClientMinimum;
	protected float fpsClientMaximum;
	// AI Processing
	protected int aiCount;
	protected float aiLoadAverage;
	// Database Queries
	protected float databaseAverageResponseTime;
	// Script Execution Time
	protected float scriptAverageCompleteInSeconds;
	// Server Uptime and Stability
	protected string serverOnlineAt;
	protected int serverUptimeInSeconds;
	// Datetime
	protected string dateTime = SCR_DateTimeHelper.GetDateTimeLocal();
	// Interval
	protected int monitoringIntervalInMinutes = 5;
	
	private bool started = false;
	// function to start monitoring at interval 
	void startMonitoringWithInterval()
	{
		if (started)
			return;
		assess(); // run
		GetGame().GetCallqueue().CallLater(assess, monitoringIntervalInMinutes * 60 * 1000, true);
		started = true;
	}
	// function to update variables
	void assess()
	{
		//if (!ServerPerformanceMonitor)
		//	return;
		//Print("duh");
		vPlayerCount playerCountClass = new vPlayerCount();
		vMemoryUsage memoryUsageClass = new vMemoryUsage();
		vFileSizeAndIoOperations fsioo = new vFileSizeAndIoOperations();
		vNetworkLatency networkLatency = new vNetworkLatency();
		vFrameRate frameRate = new vFrameRate();
	}
	/* Functions to update the variable */
	void playerCountUpdate(int variable)
	{
		playerCount = variable;
	}
	void cpuUsageUpdate(float variable)
	{
		cpuUsage = variable;
	}
	void memoryUsageUpdate(int variable)
	{
		memoryUsage = variable;
	}
	void fileSizeAndIoOperationsUpdate(string sizeOfDatabase_, int readOperations_, int writeOperations_)
	{
		sizeOfDatabase = sizeOfDatabase_;
		readOperations = readOperations_;
		writeOperations = writeOperations_;
	}
	void networkLatencyUpdate(float networkPingAverageOfPlayers_, float networkPingMinimumOfPlayers_, float networkPingMaximumOfPlayers_)
	{
		networkPingAverageOfPlayers = networkPingAverageOfPlayers_;
		networkPingMinimumOfPlayers = networkPingMinimumOfPlayers_;
		networkPingMaximumOfPlayers = networkPingMaximumOfPlayers_;
	}
	void frameRateUpdate(float fpsServer_, float fpsClientAverage_, float fpsClientMaximum_, float fpsClientMinimum_)
	{
		fpsServer = fpsServer_;
		fpsClientAverage = fpsClientAverage_;
		fpsClientMaximum = fpsClientMaximum_;
		fpsClientMinimum = fpsClientMinimum_;
	}
}
ref Monitoring ServerPerformanceMonitor = new ref Monitoring();
//ServerPerformanceMonitor.startMonitoringWithInterval();

