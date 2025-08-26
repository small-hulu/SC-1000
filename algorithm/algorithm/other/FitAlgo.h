#ifndef FITALGO_H
#define FITALGO_H

#include <QObject>
#include <QPointF>

/*
const double alpha = -1.0;     // reflection coefficient 反射系数
const  double beta = 0.5;   // contraction coefficient 收缩系数
const  double gamma = 2.0;      // expansion coefficient 膨胀系数
const double root2 = 1.414214; // square root of 2    2 的平方根
const  double maxError = 1e-10;
*/
constexpr int getNumParams = 4;  // 获取参数个数

class FitAlgo : public QObject {
    Q_OBJECT

public:
    explicit FitAlgo(QObject* parent = nullptr);
    ~FitAlgo();

public:
    //    typedef std::function<double(double[], double)>  Func;
    static const int
        IterFactor;  // 表示 Nelder-Mead simplex
                     // 算法中的迭代因子。这个因子用于设置最大迭代次数的倍数，控制算法的收敛速度
    static const int
        defaultRestarts;  // 这个值用于控制拟合算法在迭代中重新初始化的次数，以帮助算法跳出局部最优解，提高全局搜索的效果

    const double alpha;     // reflection coefficient
    const double beta;      // contraction coefficient
    const double gamma;     // expansion coefficient
    const double root2;     // square root of 2
    const double maxError;  // 最大误差阈值

    QVector<QPointF> m_pDataPt;  // 指向数据点数组的指针

    int m_ptmaxnum;  // 能够容纳的数据点的最大数量
    int m_ptnum;
    // 这个变量在拟合过程中用于跟踪已添加的数据点数量
private:
    int numPoints;  // number of data points //表示拟合算法中参与拟合的数据点的数量
    int numParams;  // number of parametres //参数的个数
    int numVertices;  // numParams+1 (includes sumLocalResiduaalsSqrd)  表示拟合算法中的顶点数量
    int maxIter;   // maximum number of iterations per restart 表示 Nelder-Mead simplex
                   // 算法的最大迭代次数
    int restarts;  // number of times to restart simplex after first soln. 表示 Nelder-Mead simplex
                   // 算法中的重新启动次数
    int nRestarts;  // the number of restarts that occurred
                    // //该变量用于记录算法在执行过程中进行重新启动的次数
    int numIter;    // number of iterations so far     表示 Nelder-Mead simplex
                  // 算法的迭代次数。该变量用于记录拟合算法执行的迭代次数。
    int worst;      // worst current parametre            拟合效果最差的顶点的索引
    int nextWorst;  // 2nd worst current parametre estimates   拟合效果次差的顶点的索引
    int best;       // best current parametre estimates        拟合效果最好的顶点的索引
    double next[getNumParams + 1];  // 这个数组在算法的迭代过程中被用来计算和更新新的顶点。  // new
                                    // vertex to be tested

    double simp[getNumParams + 1][getNumParams + 1];  // 这个数组用于存储算法中的顶点信息

public:
    void   add_Data_Point(QVector<QPointF>& _pDataPt);    // 提添加数据点
    bool   GetLinear(double& pxishu, double& pchangshu);  // pxishu = k  pchangshu=b
    bool   GetRodbard(double& a, double& b, double& c,
                      double& d);  // 该函数的作用是根据类中存储的数据点，拟合出 Rodbard 函数的参数
    double f(double p[], double x);
    double df(double p[], double x);     // 一阶导数
    double dff(double p[], double x);    // 二阶导数
    double dfff(double p[], double x);   // 三阶导数
    double dffff(double p[], double x);  // 三阶导数

    //  double binarySearch(Func _f, double P[], double right);

    //  double newton_method(Func _f, Func _df, double P[], double min);
    QPointF findPointWithMinY(QVector<QPointF> points);

private:                                                     // 四参数
    void doFit(double& a, double& b, double& c, double& d);  // 用于执行拟合算法的主要拟合过程
    void restart(int n);  // 根据当前状态重新初始化顶点集，以期望通过新的起点获得更好的拟合结果
                          // ,参数 n 是当前最差的顶点的索引
    void sumResiduals(double x[]);  // 用于计算拟合函数在当前参数下的所有数据点的残差平方和

    void order();  // 该函数的主要目的是在拟合算法的迭代过程中根据拟合误差对顶点进行排序
    void newVertex();  // 于更新当前最差的顶点（worst vertex）的值为新的顶点值
signals:
};

#endif  // FITALGO_H
