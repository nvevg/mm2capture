/*! \file Input data parser (so called "decoder") and parsing backends implementation
 */
#ifndef _MODES_DECODER_H
#define _MODES_DECODER_H
#include <QScopedPointer>
#include <QScopedArrayPointer>
#include <QVector>

#include "modes-data.h"

namespace MM2Capture {

/*! Parsing backend for BEAST input format
 */
class DecoderBackendBeast {
public:
    DecoderBackendBeast(): m_nPayload{0}, m_state{BeastDecoderState::FindEsc},
        m_buffer{new char[BUFFER_LENGTH_BYTES]} {
    }
    /*! \brief Tries to parse input data, (hopefully) containing sequence of BEAST data frames and store resulting ModesData objects in outMessages.
     * \returns Number of succesfully parsed messages (equal with outMessages.length())
     * \param [in] data - input bytearray (for example, just read from input AbstractInputFeed)
     * \param [out] outMessages - vector to push_back parsed messages
     */
    unsigned tryDecode(const QByteArray &data, QVector<ModesData> &outMessages);
private:
    enum class BeastDecoderState {
        FindEsc,
        FindType,
        Data,
        Quote
    };

    static const int BUFFER_LENGTH_BYTES = 128;

    unsigned getFrameLength(char);
    unsigned m_nPayload; // current m_buffer length
    unsigned m_frameLength; // expected frame length
    BeastDecoderState m_state;
    QScopedArrayPointer<char> m_buffer; // current frame data
};

/*! \brief Parser frontend.
 *  \details ModesDecoder tries to determine input bytestream format (while only BEAST) and uses
 * approprite parsing backend.
 */
class ModesDecoder {
public:
    ModesDecoder(): m_type{ModesData::MessageType::None},
        m_pBeastDecoder{new DecoderBackendBeast()} {
    }
    /*!
     * \brief Tries to determine input data format to use appropriate backend.
     * \details Makes a call to appropriate parsing backend.
     * \returns Number of succesfully parsed messages (equal with outMessages.length())
     * \param [in] data - input bytearray (for example, just read from input AbstractInputFeed)
     * \param [out] outMessages - vector to push_back parsed messages
     */
    unsigned tryDecode(const QByteArray &data, QVector<ModesData> &outMessages);
private:
    ModesData::MessageType m_type;
    QScopedPointer<DecoderBackendBeast> m_pBeastDecoder;
};

}
#endif
