#include "WSetting_PaperPrintConfig.h"

#include <QPainter>

#include "WSetting_PaperPrintFormat.h"
#include "ui_WSetting_PaperPrintConfig.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
QPixmap PaperPrintConfig::get_testData_curvePlot() const {
    QPixmap __pixmap(560, 100);
    __pixmap.fill(QColor(245, 248, 252));
    QPainter __painter;
    if (__painter.begin(&__pixmap)) {
        QPen __pen;
        __pen.setColor(QColor(62, 137, 249));
        __pen.setWidthF(2);
        __painter.setPen(__pen);

        double plot_w   = 560;
        double plot_h   = 100;
        int    lefTop_x = 0;
        int    lefTop_y = 0;

        double          _paint_x, _paint_y;
        QVector<double> curveData = {
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0,         0,         0,         0,         0,         0,         0,         0,
            0.001465,  0.004395,  0.012451,  0.023438,  0.043945,  0.068848,  0.103271,  0.146484,
            0.194824,  0.254883,  0.317871,  0.392578,  0.470215,  0.558105,  0.651855,  0.751465,
            0.862032,  0.974064,  1.100866,  1.227668,  1.371933,  1.517908,  1.681266,  1.852241,
            2.038587,  2.238316,  2.456286,  2.685002,  2.943291,  3.204852,  3.505894,  3.808329,
            4.151844,  4.500405,  4.888367,  5.282238,  5.718526,  6.157072,  6.640745,  7.124419,
            7.650337,  8.176256,  8.740142,  9.304028,  9.899151,  10.494275, 11.11202,  11.729765,
            12.361248, 12.994197, 13.631836, 14.27124,  14.909409, 15.54167,  16.171556, 16.788109,
            17.400833, 17.998093, 18.591525, 19.168405, 19.740691, 20.300934, 20.851437, 21.395928,
            21.930895, 22.459797, 22.984497, 23.505615, 24.024902, 24.544189, 25.059814, 25.571411,
            26.082275, 26.588013, 27.095947, 27.602783, 28.111084, 28.618286, 29.123657, 29.629578,
            30.133667, 30.636887, 31.140106, 31.636505, 32.132904, 32.622116, 33.111328, 33.596191,
            34.081055, 34.561523, 35.040527, 35.515137, 35.985352, 36.452637, 36.914063, 37.372559,
            37.82373,  38.271973, 38.711426, 39.146484, 39.574219, 39.997559, 40.413574, 40.828125,
            41.232422, 41.636719, 42.029297, 42.42041,  42.802734, 43.180481, 43.551773, 43.919952,
            44.278633, 44.637314, 44.986084, 45.334854, 45.676094, 46.016235, 46.349487, 46.680542,
            47.005005, 47.328369, 47.645142, 47.958984, 48.268066, 48.569824, 48.868652, 49.160156,
            49.447632, 49.729614, 50.005737, 50.278198, 50.544067, 50.808838, 51.068481, 51.328125,
            51.584473, 51.839355, 52.091309, 52.338867, 52.583496, 52.82373,  53.059204, 53.293213,
            53.521729, 53.750244, 53.974731, 54.197754, 54.41748,  54.634277, 54.84668,  55.056152,
            55.26123,  55.463379, 55.662598, 55.858887, 56.052246, 56.241211, 56.428711, 56.611816,
            56.794922, 56.975098, 57.155273, 57.331055, 57.506836, 57.676758, 57.84668,  58.012207,
            58.177734, 58.340332, 58.504395, 58.665527, 58.828125, 58.987793, 59.144531, 59.296875,
            59.444824, 59.588379, 59.730469, 59.869629, 60.008789, 60.143555, 60.279785, 60.411621,
            60.546387, 60.679688, 60.812988, 60.946289, 61.07666,  61.207031, 61.335938, 61.463379,
            61.589355, 61.712402, 61.83252,  61.949707, 62.06543,  62.178223, 62.289551, 62.399414,
            62.506348, 62.613281, 62.71875,  62.824219, 62.928223, 63.032227, 63.133301, 63.234375,
            63.333984, 63.432129, 63.530273, 63.624023, 63.717773, 63.807129, 63.897949, 63.987305,
            64.078125, 64.168945, 64.256836, 64.344727, 64.428223, 64.511719, 64.59375,  64.675781,
            64.756348, 64.836914, 64.914551, 64.992188, 65.068359, 65.144531, 65.219238, 65.293945,
            65.365723, 65.4375,   65.507813, 65.578125, 65.648438, 65.71875,  65.787598, 65.856445,
            65.922363, 65.988281, 66.052734, 66.117188, 66.180176, 66.243164, 66.303223, 66.363281,
            66.421875, 66.480469, 66.539063, 66.597656, 66.65625,  66.714844, 66.771973, 66.830566,
            66.884766, 66.943359, 66.997559, 67.054688, 67.110352, 67.163086, 67.217285, 67.26709,
            67.319824, 67.371094, 67.422363, 67.473633, 67.521973, 67.570313, 67.618652, 67.665527,
            67.715332, 67.762207, 67};
        for (int i = 0; i < (int)curveData.size(); i += 1) {
            _paint_x = (double)(i + 1) / 1000.0 * plot_w;
            _paint_y = curveData.at(i) / 100.0 * plot_h / 2.0;
            _paint_y = plot_h / 2.0 - _paint_y;
            __painter.drawPoint(QPointF(_paint_x + lefTop_x, _paint_y + lefTop_y));

            _paint_y = plot_h - _paint_y;
            __painter.drawPoint(QPointF(_paint_x + lefTop_x, _paint_y + lefTop_y));
        }
        __painter.end();
    }
    return __pixmap;
}

