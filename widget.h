#ifndef WIDGET_H
#define WIDGET_H

#include "qcustomplot.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QVBoxLayout* vBoxLayoutMain;
    QCustomPlot* customPlot;

    QCPItemTracer* tracer;
    QCPItemText *tracerLabel;

    void loadBasicQCustomPlot();    // 基本绘图
    void loadSimpleQCustomPlot();   // 衰减余弦函数及其指数
    void loadMutiAxisQCustomPlot(); // 多轴和高级样式
    void loadDateQCustomPlot(); // 日期和时间数据
};
#endif // WIDGET_H
