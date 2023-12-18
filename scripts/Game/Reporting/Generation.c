class AT_Report_Generation
{

	private string reportHTMLBase = "<title>Report for %1</title>";
	
	//! function returns string populated with html
	string html()
	{
		return string.Format(reportHTMLBase, SCR_DateTimeHelper.GetDateTimeLocal());
	}

	
	//! For Test
//	void AT_Report_Generation()
//	{
//		Print(html(), LogLevel.WARNING);
//	}
}
//! For Test
//ref AT_Report_Generation testAT = new AT_Report_Generation();