#ifndef ICOREAUDIO_HPP
# define ICOREAUDIO_HPP



enum eSound { Theme, Croc, Damage, Failure };

class ICoreAudio {
    public:
        virtual ~ICoreAudio(void) {}

        virtual void        play(eSound sound) const = 0;
};

#endif
