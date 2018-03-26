#pragma once
#include <iostream>//uint32_t
#include "settings.h"

class value_modifier;

class progress {
    friend class value_modifier;
    using val_t = uint32_t;

    const val_t ticks;
    val_t state;
public:
    template <class duration_t>
    progress(std::chrono::duration<duration_t> duration) :
        ticks(std::chrono::duration_cast<decltype(settings::internal::delay)>(duration) / settings::internal::delay),
        state(0)
    {}

    void tick() {
        state++;
    }

    bool is_finished() const {
        return state == ticks;
    }
};

class value_modifier {
    progress progress_p;
public:
    value_modifier(progress p) : progress_p(p) {}

    template <class T>
    T linear(T from, T to) {
        return from + static_cast<float>(progress_p.ticks) / progress_p.state * (to - from);
    }
};