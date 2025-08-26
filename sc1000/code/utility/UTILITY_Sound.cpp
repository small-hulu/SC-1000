#include "UTILITY_Sound.h"

#include <QDebug>

namespace UTILITY {

void Sound::Init_tts(QTextToSpeech *tts) {
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

QTextToSpeech *Sound::Speech_Word(const QString &word, double volume) {
    QTextToSpeech *tts = new QTextToSpeech();
    Init_tts(tts);
    tts->setVolume(volume);

    QObject::connect(tts, &QTextToSpeech::stateChanged, [tts](QTextToSpeech::State state) {
        /// 语音合成结束 | 语音合成出错
        /// 两者均在用户层结束，后面的都给内核层处理
        if (state == QTextToSpeech::State::Ready) {
            tts->deleteLater();
        } else if (state == QTextToSpeech::BackendError) {
            qWarning() << "语音合成异常";
            tts->deleteLater();
        }
    });

    /// 异步播放的
    tts->say(word);
    return tts;
}

}  // namespace UTILITY
