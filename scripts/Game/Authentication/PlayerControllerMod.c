//! mod IEntity for auth  : we only want session id saved on client
modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnSessionIdUpdated", condition: RplCondition.Custom, customConditionName: "OnSessionIdUpdatedCondition")]
	ref array<string> m_aSessionUid = new array<string>();
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method(string username, string password, int playerId)
	{
		// prevent useless network messages
		if (!Account.validate(password))
			return;
		
		// login or register
		if (Password_Storage.usernameExists(username))
		{
			//! Attempt login on server credentials 
			
			//! false = wrong password, true = right password 
			if (!Password_Storage.compareForLogin(username, password))
				return;
			
			Session_Init session = new ref Session_Init();
			Session_Structure session_data = new Session_Structure();
			session_data.sessionId = session.mySessionId;
			session_data.data = username;
			session_data.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
			session_data.role = session.getRoleFromUsername(username);
			
			//! Add session to server cache 
			AT_GLOBALS.server.SESSION_CACHE.AddSession(session.mySessionId, session_data);
			
			//Print(session.mySessionId);
			//! Update variable
			m_aSessionUid.Insert(session.mySessionId);
			m_aSessionUid.Insert(playerId.ToString());
			Replication.BumpMe(); // tell the Replication system this entity has changes to be broadcast
		}
		else 
		{
			// Register Account On Server 
			Password_Storage_Password password_storing = new Password_Storage_Password(password, username);
			Password_Storage.addPasswordToDatabase(password_storing);
			
			//Set default role to profile 
			AT_GLOBALS.server.rbac_STORAGE.updateOrAddRole(
				username, 
				AT_GLOBALS.server.Roles.Get(AT_GLOBALS.server.Roles.Count() - 1) //Will get last role in array 
			);
		}
	}
	
	//! called after session id updated 
	protected void OnSessionIdUpdated()
	{
		//Print("SessionId Updated");
		
		string sessionFor = m_aSessionUid.Get(1);
		if (sessionFor.Compare(GetPlayerId().ToString(), true) < 0)
			return;
		else
			AT_GLOBALS.client.sessionId = m_aSessionUid.Get(0);
		
		Print("proxy-side code");
		Print(AT_GLOBALS.client.sessionId);
	}
	
	protected bool OnSessionIdUpdatedCondition()
	{
		if (AT_GLOBALS.client.sessionId.IsEmpty())
			return true;
		else 
			return false;
	}
}

//!TODO check sessionid against playerId on server
// example password TEstTest0192837465