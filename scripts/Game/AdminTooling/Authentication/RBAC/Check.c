class CheckRolesAndPermission
{
	// do have permission
	static bool hasPermission(string permission, string role)
	{
		if (!hasRole(role))
			return false;
		
		bool hasThePermissionYet = false;
		foreach (array<string> permissions : Permissions)
		{
			if (permissions.Get(0) == role) {
				hasThePermissionYet = permissions.Contains(permission);
			}
			else
				continue;
		}
		
		return hasThePermissionYet;
	}
	
	// do have role 
	static bool hasRole(string role)
	{
		return myRolesClass.myRoles.Contains(role);
	}
	
	///
}