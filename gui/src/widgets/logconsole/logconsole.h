//
// Created by alex on 7/20/19.
//

#ifndef GIE_LOGCONSOLE_H
#define GIE_LOGCONSOLE_H

#include <QWidget>
#include <QTextEdit>

class LogConsole: public QWidget
{
    Q_OBJECT
public:
    explicit LogConsole(QWidget* parent = nullptr);

public Q_SLOTS:
    void logError(const QString&);
    void logWarning(const QString&);

private:
    QTextEdit* m_logArea;
};


#endif //GIE_LOGCONSOLE_H
