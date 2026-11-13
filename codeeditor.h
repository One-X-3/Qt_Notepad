#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    void setLineNumberAreaVisible(bool visible);
    bool isLineNumberAreaVisible() const;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    // 重写 resizeEvent，用于调整行号区域的宽度和位置
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // 更新行号区域的宽度
    void updateLineNumberAreaWidth(int newBlockCount);
    // 当文本改变或滚动时，更新行号区域
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    bool m_lineNumberAreaVisible = true; // 控制是否显示行号
};

// 定义行号区域的内部类
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
