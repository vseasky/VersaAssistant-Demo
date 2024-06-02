#include "VersaQCustomPlot.h"
#include <QAction>
#include <QDebug>


VersaQCustomPlot::VersaQCustomPlot(QWidget *parent, bool enableGPU) : QCustomPlot(parent)
{
    // 添加32种优化的颜色值到graphColor向量
    graphColor.append(QColor("#1f77b4"));  // 蓝色
    graphColor.append(QColor("#ff7f0e"));  // 橙色
    graphColor.append(QColor("#2ca02c"));  // 绿色
    graphColor.append(QColor("#d62728"));  // 红色
    graphColor.append(QColor("#9467bd"));  // 紫色
    graphColor.append(QColor("#8c564b"));  // 棕色
    graphColor.append(QColor("#e377c2"));  // 粉红色
    graphColor.append(QColor("#7f7f7f"));  // 灰色
    graphColor.append(QColor("#bcbd22"));  // 黄绿色
    graphColor.append(QColor("#17becf"));  // 青色
    graphColor.append(QColor("#9edae5"));  // 淡青色
    graphColor.append(QColor("#98df8a"));  // 淡绿色
    graphColor.append(QColor("#ff9896"));  // 淡红色
    graphColor.append(QColor("#c5b0d5"));  // 淡紫色
    graphColor.append(QColor("#c49c94"));  // 淡棕色
    graphColor.append(QColor("#f7b6d2"));  // 淡粉红色
    graphColor.append(QColor("#dbdb8d"));  // 淡黄绿色
    graphColor.append(QColor("#aec7e8"));  // 淡蓝色
    graphColor.append(QColor("#ffbb78"));  // 淡橙色
    graphColor.append(QColor("#2ca089"));  // 海绿色
    graphColor.append(QColor("#ff4500"));  // 橙红色
    graphColor.append(QColor("#32cd32"));  // 酸橙绿色
    graphColor.append(QColor("#0000ff"));  // 纯蓝色
    graphColor.append(QColor("#daa520"));  // 金色
    graphColor.append(QColor("#808080"));  // 深灰色
    graphColor.append(QColor("#8a2be2"));  // 蓝紫色
    graphColor.append(QColor("#fa8072"));  // 浅肉色
    graphColor.append(QColor("#4682b4"));  // 钢蓝色
    graphColor.append(QColor("#708090"));  // 石板灰
    graphColor.append(QColor("#6a5acd"));  // 石板蓝
    graphColor.append(QColor("#228b22"));  // 森林绿
    graphColor.append(QColor("#d2691e"));  // 巧克力色

    setNoAntialiasingOnDrag(false);
    setOpenGl(enableGPU);
    // setPlottingHint(QCP::phFastPolylines, true);
    setAntialiasedElements(QCP::aeAll);

    this->setInteractions(QCP::iRangeDrag |           // 允许用户拖动图表中的可视区域，以改变 x 和 y 轴的范围。
                          QCP::iRangeZoom |           // 允许用户使用鼠标滚轮或拖动来缩放图表的可视区域，以改变 x 和 y 轴的范围。
                          QCP::iSelectAxes |       // 允许用户选择坐标轴。
                          QCP::iSelectLegend |       // 允许用户选择图例项（如果图表中有图例的话）。
                          QCP::iSelectPlottables); // 允许用户选择图表中的数据（如曲线、柱状图等）。

    // 设置QCustomPlot控件的坐标轴框，让坐标轴框围满整个绘图区域
    this->axisRect()->setupFullAxesBox();
    // 将QCustomPlot控件的第二个x轴设置为可见
    this->xAxis2->setVisible(true);
    // 将QCustomPlot控件的第二个x轴的刻度标签设置为不可见，也就是不显示刻度标签
    this->xAxis2->setTickLabels(false);
    // 将QCustomPlot控件的第二个y轴设置为可见
    this->yAxis2->setVisible(true);
    // 将QCustomPlot控件的第二个y轴的刻度标签设置为不可见，也就是不显示刻度标签
    this->yAxis2->setTickLabels(false);


    // 设置x轴的小数位数为2
    xAxis->setNumberFormat("f"); // 使用浮点数格式
    xAxis->setNumberPrecision(3); // 设置小数精度为2

    // 设置y轴的小数位数为3
    yAxis->setNumberFormat("f"); // 使用浮点数格式
    yAxis->setNumberPrecision(3); // 设置小数精度为3

    // 使上下两个X轴的范围总是相等，使左右两个Y轴的范围总是相等
    connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this->xAxis2, SLOT(setRange(QCPRange)));
    connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)),
            this->yAxis2, SLOT(setRange(QCPRange)));
    // 选中图例显示
    connect(this, &QCustomPlot::selectionChangedByUser,
            this, &VersaQCustomPlot::selectionChanged);

    // 双击修改坐标轴名称
    connect(this, &QCustomPlot::axisDoubleClick,
            this, &VersaQCustomPlot::axisLabelDoubleClick);

    // 双击修改图例名称
    connect(this, &QCustomPlot::legendDoubleClick,
            this, &VersaQCustomPlot::legendDoubleClick);

    // 单击图形时在状态栏中显示消息的连接槽：
    // connect(this, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
    // 显示图例名称
    this->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->legend->setFont(legendFont);
    this->legend->setSelectedFont(legendFont);
    this->legend->setSelectableParts(QCPLegend::spItems);

    // 追踪光标
    plotTracer = new VersaQCustomPlotTracer(this);

    this->xAxis->setRange(0, 5);
    this->yAxis->setRange(0, 20);

    this->xAxis->ticker()->setTickCount(8);
    this->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);
    this->yAxis->ticker()->setTickCount(8);
    this->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);

    connect(&refreshTimer, &QTimer::timeout,
            this, &VersaQCustomPlot::GraphRefreshSlot);

    clearGraphs();
    refreshTimer.setTimerType(Qt::PreciseTimer);
    stopRefreshTimer();
    startRefreshTimer();

    // 鼠标右键逻辑
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequest(QPoint)));
    // 获取当前时间
    startTime = QDateTime::currentDateTime();
}

