#ifndef TIMER_HPP
# define TIMER_HPP


# include <time.h>
# include <sys/timeb.h>
// # include <unistd.h>
// # include <stdlib.h>


class Timer {
	private:
		int		_refTimer;
		int		_newTimer;
		float	_diffTime;

		int 	_getMilliCount();

	public:
		Timer();
		Timer(float diff);
		// Timer(Timer const & src);
		~Timer();
		void 				changeDiff(float newdiff);
		void        		resetDiff(float diff);
		bool				update();

		// Timer &			operator=(Timer const & rhs);
};


#endif
