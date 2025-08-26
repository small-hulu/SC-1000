#ifndef EQUIPSINGLETON_H_1709715497
#define EQUIPSINGLETON_H_1709715497

#include <functional>


template <typename Class>
class ISingleton {
private:
    ISingleton(const ISingleton&)  = delete;
    ISingleton(ISingleton&&)       = delete;
    Class& operator=(const Class&) = delete;
    Class& operator=(Class&&)      = delete;

protected:
    ISingleton() = default;

public:
    static Class& instance() {
        static Class obj{};
        return obj;
    }
};


#endif  // EQUIPSINGLETON_H_1709715497
