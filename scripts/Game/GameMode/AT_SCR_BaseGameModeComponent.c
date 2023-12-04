modded class SCR_BaseGameMode
{
	private IEntity m_owner;
	private bool ready;
	private PlayerManager pm;

	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		//if (Replication.IsServer())
		//	return;
		
		if (Debug.KeyState(KeyCode.KC_F1))
		{
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PM);
			Debug.ClearKey(KeyCode.KC_F1);
		};
		
		if (Debug.KeyState(KeyCode.KC_F5))
		{
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_AUTH);
			Debug.ClearKey(KeyCode.KC_F5);
		};
		
		if (Debug.KeyState(KeyCode.KC_F6))
		{
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PlayerDatabase);
			Debug.ClearKey(KeyCode.KC_F6);
		};
		
		if (Debug.KeyState(KeyCode.KC_F7))
		{
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_Reporting);
			Debug.ClearKey(KeyCode.KC_F7);
		};
	};

	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		if (SCR_Global.IsEditMode())
			return;

		if (!Replication.IsServer())
			return;

		pm = GetGame().GetPlayerManager();
		if (!pm)
		{
			Debug.Error("AT_SCR_BaseGameModeComponent->EOnInit !|! Failed to get player manager!");
			return;
		};

		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
		{
			Debug.Error("AT_SCR_BaseGameModeComponent->EOnInit !|! Failed to get game mode!");
			return;
		};
		
		//if (!AT_DB.doesAdminConfigExist())
		//	AT_DB.createAdminConfig();
		
		ServerPerformanceMonitor.startMonitoringWithInterval();
		BackupAndRestore backupAndRestore = new BackupAndRestore();

		ready = true;
		Print("AT_SCR_BaseGameModeComponent->EOnInit | Ready", LogLevel.NORMAL);
		AT_DB.saveLog(
			"AT_SCR_BaseGameModeComponent -> EOnInit -> Ready", 
			LogLevel.NORMAL
		);
	};
	
	override void OnPlayerAuditSuccess(int iPlayerID)
	{
		super.OnPlayerAuditSuccess(iPlayerID);
		if (!Replication.IsServer())
			return;
		
		string playerBiUid = AT_MainStatic.getUid(iPlayerID);
		if (!playerBiUid)
		{
			//Print("AT_SCR_BaseGameModeComponent->OnPlayerAuditSuccess | Dedicated server is not correctly configured to connect to the BI backend.", LogLevel.ERROR);
			Debug.Error("Dedicated server is not correctly configured to connect to the BI backend.");
			return;
		}
		
		string playerName = GetGame().GetPlayerManager().GetPlayerName(iPlayerID);
		
		Print("AT_SCR_BaseGameModeComponent->OnPlayerAuditSuccess | " + playerName + " | " + playerBiUid, LogLevel.NORMAL);
		
		AT_DB.saveLog(
			string.Format("OnPlayerAuditSuccess -> %1 -> %2 -> %3", iPlayerID, playerName, playerBiUid), 
			LogLevel.SPAM,
			playerName,
			playerBiUid,
		 	iPlayerID.ToString(),
			"OnPlayerAuditSuccess"
		);
		
		//AT_Database_Data_Player player = AT_DB.getPlayer(playerBiUid);
		//if (!player)
		//	AT_DB.saveNewPlayer(playerBiUid);
	}
}
