//! mod IEntity for auth  : we only want session id saved on client
modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnSessionIdUpdated")]
	ref array<string> m_aSessionUid = new array<string>();
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method(string username, string password)
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
			
			//! Add session to server cache 
			SESSION_CACHE.AddSession(session.mySessionId, session_data);
			
			//Print(session.mySessionId);
			//! Update variable
			m_aSessionUid.Insert(session.mySessionId);
			Replication.BumpMe(); // tell the Replication system this entity has changes to be broadcast
		}
		else 
		{
			// Register Account On Server 
			Password_Storage_Password password_storing = new Password_Storage_Password(password, username);
			Password_Storage.addPasswordToDatabase(password_storing);
		}
	}
	
	//! called after session id updated 
	protected void OnSessionIdUpdated()
	{
		Print("proxy-side code");
		Print("SessionId Updated");
	}
}

//!TODO check sessionid against playerId on server
// example password TEstTest0192837465