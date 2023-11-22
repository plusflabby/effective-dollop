/*
methods for storing hashed passwords and salts
*/

class Password_Storage
{
	// store Password_Storage_Password
	void store(string playerBIUID, Password_Storage_Password password)
	{
		passwordStorage.addPasswordToInMemoryCache(playerBIUID, password);
	}
	
	// compare Password_Storage_Password
	bool compare(Password_Storage_Password one, Password_Storage_Password two)
	{
		 return one.password == two.password;
	}
}

ref Password_Storage_Cache passwordStorage = new Password_Storage_Cache();
class Password_Storage_Cache
{
	private ref map<string, Password_Storage_Password> password_storage = new map<string,Password_Storage_Password>();
	
	void addPasswordToInMemoryCache(string AccountUID, Password_Storage_Password password)
	{
		password_storage.Insert(AccountUID, password);
	}
}

class Password_Storage_Files
{
	
}

class Password_Storage_Password
{
	string password;
	
	void Password_Storage_Password(string passwordStr)
	{
		string hash = Hashing.performHash(passwordStr).ToString();
		string salt = Salts.generateSalt();
		password = hash + ":" + salt;
	}
}