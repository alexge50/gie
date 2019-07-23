//
// Created by alex on 7/20/19.
//

#include "logconsole.h"

#include <QVBoxLayout>

LogConsole::LogConsole(QWidget *parent):
    QWidget{parent}
{
    auto vlayout = new QVBoxLayout{this};
    vlayout->addWidget(m_logArea = new QTextEdit{});

    m_logArea->setReadOnly(true);
}

void LogConsole::logError(const QString& message)
{
    m_logArea->setTextColor(QColor(255, 0, 0));
    m_logArea->append(message);
}

void LogConsole::logWarning(const QString& message)
{
    m_logArea->setTextColor(QColor(255, 255, 0));
    m_logArea->append(message);
}
