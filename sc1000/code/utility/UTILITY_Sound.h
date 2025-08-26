#ifndef UTILITY_SOUND_H_1723188833
#define UTILITY_SOUND_H_1723188833

#include <QTextToSpeech>

namespace UTILITY {
struct Sound {
    static void Init_tts(QTextToSpeech* tts);

    static QTextToSpeech* Speech_Word(const QString& word, double volume = 1);
};
}  // namespace UTILITY

#endif  // UTILITY_SOUND_H_1723188833
