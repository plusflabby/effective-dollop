modded class SCR_PlayerController
{
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void authorityKick(int playerId)
	{
		Print("Kicking id:" + playerId);
		GetGame().GetPlayerManager().KickPlayer(playerId, PlayerManagerKickReason.KICK);
		AT_DB.saveLog(
			string.Format("AT -> SCR_PlayerController -> authorityKick -> %1", playerId), 
			LogLevel.WARNING,
		 	playerId.ToString(),
			"SCR_PlayerController",
			"authorityKick"
		);
	}
	void requestKick(int playerId)
	{
		Rpc(authorityKick, playerId);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void authorityTeleport(int playerId, vector toPosition)
	{
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
	}
	void requestTeleport(int playerId, vector position)
	{
		Rpc(authorityTeleport, playerId, position);
	}
	
	override void OnInit(IEntity owner)
	{
		if (Replication.IsClient())
			GetGame().GetCallqueue().CallLater(loop, 1000, true);
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (Debug.KeyState(KeyCode.KC_F1))
		{
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PM);
			Debug.ClearKey(KeyCode.KC_F1);
		};
		super.EOnFrame(owner, timeSlice);
	};
	
	protected void loop()
	{
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
						requestKick(player.id);
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.TeleportThere:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						vector position;
						SCR_WorldTools.FindEmptyTerrainPosition(position, GetGame().GetPlayerManager().GetPlayerControlledEntity(player.id).GetOrigin(), 5.0, 0.5, 4);
						
						requestTeleport(SCR_PlayerController.GetLocalPlayerId(), position);
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.TeleportHere:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						vector position;
						SCR_WorldTools.FindEmptyTerrainPosition(position, SCR_PlayerController.GetLocalMainEntity().GetOrigin(), 3.0, 0.5, 4);
						
						requestTeleport(player.id, position);
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					
					case AT_Events.SessionUpdate:
					{
						array<string> data = array<string>.Cast(ev.getData());
						Rpc(RpcAsk_Authority_Method, data.Get(0), data.Get(1));
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
							AT_MainStatic.stringArayToString(array<string>.Cast(ev.getData()))
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
	
	
};
