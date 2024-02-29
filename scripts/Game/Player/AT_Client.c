modded class SCR_PlayerController : PlayerController
{
	protected bool hasGodMode = false;
	protected PlayerManager playerManager;
	protected SCR_DamageManagerComponent damageManager;
	protected SCR_CharacterDamageManagerComponent characterDamageManager;
	protected IEntity ownerI;
	
	protected void heal(IEntity player)
	{
		damageManager = SCR_DamageManagerComponent.GetDamageManager(player);
		if (!damageManager)
		{
			Print("Failed to get damageManager in AT_Client", LogLevel.ERROR);
			return;
		}
		
		if (damageManager.CanBeHealed())
			damageManager.FullHeal();
	}
	
	override void EOnInit(IEntity owner)
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("EOnInit", LogLevel.WARNING);
		
		if (!Replication.IsClient())
		{
			Print("Failed to be client in AT_Client", LogLevel.ERROR);
			return;
		}
		
		/*
		
		RplComponent rpl = RplComponent.Cast(controlledEntity.FindComponent(RplComponent));
		if (rpl)
		{
			rpl.GiveExt(RplIdentity.Local(), false);
			m_MainEntityID = rpl.Id();
		}
		
		*/
		
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
//		if (AT_Global.client.DEBUG)
//				Print("Loop", LogLevel.WARNING);
		
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
					case AT_Events.PlayerGodMode:
					{
						// Get menu listbox selected player
						AT_playerData player = AT_playerData.Cast(ev.getData());
						
		Print(player.id.ToString(), LogLevel.ERROR);
						//check if enabled and turn off if yes
						SCR_PlayerController playerScr = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(player.id));
						if (!playerScr)
						{
							Debug.Error("PlayerController not casted to SCR_PlayerController");
							break;
						}
						
						// Disable take damage
						Rpc(RpcAsk_Authority_Method_DisablePlayerDamage, player.id, !playerScr.hasGodMode);
						
						// Heal
						heal(playerManager.GetPlayerControlledEntity(player.id));
						
						//Remove from events
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
					}
					default:
						if (AT_Global.client.DEBUG)
							Print(string.Format("AT_Event %1 does not exists on switch", ev.getName()), LogLevel.ERROR);
					
						AT_Global.client.AT_EVENT_CLASS.remove(i);
						break;
				}
			}
		}
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_DisablePlayerDamage(int pId, bool enable)
	{
		IEntity pEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(pId);
		if (!pEntity)
		{
			Print("!pEntity", LogLevel.ERROR);
			return;
		}
		ChimeraCharacter char = ChimeraCharacter.Cast(pEntity);
		if (!char)
		{
			Print("!char", LogLevel.ERROR);
			return;
		}
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMgr)
		{
			Print("SCR_CharacterDamageManagerComponent not casted to SCR_DamageManagerComponent", LogLevel.ERROR);
			return;
		}
		damageMgr.EnableDamageHandling(enable);
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