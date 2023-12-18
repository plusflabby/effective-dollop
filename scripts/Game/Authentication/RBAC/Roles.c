class RolesClass
{
	ref array<string> myRoles = new array<string>();
	
	void assign(string nameOfRole)
	{
		if (!myRoles.Contains(nameOfRole))
			myRoles.Insert(nameOfRole);
	}
	
	void modify(RolesModify action, string role)
	{
		switch (action)
		{
			case RolesModify.REMOVE:
			{
				if (myRoles.Contains(role))
					myRoles.Remove(myRoles.Find(role));
				break;
			}
		}
	}
}

enum RolesModify
{
	REMOVE
}

ref RolesClass myRolesClass = new RolesClass();