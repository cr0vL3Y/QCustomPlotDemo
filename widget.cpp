#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    vBoxLayoutMain = new QVBoxLayout(this);
    setLayout(vBoxLayoutMain);

    customPlot = new QCustomPlot(this);
    vBoxLayoutMain->addWidget(customPlot);

    // loadBasicQCustomPlot();
    // loadSimpleQCustomPlot();
    // loadMutiAxisQCustomPlot();
    // loadDateQCustomPlot();
    loadBarChatQCustomPlot();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadBasicQCustomPlot()
{
    QVector<double> x(101), y(101); // 键和值
    for (int i = 0; i < 101; i++)
    {
        x[i] = i / 50.0 - 1;
        y[i] = x[i] * x[i];
    }
    customPlot->addGraph(); // 创建新图表
    customPlot->graph(0)->setData(x, y); // 分配数据点

    // 默认情况下，QCustomPlot 有四个轴：
    // customPlot->xAxis、yAxis、xAxis2和yAxis 2，类型为QCPAxis
    // 分别对应于底部、左侧、顶部、右侧轴
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    customPlot->xAxis->setRange(-1, 1); // 定义图的哪部分可见
    customPlot->yAxis->setRange(0, 1);

    customPlot->xAxis->grid()->setSubGridVisible(true);    // gridlines 栅格线

    // 对显示的图形进行任何类型的修改，都需要调用customPlot->replot();
    // 当 QCustomPlot 调整大小或者触发内置用户交互时，会自定发生重绘
    customPlot->replot();
}

void Widget::loadSimpleQCustomPlot()
{
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));   // 第一个图形的线条颜色
    customPlot->graph(0)->setBrush(QBrush(QColor("#9575cd"))); // 第一个图形填充的颜色

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));

    // 生成数据
    QVector<double> x(251), y0(251), y1(251);
    for (int i = 0; i < 251; ++i)
    {
        x[i] = i;
        y0[i] = qExp(-i / 150.0) * qCos(i / 10.0); // 指数衰减余弦
        y1[i] = qExp(-i / 150.0);            // 指数包络
    }

    customPlot->xAxis2->setVisible(true);   // 设置顶轴可见
    customPlot->xAxis2->setTickLabels(true);   // 不显示刻度
    customPlot->yAxis2->setVisible(true);   // 设置右侧轴可见
    customPlot->yAxis2->setTickLabels(false);   // 不显示刻度

    // pass data to graphs
    customPlot->graph(0)->setData(x, y0);
    customPlot->graph(1)->setData(x, y1);

    // 设置 图表0自动缩放，开启后会使图表0完全符合可视区域
    customPlot->graph(0)->rescaleAxes();    // 默认值是false
    // 对图表1作同样的设置，但是扩大了范围
    customPlot->graph(1)->rescaleAxes(true);    // 也可以只调用 rescaleAxes()

    // 允许用户用鼠标拖动轴的范围，用鼠标滚轮缩放，并通过单击选择图形
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    // customPlot->legend->setVisible(true);
}