VersaQCustomPlot::~VersaQCustomPlot()
{
    refresh(eRefreshNone);
    isRefreshTimerEnable = false;
    isRefreshTimerEnable = false; // 是否启动定时器
    isRefreshGraphs = false;          // 是否有波形
    keyPress_X = false;
    keyPress_V = false;
    stopRefreshTimer();
    removeAllGraphs();
    if (!plotTracer.isNull())
    {
        delete plotTracer;
        plotTracer = nullptr;
    }
}

/*!
 * 启动波形刷新定时器
 */
void VersaQCustomPlot::startRefreshTimer(void)
{
    isRefreshTimerEnable = true;
    if (!refreshTimer.isActive())
    {
        refreshTimer.start(GraphShowTimerSet);
    }
}

/*!关闭波形刷新定时器
 */
void VersaQCustomPlot::stopRefreshTimer(void)
{
    isRefreshTimerEnable = false;
    if (refreshTimer.isActive())
    {
        refreshTimer.stop();
    }
}

/*!
 * 刷新波形
 */
void VersaQCustomPlot::refreshGraphs()
{
    endTime = QDateTime::currentDateTime();
    // 计算时间差
    qint64 elapsedTime = startTime.msecsTo(endTime);
    // 打印时间差（以毫秒为单位）
    if (elapsedTime >= 10)
    {
        mutex.lock();
        if (isRefreshreplot == false)
        {
            if (isGraphsyAxisAuto == true)
            {
                this->yAxis->rescale(true);
            }
            if (isGraphsxAxisAuto == true)
            {
                // 实时显示将其对齐到最大值处
                GraphsxAxisLength = this->xAxis->range().upper - this->xAxis->range().lower;
                this->xAxis->setRange(GraphsxAxisEnd, GraphsxAxisLength, Qt::AlignRight);
            }
            refreshPlotTracer();
        } else
        {
            switch (refreshAction)
            {
                case eRefreshNone:
                {
                    break;
                }
                case eRefreshData:
                {
                    this->replot(QCustomPlot::rpQueuedReplot);
                    refreshAction = eRefreshNone;
                    break;
                }
                case eRefreshMouseWhell:
                {
                    refreshAction = eRefreshNone;
                    break;
                }
                case eRefreshMouseMove:
                {
                    this->replot(QCustomPlot::rpQueuedReplot);
                    refreshAction = eRefreshNone;
                    break;
                }
                case eRefreshMousePress:
                {
                    refreshAction = eRefreshNone;
                    break;
                }
                case eRefreshButtonPress:
                {
                    refreshAction = eRefreshNone;
                    break;
                }
                case eRefreshSaveWaveData:
                {
                    break;
                }
                case eRefreshReadWaveData:
                {
                    break;
                }
            }
        }
        mutex.unlock();
        isRefreshreplot = !isRefreshreplot;
        startTime = QDateTime::currentDateTime();
    }
}

