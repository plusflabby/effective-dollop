class vPlayerCount : vSetMod
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
		vSetModSTART();
		playerCount = GetGame().GetPlayerManager().GetAllPlayerCount();
		setValue();
	}
	void ~vPlayerCount()
	{
		playerCount = 0;
		vSetModSTOP();
	}
}