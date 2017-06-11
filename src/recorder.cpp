#include "recorder.h"
#include <stdexcept>
#include <QVector>
#include "modes/modes-data.h"
#include "network/feed-counter.h"

using namespace MM2Capture;

Recorder::Recorder(QObject *prnt): QThread(prnt),
    m_pDbWriter{new DBWriter()}
{
}

void
Recorder::startWork() {
    if (m_strOutFile.isEmpty())
        throw std::runtime_error("Recorder::start(): empty filrname");
    if (!m_pInputStream)
        throw std::runtime_error("Recorder::start(): no input");
    m_pDbWriter->setFilename(m_strOutFile);
    m_pDbWriter->open(m_pInputStream->id());
    m_pInputStream->start();
}

void
Recorder::stopWork()
{
    m_pDbWriter->close();
    m_pInputStream->stop();
}

void
Recorder::run() {
    try {
        startWork();
    } catch (const std::runtime_error &exc) {
        emit error(QString(exc.what()));
        stopWork();
        return;
    }
    QVector<ModesData> msgs;
    while (true) {
        if (isInterruptionRequested()) {
            stopWork();
            return;
        }
        if (*m_pInputStream >> msgs) {
            unsigned nMsgs = msgs.size();
            while (nMsgs) {
                nMsgs -= m_pDbWriter->addMessages(msgs);
            }
            emit networkStatsUpdated(m_pInputStream->stats());
        }
    }
    stopWork();
}
