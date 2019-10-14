#include <string>

#define COMPHO_REQUIRE_MSG(pred, msg)                                          \
    (compho::__require((pred), #pred, msg, __FILE__, __LINE__))
#define COMPHO_REQUIRE(pred)                                                   \
    (compho::__require((pred), #pred, "", __FILE__, __LINE__))

namespace compho {
void __require(bool pred, const std::string &str_pred, const std::string &msg,
               std::string filename, int line);
}
