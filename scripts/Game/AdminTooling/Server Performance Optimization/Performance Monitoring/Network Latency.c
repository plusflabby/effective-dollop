class vNetworkLatency : vSetMod
{
	//variables
	private float networkPingAverageOfPlayers = 0.0;
	private float networkPingMinimumOfPlayers = 0.0;
	private float networkPingMaximumOfPlayers = 0.0;
	
	// function 
	void setValue()
	{
		ServerPerformanceMonitor.networkLatencyUpdate(networkPingAverageOfPlayers, networkPingMaximumOfPlayers, networkPingMinimumOfPlayers);
	}
	
	// constructor and deconstructor
	void ~vNetworkLatency()
	{
		networkPingAverageOfPlayers = 0.0;
		networkPingMinimumOfPlayers = 0.0;
		networkPingMaximumOfPlayers = 0.0;
		vSetModSTOP();
	}
	void vNetworkLatency()
	{
		vSetModSTART();
		array<int> players = new array<int>();
		GetGame().GetPlayerManager().GetPlayers(players);
		//players.Compact();
		
		if (players.Count() > 0)
		{
			array<float> pings = new array<float>();
			foreach (int playerId : players)
			{
				PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
				
				float playerPing = Replication.GetConnectionStats(playerController.GetRplIdentity()).GetRoundTripTimeInMs();
				pings.Insert(playerPing);
			}
			
			//pings.Compact();
			
			float total = 0.0;
			float min = pings.Get(0);
			float max = pings.Get(0);
			
			foreach (float ping : pings)
			{
				if (ping > max)
					max = ping;
				
				if (ping < min)
					min = ping;
				
				total = total + ping;
			}
			
			float average = total / pings.Count();
			
			
			networkPingAverageOfPlayers = average;
			networkPingMinimumOfPlayers = min;
			networkPingMaximumOfPlayers = max;
		}
		
		setValue();
	}
}