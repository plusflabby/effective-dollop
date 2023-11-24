class vCpuUsage
{
	private float cpuUsage = 0.0;
	
	void setValue()
	{
		ServerPerformanceMonitor.cpuUsageUpdate(cpuUsage);
	}
	
	void vCpuUsage()
	{
		cpuUsage = 0.0;
		setValue();
	}
	void ~vCpuUsage()
	{
		cpuUsage = 0.0;
	}
}