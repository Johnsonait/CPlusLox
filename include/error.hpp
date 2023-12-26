#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

namespace Lox {

class Error {
public:
    Error() noexcept = default;
    Error(const Error&) = default;
    explicit Error(const std::string& what) : _what{what}
    {}
    virtual ~Error() = default;
    virtual std::string what() {return _what;}

private:
    std::string _what;

};

} // Lox namespace

#endif