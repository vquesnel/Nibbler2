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
		int		_diffTime;

		int 	_getMilliCount();

	public:
		Timer();
		Timer(int diff);
		// Timer(Timer const & src);
		~Timer();

		bool				update();

		// Timer &			operator=(Timer const & rhs);
};


#endif
