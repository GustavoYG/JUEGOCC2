#ifndef TIMERFUNCTOR_H
#define TIMERFUNCTOR_H

#include <SFML/System/Clock.hpp>

class TimerFunctor {
public:
    TimerFunctor(float delay);
    bool operator()();

private:
    float delay_;
    sf::Clock clock_;
    float elapsedTime_;
};

#endif // TIMERFUNCTOR_H
