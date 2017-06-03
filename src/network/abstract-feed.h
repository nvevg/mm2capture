/*! \file Message input stream implementation
 */
#ifndef _ABSTRACT_FEED_H
#define _ABSTRACT_FEED_H

#include <QSharedPointer>
#include <QVector>
#include "../modes/modes-data.h"
#include "feed-counter.h"

namespace MM2Capture {
class ModesDecoder;
/*!
 * \brief Abstract message input stream.
 */
class AbstractInputFeed {
public:
    AbstractInputFeed();
    //! Puts stream into the running state.
    virtual void start() = 0;
    //! Terminates stream.
    virtual void stop() = 0;
    virtual ~AbstractInputFeed() { }
    //! Returns autogenerated input's ID.
    const QString &id() const {
        return m_strId;
    }
    //! Returns feed stats accumulator (see FeedCounter)
    const FeedCounter &stats() const {
        return m_stats;
    }
    /*!
     * \brief Indicates some data was read and parsed.
     * \details Indicates if some messages was produced during the last
     * data fetch attempt
     * \returns true if messages fetched; otherwise false
     */
    virtual operator bool() const {
        return false;
    }
    /*! \brief Fetches bytes and tries to parse it.
     *  \param [out] out - resulting messages would pe pushed here
     */
    virtual AbstractInputFeed &operator>>(QVector<ModesData> &out) = 0;
protected:
    QSharedPointer<ModesDecoder> m_pDecoder;
    QString m_strId;
    FeedCounter m_stats;
};

}
#endif
