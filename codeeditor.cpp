#include "codeeditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    // 连接信号与槽
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, [=]() {
        // 当光标位置改变时，触发 updateRequest 更新行号高亮
        updateLineNumberArea(cursorRect(), 0);
    });

    updateLineNumberAreaWidth(0);
}

void CodeEditor::setLineNumberAreaVisible(bool visible)
{
    m_lineNumberAreaVisible = visible;
    lineNumberArea->setVisible(visible);
    updateLineNumberAreaWidth(0); // 重新计算边距
}

bool CodeEditor::isLineNumberAreaVisible() const
{
    return m_lineNumberAreaVisible;
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    if (m_lineNumberAreaVisible) {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    } else {
        setViewportMargins(0, 0, 0, 0);
    }
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (!m_lineNumberAreaVisible) return;

    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

int CodeEditor::lineNumberAreaWidth()
{
    if (!m_lineNumberAreaVisible) return 0;

    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    // 根据字体计算宽度
    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    if (!m_lineNumberAreaVisible) return;

    QPainter painter(lineNumberArea);
    // 绘制背景色
    painter.fillRect(event->rect(), QColor(240, 240, 240));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    // 遍历可见的文本块，绘制行号
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::darkGray);
            painter.drawText(0, top, lineNumberArea->width() - 5, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
