#ifndef hiResTime_h__
#define hiResTime_h__

#include <windows.h>

namespace aks
{
    namespace time
    {
		#define LRGINT LARGE_INTEGER
		
		typedef struct
		{
		    LRGINT start;
		    LRGINT stop;
		} Timer;
		
		class HighResStopWatch
		{
		private:
            double TotalTimeElapsedSinceClockInitialization;
            Timer TotalTimeWatch;
			Timer StopWatch;
			LRGINT frequency;
            double LargeIntToSeconds( LRGINT & L)
            {
                return ((double)L.QuadPart /(double)frequency.QuadPart);
            }
            double getElapsedTime( Timer& aTimer )
            {
                LRGINT time;
                time.QuadPart = aTimer.stop.QuadPart - aTimer.start.QuadPart;
                return LargeIntToSeconds( time ) ;
            }
		public:
			HighResStopWatch( void )
            {
                StopWatch.start.QuadPart = 0;
                StopWatch.stop.QuadPart = 0;	
                QueryPerformanceFrequency( &frequency );
                QueryPerformanceCounter( &TotalTimeWatch.start );
            }
			void startTimer( void )
            {
                QueryPerformanceCounter(&StopWatch.start);
            }
			void stopTimer( void )
            {
                QueryPerformanceCounter(&StopWatch.stop);
            }
			double getElapsedTime( void )
            {
                return getElapsedTime( StopWatch );
            }
            double getTotalTimeElapsedSinceInitialization( void )
            {
                QueryPerformanceCounter( &TotalTimeWatch.stop );
                return getElapsedTime( TotalTimeWatch );           
            }
		};
    }
}

#endif // hiResTime_h__
