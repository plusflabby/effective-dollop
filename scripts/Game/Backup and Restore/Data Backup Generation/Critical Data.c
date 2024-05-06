class CriticalDataToBackup
{
	/* variables */
	private ref array<string> extensions = {".ATbin", ".ATjson"};
	private ref array<string> files = new array<string>();
	
	/* constructor */
	void CriticalDataToBackup()
	{
		foreach (string extension : extensions)
		{
			FileIO.FindFiles(files.Insert, "$profile:", extension);
		}
	}
	
	/* functions */
	//! get the files to be included in backup
	array<string> getFiles()
	{
		return files;
	}
	
	/* de-constructor */
	void ~CriticalDataToBackup()
	{
		extensions = {};
		files = {};
	}
}