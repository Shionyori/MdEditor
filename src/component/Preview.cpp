// src/Preview.cpp
#include "Preview.h"
#include "../utils/MarkdownRenderer.h"
#include <QTimer>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QUrl>

Preview::Preview(Editor* editor, QWidget *parent)
    : QWebEngineView(parent), editor(editor)
{
    // 防抖：窗口 resize 后 n ms 内不再调用 setHtml
    resizeTimer = new QTimer(this);
    resizeTimer->setSingleShot(true);
    resizeTimer->setInterval(10);
    connect(resizeTimer, &QTimer::timeout,
            this, &Preview::updatePreview);

    // 编辑器内容变化时立即刷新
    connect(editor, &QTextEdit::textChanged,
            this, &Preview::updatePreview);
}

void Preview::updatePreview()
{
    QString markdownText = editor->toPlainText();
    QString htmlText = MarkdownRenderer::markdownToHtml(markdownText);

    QString baseUrl;
    QString currentFilePath = editor->getCurrentFilePath();
    if (!currentFilePath.isEmpty()) {
        baseUrl = QFileInfo(currentFilePath).absolutePath();
    } else {
        // 对于未保存的文件（如默认文档），默认以 res 目录作为基准路径
        baseUrl = QApplication::applicationDirPath() + "/res";
    }
    
    if (!baseUrl.endsWith("/")) baseUrl += "/";

    setHtml(htmlText, QUrl::fromLocalFile(baseUrl));
}

// 覆盖 resizeEvent，只在最后一次 resize 完成后刷新
void Preview::resizeEvent(QResizeEvent *event)
{
    QWebEngineView::resizeEvent(event);
    resizeTimer->start();   // 重启防抖计时
}