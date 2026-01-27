// src/MarkdownRenderer.cpp
#include "MarkdownRenderer.h"
#include <cmark.h>

QString MarkdownRenderer::markdownToHtml(const QString& markdown)
{
    QByteArray utf8 = markdown.toUtf8();
    char* html = cmark_markdown_to_html(utf8.constData(), utf8.size(), CMARK_OPT_DEFAULT);
    
    if (!html) {
        return "<html><body><p>Error rendering markdown.</p></body></html>";
    }

    QString result = QString::fromUtf8(html);
    
    // 使用 cmark 的分配器释放内存，避免 debug/release CRT 不匹配导致的问题
    cmark_mem *mem = cmark_get_default_mem_allocator();
    mem->free(html);

    // 添加 CSS 样式控制图片大小 (最大宽度100%，保持纵横比)
    QString css = R"(
        <style>
            img {
                max-width: 100%;
                height: auto;
            }
            body { 
                font-family: "Segoe UI", sans-serif; 
                line-height: 1.6; 
                padding: 20px;
                color: #333;
            }
            code { 
                background-color: #f0f0f0; 
                padding: 2px 4px; 
                border-radius: 4px; 
            }
        </style>
    )";

    return "<html><head>" + css + "</head><body>" + result + "</body></html>";
}