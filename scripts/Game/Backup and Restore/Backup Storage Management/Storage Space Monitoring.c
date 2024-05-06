class StorageSpaceMonitoring
{
	private string directory;
	
	void StorageSpaceMonitoring(string directoryToMonitor)
	{
		directory = directoryToMonitor;
	}
	void ~StorageSpaceMonitoring()
	{
		directory = "";
	}
	
	//! Check if there is space for backup file
	bool isSpaceLeft()
	{
		array<string> backupFiles = new array<string>();
		FileIO.FindFiles(backupFiles.Insert, "$profile:", BackupAndRestore.backupsExtension);
		
		if (backupFiles.Count() == 0)
			return true;
		
		if (backupFiles.Count() > BackupAndRestore.backUpLimit)
		{
			Print("Backup limit reached", LogLevel.WARNING);
			return false;
		}
		
		return true;
		/*
		array<int> fileSize = new array<int>;
		foreach (string backupFile : backupFiles)
		{
			FileHandle fileHandle = FileIO.OpenFile(backupFile, FileMode.READ);
			int fileSizeInBytes = fileHandle.GetLength();
			fileHandle.Close();
			fileSize.Insert(fileSizeInBytes);
		}
		
		
		return false;
		*/
	}
}