bool VersaQCustomPlot::refresh(RefreshActionEnum action)
{
    if (refreshAction == eRefreshNone)
    {
        refreshAction = action;
        return true;
    } else
    {
        return false;
    }
}

/*!
 * 选择已更改
 */
void VersaQCustomPlot::selectionChanged(void)
{
    // 同步选择顶部和底部轴，并将轴和刻度标签作为一个可选对象进行处理：
    if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
        this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        this->xAxis2->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
        this->xAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
    }
    // 同步选择左轴和右轴，并将轴和刻度标签作为一个可选对象处理：
    if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
        this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        this->yAxis2->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
        this->yAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
    }

    // 将图形的选择与相应图例项的选择同步：
    for (int i = 0; i < this->graphCount(); ++i)
    {
        QCPGraph *graph = this->graph(i);
        QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
    }
}

/*!
 * 轴标签双击
 * @param axis
 * @param part
 */
void VersaQCustomPlot::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
    // 双击轴标签，设置轴标签
    if (part == QCPAxis::spAxisLabel)
    {
        bool ok;
        QString newLabel = QInputDialog::getText(this,
                                                 tr("更改坐标名称"),
                                                 tr("新的坐标名称"), QLineEdit::Normal, axis->label(), &ok);
        if (ok)
        {
            axis->setLabel(newLabel);
            refresh(eRefreshButtonPress);
        }
    }
}

/*!
 * 图例双击
 * @param legend
 * @param item
 */
void VersaQCustomPlot::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
    // 通过双击图形的图例项来重命名图形
    Q_UNUSED(legend)

    if (item)
    {
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem *>(item);
        bool ok;
        QString newName =
                QInputDialog::getText(this,
                                      tr("更改曲线名称"),
                                      tr("新的曲线名称"),
                                      QLineEdit::Normal, plItem->plottable()->name(),
                                      &ok, Qt::WindowCloseButtonHint);
        if (ok)
        {
            plItem->plottable()->setName(newName);
            refresh(eRefreshButtonPress);
        }
    }
}

/*!
 * 选定的图形颜色集
 */
void VersaQCustomPlot::selectedGraphsColorSet(void)
{
    QColor color = QColorDialog::getColor(Qt::red, this,
                                          tr("设置曲线颜色"),
                                          QColorDialog::ShowAlphaChannel);
    QPen graphPen;
    graphPen.setColor(color);
    graphPen.setWidthF(4);
    this->selectedGraphs().first()->setPen(graphPen);
}

/*!
 * 移动图例
 */
void VersaQCustomPlot::moveLegend(void)
{
    // 确保此槽确实由上下文菜单操作调用，以便它携带我们需要的数据
    if (QAction *contextAction = qobject_cast<QAction *>(
            sender()))
    {
        bool ok;
        int dataInt = contextAction->data().toInt(&ok);
        if (ok)
        {
            this->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)
                    dataInt);
        }
    }
}

/*!
 * 添加波形图，随机颜色
 */
void VersaQCustomPlot::addRandomGraph(QColor color)
{
    this->addGraph();
    // 设置名称
    // if (this->pName[this->graphCount() - 1] != nullptr)
    // {
    // 	// this->graph()->setName(this->pName[this->graphCount() - 1]);
    // }
    // 设置数据线形状为直线
    this->graph()->setLineStyle((QCPGraph::LineStyle::lsLine));
    // 设置数据点为圆形，填充为画笔的颜色
    this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc,
                                                   1)); // 数据点
    this->graph()->data()->clear();
    QPen graphPen;

    graphPen.setColor(color);
    graphPen.setWidthF(3);
    // 设置颜色
    this->graph()->setPen(graphPen);
    refresh(eRefreshData);
}

