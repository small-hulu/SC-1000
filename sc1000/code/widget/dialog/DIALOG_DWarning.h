#ifndef WIDGET_WARNINGDIALOG_H_1715828699_123
#define WIDGET_WARNINGDIALOG_H_1715828699_123

#include <QSharedPointer>
#include <QTextToSpeech>
#include <QWidget>

namespace Ui {
class DWarning;
}

namespace WIDGET::DIALOG {
class DWarningDialog : public QWidget {
    Q_OBJECT

private:
    QSharedPointer<Ui::DWarning> ui;

private:
    /// 警告语音播放
    QTextToSpeech* m_tts = nullptr;

public:
    DWarningDialog(QWidget* parent);
    DWarningDialog(const QString& text, QWidget* parent);
    DWarningDialog(const QString& text, const QString& title, QWidget* parent);

public:
    //! 语音播报是异步操作
    //! 请不要重复调用下面两个函数，一把只调用一次即可
    void Speech_once(const QString& word, double volume = 1.0);
    void Speech_keep(const QString& word, double volume = 1.0);

private:
    void Init_tts(QTextToSpeech* tts);
};

}  // namespace WIDGET::DIALOG

#endif  // WIDGET_WARNINGDIALOG_H_1715828699_123
