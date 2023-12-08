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
		
		//! Make sure username is not already taken 
		if (Password_Storage.usernameExists(username))
		{
			Debug.Error("Account->registration: username exists");
			return false;
		}
		
		 
		Password_Storage_Password password_storing = new Password_Storage_Password(password, username);
		Password_Storage.addPasswordToDatabase(password_storing);
		
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
	static bool validate(string password)
	{
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
			Debug.Error("Account->login: validate err");
			return false;
		}
		 
//		typename type = String("Password_Storage_Password").ToType();
//		EDF_DbFindResultMultiple<EDF_DbEntity> ttttt = atDB.FindAll(type, EDF_DbFind.Field("accountUid").Contains(username));
//		array<ref EDF_DbEntity> results = ttttt.GetEntities();
//		
//		foreach (EDF_DbEntity result : results)
//		{
//			Password_Storage_Password passwordCheck = Password_Storage_Password.Cast(result);
//			Print(passwordCheck.accountUid);
//		}
//		
//		return true;
		
		if (!Password_Storage.compareForLogin(username, password))
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