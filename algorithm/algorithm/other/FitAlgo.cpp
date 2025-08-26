#include "FitAlgo.h"

#include <QDebug>

#include "cmath"

const int FitAlgo::IterFactor      = 1000;  // 默认为500
const int FitAlgo::defaultRestarts = 2;

FitAlgo::FitAlgo(QObject* parent)
    : QObject(parent), alpha(-1.0), beta(0.5), gamma(2.0), root2(1.414214), maxError(1e-10) {
    // alpha, beta, gamma 是单纯形算法的参数   root2 是一个常数 √2   maxError 是拟合的最大误差。
    m_ptmaxnum = 10;  // 数据点的最大数量
    m_ptnum    = 0;   // 当前数量。

    // 四参数
    numParams   = 4;              // 拟合参数的数量
    numVertices = numParams + 1;  // 顶点的数量，比参数数量多1

    // restarts 和 nRestarts 分别是重启次数和已重启次数
    restarts  = defaultRestarts;
    nRestarts = 0;
}

// 添加数据点
void FitAlgo::add_Data_Point(QVector<QPointF>& _pDataPt) {
    m_pDataPt = _pDataPt;
    m_ptnum   = m_pDataPt.count();
}

// 线性拟合
bool FitAlgo::GetLinear(double& pxishu, double& pchangshu) {
    if (pxishu == 0 || pchangshu == 0)  // 参数和数据地址一个都不能为0
        return false;
    if (m_ptnum < 2) {  // 参与直线拟合的点数小于2个
        pxishu    = 1;
        pchangshu = 0;
        return true;
    }
    double u11 = 0, u12 = 0, u21 = 0, u22 = 0, c1 = 0, c2 = 0;
    double a, b;
    for (int i = 0; i < m_ptnum; i++)  // 列出方程U(a,b)' = c
    {
        u21 += m_pDataPt[i].x();
        u22 += m_pDataPt[i].x() * m_pDataPt[i].x();
        c1  += m_pDataPt[i].y();
        c2  += m_pDataPt[i].x() * m_pDataPt[i].y();
    }
    u12       = u21;
    u11       = m_ptnum;
    // 开始求解 y=ax+b
    b         = (c1 * u22 - c2 * u12) / (u11 * u22 - u12 * u21);
    a         = (c1 * u21 - c2 * u11) / (u21 * u12 - u22 * u11);
    pxishu    = a;
    pchangshu = b;
    return true;
}

bool FitAlgo::GetRodbard(double& a, double& b, double& c, double& d) {
    if (m_ptnum < 3) {  // 参与四参数拟合的点数小于3个
        a = 0;
        b = 2;
        c = 1;
        d = 100;
        return true;
    }
    numPoints     = m_ptnum;
    numParams     = getNumParams;   // 4
    numVertices   = numParams + 1;  // need 1 more vertice than parametres,
    // 初始化
    double firstx = m_pDataPt[0].x();
    double firsty = m_pDataPt[0].y();
    double lastx  = m_pDataPt[numPoints - 1].x();
    double lasty  = m_pDataPt[numPoints - 1].y();
    double xmean  = (firstx + lastx) / 2.0;  ////0.375
    //    double ymean = (firsty+lasty)/2.0;		//0.8435       不一样
    //    double miny=firsty, maxy=firsty;
    double slope  = 1.0;
    if ((lastx - firstx) != 0.0)
        slope = (lasty - firsty) / (lastx - firstx);
    else
        slope = 1.0;
    //    double yintercept = firsty - slope * firstx;

    maxIter   = IterFactor * numParams * numParams;  // 最大迭代次数     1000*4*4
    restarts  = defaultRestarts;                     // 重启次数     2
    nRestarts = 0;                                   // 当前重启次数

    simp[0][0] = firsty;
    simp[0][1] = 1.0;
    simp[0][2] = xmean;
    simp[0][3] = lasty;

    doFit(a, b, c, d);
    return true;
}

