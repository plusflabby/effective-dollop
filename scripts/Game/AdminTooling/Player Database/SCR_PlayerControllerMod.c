modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnPlayerDatabaseUpdated")]
	ref PDI_Result playerDatabaseResult;
	
	//! called after playerDatabaseResult is updated 
	protected void OnPlayerDatabaseUpdated()
	{
		Print("proxy-side code");
		Print(playerDatabaseResult);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerDatabaseSearch(string searchValue)
	{
		playerDatabaseResult = PlayerDatabaseIntergration.findPlayerProfile(searchValue);
		Replication.BumpMe(); // tell the Replication system this entity has changes to be broadcast
	}
}