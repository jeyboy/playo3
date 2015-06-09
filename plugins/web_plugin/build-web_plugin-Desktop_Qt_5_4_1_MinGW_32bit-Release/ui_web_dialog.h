/********************************************************************************
** Form generated from reading UI file 'web_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEB_DIALOG_H
#define UI_WEB_DIALOG_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_WebDialog
{
public:
    QGridLayout *gridLayout;
    QWebView *webView;

    void setupUi(QDialog *WebDialog)
    {
        if (WebDialog->objectName().isEmpty())
            WebDialog->setObjectName(QStringLiteral("WebDialog"));
        WebDialog->resize(684, 383);
        gridLayout = new QGridLayout(WebDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(3, 3, 3, 3);
        webView = new QWebView(WebDialog);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));

        gridLayout->addWidget(webView, 0, 0, 1, 1);


        retranslateUi(WebDialog);

        QMetaObject::connectSlotsByName(WebDialog);
    } // setupUi

    void retranslateUi(QDialog *WebDialog)
    {
        WebDialog->setWindowTitle(QApplication::translate("WebDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class WebDialog: public Ui_WebDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEB_DIALOG_H