/*!
 * 删除所选图形
 */
void VersaQCustomPlot::removeSelectedGraph(void)
{
    if (this->selectedGraphs().size() > 0)
    {
        this->removeGraph(this->selectedGraphs().first());
        refresh(eRefreshData);
    }
}

/*!
 * 删除所有图形
 */
void VersaQCustomPlot::removeAllGraphs(void)
{
    refresh(eRefreshNone);;
    isRefreshGraphs = false;
    this->clearGraphs();
    refreshGraphsCount = 0;
}

/*!
 * 隐藏所选图形
 */
void VersaQCustomPlot::hideSelectedGraph()
{
    if (this->selectedGraphs().size() > 0)
    {
        // 获取图像编号
        int index = 0;
        for (; index < this->graphCount(); index++)
        {
            if (this->graph(index)->name() == this->selectedGraphs().first()->name())
            {
                break;
            }
        }
        // 可见性控制
        if (this->selectedGraphs().first()->visible())
        {
            this->selectedGraphs().first()->setVisible(false);
            this->legend->item(index)->setTextColor(Qt::gray);
        } else
        {
            this->selectedGraphs().first()->setVisible(true);
            this->legend->item(index)->setTextColor(Qt::black);
        }
        refresh(eRefreshData);
    }
}

/*!
 * 隐藏所有图形
 */
void VersaQCustomPlot::hideAllGraph()
{
    int index = 0;
    for (index = 0; index < this->graphCount(); index++)
    {
        this->graph(index)->setVisible(false);
        this->legend->item(index)->setTextColor(Qt::gray);
    }
    refresh(eRefreshData);
}

/*!
 * 显示所有图表
 */
void VersaQCustomPlot::showAllGraph()
{
    int index = 0;
    for (; index < this->graphCount(); index++)
    {
        this->graph(index)->setVisible(true);
        this->legend->item(index)->setTextColor(Qt::black);
    }
    refresh(eRefreshData);
}

/*!
 * 是否所有图形都隐藏
 * @return
 */
bool VersaQCustomPlot::isAllGraphHide(void)
{
    int index = 0;
    for (; index < this->graphCount(); index++)
    {
        if (this->graph(index)->visible())
        {
            return false;
        }
    }
    return true;
}

/*!
 * 是否有图形隐藏
 * @return
 */
bool VersaQCustomPlot::ishaveGraphHide(void)
{
    int index = 0;
    for (; index < this->graphCount(); index++)
    {
        if (!this->graph(index)->visible())
        {
            return true;
        }
    }
    return false;
}

/*!
 * 上下文菜单请求
 * @param pos
 */
