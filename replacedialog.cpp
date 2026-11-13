#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QMessageBox>


ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog),
        m_textEdit(nullptr)
{
    ui->setupUi(this);

    // 默认选中向下查找
        ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::setTextEdit(QPlainTextEdit *edit)
{
    m_textEdit = edit;
}

// 核心查找逻辑
bool ReplaceDialog::doFindNext()
{
    if (!m_textEdit) return false;

    QString target = ui->FindTargetText->text();
    if (target.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入查找内容！");
        return false;
    }

    // 查找什么文本，获取文本框内容
    QTextDocument::FindFlags flags;
    if (ui->cbCaseSensitive->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (ui->rbUp->isChecked()) {
        flags |= QTextDocument::FindBackward;
    }

    // 执行查找
    bool found = m_textEdit->find(target, flags);

    // 循环查找
    if (!found && ui->rbDown->isChecked()) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        m_textEdit->setTextCursor(cursor);
        found = m_textEdit->find(target, flags);
    } else if (!found && ui->rbUp->isChecked()) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        m_textEdit->setTextCursor(cursor);
        found = m_textEdit->find(target, flags);
    }

    if (!found) {
        QMessageBox::information(this, "查找", "未找到指定的内容: " + target);
    }

    return found;
}

// 查找下一个
void ReplaceDialog::on_btFindNext_clicked()
{
    doFindNext();
}

// 单次替换
void ReplaceDialog::on_btReplace_clicked()
{
    if (!m_textEdit) return;

    QString target = ui->FindTargetText->text();
    QString replaceStr = ui->ReplaceText->text();

    if (target.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入查找内容！");
        return;
    }

    QTextCursor cursor = m_textEdit->textCursor();

    // 如果当前选中的文本就是我们要找的，直接替换
    if (cursor.hasSelection() && cursor.selectedText() == target) {
        cursor.insertText(replaceStr);
        // 替换后，自动查找下一个
        doFindNext();
    } else {
        // 如果没有选中要查找的词，则先执行一次查找
        if (doFindNext()) {
            // 找到之后，再次点击替换才会替换
        }
    }
}

// 全部替换
void ReplaceDialog::on_btReplaceAll_clicked()
{
    if (!m_textEdit) return;

    QString target = ui->FindTargetText->text();
    QString replaceStr = ui->ReplaceText->text();

    if (target.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入查找内容！");
        return;
    }

    //将光标移动到文档开头
    QTextCursor cursor = m_textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    m_textEdit->setTextCursor(cursor);

    int replaceCount = 0; // 记录替换了多少次

    //循环查找并替换
    while (true) {
        // 每次替换前，重置查找标志
        QTextDocument::FindFlags flags;
        if (ui->cbCaseSensitive->isChecked()) {
            flags |= QTextDocument::FindCaseSensitively;
        }

        // 执行查找
        bool found = m_textEdit->find(target, flags);

        if (found) {
            // 找到后，光标会选中该词，直接插入替换文本
            QTextCursor currentCursor = m_textEdit->textCursor();
            currentCursor.insertText(replaceStr);

            m_textEdit->setTextCursor(currentCursor);
            replaceCount++;
        } else {
            // 找不到就跳出循环
            break;
        }
    }

    //反馈结果
    if (replaceCount > 0) {
        QMessageBox::information(this, "提示",
            QString("成功替换了 %1 处内容！").arg(replaceCount));
    } else {
        QMessageBox::information(this, "提示", "未找到指定的内容，未进行任何替换。");
    }
}

// 取消按钮关闭窗口
void ReplaceDialog::on_btCancel_clicked()
{
    this->reject();
}
