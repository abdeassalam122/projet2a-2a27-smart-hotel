// Dans logviewer.h
#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QTextEdit>
#include <QString>

class LogViewer : public QDialog
{
    Q_OBJECT  // Cette macro est nécessaire pour le mécanisme de signaux/slots de Qt

public:
    explicit LogViewer(QWidget *parent = nullptr);
    ~LogViewer();

    static void writeLog(const QString &action, const QString &details);


private:
    QTextEdit *logViewer;
    static QString logFilePath;

    void loadLogs();  // Méthode pour charger et afficher les logs
};

#endif // LOGVIEWER_H
