#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length: " + QString::number(0) + "   lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "   Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("杜奕贤"));
    ui->statusbar->addPermanentWidget(author);

    //初始化文件路径和标题
    currentFilePath = "";

    // 连接文档修改信号，用于标题栏显示 * 号
    connect(ui->textEdit->document(), &QTextDocument::modificationChanged,
            this, &MainWindow::documentWasModified);

    setWindowTitle("无标题 - OneX文本编辑器[*]");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg;
    dlg.exec();
}

//新建文件
void MainWindow::on_actionNew_triggered()
{
    if (maybeSave()) { // 如果允许继续（已保存或用户放弃保存）
           ui->textEdit->clear();
           currentFilePath = "";
           ui->textEdit->document()->setModified(false); // 重置修改状态
           setWindowTitle("无标题 - OneX文本编辑器[*]");
       }
}

//打开文件
void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave()) {
            QString fileName = QFileDialog::getOpenFileName(this,
                "打开文件", QString(), "文本文件 (*.txt);;所有文件 (*)");

            if (!fileName.isEmpty()) {
                QFile file(fileName);
                // 边界处理：打开失败提示
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QMessageBox::warning(this, "错误", "无法打开文件: " + file.errorString());
                    return;
                }

                QTextStream in(&file);
                ui->textEdit->setPlainText(in.readAll());
                file.close();

                currentFilePath = fileName;
                ui->textEdit->document()->setModified(false);
                setCurrentFile(currentFilePath); // 更新标题
            }
        }
}

// 保存文件
void MainWindow::on_actionSave_triggered()
{
    if (currentFilePath.isEmpty()) {
        // 如果是新建的没有路径，直接跳转到另存为
        on_actionSaveAs_triggered();
    } else {
        // 已有路径，直接保存
        saveFile(currentFilePath);
    }
}

//另存为文件
void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "另存为", QString(), "文本文件 (*.txt);;所有文件 (*)");

    if (fileName.isEmpty()) return; // 用户取消了

    currentFilePath = fileName;
    if (saveFile(currentFilePath)) {
        setCurrentFile(currentFilePath); // 更新标题
    }
}

// 实际写入文件的底层函数
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法保存文件: " + file.errorString());
        return false;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    // 保存成功后，清除修改标记
    ui->textEdit->document()->setModified(false);
    return true;
}

// 检查未保存状态
bool MainWindow::maybeSave()
{
    if (!ui->textEdit->document()->isModified()) {
        return true; // 没有修改，直接放行
    }

    QMessageBox::StandardButton ret = QMessageBox::warning(this, "提示",
        "文档已被修改，是否保存？",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (ret == QMessageBox::Save) {
        on_actionSave_triggered(); // 触发保存
        //如果用户点保存后又取消了保存路径，这里需要再判断一次是否真的保存成功了
        return !ui->textEdit->document()->isModified();
    } else if (ret == QMessageBox::Cancel) {
        return false; // 取消操作
    }

    return true; // Discard (不保存)
}

// 更新标题栏文件名
void MainWindow::setCurrentFile(const QString &fileName)
{
    QString shownName = "无标题";
    if (!fileName.isEmpty()) {
        shownName = QFileInfo(fileName).fileName(); // 只提取文件名
    }
    // [*] 是占位符，Qt 会根据修改状态自动在前面显示 * 号
    setWindowTitle(QString("%1 - OneX文本编辑器[*]").arg(shownName));
}

// 响应文档修改信号
void MainWindow::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept(); // 允许关闭
    } else {
        event->ignore(); // 阻止关闭
    }
}
