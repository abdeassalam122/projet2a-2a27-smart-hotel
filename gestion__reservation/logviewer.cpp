// Dans logviewer.cpp
#include "logviewer.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QDateTime>

QString LogViewer::logFilePath = "application.log"; // Chemin du fichier log

LogViewer::LogViewer(QWidget *parent)
    : QDialog(parent), logViewer(new QTextEdit(this))
{
    setWindowTitle("Application Logs");

    QVBoxLayout *layout = new QVBoxLayout(this);
    logViewer->setReadOnly(true);
    layout->addWidget(logViewer);

    QPushButton *refreshButton = new QPushButton("Refresh Logs", this);
    connect(refreshButton, &QPushButton::clicked, this, &LogViewer::loadLogs);
    layout->addWidget(refreshButton);

    loadLogs(); // Charger les logs au démarrage
}

LogViewer::~LogViewer() {}

void LogViewer::loadLogs()
{
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&logFile);
        logViewer->setPlainText(stream.readAll());
        logFile.close();
    } else {
        logViewer->setPlainText("Unable to load logs.");
    }
}

// Définition de la fonction writeLog avec deux paramètres
void LogViewer::writeLog(const QString &action, const QString &details) {
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            << " - " << action << details << "\n";
    }
}

