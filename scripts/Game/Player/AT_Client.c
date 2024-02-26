modded class SCR_PlayerController
{
	override void EOnInit(IEntity owner)
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("EOnInit", LogLevel.WARNING);
		
		GetGame().GetCallqueue().CallLater(addJoinDateTime, 10000, false);
		GetGame().GetCallqueue().CallLater(updatePlayTime, 300000, true);
		GetGame().GetCallqueue().CallLater(loop, 1000, true);
		GetGame().GetInputManager().AddActionListener("AdminTooling", EActionTrigger.VALUE, openAdminToolingMenu);
	};

	private void openAdminToolingMenu()
	{
		if (Debug.KeyState(KeyCode.KC_F1))
		{
			if (AT_Global.client.DEBUG)
				Print("Dashboard open", LogLevel.WARNING);
			
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_DASHBOARD);
			Debug.ClearKey(KeyCode.KC_F1);
		};
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void authorityKick(int playerId, string sessionId)
	{
		if (AT_MainStatic.verifySession(sessionId) == false)
			return;
			
		Print("Kicking id:" + playerId);
		GetGame().GetPlayerManager().KickPlayer(playerId, PlayerManagerKickReason.KICK);
		AT_DB.saveLog(
			string.Format("AT -> SCR_PlayerController -> authorityKick -> %1", playerId), 
			LogLevel.WARNING,
		 	playerId.ToString(),
			"SCR_PlayerController",
			"authorityKick"
		);
		AT_DB.storeAdminAction(
			string.Format("AT -> SCR_PlayerController -> authorityKick -> %1", playerId),
			AT_MainStatic.getUid(playerId)
		);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void authorityTeleport(int playerId, vector toPosition, string sessionId)
	{
		if (AT_MainStatic.verifySession(sessionId) == false)
			return;
		
		Print("Teleporting id:" + playerId);
		Print("Teleporting toPosition:" + toPosition.ToString());
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		toPosition = toPosition + "0 1 0";
		SCR_Global.TeleportPlayer(playerId, toPosition, SCR_EPlayerTeleportedReason.DEFAULT);
		AT_DB.saveLog(
			string.Format("AT -> SCR_PlayerController -> authorityTeleport -> %1 -> ", playerId, toPosition.ToString()), 
			LogLevel.WARNING,
		 	playerId.ToString(),
		 	toPosition.ToString(),
			"SCR_PlayerController",
			"authorityTeleport"
		);
		AT_DB.storeAdminAction(
			string.Format("AT -> SCR_PlayerController -> authorityTeleport -> %1 -> ", playerId, toPosition.ToString()), 
			AT_MainStatic.getUid(playerId)
		);
	}
	
	private void loop()
	{
		if (AT_Global.client.DEBUG)
				Print("Loop", LogLevel.WARNING);
		
		if (AT_Global.client.AT_EVENT_CLASS.getAll().Count() > 0)
		{
			for (int i = 0; i < AT_Global.client.AT_EVENT_CLASS.getAll().Count(); i++)
			{
				AT_Event ev = AT_Event.Cast(AT_Global.client.AT_EVENT_CLASS.getAll().Get(i));
				if (!ev)
					return;
				
				switch (ev.getName())
				{
					case AT_Events.Kick:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						Rpc(authorityKick, player.id, AT_GLOBALS.client.sessionId);
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.TeleportThere:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						vector position;
						SCR_WorldTools.FindEmptyTerrainPosition(position, GetGame().GetPlayerManager().GetPlayerControlledEntity(player.id).GetOrigin(), 5.0, 0.5, 4);
						
						Rpc(authorityTeleport, SCR_PlayerController.GetLocalPlayerId(), position, AT_GLOBALS.client.sessionId);
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.TeleportHere:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						vector position;
						SCR_WorldTools.FindEmptyTerrainPosition(position, SCR_PlayerController.GetLocalMainEntity().GetOrigin(), 3.0, 0.5, 4);
						
						Rpc(authorityTeleport, player.id, position, AT_GLOBALS.client.sessionId);
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					
					case AT_Events.SessionUpdate:
					{
						array<string> data = array<string>.Cast(ev.getData());
						Rpc(RpcAsk_Authority_Method, data.Get(0), data.Get(1), GetPlayerId());
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					
					case AT_Events.PlayerDatabaseSearch:
					{
						array<string> data = array<string>.Cast(ev.getData());
						Rpc(RpcAsk_Authority_Method_PlayerDatabaseSearch, data.Get(0));
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					
					case AT_Events.PlayerProfileGetData:
					{
						Rpc(
							RpcAsk_Authority_Method_PlayerProfile,
							AT_MainStatic.stringArayToString(array<string>.Cast(ev.getData())),
							AT_GLOBALS.client.sessionId
						);
						
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					
					case AT_Events.Spectate:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						
						GetGame().GetCameraManager().SetCamera(CameraBase.Cast(GetGame().GetPlayerManager().GetPlayerController(player.id).GetControlledEntity()));
						
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
				}
			}
		}
	}
	
	private void updatePlayTime()
	{
		if (AT_Global.client.DEBUG)
				Print("updatePlayTime()", LogLevel.WARNING);
		
		Rpc(RpcAsk_Authority_Method_PlayerProfile_PlayTime, GetPlayerId());
	}
	
	private void addJoinDateTime()
	{
		if (AT_Global.client.DEBUG)
				Print("addJoinDateTime()", LogLevel.WARNING);
		
		Rpc(RpcAsk_Authority_Method_PlayerProfile_Join, GetPlayerId());
	}
};

