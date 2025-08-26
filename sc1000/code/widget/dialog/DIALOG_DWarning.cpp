#include "DIALOG_DWarning.h"

#include "ui_DIALOG_DWarning.h"

namespace WIDGET::DIALOG {

DWarningDialog::DWarningDialog(QWidget* parent) : QWidget(parent), ui(new Ui::DWarning{}) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground);
    if (parent != nullptr) {
        setGeometry(parent->rect());
    }

    m_tts = new QTextToSpeech(this);
    Init_tts(m_tts);

    connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
        this->close();
        deleteLater();
    });
}

DWarningDialog::DWarningDialog(const QString& text, QWidget* parent) : DWarningDialog(parent) {
    ui->label->setText(text);
}

DWarningDialog::DWarningDialog(const QString& text, const QString& title, QWidget* parent)
    : DWarningDialog(parent) {
    ui->label->setText(text);
    ui->lab_title->setText(title);
}

void DWarningDialog::Speech_once(const QString& word, double volume) {
    m_tts->setVolume(volume);
    /// 异步播放的
    m_tts->say(word);
}

void DWarningDialog::Speech_keep(const QString& word, double volume) {
    connect(m_tts, &QTextToSpeech::stateChanged, [this, word](QTextToSpeech::State state) {
        /// 语音合成结束 | 语音合成出错
        /// 两者均在用户层结束，后面的都给内核层处理
        if (state == QTextToSpeech::State::Ready) {
            m_tts->say(word);
        } else if (state == QTextToSpeech::BackendError) {
            qWarning() << "语音合成异常";
        }
    });
    m_tts->setVolume(volume);
    m_tts->say(word);
}

void DWarningDialog::Init_tts(QTextToSpeech* tts) {
    assert(tts);
    tts->setLocale(QLocale::Chinese);
    tts->setVolume(1.0);
    auto voiceList = tts->availableVoices();
    for (auto voice : voiceList) {
        if (voice.name().contains("Huihui")) {
            tts->setVoice(voice);
            break;
        }
    }
}

}  // namespace WIDGET::DIALOG
