#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog),
        m_textEdit(nullptr)
{
    ui->setupUi(this);

    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::setTextEdit(QPlainTextEdit *edit)
{
    m_textEdit = edit;
}

// 核心查找逻辑
bool SearchDialog::doFindNext()
{
    if (!m_textEdit) return false;

    QString target = ui->SearchText->text();
    if (target.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入查找内容！");
        return false;
    }

    QTextDocument::FindFlags flags;
    if (ui->checkBox->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively; // 区分大小写
    }
    if (ui->rbUp->isChecked()) {
        flags |= QTextDocument::FindBackward; // 向上查找
    }

    // 执行查找
    bool found = m_textEdit->find(target, flags);

    // 如果没找到，且是向下查找，则光标回到文件开头再找一次
    if (!found && ui->rbDown->isChecked()) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start); // 移动到开头
        m_textEdit->setTextCursor(cursor);

        found = m_textEdit->find(target, flags);
    }
    // 如果没找到，且是向上查找，则光标回到文件末尾再找一次
    else if (!found && ui->rbUp->isChecked()) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::End); // 移动到末尾
        m_textEdit->setTextCursor(cursor);

        found = m_textEdit->find(target, flags);
    }

    if (!found) {
        QMessageBox::information(this, "查找", "未找到指定的内容: " + target);
    }

    return found;
}


void SearchDialog::on_btFindNext_clicked()
{
    doFindNext();
}


void SearchDialog::on_btCancel_clicked()
{
    this->reject(); // 关闭对话框
}

