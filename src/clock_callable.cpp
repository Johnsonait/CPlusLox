#include "../include/clock_callable.hpp"

namespace Lox {

int ClockCallable::arity() {return 0;}

Value ClockCallable::call(Interpreter* interpreter, std::list<Value>& args) {
    using namespace std::chrono;

    // Grab ms since 1970 and cast to double
    auto now = duration<double>(duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();

    return Value{now};
}

} // Lox namespace