void FitAlgo::doFit(double& a, double& b, double& c, double& d) {
    int i;
    restart(0);
    numIter                     = 0;
    bool   done                 = false;
    double center[getNumParams] = {0};  // mean of simplex vertices  单点的平均值
    while (!done) {
        numIter++;
        for (i = 0; i < numParams; i++) {
            center[i] = 0.0;  // center:{0,0,0,0}
        }
        // get mean "center" of vertices, excluding worst 得到各点的平均值
        for (i = 0; i < numVertices; i++) {
            if (i != worst) {
                for (int j = 0; j < numParams; j++) center[j] = center[j] + simp[i][j];
            }
        }
        // center:8.90,5.58,1.91,0.9939
        // Reflect worst vertex through centre
        for (int i = 0; i < numParams; i++) {
            center[i] /= numParams;
            next[i]    = center[i] + alpha * (simp[worst][i] - center[i]);
        }
        sumResiduals(next);  // 更改next
        // if it's better than the best...
        if (next[numParams] <= simp[best][numParams]) {
            newVertex();  // 修改simp
            // try expanding it
            for (int i = 0; i < numParams; i++)
                next[i] = center[i] + gamma * (simp[worst][i] - center[i]);
            sumResiduals(next);
            // if this is even better, keep it
            if (next[numParams] <= simp[worst][numParams]) newVertex();
        }
        // else if better than the 2nd worst keep it...
        else if (next[numParams] <= simp[nextWorst][numParams]) {
            newVertex();
        }
        // else try to make positive contraction of the worst
        else {
            for (int i = 0; i < numParams; i++)
                next[i] = center[i] + beta * (simp[worst][i] - center[i]);
            sumResiduals(next);
            // if this is better than the second worst, keep it.
            if (next[numParams] <= simp[nextWorst][numParams]) {
                newVertex();
            }
            // if all else fails, contract simplex in on best
            else {
                for (int i = 0; i < numVertices; i++) {
                    if (i != best) {
                        for (int j = 0; j < numVertices; j++)  // add by zl,j=0
                            simp[i][j] = beta * (simp[i][j] + simp[best][j]);
                        sumResiduals(simp[i]);
                    }
                }
            }
        }
        order();
        double rtol = 2 * fabs(simp[best][numParams] - simp[worst][numParams]) /
                      (fabs(simp[best][numParams]) + fabs(simp[worst][numParams]) + 0.0000000001);
        if (numIter >= maxIter)
            done = true;
        else if (rtol < maxError) {
            restarts--;
            if (restarts < 0)
                done = true;
            else
                restart(best);
        }
    }
    order();
    double* pdata = simp[best];
    a             = pdata[0];
    b             = pdata[1];
    c             = pdata[2];
    d             = pdata[3];
}

void FitAlgo::restart(int n) {
    int    i;
    double step[4], p[4], q[4];
    // Copy nth vertice of simplex to first vertice
    for (i = 0; i < numParams; i++) {
        simp[0][i] = simp[n][i];
    }
    sumResiduals(simp[0]);  // Get sum of residuals^2 for first vertex
    for (i = 0; i < numParams; i++) {
        step[i] = simp[0][i] / 2.0;  // Step half the parametre value
        if (step[i] == 0.0)          // We can't have them all the same or we're going nowhere
            step[i] = 0.01;
    }
    // Some kind of factor for generating new vertices
    for (i = 0; i < numParams; i++) {
        p[i] = step[i] * (sqrt(double(numVertices)) + numParams - 1.0) / (numParams * root2);
        q[i] = step[i] * (sqrt(double(numVertices)) - 1.0) / (numParams * root2);
    }
    // Create the other simplex vertices by modifing previous one.
    for (i = 1; i < numVertices; i++) {
        for (int j = 0; j < numParams; j++) {
            simp[i][j] = simp[i - 1][j] + q[j];
        }
        simp[i][i - 1] = simp[i][i - 1] + p[i - 1];
        sumResiduals(simp[i]);
    }
    // Initialise current lowest/highest parametre estimates to simplex 1
    best      = 0;
    worst     = 0;
    nextWorst = 0;
    order();
    nRestarts++;
}

