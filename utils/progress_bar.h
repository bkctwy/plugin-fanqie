#pragma once
#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <fmt/core.h>

void progress_bar(int total, int current)
{
    // 计算进度百分比
    float progress = static_cast<float>(current) / total;
    int bar_width = 50; // 进度条宽度

    // 构建进度条字符串
    std::string bar;
    bar.reserve(bar_width + 10); // 预分配内存
    bar += "[";

    int pos = static_cast<int>(bar_width * progress);
    for (int i = 0; i < bar_width; ++i)
    {
        if (i < pos)
            bar += "=";
        else if (i == pos)
            bar += ">";
        else
            bar += " ";
    }
    bar += "]";

    // 使用 fmt 格式化输出
    fmt::print("\r{} {:3.0f}%", bar, progress * 100.0f);
    std::fflush(stdout); // 刷新输出
}

#endif // PROGRESS_BAR_H