void VersaQCustomPlot::contextMenuRequest(QPoint pos)
{
    if (isRefreshTimerEnable == false)
    {
        return;
    }
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    if (this->legend->selectTest(pos, false) >= 0) // 请求图例的上下文菜单
    {
        menu->addAction(tr("移动到左上角"), this,
                        SLOT(moveLegend()))
            ->setData((int) (Qt::AlignTop | Qt::AlignLeft));

        menu->addAction(tr("移动到右上角"), this,
                        SLOT(moveLegend()))
            ->setData((int) (Qt::AlignTop | Qt::AlignRight));

        menu->addAction(tr("移动到右下角"), this,
                        SLOT(moveLegend()))
            ->setData((int) (Qt::AlignBottom | Qt::AlignRight));

        menu->addAction(tr("移动到左下角"), this,
                        SLOT(moveLegend()))
            ->setData((int) (Qt::AlignBottom | Qt::AlignLeft));

        menu->addSeparator();
    }

    if (isRefreshGraphs == false)
    {
        QAction *action = menu->addAction(tr("自动添加多条曲线"), this,
                                          SLOT(addGraphs()));
    } else
    {
        menu->addAction(tr("移除所有曲线"), this,
                        SLOT(removeAllGraphs()));
        menu->addSeparator();
        if (isGraphsyAxisAuto == false)
        {
            menu->addAction(tr("Y-自动"), this, [=] { isGraphsyAxisAuto = true; });
        } else
        {
            menu->addAction(tr("Y-关闭自动"), this, [=] { isGraphsyAxisAuto = false; });
        }

        if (isGraphsxAxisAuto == false)
        {
            menu->addAction(tr("X-自动"), this, [=] { isGraphsxAxisAuto = true; });
        } else
        {
            menu->addAction(tr("X-关闭自动"), this, [=] { isGraphsxAxisAuto = false; });
        }

        // 选择了曲线
        if (this->selectedGraphs().size() > 0)
        {
            menu->addSeparator();
            menu->addAction(tr("设置曲线颜色"), this,
                            SLOT(selectedGraphsColorSet()));
            // 所选曲线是否可见
            if (this->selectedGraphs().first()->visible())
            {
                menu->addAction(tr("隐藏所选曲线"), this,
                                SLOT(hideSelectedGraph()));
            } else
            {
                menu->addAction(tr("显示所选曲线"), this,
                                SLOT(hideSelectedGraph()));
            }
        }
        menu->addSeparator();
        // 不是所有曲线被隐藏
        if (!isAllGraphHide())
        {
            menu->addAction(tr("隐藏所有曲线"), this, SLOT(hideAllGraph()));
        }
        // 有曲线未被显示
        if (ishaveGraphHide())
        {
            menu->addAction(tr("显示所有曲线"), this, SLOT(showAllGraph()));
        }
    }

    menu->addSeparator();

    menu->addAction(tr("保存波形数据"),
                    this, &VersaQCustomPlot::saveWaveformData);
    menu->addAction(tr("保存波形图片"),
                    this, &VersaQCustomPlot::saveWaveformImage);
    menu->addSeparator();
    menu->addAction(tr("导入波形数据"),
                    this, &VersaQCustomPlot::readWaveformData);

    menu->popup(this->mapToGlobal(pos));
}

void VersaQCustomPlot::addGraphs(void)
{
    if (refreshGraphsCount > 0)
    {
        this->clearGraphs();
        lastKeys.resize(refreshGraphsCount);
        for (int igraph = 0; igraph < refreshGraphsCount; igraph++)
        {
            addRandomGraph(graphColor[igraph]);
            lastKeys[igraph] = 0;
        }
        isRefreshGraphs = true;
    }
}

/*!
 * 更新显示数据
 * @param keys
 * @param values
 */
void VersaQCustomPlot::addGraphsData(const QVector<double> &keys, const QVector<double> &values)
{
    // 更新数据长度
    refreshGraphsCount = qMin(keys.size(), values.size());
    if (isRefreshGraphs == false)
    {
        return;
    }
    if (refreshGraphsCount > 0)
    {
        // 保存和导入数据时不刷新
        if ((refreshAction != eRefreshSaveWaveData))
        {

            for (int iGraph = 0; (iGraph < qMin(this->graphCount(), refreshGraphsCount)); iGraph++)
            {
                // 更新结尾
                if (keys[iGraph] > lastKeys[iGraph])
                {
                    if (keys[iGraph] > GraphsxAxisEnd)
                    {
                        GraphsxAxisEnd = keys[iGraph];
                    }
                    this->graph(iGraph)->addData(keys[iGraph], values[iGraph]);
                }
            }
            // 从文件读取，则不使用数据刷新
            if (refreshAction != eRefreshReadWaveData)
            {
                refresh(eRefreshData);
            }
        }
        lastKeys = keys;
    }
}

// 更新显示波形
void VersaQCustomPlot::GraphRefreshSlot(void)
{
    if (isRefreshTimerEnable == false)
    {
        return;
    }
    refreshGraphs();
    // if (RefreshGraphs == true)
    // {
    // 	// 删除超过容量的数据
    // 	for (uint32_t iGraph = 0; iGraph < this->graphCount(); iGraph++)
    // 	{
    // 		int length = this->graph(iGraph)->data()->size();
    // 		if (length > GraphsDataMaxLength)
    // 		{
    // 			double removeBefore = this->graph(iGraph)->data()->at(length - GraphsDataMaxLength)->mainKey();
    // 			this->graph(iGraph)->data()->removeBefore(removeBefore);
    // 		}
    // 	}
    // }
}

