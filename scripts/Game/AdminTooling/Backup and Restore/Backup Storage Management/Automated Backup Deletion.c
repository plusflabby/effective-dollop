class AutomatedBackupDeletion
{
	private ref array<string> backupFiles = new array<string>();
	
	void AutomatedBackupDeletion()
	{
		//if (BackUpLocation.PROFILE == BackUpLocation.PROFILE)
		System.FindFiles(backupFiles.Insert, "$profile:", BackupAndRestore.backupsExtension);
	}
	
	//! TODO delete backups based upon retention policy
}