void Widget::loadMutiAxisQCustomPlot()
{
    // 句点作为小数点分隔符，逗号作为千位分隔符
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    customPlot->legend->setVisible(true);   // 显示图例

    QFont legendFont = font(); // 以窗体字体开始
    legendFont.setPointSize(9); // 让图例小一点

    customPlot->setFont(legendFont);
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));

    //默认情况下，图例位于主轴矩形的插入布局中。这就是我们如何访问它来更改图例位置:
    customPlot->axisRect()->insetLayout()->setInsetAlignment(1, Qt::AlignBottom | Qt::AlignRight);

    // 设置图表 0:键轴左侧，值轴底部
    // 讲包含类似 Left Maxwell 函数
    customPlot->addGraph(customPlot->yAxis, customPlot->xAxis);
    customPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    // customPlot->graph(0)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // fill with texture of specified image
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    customPlot->graph(0)->setName("左麦克斯韦函数");

    // 图 1 的设置：关键轴底部，值轴左侧（这些是默认轴）
    // 将包含带有误差线的 bottom maxwell 类函数
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    // customPlot->graph(1)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // same fill as we used for graph 0
    customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    customPlot->graph(1)->setName("底部麦克斯韦函数");
    QCPErrorBars *errorBars = new QCPErrorBars(customPlot->xAxis, customPlot->yAxis);
    errorBars->removeFromLegend();
    errorBars->setDataPlottable(customPlot->graph(1));

    // 图2的设置: 关键轴顶部，值轴右侧
    // 将包含低频跳动的高频正弦:
    customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    customPlot->graph(2)->setPen(QPen(Qt::blue));
    customPlot->graph(2)->setName("高频正弦波");

    // 图 3 的设置：与图 2 相同的轴
    // 将包含图 2 的低频跳动包络线
    customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidthF(4);
    customPlot->graph(3)->setPen(blueDotPen);
    customPlot->graph(3)->setName("正弦包络线");

    // 图4的设置: 键轴右侧，值轴顶部
    // 将包含具有一些随机扰动的抛物线分布的数据点
    customPlot->addGraph(customPlot->yAxis2, customPlot->xAxis2);
    customPlot->graph(4)->setPen(QColor(50, 50, 50, 255));
    customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(4)->setName("二次函数周围的一些随机数据");

    // 生成数据
    QVector<double> x0(25), y0(25);
    QVector<double> x1(15), y1(15), y1err(15);
    QVector<double> x2(250), y2(250);
    QVector<double> x3(250), y3(250);
    QVector<double> x4(250), y4(250);
    for (int i = 0; i < 25; ++i) // data for graph 0
    {
        x0[i] = 3 * i / 25.0;
        y0[i] = qExp(-x0[i] * x0[i] * 0.8) * (x0[i] * x0[i] + x0[i]);
    }
    for (int i = 0; i < 15; ++i) // data for graph 1
    {
        x1[i] = 3 * i / 15.0;;
        y1[i] = qExp(-x1[i] * x1[i]) * (x1[i] * x1[i]) * 2.6;
        y1err[i] = y1[i] * 0.25;
    }
    for (int i = 0; i < 250; ++i) // data for graphs 2, 3 and 4
    {
        x2[i] = i / 250.0 * 3 * M_PI;
        x3[i] = x2[i];
        x4[i] = i / 250.0 * 100 - 50;
        y2[i] = qSin(x2[i] * 12) * qCos(x2[i]) * 10;
        y3[i] = qCos(x3[i]) * 10;
        y4[i] = 0.01 * x4[i] * x4[i] + 1.5 * (rand() / (double)RAND_MAX - 0.5) + 1.5 * M_PI;
    }

    // pass data to graphs
    customPlot->graph(0)->setData(x0, y0);
    customPlot->graph(1)->setData(x1, y1);
    errorBars->setData(y1err);
    customPlot->graph(2)->setData(x2, y2);
    customPlot->graph(3)->setData(x3, y3);
    customPlot->graph(4)->setData(x4, y4);

    // 设置顶轴、右侧轴可见
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);

    // 设置适合显示的数据范围
    customPlot->xAxis->setRange(0, 2.7);
    customPlot->yAxis->setRange(0, 2.6);
    customPlot->xAxis2->setRange(0, 3.0 * M_PI);
    customPlot->yAxis2->setRange(-70, 35);

    // 在顶轴显示 π 的标签
    customPlot->xAxis2->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));

    // 添加标题布局元素
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "一张图中显示多张图表", QFont("HarmonyOS Sans SC", 12, QFont::Bold)));
    // customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Way too many graphs in one plot", QFont("sans", 12, QFont::Bold)));

    // 设置各轴文本
    customPlot->xAxis->setLabel("带向外刻度的底部轴");
    customPlot->yAxis->setLabel("左侧轴文本");
    customPlot->xAxis2->setLabel("顶部轴文本");
    customPlot->yAxis2->setLabel("右侧轴文本");

    // 使底轴上的刻度向外：
    customPlot->xAxis->setTickLength(0, 5); // 主Tick
    customPlot->xAxis->setSubTickLength(0, 3);  // 副Tick

    // 使右轴上的刻度向内和向外：
    customPlot->yAxis2->setTickLength(3, 3);
    customPlot->yAxis2->setSubTickLength(1, 1);
}