/*!
 * 读取波形数据
 */
void VersaQCustomPlot::readWaveformData(void)
{
    // 无波形数据才可以导入
    if (isRefreshGraphs == false)
    {
        if (refresh(eRefreshReadWaveData))
        {
            QString dirpath = QFileDialog::getOpenFileName(this, QStringLiteral("导入波形数据"),
                                                           qApp->applicationDirPath() + "/plot.csv",
                                                           QString(tr("*.csv")));
            if (dirpath != NULL)
            {
                QFile file(dirpath);
                if (!file.open(QIODevice::ReadOnly))
                {
                    QMessageBox::critical(NULL,
                                          tr("提示"),
                                          tr("无法打开该文件！"));
                } else
                {
                    QStringList list;
                    list.clear();
                    QTextStream csvStream(&file);
                    // 遍历行
                    this->clearGraphs();
                    // 读取一行文本
                    QString fileHeadLine = csvStream.readLine();
                    list = fileHeadLine.split(",",
                                              QString::SkipEmptyParts);
                    refreshGraphsCount = list.size() - 1;
                    // 添加曲线
                    addGraphs();
                    for (int iGraphsData = 0; !csvStream.atEnd(); iGraphsData++)
                    {
                        QString fileDataLine = csvStream.readLine();
                        QStringList listData = fileDataLine.split(",",
                                                                  QString::KeepEmptyParts);
                        // 根据","开分隔开每行的列
                        QVector<double> key;
                        QVector<double> value;
                        key.resize(listData.size() - 1);
                        value.resize(listData.size() - 1);
                        for (int iGraphs = 1; iGraphs < listData.size(); iGraphs++)
                        {
                            key[iGraphs - 1] = listData[0].toFloat();
                            value[iGraphs - 1] = listData[iGraphs].toFloat();
                        }
                        addGraphsData(key, value);
                    }
                    file.close();
                    // 加载完成
                    refreshAction = eRefreshData;
                    return;
                }
                refreshAction = eRefreshNone;
            } else
            {
                refreshAction = eRefreshNone;
            }
        } else
        {
            QMessageBox::critical(NULL, tr("提示"),
                                  tr("请停止数据刷新操作！"));
            refreshAction = eRefreshNone;
        }
    } else
    {
        QMessageBox::critical(NULL, tr("提示"),
                              tr("已经存在波形数据，无法导入，请清空波形后导入！"));
        refreshAction = eRefreshNone;
    }
    refreshAction = eRefreshNone;
}

/*!
 * 存储波形数据
 */

void VersaQCustomPlot::saveWaveformData(void)
{
    // 有无波形数据
    if (isRefreshGraphs == true)
    {
        // 如果无动作
        if (refresh(eRefreshSaveWaveData))
        {

            QString dirpath = QFileDialog::getSaveFileName(this, QStringLiteral("保存波形数据"),
                                                           qApp->applicationDirPath() + "/plot.csv",
                                                           QString(tr("*.csv")));
            if (dirpath != NULL)
            {
                QFile file(dirpath);
                // 方式：Append为追加，WriteOnly，ReadOnly
                if (!file.open(QIODevice::WriteOnly))
                {
                    QMessageBox::critical(NULL, tr("提示"),
                                          tr("无法创建文件！"));
                } else
                {
                    QTextStream stream(&file);
                    {
                        // 添加标题栏
                        {
                            stream << (tr("time(s),"));
                            int iGraphIndex = 0;
                            for (iGraphIndex = 0; iGraphIndex < this->graphCount() - 1; iGraphIndex++)
                            {
                                stream << (this->graph(iGraphIndex)->name() + ",");
                            }
                            stream << (this->graph(iGraphIndex)->name() + "\n");
                        }
                    }
                    // 遍历数据
                    for (int iGraphData = 0; iGraphData < this->graph(0)->dataCount(); iGraphData++)
                    {
                        // 添加时间轴
                        stream << (QString::number((this->graph(0)->data()->at(iGraphData)->key),
                                                   'f', 6) +
                                   ",");
                        // 遍历曲线
                        int iGraphIndex = 0;
                        for (iGraphIndex = 0; iGraphIndex < this->graphCount() - 1; iGraphIndex++)
                        {
                            // 添加数据
                            stream << (QString::number((this->graph(iGraphIndex)->data()->at(iGraphData)->value),
                                                       'f', 6) +
                                       ",");
                        }
                        stream << (QString::number((this->graph(iGraphIndex)->data()->at(iGraphData)->value),
                                                   'f', 6) +
                                   "\n");
                    }
                    refreshAction = eRefreshNone;
                    file.close();
                    return;
                }
                refreshAction = eRefreshNone;
            } else
            {
                refreshAction = eRefreshNone;
            }
        } else
        {
            QMessageBox::critical(NULL, tr("提示"),
                                  tr("请停止数据刷新操作！"));
            refreshAction = eRefreshNone;
        }
    } else
    {
        QMessageBox::critical(NULL, tr("提示"),
                              tr("无任何波形可供保存！"));
        refreshAction = eRefreshNone;
    }
    refreshAction = eRefreshNone;
}

