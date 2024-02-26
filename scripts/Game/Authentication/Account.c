class Account
{
	private ref Session_Init session;
	
	// registration
	bool registration(string username, string password)
	{
		if (!validate(password))
		{
			Print("Account->registration: validate err");
			return false;
		}
		
		//! Make sure username is not already taken 
		if (Password_Storage.usernameExists(username))
		{
			Print("Account->registration: username exists");
			return false;
		}
		 
		Password_Storage_Password password_storing = new Password_Storage_Password(password, username);
		Password_Storage.addPasswordToDatabase(password_storing);
		
		session = new ref Session_Init();
		Session_Structure session_data = new Session_Structure();
		session_data.sessionId = session.mySessionId;
		session_data.data = username;
		session_data.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
		AT_GLOBALS.server.SESSION_CACHE.AddSession(session.mySessionId, session_data);
		
		//myRolesClass.assign("user");
		
		return true;
	}
	
	// Validate user credentials before creating a session
	static bool validate(string password)
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("validate()", LogLevel.WARNING);
		
		if (Policy.minimumLength(password))
			return false;
		if (Policy.maximumLength(password))
			return false;
		if (Policy.easyGuessable(password))
			return false;
		if (!Policy.complexity(password))
			return false;
		
		return true;
	};
	
	// login
	bool login(string username, string password)
	{
		if (!validate(password))
		{
			Print("Account->login: validate err");
			return false;
		}
		
		if (!Password_Storage.compareForLogin(username, password))
			return false;
		
		session = new ref Session_Init();
		Session_Structure session_data = new Session_Structure();
		session_data.sessionId = session.mySessionId;
		session_data.data = username;
		session_data.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
		AT_GLOBALS.server.SESSION_CACHE.AddSession(session.mySessionId, session_data);
		
		//myRolesClass.assign("user");
		return true;
	}
}