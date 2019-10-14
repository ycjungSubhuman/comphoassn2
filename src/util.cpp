#include "util.hpp"
#include <sstream>
#include <stdexcept>

namespace compho {
void __require(bool pred, const std::string &str_pred, const std::string &msg,
               std::string filename, int line) {
    if (!pred) {
        std::stringstream ss;
        ss << "Requirement not met: " << str_pred << " at " << filename << "("
           << line << ") " << msg << std::endl;
        std::string msg_err = ss.str();
        throw std::runtime_error(msg_err);
    }
}
} // namespace compho
