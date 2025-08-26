#ifndef APPINIT_H_1696816640
#define APPINIT_H_1696816640

namespace CONTROLLER {
class AppInit final {
public:
    static bool App_Init();
    static bool App_Exit();

private:
    static void print_debug();

    static QString init_message();

    static bool init_log();
    static bool init_font();
    static bool init_instance();
};
}  // namespace CONTROLLER

#endif  // APPINIT_H_1696816640