void FitAlgo::sumResiduals(double x[]) {
    x[numParams] = 0.0;
    for (int i = 0; i < numPoints; i++)
        x[numParams] = x[numParams] + pow((f(x, m_pDataPt[i].x()) - m_pDataPt[i].y()), 2);
}

double FitAlgo::f(double p[], double x) {
    double y;
    double ex;
    if (x == 0.0)
        ex = 0.0;
    else
        ex = exp(log(fabs(x / p[2])) * p[1]);
    y = p[0] - p[3];
    y = y / (1.0 + ex);
    return (y + p[3]);
}

double FitAlgo::df(double p[], double x) {
    double a = p[0];
    double b = p[1];
    double c = p[2];
    double d = p[3];
    double dy;
    double ex1;
    double ex2;

    if (x == 0.0)
        ex1 = 0.0;
    else
        ex1 = b * pow(fabs(c), b) * pow(x, b - 1) * (d - a);
    ex2 = pow(pow(x, b) + pow(fabs(c), b), 2);
    dy  = ex1 / ex2;
    return dy;
}

double FitAlgo::dff(double p[], double x) {
    double a = p[0];
    double b = p[1];
    double c = fabs(p[2]);
    double d = p[3];
    double dyy;
    double numerator =
        -(((pow(b, 2) + b) * pow(c, b) * d + ((-a * pow(b, 2)) - a * b) * pow(c, b)) *
              abs(pow(x, 2 * b)) +
          ((b - pow(b, 2)) * pow(c, 2 * b) * d + (a * pow(b, 2) - a * b) * pow(c, 2 * b)) *
              abs(pow(x, b)));

    double denominator =
        pow(x, 2) * fabs(pow(x, 3 * b)) + 3 * pow(c, b) * pow(x, 2) * fabs(pow(x, 2 * b)) +
        3 * pow(c, 2 * b) * pow(x, 2) * fabs(pow(x, b)) + pow(c, 3 * b) * pow(x, 2);

    dyy = numerator / denominator;
    return dyy;
}

double FitAlgo::dfff(double p[], double x) {
    double a = p[0];
    double b = p[1];
    double c = p[2];
    double d = p[3];
    double dyyy;
    dyyy = (((pow(b, 3) + 3 * pow(b, 2) + 2 * b) * pow(c, b) * d +
             ((-a * pow(b, 3)) - 3 * a * pow(b, 2) - 2 * a * b) * pow(c, b)) *
                pow(abs(x), 3 * b) +
            ((4 * b - 4 * pow(b, 3)) * pow(c, 2 * b) * d +
             (4 * a * pow(b, 3) - 4 * a * b) * pow(c, 2 * b)) *
                pow(abs(x), 2 * b) +
            ((pow(b, 3) - 3 * pow(b, 2) + 2 * b) * pow(c, 3 * b) * d +
             ((-a * pow(b, 3)) + 3 * a * pow(b, 2) - 2 * a * b) * pow(c, 3 * b)) *
                pow(abs(x), b)) /
           (pow(x, 3) * pow(fabs(x), 4 * b) + 4 * pow(c, b) * pow(x, 3) * pow(abs(x), 3 * b) +
            6 * pow(c, 2 * b) * pow(x, 3) * pow(abs(x), 2 * b) +
            4 * pow(c, 3 * b) * pow(x, 3) * pow(fabs(x), b) + pow(c, 4 * b) * pow(x, 3));

    return dyyy;
}

