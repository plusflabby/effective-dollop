modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnPlayerDatabaseUpdated")]
	ref PDI_Result_Class playerDatabaseResult = PDI_Result_Class();
	
	//! called after playerDatabaseResult is updated 
	protected void OnPlayerDatabaseUpdated()
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("OnPlayerDatabaseUpdated()"+playerDatabaseResult, LogLevel.WARNING);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerDatabaseSearch(string searchValue)
	{
		PDI_Result result = PlayerDatabaseIntergration.findPlayerProfile(searchValue);
		Print(playerDatabaseResult);
		Print(result);
		array<string> results = new array<string>();
		if (result.player)
		{
			results.Insert(result.player.m_sBiUID);
		}
		if (result.results)
		{
			foreach (DB_PlayerProfile profile : result.results)
			{
				results.Insert(profile.m_sBiUID);
			}
		}
		
		playerDatabaseResult = PDI_Result_Class.createClass(PDI_Results.SUCCESS, results);
		Replication.BumpMe(); // tell the Replication system this entity has changes to be broadcast
	}
}