void Widget::loadDateQCustomPlot()
{
    // 设置字符为英文
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    customPlot->setOpenGl(true);

    // 获取当前时间的秒数，用它作为数据的时间起点
    double now = QDateTime::currentDateTime().toMSecsSinceEpoch();  // 将当前时间转化为时间戳
    srand(8);   // 设置随机数种子，使我们总是得到相同的随机数据

    // 创建多个图表
    for (int gi = 0; gi < 5; ++gi)
    {
        customPlot->addGraph();
        QColor color(20 + 200 / 4.0 * gi, 70 * (1.6 - gi / 4.0), 150, 150);
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(QPen(color.lighter(200)));
        customPlot->graph()->setBrush(QBrush(color));

        // 生成随机数
        QVector<QCPGraphData> timeData(250);
        for (int i = 0; i < 250; ++i)
        {
            timeData[i].key = now + 24 * 3600 * i;
            if (i == 0)
            {
                timeData[i].value = (i / 50.0 + 1) * (rand() / (double)RAND_MAX - 0.5);
            }
            else
            {
                timeData[i].value = qFabs(timeData[i - 1].value) * (1 + 0.02 / 4.0 * (4 - gi)) + (i / 50.0 + 1) * (rand() / (double)RAND_MAX - 0.5);
            }
        }
        customPlot->graph()->data()->set(timeData);
    }

    // 配置底部轴显示日期而不是数字
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    customPlot->xAxis->setTicker(dateTicker);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(10, "a bit\nlow");
    textTicker->addTick(50, "quite\nhigh");
    customPlot->yAxis->setTicker(textTicker);

    // 为底部和左侧轴刻度标签设置更紧凑的字体大小
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    customPlot->xAxis->setLabel("日期");
    customPlot->yAxis->setLabel("随机值");

    // 使上轴和右轴可见，但没有刻度线和标签：
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);

    // 设置轴线范围
    customPlot->xAxis->setRange(now, now + 24 * 3600 * 249);
    customPlot->yAxis->setRange(0, 60);

    // 设置图例
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));

    customPlot->yAxis->rescale(true);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void Widget::loadBarChatQCustomPlot()
{
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    customPlot->setBackground(QBrush(gradient));

    // 创建空的柱状图对象
    QCPBars* regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars* nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars* fossil =  new QCPBars(customPlot->xAxis, customPlot->yAxis);
    regen->setAntialiased(false);   // 提供更清晰的像素对齐的条边框
    nuclear->setAntialiased(false);
    fossil->setAntialiased(false);
    regen->setStackingGap(1);
    nuclear->setStackingGap(1);
    fossil->setStackingGap(1);

    // 设置名称和颜色
    fossil->setName("Fossil fuels - 化石燃料");
    fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    fossil->setBrush(QColor(111, 9, 176));

    nuclear->setName("Nuclear - 核能");
    nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
    nuclear->setBrush(QColor(250, 170, 20));

    regen->setName("Regenerative - 可再生能源");
    regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    regen->setBrush(QColor(0, 168, 140));

    // 彼此之间堆叠
    nuclear->moveAbove(fossil);
    regen->moveAbove(nuclear);

    // 配置x轴标签
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    labels << "USA 美国" << "Japan 日本" << "Germany 德国" << "France 法国" << "UK 英国" << "Italy 意大利" << "Canada 加拿大";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);

    customPlot->xAxis->setTicker(textTicker);
    customPlot->xAxis->setTickLabelRotation(60);    // 旋转角度
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setRange(0, 8);
    customPlot->xAxis->setBasePen(QPen(Qt::white));
    customPlot->xAxis->setTickPen(QPen(Qt::white));
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);

    // 配置 y 轴
    customPlot->yAxis->setRange(0, 12.1);
    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel("2007 年度人均功耗");
    customPlot->yAxis->setBasePen(QPen(Qt::white));
    customPlot->yAxis->setTickPen(QPen(Qt::white));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // 添加数据
    QVector<double> fossilData, nuclearData, regenData;
    fossilData  << 0.86 * 10.5 << 0.83 * 5.5 << 0.84 * 5.5 << 0.52 * 5.8 << 0.89 * 5.2 << 0.90 * 4.2 << 0.67 * 11.2;
    nuclearData << 0.08 * 10.5 << 0.12 * 5.5 << 0.12 * 5.5 << 0.40 * 5.8 << 0.09 * 5.2 << 0.00 * 4.2 << 0.07 * 11.2;
    regenData   << 0.06 * 10.5 << 0.05 * 5.5 << 0.04 * 5.5 << 0.06 * 5.8 << 0.02 * 5.2 << 0.07 * 4.2 << 0.25 * 11.2;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    regen->setData(ticks, regenData);

    // 配置图例
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);

    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
