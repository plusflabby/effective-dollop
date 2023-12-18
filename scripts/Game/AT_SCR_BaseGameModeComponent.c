//
//class AT_Init : SCR_BaseGameModeComponent
//{
////	override void EOnFrame(IEntity owner, float timeSlice)
////	{
////		if (Debug.KeyState(KeyCode.KC_F1))
////		{
////			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PM);
////			Debug.ClearKey(KeyCode.KC_F1);
////		};
////		super.EOnFrame(owner, timeSlice);
////	};
//	
//	override void OnPlayerAuditSuccess(int iPlayerID)
//	{
//		if (Replication.IsRunning() && Replication.IsServer())
//		{
//			string playerBiUid = AT_MainStatic.getUid(iPlayerID);
//			string playerName = GetGame().GetPlayerManager().GetPlayerName(iPlayerID);
//			
//			//! create profile if none for player
//			if (PlayerDatabaseIntergration)
//			{
//				if (!PlayerDatabaseIntergration.findProfile(playerBiUid))
//					PlayerDatabaseIntergration.generateNewProfile(playerBiUid, playerName);
//				else
//				{
//					//! Check if player changed name
//					if (!PlayerDatabaseIntergration.profileNameIsStored(playerBiUid, playerName))
//						PlayerDatabaseIntergration.updateProfileNames(playerName, playerBiUid);
//				}
//			}
//		}
//		
//		super.OnPlayerAuditSuccess(iPlayerID);
//	}
//}
//
//
