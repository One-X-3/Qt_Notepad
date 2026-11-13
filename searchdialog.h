#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QTextDocument>
#include <QPlainTextEdit>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

    void setTextEdit(QPlainTextEdit *edit);

    // 执行查找，返回是否找到
       bool doFindNext();

private slots:
    void on_btFindNext_clicked();

    void on_btCancel_clicked();

private:
    Ui::SearchDialog *ui;
    QPlainTextEdit *m_textEdit;
};

#endif // SEARCHDIALOG_H
