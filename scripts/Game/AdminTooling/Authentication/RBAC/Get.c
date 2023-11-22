class RBAC_Get
{
	// display menu - dynamically display or hide menu options
	static bool canDisplayThis(string requiredRole, string requiredPermission)
	{
		return CheckRolesAndPermission.hasPermission(requiredPermission, requiredRole);
	}
	
	// display my roles 
	static array<string> getRoles()
	{
		return myRolesClass.myRoles;
	}
}