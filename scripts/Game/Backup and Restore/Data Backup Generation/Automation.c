/*
Develop scripts to automate the backup process, ensuring regular and consistent data backups.

class Automation
{
	///////////////
	// functions //
	///////////////
	
	//! Run backup and save in server's profile folder
	//! TODO Backup passwords
	void runBackup()
	{
		Print("Starting backup", LogLevel.WARNING);
		
		// Critical data to backup
		array<string> files = {};
		FileIO.FindFiles(files.Insert, "$profile:", ".ATbin");
		System.FindFiles(files.Insert, "$profile:", ".ATjson");
		
		int year, month, day, hour, minute, second;
		System.GetYearMonthDay(year, month, day);
		System.GetHourMinuteSecond(hour, minute, second);
		//string dateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		
		// Make backup directory(s)
		string dirForBackup = "$profile:"+"/AdminTooling/Backup/"+year.ToString()+"/"+month.ToString()+"/"+day.ToString();
		SCR_FileIOHelper.CreateDirectory(dirForBackup);
		
		string fileForBackup = dirForBackup + "/" + hour.ToString() + "h" + minute.ToString() + "m" + second.ToString() + "s.ATbackup";
		SCR_BinSaveContext backup = new SCR_BinSaveContext();
		backup.SaveToFile(fileForBackup);
	}
}
*/