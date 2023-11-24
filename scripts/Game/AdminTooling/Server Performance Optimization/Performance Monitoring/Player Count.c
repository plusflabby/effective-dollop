class vPlayerCount
{
	private int playerCount = 0;
	
	// set updated value
	void setValue()
	{
		ServerPerformanceMonitor.playerCountUpdate(playerCount);
	}
	
	// 
	void vPlayerCount()
	{
		playerCount = GetGame().GetPlayerManager().GetAllPlayerCount();
		setValue();
	}
	void ~vPlayerCount()
	{
		playerCount = 0;
	}
}