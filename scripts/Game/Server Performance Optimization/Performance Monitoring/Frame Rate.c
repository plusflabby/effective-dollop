class vFrameRate : vSetMod
{
	// variables 
	private float fpsServer = 0.0;
	private float fpsClientAverage = 0.0;
	private float fpsClientMinimum = 0.0;
	private float fpsClientMaximum = 0.0;
	
	// function 
	void setValues()
	{
		if (ServerPerformanceMonitor)
			ServerPerformanceMonitor.frameRateUpdate(fpsServer, fpsClientAverage, fpsClientMinimum, fpsClientMaximum);
	}
	
	// deconstructor and constructor
	void vFrameRate()
	{
		vSetModSTART();
		fpsServer = System.GetFPS();
		
		//Print("SERVER FPS"+fpsServer);
		
		//if (Replication.IsRunning())
		//{
			array<int> players = new array<int>();
			GetGame().GetPlayerManager().GetPlayers(players);
			
			array<float> fps = new array<float>();
			foreach (int playerId : players)
			{
				SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
				if (!playerController)
				{
					Debug.Error("PlayerController not casted to SCR_PlayerController");
					return;
				}
				
				fps.Insert(playerController.getPlayerFPS());
			}
			//Print("-------"+fps.Count().ToString());
		
			if (fps.Count() > 0)
			{
				float totalFps = 0.0;
				float minFps = fps.Get(0);
				float maxFps = fps.Get(0);
				
				foreach (float clientFps : fps)
				{
					if (clientFps > maxFps)
						maxFps = clientFps;
					if (clientFps < minFps)
						minFps = clientFps;
					
					totalFps = totalFps + clientFps;
				}
				
				float averageFps = totalFps / fps.Count();
				
				fpsClientAverage = averageFps;
				fpsClientMinimum = minFps;
				fpsClientMaximum = maxFps;
				//Print("0000000");
			}
		//}
		
		setValues();
	}
	void ~vFrameRate()
	{
		fpsServer = 0.0;
		fpsClientAverage = 0.0;
		fpsClientMinimum = 0.0;
		fpsClientMaximum = 0.0;
		vSetModSTOP();
	}
}

modded class SCR_PlayerController
{
	float getPlayerFPS()
	{
		return System.GetFPS();
	}
}

//ref vFrameRate vFrameT = new vFrameRate();