#ifndef CUMIXING_H_1718778164
#define CUMIXING_H_1718778164

#include <QtGlobal>
#include <atomic>

namespace COM {
class CUMixing {
private:
    std::atomic_bool is_mixingOccupy{};

public:
    CUMixing();
    Q_DISABLE_COPY_MOVE(CUMixing)

public:
    //! 注意，在机器上一共有4个混匀孔
    //! 而迎凯只用其中两个
    //! 但我们目前只用一个，暂时允许先转发一下
    bool Is_mixingOccupy() {
        return is_mixingOccupy;
    }
    void Set_mixingOccupy(bool flag) {
        is_mixingOccupy = flag;
    }
};
}  // namespace COM
#endif  // CUMIXING_H_1718778164
