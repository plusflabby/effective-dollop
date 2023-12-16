/*
methods for storing hashed passwords and salts
*/

class Password_Storage
{
	private static typename type = String("Password_Storage_Password").ToType();
	// store Password_Storage_Password
//	void store(string playerBIUID, Password_Storage_Password password)
//	{
//		//passwordStorage.addPasswordToInMemoryCache(playerBIUID, password);
//		
//	}
	static void addPasswordToDatabase(Password_Storage_Password password)
	{
		Password_Storage_Files.storeOrUpdatePassword(password);
	}
	
	static bool compareForLogin(string username, string password)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search = AT_GLOBALS.server.atDB.FindAll(type, EDF_DbFind.Field("accountUid").Contains(username));
		array<ref EDF_DbEntity> results = search.GetEntities();
		if (results.Count() < 1)
			return false;
		
		//! Take first result and verify password = password's
		Password_Storage_Password passwordOfAccount = Password_Storage_Password.Cast(results.Get(0));
		return passwordOfAccount.compare(password);
	}
	
	static bool usernameExists(string username)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search = AT_GLOBALS.server.atDB.FindAll(type, EDF_DbFind.Field("accountUid").Contains(username));
		return search.GetEntities().Count() > 0;
	}
}

//ref Password_Storage_Cache passwordStorage = new Password_Storage_Cache();
//class Password_Storage_Cache
//{
//	private ref map<string, ref Password_Storage_Password> password_storage = new map<string, ref Password_Storage_Password>();
//	
//	void addPasswordToInMemoryCache(string AccountUID, Password_Storage_Password password)
//	{
//		password_storage.Insert(AccountUID, password);
//	}
//	
//	bool compare(Password_Storage_Password one, Password_Storage_Password two)
//	{
//		Print("one"+one);
//		Print("two"+two);
//		Print("one.password.Compare(two.password, true)"+one.password.Compare(two.password, true));
//		//Print("one.password.Compare(two, true)"+one.password.Compare(two, true)); //false
//		return one.password.Compare(two.password, true) == 0;
//	}
//	bool compare2(string username, string password_storing)
//	{
//		Print("username"+username);
//		if (!password_storage.Contains(username))
//			return false;
//		Password_Storage_Password password = new Password_Storage_Password(password_storing, username);
//		//return compare(password_storage.Get(username), password);
//		return compare3(password_storing, password_storage.Get(username));
//	}
//	bool compare3(string password, Password_Storage_Password password_)
//	{
//		return password_.compare(password);
//	}
//}

class Password_Storage_Password : EDF_DbEntity
{
	string password;
	string salt;
	string accountUid;
	
	void Password_Storage_Password(string passwordStr, string accountUid_)
	{
		string hash = Hashing.performHash(passwordStr).ToString();
		salt = Salts.generateSalt();
		password = hash + ":" + salt;
		accountUid = accountUid_;
		SetId(EDF_DbEntityIdGenerator.Generate());
	}
	bool compare(string password_)
	{
		string hash = Hashing.performHash(password_).ToString();
		string password2 = hash + ":" + salt;
		
		return password.Compare(password2) == 0;
	}
}

class Password_Storage_Files
{
	
	static void storeOrUpdatePassword(Password_Storage_Password password)
	{
		AT_GLOBALS.server.atDB.AddOrUpdateAsync(password, AT_GLOBALS.server.variableOne);
	}
	
}