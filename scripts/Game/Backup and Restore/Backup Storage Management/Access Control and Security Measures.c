class AccessControlAndSecurityMeasures
{
	//! Check for permission to control backups 
	bool hasPermissionToControlBackups()
	{
		return CheckRolesAndPermission.hasPermission("Backup Control", "administrator");
	}
}