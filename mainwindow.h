#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
#include <QFont>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    // 文档修改状态响应槽函数
    void documentWasModified();


    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionSelectAll_triggered();

    void on_actionFontColor_triggered();

    void on_actionEditorBackgroundColor_triggered();

    void on_actionFontBackgroundColor_triggered();

    void on_actionAutoLineWrap_triggered();

    void on_actionFont_triggered();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

      QString currentFilePath; // 记录当前打开文件的绝对路径

      void setCurrentFile(const QString &fileName); // 更新窗口标题
      bool maybeSave(); // 检查是否有未保存的修改，并询问用户
      bool saveFile(const QString &fileName); // 实际执行保存动作的底层函数

      void updateEditActions();//用于动态控制按钮可用性

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
