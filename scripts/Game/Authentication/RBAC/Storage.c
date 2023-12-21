class atRBAC_STORAGE
{
	private static typename type = String("RBAC_STORE").ToType();
	
	//! Will add or update in database 
	void updateOrAddRole(string username, string role)
	{
		RBAC_STORE rbac = new RBAC_STORE();
		rbac.username = username;
		rbac.role = role;
		AT_GLOBALS.server.atDB.AddOrUpdateAsync(rbac, AT_GLOBALS.server.variableOne);
	}
	
	//! Will get role from database 
	string get(string username)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search = AT_GLOBALS.server.atDB.FindAll(
			type, 
			EDF_DbFind.Field("username").Contains(username)
		);
		array<ref EDF_DbEntity> results = search.GetEntities();
		if (results.Count() < 1)
			return string.Empty;
		
		RBAC_STORE result = RBAC_STORE.Cast(results.Get(0));
		return result.role;
	}
}

class RBAC_STORE : EDF_DbEntity
{
	string username;
	string role;
	void RBAC_STORE()
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
	}
}