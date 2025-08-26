#ifndef CUPHOTOELECTRIC_H_1718778677
#define CUPHOTOELECTRIC_H_1718778677

#include <QDebug>
#include <QtGlobal>
#include <atomic>

namespace COM {
class CUPhotoelectric {
private:
    std::atomic_bool is_occupy{};

public:
    CUPhotoelectric();
    Q_DISABLE_COPY_MOVE(CUPhotoelectric)

public:
    void Reset() {
        if (is_occupy == true) {
            qWarning() << QString("光电标志重置 true");
            is_occupy = false;
        }
    }

public:
    bool Is_occupy() {
        return is_occupy;
    }
    void Set_occupy(bool flag) {
        is_occupy = flag;
    }
};
}  // namespace COM

#endif  // CUPHOTOELECTRIC_H_1718778677
