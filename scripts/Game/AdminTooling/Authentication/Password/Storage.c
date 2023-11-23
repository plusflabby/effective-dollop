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
}

ref Password_Storage_Cache passwordStorage = new Password_Storage_Cache();
class Password_Storage_Cache
{
	private ref map<string, ref Password_Storage_Password> password_storage = new map<string, ref Password_Storage_Password>();
	
	void addPasswordToInMemoryCache(string AccountUID, Password_Storage_Password password)
	{
		password_storage.Insert(AccountUID, password);
	}
	
	bool compare(Password_Storage_Password one, Password_Storage_Password two)
	{
		Print("one"+one);
		Print("two"+two);
		Print("one.password.Compare(two.password, true)"+one.password.Compare(two.password, true));
		//Print("one.password.Compare(two, true)"+one.password.Compare(two, true)); //false
		return one.password.Compare(two.password, true) == 0;
	}
	bool compare2(string username, string password_storing)
	{
		Print("username"+username);
		if (!password_storage.Contains(username))
			return false;
		Password_Storage_Password password = new Password_Storage_Password(password_storing);
		//return compare(password_storage.Get(username), password);
		return compare3(password_storing, password_storage.Get(username));
	}
	bool compare3(string password, Password_Storage_Password password_)
	{
		return password_.compare(password);
	}
}

class Password_Storage_Files
{
	
}

class Password_Storage_Password
{
	string password;
	string salt;
	
	void Password_Storage_Password(string passwordStr)
	{
		string hash = Hashing.performHash(passwordStr).ToString();
		salt = Salts.generateSalt();
		password = hash + ":" + salt;
	}
	bool compare(string password_)
	{
		string hash = Hashing.performHash(password_).ToString();
		string password2 = hash + ":" + salt;
		
		return password.Compare(password2, true) == 0;
	}
}