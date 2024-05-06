class vFileSizeAndIoOperations : vSetMod
{
	// Variables
	private string sizeOfDatabase = "0 B";
	private int readOperations;
	private int writeOperations;
	
	// function
	void setValue()
	{
		ServerPerformanceMonitor.fileSizeAndIoOperationsUpdate(sizeOfDatabase, readOperations, writeOperations);
	}
	
	// constructors
	void vFileSizeAndIoOperations()
	{
		vSetModSTART();
		//
		//Print("file", LogLevel.NORMAL);
		array<string> files = {};
		FileIO.FindFiles(files.Insert, "$profile:/.db/AdminTooling", ".json");
		
		// probably will be limited if size > int
		int filesSize = 0;
		foreach (string file : files)
		{
			//Print(file, LogLevel.NORMAL);
			FileHandle fileHandler = FileIO.OpenFile(file, FileMode.READ);
			int fileSizeInBytes = fileHandler.GetLength();
			fileHandler.Close();
			
			if (filesSize + fileSizeInBytes > filesSize.MAX)
				filesSize = filesSize.MAX;
			else
				filesSize = filesSize + fileSizeInBytes;
		}
		
		//Print(SCR_ByteFormat.GetReadableSize(filesSize + 0.0), LogLevel.NORMAL);
		sizeOfDatabase = SCR_ByteFormat.GetReadableSize(filesSize + 0.0);
		
		setValue();
	}
	
	void ~vFileSizeAndIoOperations()
	{
		sizeOfDatabase = "0 B";
		vSetModSTOP();
	}
}

//ref vFileSizeAndIoOperations vfszio = new vFileSizeAndIoOperations();