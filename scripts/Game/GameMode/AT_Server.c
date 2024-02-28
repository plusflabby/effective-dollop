modded class SCR_BaseGameModeComponent
{
	static bool pl_srv = false;

//	override void EOnFrame(IEntity owner, float timeSlice)
//	{
//		if (Debug.KeyState(KeyCode.KC_F1))
//		{
//			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PM);
//			Debug.ClearKey(KeyCode.KC_F1);
//		};
//		super.EOnFrame(owner, timeSlice);
//	};

	override void OnPlayerAuditSuccess(int playerId)
	{
		super.OnPlayerAuditSuccess(playerId);

		if (Replication.IsRunning() && Replication.IsServer())
		{
			string playerBiUid = AT_MainStatic.getUid(playerId);
			string playerName = GetGame().GetPlayerManager().GetPlayerName(playerId);

			//! create profile if none for player
			if (PlayerDatabaseIntergration)
			{
				if (PlayerDatabaseIntergration.findProfile(playerBiUid) == false)
					PlayerDatabaseIntergration.generateNewProfile(playerBiUid, playerName);
				else
				{
					//! Check if player changed name
//					if (PlayerDatabaseIntergration.profileNameIsStored(playerBiUid, playerName) == false)
//						PlayerDatabaseIntergration.updateProfileNames(playerName, playerBiUid);
				}
			}
		}
	}

	override void OnPostInit(IEntity owner)
	{
		if (!pl_srv)
		{
			RplComponent rpl = RplComponent.Cast(m_pGameMode.FindComponent(RplComponent));

			if (!rpl || rpl.IsProxy())
			{
				Print("Failed to get rpl in AT_server", LogLevel.ERROR);
				return;
			}
			SCR_BaseGameModeComponent.pl_srv = true;


			if (!AT_GLOBALS.server.network.isSetUp())
				AT_GLOBALS.server.network.SetUp();

			//! Game -> API, every 10 seconds
			GetGame().GetCallqueue().CallLater(sendPlayerListToAPI, 15000, true);
			
			//! Game -> API -> Game, every 10 seconds, for tasks like kick
			GetGame().GetCallqueue().CallLater(runTasksFromAPI, 10000, true);
			Print("runTasksFromAPI", LogLevel.WARNING);
		}
	}

	//! Server function to ..
	private void sendPlayerListToAPI()
	{
		if (!AT_GLOBALS.server.network.isSetUp())
			return;
		
		if (GetGame().GetPlayerManager().GetPlayerCount() == 0)
			return;
		
		string pl_str = string.Empty;
		array<int> outPlayers = new array<int>();
		GetGame().GetPlayerManager().GetPlayers(outPlayers);
		foreach (int id : outPlayers)
		{
			pl_str += string.Format(
				"%1\t%2\t%3\n",
				id.ToString(),
				GetGame().GetBackendApi().GetPlayerIdentityId(id),
				GetGame().GetPlayerManager().GetPlayerName(id)
			)
		}

		AT_GLOBALS.server.net_2.POST_without_response(pl_str, API_Post_Types.PlayerList);
		//! Debug 
		//AT_GLOBALS.server.net_2.POST_with_response(pl_str, API_Post_Types.DEBUG);
	}
	
	private void runTasksFromAPI()
	{
		if (!AT_GLOBALS.server.network.isSetUp())
			return;
		
		AT_GLOBALS.server.net_3.getTaskList();
	}
}
