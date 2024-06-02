#ifndef VERSA_QCUSTOMPLOT_H
#define VERSA_QCUSTOMPLOT_H

#include <QPointer>
#include <qcustomplot.h>
#include <VersaQCustomPlotTracer.h>
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QThread>
#include <QVariant>

/*
 *****************************
 ************波形绘制************
 ******************************
 */

class VersaQCustomPlot : public QCustomPlot
{
Q_OBJECT
public:
    enum RefreshActionEnum
    {
        eRefreshNone,
        eRefreshData,
        eRefreshMouseWhell,
        eRefreshMouseMove,
        eRefreshMousePress,
        eRefreshButtonPress,
        eRefreshSaveWaveData,
        eRefreshReadWaveData
    };

    explicit VersaQCustomPlot(QWidget *parent = nullptr, bool enableGPU = true);

    ~VersaQCustomPlot();

    QPointer<VersaQCustomPlotTracer> plotTracer = nullptr; //坐标跟随鼠标.使用时创建
    int GraphShowTimerSet = 20;//波形刷新

    void startRefreshTimer(void);

    void stopRefreshTimer(void);

    void refreshGraphs();

    bool refresh(RefreshActionEnum action);

public slots:

    void selectionChanged(void);// 选择已更改
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);

    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);

    void selectedGraphsColorSet(void);

    void moveLegend(void);

    void addRandomGraph(QColor color);

    void removeSelectedGraph(void);

    void removeAllGraphs(void);

    void hideSelectedGraph(void);

    void hideAllGraph(void);

    void showAllGraph(void);

    bool isAllGraphHide(void);

    bool ishaveGraphHide(void);

    void contextMenuRequest(QPoint pos);

    void addGraphs(void);

    void addGraphsData(const QVector<double> &keys, const QVector<double> &values);

    void GraphRefreshSlot(void);

    void readWaveformData(void);

    void saveWaveformData(void);

    void saveWaveformImage(void);

protected:

    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

//    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

signals:

private:
    QDateTime startTime;
    QDateTime endTime;
    QMutex mutex;

    void refreshPlotTracer(void);

    QVector<QColor> graphColor;
    QVector<double> lastKeys;
    double pos_x = 0;
    double pos_y = 0;
    QTimer refreshTimer;

    double GraphsxAxisLength = 0;
    double GraphsxAxisEnd = 0;

    int GraphsDataMaxLength = 10240;

    int refreshGraphsCount = 0;
    bool isGraphsyAxisAuto = false;
    bool isGraphsxAxisAuto = false;
    bool isRefreshTimerEnable = false;//是否启动定时器
    bool isRefreshGraphs = false; //是否已经有波形数据

    bool isRefreshreplot = false;
    RefreshActionEnum refreshAction = eRefreshNone;

    // bool RefreshGraphs = false;//是否更新了波形数据
    // bool RefreshMouseWhell = false;
    // bool RefreshMousePress = false;
    // bool RefreshMouseMove = false;
    bool keyPress_X = false;
    bool keyPress_V = false;
};


#endif // MYQCUSTOMPLOT_H
