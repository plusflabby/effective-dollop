class Account
{
	private ref Session_Init session;
	
	// registration
	bool registration(string username, string password)
	{
		if (!validate(password))
		{
			Debug.Error("Account->registration: validate err");
			return false;
		}
		 
		Password_Storage_Password password_storing = new Password_Storage_Password(password);
		passwordStorage.addPasswordToInMemoryCache(username, password_storing);
		
		session = new ref Session_Init();
		Session_Structure session_data = new Session_Structure();
		session_data.sessionId = session.mySessionId;
		session_data.data = username;
		session_data.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
		SESSION_CACHE.AddSession(session.mySessionId, session_data);
		
		myRolesClass.assign("user");
		
		return true;
	}
	
	// Validate user credentials before creating a session
	bool validate(string password)
	{
		Policy passwordPolicy = new Policy();
		if (passwordPolicy.minimumLength(password))
			return false;
		if (passwordPolicy.maximumLength(password))
			return false;
		if (passwordPolicy.easyGuessable(password))
			return false;
		if (!passwordPolicy.complexity(password))
			return false;
		
		return true;
	};
	
	// login
	bool login(string username, string password)
	{
		if (!validate(password))
		{
			Debug.Error("Account->login: validate err");
			return false;
		}
		 
		if (!passwordStorage.compare2(username, password))
			return false;
		
		session = new ref Session_Init();
		Session_Structure session_data = new Session_Structure();
		session_data.sessionId = session.mySessionId;
		session_data.data = username;
		session_data.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
		SESSION_CACHE.AddSession(session.mySessionId, session_data);
		
		myRolesClass.assign("user");
		return true;
	}
}