void VersaQCustomPlot::saveWaveformImage(void)
{
    if (isRefreshGraphs == true)
    {
        if (refresh(eRefreshSaveWaveData))
        {
            QString dirpath = QFileDialog::getSaveFileName(this, QStringLiteral("保存为图片"),
                                                           qApp->applicationDirPath() + "/plot.png",
                                                           QString(tr("*.png")));
            if (dirpath != NULL)
            {
                savePng(dirpath);
                refreshAction = eRefreshNone;
                return;
            } else
            {
                refreshAction = eRefreshNone;
            }
        } else
        {
            QMessageBox::critical(NULL, tr("提示"),
                                  tr("请停止数据刷新操作！"));
            refreshAction = eRefreshNone;
        }
    } else
    {
        QMessageBox::critical(NULL, tr("提示"),
                              tr("无任何波形可供保存！"));
        refreshAction = eRefreshNone;
    }
    refreshAction = eRefreshNone;
}

/*!
 * 鼠标键盘事件
 * @param event
 */
void VersaQCustomPlot::wheelEvent(QWheelEvent *event)
{
    if ((refreshAction == eRefreshSaveWaveData) || (refreshAction == eRefreshReadWaveData))
    {
        event->ignore();
        return;
    }
    // 如果选择了轴，只允许缩放该轴的方向
    if (refresh(eRefreshMouseWhell))
    {
        if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) | keyPress_X)
        {
            this->axisRect()->setRangeZoom(this->xAxis->orientation());
        } else if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) | keyPress_V)
        {
            this->axisRect()->setRangeZoom(this->yAxis->orientation());
        } else
        {
            this->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        }
        isRefreshreplot = false;
    }
    QCustomPlot::wheelEvent(event);
}

void VersaQCustomPlot::mousePressEvent(QMouseEvent *event)
{
    if ((refreshAction == eRefreshSaveWaveData) || (refreshAction == eRefreshReadWaveData))
    {
        event->ignore();
        return;
    }
    if (refresh(eRefreshMousePress))
    {
        if (event->button() == Qt::LeftButton)
        {
            // 处理鼠标左键按下事件
            // 如果选择了一个轴，只允许拖动该轴的方向
            // 如果没有选择轴，两个方向都可以拖动
            if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) | keyPress_X)
            {
                this->axisRect()->setRangeDrag(this->xAxis->orientation());
            } else if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) | keyPress_V)
            {
                this->axisRect()->setRangeDrag(this->yAxis->orientation());
            } else
            {
                this->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
            }
        }
        isRefreshreplot = false;
    }
    QCustomPlot::mousePressEvent(event);
}

/*光标追踪数据点*/
void VersaQCustomPlot::mouseMoveEvent(QMouseEvent *event)
{
    if ((refreshAction == eRefreshSaveWaveData) || (refreshAction == eRefreshReadWaveData))
    {
        event->ignore();
        return;
    }
    QSize viewportSize = viewport().size();
    pos_x = qMin(event->pos().x(), viewportSize.width());
    pos_y = qMin(event->pos().y(), viewportSize.height());
    if (refresh(eRefreshMouseMove))
    {
        isRefreshreplot = false;
    }
    QCustomPlot::mouseMoveEvent(event);
}

