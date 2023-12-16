//! This gonna be a big line count
class vScriptExecutionTime
{
	void vScriptExecutionTime()
	{
		float total = 0.0;
		foreach (vSetVariable var : at_scripts)
		{
			total = total + var.timeMiliSecond;
			//Print(var.className + var.timeMiliSecond.ToString());
		}
		
		if (at_scripts.Count() > 0)
			ServerPerformanceMonitor.scriptAverageCompleteInMiliSecondsUpdate(total / at_scripts.Count());
		else
			ServerPerformanceMonitor.scriptAverageCompleteInMiliSecondsUpdate(0.0);
	}
}
//ref vScriptExecutionTime vSET = new vScriptExecutionTime();

class vSetVariable
{
	float timeMiliSecond = 0.0;
	string className = "EMPTY";
}

/*

mhm

*/

//modded class vFrameRate
//{
//	private float tick;
//	void vFrameRate()
//	{
//		tick = System.GetTickCount();
//		//Print("constructred vFrameRate "+tick.ToString());
//	}
//
//	void ~vFrameRate()
//	{
//		//Print("deconstructed vFrameRate "+System.GetTickCount().ToString());
//		Print("vFrameRate SET "+System.GetTickCount(tick).ToString());
//		
//		vSetVariable script = new vSetVariable();
//		script.className = ClassName();
//		script.timeMiliSecond = System.GetTickCount(tick);
//		at_scripts.Insert(script);
//	}
//}

//modded class vPlayerCount : vSetMod
//{
//	void vPlayerCount()
//	{
//	}
//	void ~vPlayerCount()
//	{
//	}
//}

class vSetMod
{
	private float tick;
	private ref vSetVariable script;
	void vSetModSTART()
	{
		tick = System.GetTickCount();
		script = new vSetVariable();
		script.className = ClassName();
	}
	void vSetModSTOP()
	{
		script.timeMiliSecond = System.GetTickCount(tick);
		at_scripts.Insert(script);
	}
}

//modded class vNetworkLatency : vSetMod
//{
////	override void setValue()
////	{
////		super.setValue();
////	}
//	
//}