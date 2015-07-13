#ifndef EXPORT_DIALOG_H
#define EXPORT_DIALOG_H

#include <qdialog.h>
#include <qsyntaxhighlighter.h>

class IdHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    IdHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent) {}

    void highlightBlock(const QString & text) {
        QTextCharFormat sep;
        sep.setFontWeight(QFont::Bold);
        sep.setForeground(Qt::darkRed);
        setFormat(0, text.length(), sep);

        QTextCharFormat socialId;
        socialId.setForeground(Qt::darkBlue);
        QString pattern = QStringLiteral("\\b(vk|sc)[\\d\\-_]+\\b");

        QRegExp expression2(pattern);
        int index = text.indexOf(expression2);
        while (index >= 0) {
            int length = expression2.matchedLength();
            setFormat(index, length, socialId);
            index = text.indexOf(expression2, index + length);
        }
    }
};

namespace Ui { class ExportDialog; }

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