/*!
 * 刷新光标
 */
void VersaQCustomPlot::refreshPlotTracer(void)
{
    if ((plotTracer == nullptr))
    {
        return;
    }

    double x = 0;
    double y = 0;
    // 如果没有图标选中
    if (this->selectedGraphs().size() <= 0)
    {
        // 像素坐标转换为 QCustomPlot 中 x 轴对应的坐标值
        x = this->xAxis->pixelToCoord(pos_x);
        y = this->yAxis->pixelToCoord(pos_y);
        plotTracer->updatePosition(VersaQCustomPlotTracer::showTracerPoint,
                                   x,
                                   y);
    } else
    {
        static double k = 0;
        static VersaQCustomPlotTracer::TracerShowType type =
                VersaQCustomPlotTracer::TracerShowType(
                        VersaQCustomPlotTracer::showTracerAll |
                        VersaQCustomPlotTracer::showTracerTop);
        QSharedPointer<QCPGraphDataContainer> tmpContainer;
        tmpContainer = this->selectedGraphs().first()->data();
        {
            // 像素坐标转换为 QCustomPlot 中 x 轴对应的坐标值
            x = this->xAxis->pixelToCoord(pos_x);
            y = this->yAxis->pixelToCoord(pos_y);
            // 使用二分查找，需确保元素是有序的，这里X轴表示时间
            int low = 0, high = tmpContainer->size();
            while (high > low)
            {
                int middle = (low + high) / 2;

                if (x < tmpContainer->constBegin()->mainKey() ||
                    x > (tmpContainer->constEnd() - 1)->mainKey())
                {
                    break;
                }

                if (x == (tmpContainer->constBegin() + middle)->mainKey())
                {
                    y = (tmpContainer->constBegin() + middle)->mainValue();
                    break;
                }

                if (x > (tmpContainer->constBegin() + middle)->mainKey())
                {
                    low = middle;
                } else if (x < (tmpContainer->constBegin() + middle)->mainKey())
                {
                    high = middle;
                }

                // 如果光标位于两个数据点中间
                if (high - low <= 1)
                {
                    // 差值计算所在位置数据
                    double x1 = (tmpContainer->constBegin() + low)->mainKey();
                    double y1 = (tmpContainer->constBegin() + low)->mainValue();
                    double x2 = (tmpContainer->constBegin() + high)->mainKey();
                    double y2 = (tmpContainer->constBegin() + high)->mainValue();
                    k = (y2 - y1) / (x2 - x1);
                    y = y1 + k * (x - x1);
                    break;
                }
            }
        }
        if (k > 0)
        {
            type = VersaQCustomPlotTracer::TracerShowType(
                    VersaQCustomPlotTracer::showTracerAll |
                    VersaQCustomPlotTracer::showTracerBottom);
        } else if (k < 0)
        {
            type = VersaQCustomPlotTracer::TracerShowType(
                    VersaQCustomPlotTracer::showTracerAll |
                    VersaQCustomPlotTracer::showTracerTop);
        }
        QString text = "[X:" + QString::number(x, 'f', 3) + "]s [Y:" + QString::number(y, 'f', 3) + "]";
        plotTracer->setText(text);
        plotTracer->updatePosition(type, x, y);
    }
}

void VersaQCustomPlot::keyPressEvent(QKeyEvent *event)
{
    int keyPress = event->key();
    if (keyPress == Qt::Key_X)
    {
        keyPress_X = true;
    }
    if (keyPress == Qt::Key_V)
    {
        keyPress_V = true;
    }
    QCustomPlot::keyPressEvent(event);
}

void VersaQCustomPlot::keyReleaseEvent(QKeyEvent *event)
{
    int keyRelease = event->key();
    if (keyRelease == Qt::Key_X)
    {
        keyPress_X = false;
    }
    if (keyRelease == Qt::Key_V)
    {
        keyPress_V = false;
    }
    QCustomPlot::keyPressEvent(event);
}
