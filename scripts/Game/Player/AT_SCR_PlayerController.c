modded class SCR_PlayerController
{
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void authorityStartMonitoring()
	{
		
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void authorityKick(int playerId)
	{
		Print("Kicking id:" + playerId);
		GetGame().GetPlayerManager().KickPlayer(playerId, PlayerManagerKickReason.KICK);
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
	}
	void requestTeleport(int playerId, vector position)
	{
		Rpc(authorityTeleport, playerId, position);
	}
	
	override void OnInit(IEntity owner)
	{
		if (Replication.IsClient())
			GetGame().GetCallqueue().CallLater(loop, 3000, true);
		//if (Replication.IsServer() && Replication.)
			
	}
	
	protected void loop()
	{
		if (AT_EVENT_CLASS.getAll().Count() > 0)
		{
			for (int i = 0; i < AT_EVENT_CLASS.getAll().Count(); i++)
			{
				AT_Event ev = AT_Event.Cast(AT_EVENT_CLASS.getAll().Get(i));
				if (!ev)
					return;
				switch (ev.getName())
				{
					case AT_Events.Kick:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						requestKick(player.id);
						AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.TeleportThere:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						vector position;
						SCR_WorldTools.FindEmptyTerrainPosition(position, GetGame().GetPlayerManager().GetPlayerControlledEntity(player.id).GetOrigin(), 5.0, 0.5, 4);
						
						requestTeleport(SCR_PlayerController.GetLocalPlayerId(), position);
						AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.TeleportHere:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						vector position;
						SCR_WorldTools.FindEmptyTerrainPosition(position, SCR_PlayerController.GetLocalMainEntity().GetOrigin(), 3.0, 0.5, 4);
						
						requestTeleport(player.id, position);
						AT_EVENT_CLASS.remove(i);
						break;
					}
					case AT_Events.Spectate:
					{
						AT_playerData player = AT_playerData.Cast(ev.getData());
						
						// Change localcamera to target camera
						//PlayerController plc = GetGame().GetPlayerController();
						//if (plc)
						//{
						//	plc.SetCharacterCameraRenderActive(false);
						//	plc.GetPlayerCamera()
						//}
							
						
						//CameraBase camera = CameraBase.Cast(GetGame().GetPlayerManager().GetPlayerController(player.id).GetPlayerCamera());
						GetGame().GetCameraManager().SetCamera(CameraBase.Cast(GetGame().GetPlayerManager().GetPlayerController(player.id).GetControlledEntity()));
						
						AT_EVENT_CLASS.remove(i);
						break;
					}
				}
			}
		}
	}
};
