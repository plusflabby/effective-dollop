class Policy
{
	static bool easyGuessable(string password)
	{
		return password.IsEmpty();
	}
	
	static bool minimumLength(string password)
	{
		return password.Length() < 10;
	}
	
	static bool maximumLength(string password)
	{
		return password.Length() > 128;
	}
	
	static bool complexity(string password)
	{
		string charactersUppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		string charactersLowercase = "abcdefghijklmnopqrstuvwxyz";
		string numbers = "0123456789";
		
		int check = 0;
		
		for (int i = 0; i < charactersUppercase.Length(); i++)
		{
			if (password.Contains(charactersUppercase.Get(i)))
			{
				check = check + 1;
				break;
			}
		}
		
		for (int i = 0; i < charactersLowercase.Length(); i++)
		{
			if (password.Contains(charactersLowercase.Get(i)))
			{
				check = check + 1;
				break;
			}
		}
		
		for (int i = 0; i < numbers.Length(); i++)
		{
			if (password.Contains(numbers.Get(i)))
			{
				check = check + 1;
				break;
			}
		}
		
		return check == 3;
	}
}