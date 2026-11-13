#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QTextDocument>
#include <QPlainTextEdit>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

    //接收主窗口传过来的编辑器指针
    void setTextEdit(QPlainTextEdit *edit);

private slots:
    void on_btFindNext_clicked();
    void on_btReplace_clicked();
    void on_btReplaceAll_clicked();
    void on_btCancel_clicked();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit *m_textEdit; // 保存主窗口的编辑器指针

        // 执行一次查找
        bool doFindNext();
};

#endif // REPLACEDIALOG_H
