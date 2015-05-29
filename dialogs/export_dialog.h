#ifndef EXPORT_DIALOG_H
#define EXPORT_DIALOG_H

#include <QDialog>
#include <QSyntaxHighlighter>

class IdHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    IdHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent) {}

    void highlightBlock(const QString & text) {

        QTextCharFormat sep;
        sep.setForeground(Qt::darkRed);
        QString pattern = "\\W";

        QRegExp expression(pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, sep);
            index = text.indexOf(expression, index + length);
        }

        QTextCharFormat socialId;
        socialId.setFontWeight(QFont::Bold);
        socialId.setForeground(Qt::darkBlue);
        pattern = "\\*(vk|sc)\\w+";

        QRegExp expression2(pattern);
        index = text.indexOf(expression2);
        while (index >= 0) {
            int length = expression2.matchedLength();
            setFormat(index, length, socialId);
            index = text.indexOf(expression2, index + length);
        }
    }
};

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog {
    Q_OBJECT

public:
    ExportDialog(QWidget * parent);
    ~ExportDialog();

    QStringList getUids();
private slots:
    void on_uids_textChanged();

private:
    Ui::ExportDialog * ui;
    IdHighlighter * highlighter;
};

#endif // EXPORT_DIALOG_H