double FitAlgo::dffff(double p[], double x) {
    double a = p[0];
    double b = p[1];
    double c = p[2];
    double d = p[3];

    double dyyyy =
        (((pow(b, 4) + 6 * pow(b, 3) + 11 * pow(b, 2) + 6 * b) * pow(c, b) * d +
          ((-a * pow(b, 4)) - 6 * a * pow(b, 3) - 11 * a * pow(b, 2) - 6 * a * b) * pow(c, b)) *
             pow(x, 4 * b) +
         ((-11 * pow(b, 4) - 18 * pow(b, 3) + 11 * pow(b, 2) + 18 * b) * pow(c, 2 * b) * d +
          (11 * a * pow(b, 4) + 18 * a * pow(b, 3) - 11 * a * pow(b, 2) - 18 * a * b) *
              pow(c, 2 * b)) *
             pow(x, 3 * b) +
         ((11 * pow(b, 4) - 18 * pow(b, 3) - 11 * pow(b, 2) + 18 * b) * pow(c, 3 * b) * d +
          ((-11 * a * pow(b, 4)) + 18 * a * pow(b, 3) + 11 * a * pow(b, 2) - 18 * a * b) *
              pow(c, 3 * b)) *
             pow(x, 2 * b) +
         (((-pow(b, 4)) + 6 * pow(b, 3) - 11 * pow(b, 2) + 6 * b) * pow(c, 4 * b) * d +
          (a * pow(b, 4) - 6 * a * pow(b, 3) + 11 * a * pow(b, 2) - 6 * a * b) * pow(c, 4 * b)) *
             pow(x, b)) /
        (pow(x, 5 * b + 4) + 5 * pow(c, b) * pow(x, 4 * b + 4) +
         10 * pow(c, 2 * b) * pow(x, 3 * b + 4) + 10 * pow(c, 3 * b) * pow(x, 2 * b + 4) +
         5 * pow(c, 4 * b) * pow(x, b + 4) + pow(c, 5 * b) * pow(x, 4));

    return dyyyy;
}

QPointF FitAlgo::findPointWithMinY(QVector<QPointF> points) {
    // 确保数组不为空

    if (points.isEmpty()) {
        // 返回一个默认构造的 QPointF，表示无效值
        return QPointF();
    }
    // 初始化最小y值和对应的点
    qreal minY = points[0].y();

    QPointF pointWithMinY = points[0];

    // 遍历数组查找最小y值和对应的点

    foreach (QPointF point, points) {
        if (point.y() < minY) {
            minY = point.y();

            pointWithMinY = point;
        }
    }
    return pointWithMinY;
}
/*
// 牛顿法迭代函数
double FitAlgo::newton_method(Func _f,Func _df,double P[] ,double min) {

    double tolerance =1e-8;
    double x = min;

    int max_iterations =10000;

    for (int i = 0; i < max_iterations; ++i) {


        double fx = _f(P,x);
        double fpx = _df(P,x);
        // 避免除以零
        if (std::abs(fpx) < 1e-8) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // 牛顿法迭代公式
        x = x - fx / fpx;

        // 判断是否满足精度要求
        if (std::abs(fx) < tolerance) {
            return x;
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

*/
/*
// 二分法求解导数为0
double FitAlgo::binarySearch( Func _f, double P[], double right) {

    double  epsilon = 0;
    int maxIterations = 1000;
    double left=0.01;
    for (int i = 0; i < maxIterations; ++i) {
        double mid = (left + right) / 2.0;
        double derivativeValue = _f(P,mid);
        qDebug()<<"-------"<<mid<<"---"<<derivativeValue;

        if ( derivativeValue > epsilon) {
            return mid; // 找到近似极小值
        }

        if (derivativeValue > 0) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return (left + right) / 2.0; // 返回迭代结束时的值，可能是近似解
}

*/
void FitAlgo::order() {
    int i;
    for (i = 0; i < numVertices; i++) {
        if (simp[i][numParams] < simp[best][numParams]) best = i;
        if (simp[i][numParams] > simp[worst][numParams]) worst = i;
    }
    nextWorst = best;
    for (i = 0; i < numVertices; i++) {
        if (i != worst) {
            if (simp[i][numParams] > simp[nextWorst][numParams]) nextWorst = i;
        }
    }
    //        IJ.log("B: " + simp[best][numParams] + " 2ndW: " + simp[nextWorst][numParams] + " W: "
    //        + simp[worst][numParams]);
}

void FitAlgo::newVertex() {
    /*****add by zl  ******/
    for (int i = 0; i < numVertices; i++) {
        simp[worst][i] = next[i];
    }
}

FitAlgo::~FitAlgo() {}
