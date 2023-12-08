//! mod IEntity for auth  : we only want session id saved on client
modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnSessionIdUpdated")]
	string m_sSessionUid = "";
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method(string username, string password)
	{
//		if (!m_sSessionUid.IsEmpty()) // prevent useless network messages
//			return;
		
		//Account serverAccount = new Account();
		
//		m_sSessionUid = "EMPTY";
//		
//		Print(username+Replication.IsServer());
//		Print(password+Replication.IsClient());
		
		// prevent useless network messages
		if (!Account.validate(password))
			return;
		
		// login or register
		if (Password_Storage.usernameExists(username))
		{
			//! Attempt login on server credentials 
			
			//! false = fail, true = win
			bool check = Password_Storage.compareForLogin(username, password);
			if (!check)
				return;
			
			Session_Init session = new ref Session_Init();
			Session_Structure session_data = new Session_Structure();
			session_data.sessionId = session.mySessionId;
			session_data.data = username;
			session_data.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
			
			//! Add session to server cache 
			SESSION_CACHE.AddSession(session.mySessionId, session_data);
			
			//! Update variable
			m_sSessionUid = session.mySessionId;
			Replication.BumpMe(); // tell the Replication system this entity has changes to be broadcast
		}
		else 
		{
			// Register Account On Server 
			Password_Storage_Password password_storing = new Password_Storage_Password(password, username);
			Password_Storage.addPasswordToDatabase(password_storing);
			//SESSION_CACHE.AddSession(session.mySessionId, session_data);
		}
	}
	
	//! called after session id updated 
	protected void OnSessionIdUpdated()
	{
		//Print("proxy-side code");
		//Print("SessionId Updated");
	}
	
//	void PrintSessionId()
//	{
//		Print(m_sSessionUid, LogLevel.WARNING);
//	}
}

//!TODO check sessionid against playerId on server