PaperPrintConfig::PaperPrintConfig(QWidget* _par) : QWidget(_par), ui(new Ui::PaperPrintConfig{}) {
    ui->setupUi(this);

    { new PaperPrintFormat(ui->widget_paperPreview); }

    {
        /// data
        css_btn_checked =
            " image: url(:/PHOTOS/tick.png); background-color: qlineargradient(spread:pad, x1:0, "
            "y1:0, x2:0, y2:1, stop:0 rgba(85, 149, 255, 255), stop:1 rgba(45, 128, 245, "
            "255));border-radius: 8px; ";
        css_btn_unchecked =
            " background-color: #FAFBFC; border: 1px solid #C8CACC; border-radius: 8px;";
        css_btn_moveUp_able     = " image: url(:/PHOTOS/btn_moveUp_able.png);";
        css_btn_moveUp_unable   = " image: url(:/PHOTOS/btn_moveUp_unable.png);";
        css_btn_moveDown_able   = " image: url(:/PHOTOS/btn_moveDown_able.png);";
        css_btn_moveDown_unable = " image: url(:/PHOTOS/btn_moveDown_unable.png);";

        ptr_ui_btn_able      = {ui->btn_0_able,   ui->btn_1_able,   ui->btn_1_able_2,
                                ui->btn_1_able_3, ui->btn_1_able_4, ui->btn_1_able_5,
                                ui->btn_1_able_6, ui->btn_1_able_7, ui->btn_1_able_8};
        ptr_ui_btn_moveUp    = {ui->btn_0_up,   ui->btn_1_up,   ui->btn_1_up_2,
                                ui->btn_1_up_3, ui->btn_1_up_4, ui->btn_1_up_5,
                                ui->btn_1_up_6, ui->btn_1_up_7, ui->btn_1_up_8};
        ptr_ui_btn_moveDown  = {ui->btn_0_down,   ui->btn_1_down,   ui->btn_1_down_2,
                                ui->btn_1_down_3, ui->btn_1_down_4, ui->btn_1_down_5,
                                ui->btn_1_down_6, ui->btn_1_down_7, ui->btn_1_down_8};
        ptr_ui_widget_module = {ui->widget,   ui->widget_2, ui->widget_3,
                                ui->widget_4, ui->widget_5, ui->widget_6,
                                ui->widget_7, ui->widget_8, ui->widget_9};

        testData_curvePlot = get_testData_curvePlot();

        lab_paperPreview.setParent(this->ui->widget_paperPreview);
        lab_paperPreview.setGeometry(QRect(QPoint(0, 0), this->ui->widget_paperPreview->size()));
    }

    {
        /// ui
        for (int i = 0; i < ptr_ui_widget_module.size(); i += 1) {
            ptr_ui_btn_able.at(i)->setStyleSheet(css_btn_checked);
            ptr_ui_btn_moveUp.at(i)->setStyleSheet(css_btn_moveUp_able);
            ptr_ui_btn_moveDown.at(i)->setStyleSheet(css_btn_moveDown_able);
        }
    }
}

PaperPrintConfig::~PaperPrintConfig() {
    delete ui;
}

void PaperPrintConfig::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
