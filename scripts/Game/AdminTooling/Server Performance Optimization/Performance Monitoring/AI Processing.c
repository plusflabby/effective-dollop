class vAiProcessing : vSetMod
{
	void vAiProcessing()
	{
		vSetModSTART();
		ref array<AIAgent> ai = new array<AIAgent>();
		
		if (GetGame())
			if (GetGame().GetAIWorld())
				GetGame().GetAIWorld().GetAIAgents(ai);
		
		
		ServerPerformanceMonitor.aiCountUpdate(ai.Count());
	}
	
	void ~vAiProcessing()
	{
		vSetModSTOP();
	}
}