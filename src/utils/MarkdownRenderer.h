// src/MarkdownRenderer.h
#ifndef MARKDOWNRENDERER_H
#define MARKDOWNRENDERER_H

#include <QString>

class MarkdownRenderer
{
public:
    static QString markdownToHtml(const QString& markdown);
};

#endif // MARKDOWNRENDERER_H