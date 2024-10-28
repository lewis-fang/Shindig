/********************************************************************************
** Form generated from reading UI file 'FNN.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FNN_H
#define UI_FNN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FNNClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_34;
    QSpinBox *spinBox_WtsCols;
    QSpinBox *spinBox_WtsRows;
    QLabel *label_2;
    QSpinBox *spinBox_Strides;
    QSpinBox *spinBox_poolingDim1;
    QLabel *label_14;
    QSpinBox *spinBox_poolingDim2;
    QLabel *label_29;
    QLabel *label_33;
    QSpinBox *spinBox_NeuroNums;
    QSpinBox *spinBox_poolingFun;
    QPushButton *pushButton_addLayer;
    QLabel *label_4;
    QLabel *label_10;
    QComboBox *comboBox_LayerType;
    QComboBox *comboBox_padding;
    QLabel *label_5;
    QLabel *label_15;
    QLabel *label_3;
    QSpinBox *spinBox_poolingStride;
    QLineEdit *lineEdit_initSd;
    QSpinBox *spinBox_actFunction;
    QLabel *label_24;
    QLabel *label_12;
    QLabel *label_11;
    QLabel *label_22;
    QLineEdit *lineEdit_bias;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_6;
    QLabel *label_27;
    QLineEdit *lineEdit_depth;
    QPushButton *pushButton_lockModel;
    QLabel *label_16;
    QPushButton *pushButton_ClearModel;
    QPushButton *pushButton_popBackLayer;
    QLabel *label_32;
    QPushButton *pushButton;
    QLabel *label_23;
    QLabel *label_21;
    QLabel *label_30;
    QPushButton *pushButton_saveModel;
    QLineEdit *lineEdit_modelname;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QTextBrowser *textBrowser;
    QLabel *label_18;
    QSpinBox *spinBox_inputImageIndex;
    QSpinBox *spinBox_viewOutTest;
    QLabel *label_25;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QSpinBox *spinBox_cols;
    QSpinBox *spinBox_chans;
    QSpinBox *spinBox;
    QLabel *label_6;
    QLabel *label_19;
    QSpinBox *spinBox_2;
    QLabel *label_7;
    QLabel *label_9;
    QSpinBox *spinB_ExpectedOutLen;
    QLabel *label_8;
    QLabel *label_17;
    QSpinBox *spinBox_rows;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_7;
    QLabel *label_13;
    QPushButton *pushButton_ImportWts;
    QLabel *label_20;
    QPushButton *pushButton_LaunchModel;
    QLabel *label_44;
    QLineEdit *lineEdit_testImgNum;
    QPushButton *pushButton_ImportTestImg;
    QPushButton *pushButton_cifarTest;
    QWidget *widget_out;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QWidget *widget_2;
    QGridLayout *gridLayout_9;
    QLabel *label_pITURE;
    QWidget *widget_Loss;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_Loss;
    QTreeView *treeView;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_8;
    QLabel *label_37;
    QLabel *label_42;
    QPushButton *pushButton_ImportIdealOut;
    QLineEdit *lineEdit_NormalizedSHif;
    QLabel *label_36;
    QLabel *label_35;
    QFrame *line;
    QPushButton *pushButton_importImage;
    QLineEdit *lineEdit_diffLoss;
    QLabel *label_41;
    QLineEdit *lineEdit_Loss;
    QLabel *label_43;
    QLabel *label_38;
    QLabel *label_40;
    QPushButton *pushButton_cifarTrains;
    QLabel *label;
    QLabel *label_Traingmode;
    QLineEdit *lineEdit_learnRate;
    QLineEdit *lineEdit_NormalizedRange;
    QPushButton *pushButton_startTraning;
    QComboBox *comboBox_normalizedMethd;
    QLineEdit *lineEdit_maxIters;
    QLabel *label_26;
    QLabel *label_39;
    QLineEdit *lineEdit_ImgNumbers;
    QLabel *label_28;
    QSpinBox *spinBox_batchSize;
    QComboBox *comboBox_useSimd;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FNNClass)
    {
        if (FNNClass->objectName().isEmpty())
            FNNClass->setObjectName(QStringLiteral("FNNClass"));
        FNNClass->resize(1668, 918);
        centralWidget = new QWidget(FNNClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_34 = new QLabel(groupBox);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout_2->addWidget(label_34, 7, 0, 1, 1);

        spinBox_WtsCols = new QSpinBox(groupBox);
        spinBox_WtsCols->setObjectName(QStringLiteral("spinBox_WtsCols"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(spinBox_WtsCols->sizePolicy().hasHeightForWidth());
        spinBox_WtsCols->setSizePolicy(sizePolicy1);
        spinBox_WtsCols->setMinimum(1);
        spinBox_WtsCols->setValue(3);

        gridLayout_2->addWidget(spinBox_WtsCols, 2, 1, 1, 1);

        spinBox_WtsRows = new QSpinBox(groupBox);
        spinBox_WtsRows->setObjectName(QStringLiteral("spinBox_WtsRows"));
        sizePolicy1.setHeightForWidth(spinBox_WtsRows->sizePolicy().hasHeightForWidth());
        spinBox_WtsRows->setSizePolicy(sizePolicy1);
        spinBox_WtsRows->setMinimum(1);
        spinBox_WtsRows->setValue(3);

        gridLayout_2->addWidget(spinBox_WtsRows, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        spinBox_Strides = new QSpinBox(groupBox);
        spinBox_Strides->setObjectName(QStringLiteral("spinBox_Strides"));
        sizePolicy1.setHeightForWidth(spinBox_Strides->sizePolicy().hasHeightForWidth());
        spinBox_Strides->setSizePolicy(sizePolicy1);
        spinBox_Strides->setMinimum(1);

        gridLayout_2->addWidget(spinBox_Strides, 3, 1, 1, 1);

        spinBox_poolingDim1 = new QSpinBox(groupBox);
        spinBox_poolingDim1->setObjectName(QStringLiteral("spinBox_poolingDim1"));
        sizePolicy1.setHeightForWidth(spinBox_poolingDim1->sizePolicy().hasHeightForWidth());
        spinBox_poolingDim1->setSizePolicy(sizePolicy1);
        spinBox_poolingDim1->setMinimum(1);
        spinBox_poolingDim1->setSingleStep(2);
        spinBox_poolingDim1->setValue(2);

        gridLayout_2->addWidget(spinBox_poolingDim1, 12, 1, 1, 1);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 15, 0, 1, 1);

        spinBox_poolingDim2 = new QSpinBox(groupBox);
        spinBox_poolingDim2->setObjectName(QStringLiteral("spinBox_poolingDim2"));
        sizePolicy1.setHeightForWidth(spinBox_poolingDim2->sizePolicy().hasHeightForWidth());
        spinBox_poolingDim2->setSizePolicy(sizePolicy1);
        spinBox_poolingDim2->setMinimum(1);
        spinBox_poolingDim2->setSingleStep(2);
        spinBox_poolingDim2->setValue(2);

        gridLayout_2->addWidget(spinBox_poolingDim2, 13, 1, 1, 1);

        label_29 = new QLabel(groupBox);
        label_29->setObjectName(QStringLiteral("label_29"));

        gridLayout_2->addWidget(label_29, 9, 0, 1, 1);

        label_33 = new QLabel(groupBox);
        label_33->setObjectName(QStringLiteral("label_33"));

        gridLayout_2->addWidget(label_33, 11, 0, 1, 1);

        spinBox_NeuroNums = new QSpinBox(groupBox);
        spinBox_NeuroNums->setObjectName(QStringLiteral("spinBox_NeuroNums"));
        sizePolicy1.setHeightForWidth(spinBox_NeuroNums->sizePolicy().hasHeightForWidth());
        spinBox_NeuroNums->setSizePolicy(sizePolicy1);
        spinBox_NeuroNums->setMinimum(1);
        spinBox_NeuroNums->setMaximum(512);
        spinBox_NeuroNums->setValue(3);

        gridLayout_2->addWidget(spinBox_NeuroNums, 5, 1, 1, 1);

        spinBox_poolingFun = new QSpinBox(groupBox);
        spinBox_poolingFun->setObjectName(QStringLiteral("spinBox_poolingFun"));
        sizePolicy1.setHeightForWidth(spinBox_poolingFun->sizePolicy().hasHeightForWidth());
        spinBox_poolingFun->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(spinBox_poolingFun, 15, 1, 1, 1);

        pushButton_addLayer = new QPushButton(groupBox);
        pushButton_addLayer->setObjectName(QStringLiteral("pushButton_addLayer"));
        pushButton_addLayer->setStyleSheet(QStringLiteral("background-color: rgb(225, 225, 255);"));

        gridLayout_2->addWidget(pushButton_addLayer, 9, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 5, 0, 1, 1);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_2->addWidget(label_10, 12, 0, 1, 1);

        comboBox_LayerType = new QComboBox(groupBox);
        comboBox_LayerType->setObjectName(QStringLiteral("comboBox_LayerType"));

        gridLayout_2->addWidget(comboBox_LayerType, 7, 1, 1, 1);

        comboBox_padding = new QComboBox(groupBox);
        comboBox_padding->setObjectName(QStringLiteral("comboBox_padding"));

        gridLayout_2->addWidget(comboBox_padding, 11, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 16, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        spinBox_poolingStride = new QSpinBox(groupBox);
        spinBox_poolingStride->setObjectName(QStringLiteral("spinBox_poolingStride"));
        sizePolicy1.setHeightForWidth(spinBox_poolingStride->sizePolicy().hasHeightForWidth());
        spinBox_poolingStride->setSizePolicy(sizePolicy1);
        spinBox_poolingStride->setMinimum(1);
        spinBox_poolingStride->setValue(2);

        gridLayout_2->addWidget(spinBox_poolingStride, 14, 1, 1, 1);

        lineEdit_initSd = new QLineEdit(groupBox);
        lineEdit_initSd->setObjectName(QStringLiteral("lineEdit_initSd"));
        sizePolicy1.setHeightForWidth(lineEdit_initSd->sizePolicy().hasHeightForWidth());
        lineEdit_initSd->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(lineEdit_initSd, 17, 1, 1, 1);

        spinBox_actFunction = new QSpinBox(groupBox);
        spinBox_actFunction->setObjectName(QStringLiteral("spinBox_actFunction"));
        sizePolicy1.setHeightForWidth(spinBox_actFunction->sizePolicy().hasHeightForWidth());
        spinBox_actFunction->setSizePolicy(sizePolicy1);
        spinBox_actFunction->setSingleStep(1);
        spinBox_actFunction->setValue(4);

        gridLayout_2->addWidget(spinBox_actFunction, 16, 1, 1, 1);

        label_24 = new QLabel(groupBox);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_2->addWidget(label_24, 17, 0, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_2->addWidget(label_12, 14, 0, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 13, 0, 1, 1);

        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_2->addWidget(label_22, 18, 0, 1, 1);

        lineEdit_bias = new QLineEdit(groupBox);
        lineEdit_bias->setObjectName(QStringLiteral("lineEdit_bias"));

        gridLayout_2->addWidget(lineEdit_bias, 18, 1, 1, 1);


        gridLayout->addWidget(groupBox, 1, 2, 4, 3);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        gridLayout_6 = new QGridLayout(groupBox_5);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        label_27 = new QLabel(groupBox_5);
        label_27->setObjectName(QStringLiteral("label_27"));

        gridLayout_6->addWidget(label_27, 4, 0, 1, 1);

        lineEdit_depth = new QLineEdit(groupBox_5);
        lineEdit_depth->setObjectName(QStringLiteral("lineEdit_depth"));
        lineEdit_depth->setEnabled(false);
        sizePolicy1.setHeightForWidth(lineEdit_depth->sizePolicy().hasHeightForWidth());
        lineEdit_depth->setSizePolicy(sizePolicy1);

        gridLayout_6->addWidget(lineEdit_depth, 0, 1, 1, 1);

        pushButton_lockModel = new QPushButton(groupBox_5);
        pushButton_lockModel->setObjectName(QStringLiteral("pushButton_lockModel"));
        sizePolicy1.setHeightForWidth(pushButton_lockModel->sizePolicy().hasHeightForWidth());
        pushButton_lockModel->setSizePolicy(sizePolicy1);

        gridLayout_6->addWidget(pushButton_lockModel, 7, 1, 1, 1);

        label_16 = new QLabel(groupBox_5);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_6->addWidget(label_16, 7, 0, 1, 1);

        pushButton_ClearModel = new QPushButton(groupBox_5);
        pushButton_ClearModel->setObjectName(QStringLiteral("pushButton_ClearModel"));
        sizePolicy1.setHeightForWidth(pushButton_ClearModel->sizePolicy().hasHeightForWidth());
        pushButton_ClearModel->setSizePolicy(sizePolicy1);

        gridLayout_6->addWidget(pushButton_ClearModel, 5, 1, 1, 1);

        pushButton_popBackLayer = new QPushButton(groupBox_5);
        pushButton_popBackLayer->setObjectName(QStringLiteral("pushButton_popBackLayer"));
        sizePolicy1.setHeightForWidth(pushButton_popBackLayer->sizePolicy().hasHeightForWidth());
        pushButton_popBackLayer->setSizePolicy(sizePolicy1);

        gridLayout_6->addWidget(pushButton_popBackLayer, 4, 1, 1, 1);

        label_32 = new QLabel(groupBox_5);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout_6->addWidget(label_32, 5, 0, 1, 1);

        pushButton = new QPushButton(groupBox_5);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        gridLayout_6->addWidget(pushButton, 6, 1, 1, 1);

        label_23 = new QLabel(groupBox_5);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_6->addWidget(label_23, 0, 0, 1, 1);

        label_21 = new QLabel(groupBox_5);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_6->addWidget(label_21, 6, 0, 1, 1);

        label_30 = new QLabel(groupBox_5);
        label_30->setObjectName(QStringLiteral("label_30"));

        gridLayout_6->addWidget(label_30, 9, 0, 1, 1);

        pushButton_saveModel = new QPushButton(groupBox_5);
        pushButton_saveModel->setObjectName(QStringLiteral("pushButton_saveModel"));

        gridLayout_6->addWidget(pushButton_saveModel, 9, 1, 1, 1);

        lineEdit_modelname = new QLineEdit(groupBox_5);
        lineEdit_modelname->setObjectName(QStringLiteral("lineEdit_modelname"));

        gridLayout_6->addWidget(lineEdit_modelname, 8, 1, 1, 1);


        gridLayout->addWidget(groupBox_5, 1, 5, 2, 1);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        textBrowser = new QTextBrowser(groupBox_4);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        sizePolicy.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy);

        gridLayout_5->addWidget(textBrowser, 6, 1, 1, 2);

        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_5->addWidget(label_18, 4, 1, 1, 1);

        spinBox_inputImageIndex = new QSpinBox(groupBox_4);
        spinBox_inputImageIndex->setObjectName(QStringLiteral("spinBox_inputImageIndex"));
        sizePolicy1.setHeightForWidth(spinBox_inputImageIndex->sizePolicy().hasHeightForWidth());
        spinBox_inputImageIndex->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(spinBox_inputImageIndex, 3, 2, 1, 1);

        spinBox_viewOutTest = new QSpinBox(groupBox_4);
        spinBox_viewOutTest->setObjectName(QStringLiteral("spinBox_viewOutTest"));
        sizePolicy1.setHeightForWidth(spinBox_viewOutTest->sizePolicy().hasHeightForWidth());
        spinBox_viewOutTest->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(spinBox_viewOutTest, 4, 2, 1, 1);

        label_25 = new QLabel(groupBox_4);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout_5->addWidget(label_25, 3, 1, 1, 1);


        gridLayout->addWidget(groupBox_4, 3, 5, 2, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        spinBox_cols = new QSpinBox(groupBox_2);
        spinBox_cols->setObjectName(QStringLiteral("spinBox_cols"));
        spinBox_cols->setEnabled(true);
        sizePolicy1.setHeightForWidth(spinBox_cols->sizePolicy().hasHeightForWidth());
        spinBox_cols->setSizePolicy(sizePolicy1);
        spinBox_cols->setMinimum(2);
        spinBox_cols->setValue(32);

        gridLayout_3->addWidget(spinBox_cols, 2, 1, 1, 1);

        spinBox_chans = new QSpinBox(groupBox_2);
        spinBox_chans->setObjectName(QStringLiteral("spinBox_chans"));
        spinBox_chans->setEnabled(true);
        sizePolicy1.setHeightForWidth(spinBox_chans->sizePolicy().hasHeightForWidth());
        spinBox_chans->setSizePolicy(sizePolicy1);
        spinBox_chans->setMinimum(1);
        spinBox_chans->setValue(3);

        gridLayout_3->addWidget(spinBox_chans, 3, 1, 1, 1);

        spinBox = new QSpinBox(groupBox_2);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        gridLayout_3->addWidget(spinBox, 4, 1, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 6, 0, 1, 1);

        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_3->addWidget(label_19, 5, 0, 1, 1);

        spinBox_2 = new QSpinBox(groupBox_2);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));

        gridLayout_3->addWidget(spinBox_2, 5, 1, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 1, 0, 1, 1);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_3->addWidget(label_9, 3, 0, 1, 1);

        spinB_ExpectedOutLen = new QSpinBox(groupBox_2);
        spinB_ExpectedOutLen->setObjectName(QStringLiteral("spinB_ExpectedOutLen"));
        sizePolicy1.setHeightForWidth(spinB_ExpectedOutLen->sizePolicy().hasHeightForWidth());
        spinB_ExpectedOutLen->setSizePolicy(sizePolicy1);
        spinB_ExpectedOutLen->setMinimum(1);

        gridLayout_3->addWidget(spinB_ExpectedOutLen, 6, 1, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 2, 0, 1, 1);

        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_3->addWidget(label_17, 4, 0, 1, 1);

        spinBox_rows = new QSpinBox(groupBox_2);
        spinBox_rows->setObjectName(QStringLiteral("spinBox_rows"));
        spinBox_rows->setEnabled(true);
        sizePolicy1.setHeightForWidth(spinBox_rows->sizePolicy().hasHeightForWidth());
        spinBox_rows->setSizePolicy(sizePolicy1);
        spinBox_rows->setMinimum(2);
        spinBox_rows->setValue(32);

        gridLayout_3->addWidget(spinBox_rows, 1, 1, 1, 1);


        gridLayout->addWidget(groupBox_2, 1, 1, 2, 1);

        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        sizePolicy2.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy2);
        gridLayout_7 = new QGridLayout(groupBox_6);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        label_13 = new QLabel(groupBox_6);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_7->addWidget(label_13, 2, 0, 1, 1);

        pushButton_ImportWts = new QPushButton(groupBox_6);
        pushButton_ImportWts->setObjectName(QStringLiteral("pushButton_ImportWts"));
        sizePolicy1.setHeightForWidth(pushButton_ImportWts->sizePolicy().hasHeightForWidth());
        pushButton_ImportWts->setSizePolicy(sizePolicy1);

        gridLayout_7->addWidget(pushButton_ImportWts, 2, 1, 1, 1);

        label_20 = new QLabel(groupBox_6);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_7->addWidget(label_20, 0, 0, 1, 1);

        pushButton_LaunchModel = new QPushButton(groupBox_6);
        pushButton_LaunchModel->setObjectName(QStringLiteral("pushButton_LaunchModel"));
        sizePolicy.setHeightForWidth(pushButton_LaunchModel->sizePolicy().hasHeightForWidth());
        pushButton_LaunchModel->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(223, 208, 240, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(239, 231, 247, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(111, 104, 120, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(149, 139, 160, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        pushButton_LaunchModel->setPalette(palette);
        pushButton_LaunchModel->setFlat(false);

        gridLayout_7->addWidget(pushButton_LaunchModel, 5, 0, 1, 2);

        label_44 = new QLabel(groupBox_6);
        label_44->setObjectName(QStringLiteral("label_44"));

        gridLayout_7->addWidget(label_44, 1, 0, 1, 1);

        lineEdit_testImgNum = new QLineEdit(groupBox_6);
        lineEdit_testImgNum->setObjectName(QStringLiteral("lineEdit_testImgNum"));
        sizePolicy1.setHeightForWidth(lineEdit_testImgNum->sizePolicy().hasHeightForWidth());
        lineEdit_testImgNum->setSizePolicy(sizePolicy1);

        gridLayout_7->addWidget(lineEdit_testImgNum, 0, 1, 1, 1);

        pushButton_ImportTestImg = new QPushButton(groupBox_6);
        pushButton_ImportTestImg->setObjectName(QStringLiteral("pushButton_ImportTestImg"));

        gridLayout_7->addWidget(pushButton_ImportTestImg, 1, 1, 1, 1);

        pushButton_cifarTest = new QPushButton(groupBox_6);
        pushButton_cifarTest->setObjectName(QStringLiteral("pushButton_cifarTest"));

        gridLayout_7->addWidget(pushButton_cifarTest, 4, 1, 1, 1);


        gridLayout->addWidget(groupBox_6, 3, 1, 2, 1);

        widget_out = new QWidget(centralWidget);
        widget_out->setObjectName(QStringLiteral("widget_out"));
        gridLayout_4 = new QGridLayout(widget_out);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget = new QWidget(widget_out);
        widget->setObjectName(QStringLiteral("widget"));

        verticalLayout_3->addWidget(widget);


        gridLayout_4->addLayout(verticalLayout_3, 0, 1, 1, 1);

        widget_2 = new QWidget(widget_out);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMaximumSize(QSize(150, 150));
        gridLayout_9 = new QGridLayout(widget_2);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        label_pITURE = new QLabel(widget_2);
        label_pITURE->setObjectName(QStringLiteral("label_pITURE"));
        label_pITURE->setMinimumSize(QSize(128, 128));
        label_pITURE->setMaximumSize(QSize(128, 128));
        label_pITURE->setLayoutDirection(Qt::LeftToRight);
        label_pITURE->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(label_pITURE, 0, 0, 1, 1);


        gridLayout_4->addWidget(widget_2, 0, 0, 1, 1);


        gridLayout->addWidget(widget_out, 5, 1, 1, 6);

        widget_Loss = new QWidget(centralWidget);
        widget_Loss->setObjectName(QStringLiteral("widget_Loss"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_Loss->sizePolicy().hasHeightForWidth());
        widget_Loss->setSizePolicy(sizePolicy3);
        verticalLayout_6 = new QVBoxLayout(widget_Loss);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_Loss = new QVBoxLayout();
        verticalLayout_Loss->setSpacing(6);
        verticalLayout_Loss->setObjectName(QStringLiteral("verticalLayout_Loss"));

        verticalLayout_6->addLayout(verticalLayout_Loss);


        gridLayout->addWidget(widget_Loss, 5, 7, 1, 1);

        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QStringLiteral("treeView"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy4);

        gridLayout->addWidget(treeView, 1, 7, 4, 1);

        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        gridLayout_8 = new QGridLayout(groupBox_7);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        label_37 = new QLabel(groupBox_7);
        label_37->setObjectName(QStringLiteral("label_37"));

        gridLayout_8->addWidget(label_37, 12, 0, 1, 1);

        label_42 = new QLabel(groupBox_7);
        label_42->setObjectName(QStringLiteral("label_42"));

        gridLayout_8->addWidget(label_42, 0, 0, 1, 1);

        pushButton_ImportIdealOut = new QPushButton(groupBox_7);
        pushButton_ImportIdealOut->setObjectName(QStringLiteral("pushButton_ImportIdealOut"));
        sizePolicy1.setHeightForWidth(pushButton_ImportIdealOut->sizePolicy().hasHeightForWidth());
        pushButton_ImportIdealOut->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(pushButton_ImportIdealOut, 2, 1, 1, 1);

        lineEdit_NormalizedSHif = new QLineEdit(groupBox_7);
        lineEdit_NormalizedSHif->setObjectName(QStringLiteral("lineEdit_NormalizedSHif"));

        gridLayout_8->addWidget(lineEdit_NormalizedSHif, 9, 1, 1, 1);

        label_36 = new QLabel(groupBox_7);
        label_36->setObjectName(QStringLiteral("label_36"));

        gridLayout_8->addWidget(label_36, 11, 0, 1, 1);

        label_35 = new QLabel(groupBox_7);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout_8->addWidget(label_35, 7, 0, 1, 1);

        line = new QFrame(groupBox_7);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_8->addWidget(line, 6, 0, 1, 2);

        pushButton_importImage = new QPushButton(groupBox_7);
        pushButton_importImage->setObjectName(QStringLiteral("pushButton_importImage"));
        sizePolicy1.setHeightForWidth(pushButton_importImage->sizePolicy().hasHeightForWidth());
        pushButton_importImage->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(pushButton_importImage, 1, 1, 1, 1);

        lineEdit_diffLoss = new QLineEdit(groupBox_7);
        lineEdit_diffLoss->setObjectName(QStringLiteral("lineEdit_diffLoss"));
        sizePolicy1.setHeightForWidth(lineEdit_diffLoss->sizePolicy().hasHeightForWidth());
        lineEdit_diffLoss->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(lineEdit_diffLoss, 13, 1, 1, 1);

        label_41 = new QLabel(groupBox_7);
        label_41->setObjectName(QStringLiteral("label_41"));

        gridLayout_8->addWidget(label_41, 8, 0, 1, 1);

        lineEdit_Loss = new QLineEdit(groupBox_7);
        lineEdit_Loss->setObjectName(QStringLiteral("lineEdit_Loss"));
        sizePolicy1.setHeightForWidth(lineEdit_Loss->sizePolicy().hasHeightForWidth());
        lineEdit_Loss->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(lineEdit_Loss, 12, 1, 1, 1);

        label_43 = new QLabel(groupBox_7);
        label_43->setObjectName(QStringLiteral("label_43"));

        gridLayout_8->addWidget(label_43, 9, 0, 1, 1);

        label_38 = new QLabel(groupBox_7);
        label_38->setObjectName(QStringLiteral("label_38"));

        gridLayout_8->addWidget(label_38, 13, 0, 1, 1);

        label_40 = new QLabel(groupBox_7);
        label_40->setObjectName(QStringLiteral("label_40"));

        gridLayout_8->addWidget(label_40, 2, 0, 1, 1);

        pushButton_cifarTrains = new QPushButton(groupBox_7);
        pushButton_cifarTrains->setObjectName(QStringLiteral("pushButton_cifarTrains"));

        gridLayout_8->addWidget(pushButton_cifarTrains, 3, 0, 1, 2);

        label = new QLabel(groupBox_7);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_8->addWidget(label, 1, 0, 1, 1);

        label_Traingmode = new QLabel(groupBox_7);
        label_Traingmode->setObjectName(QStringLiteral("label_Traingmode"));

        gridLayout_8->addWidget(label_Traingmode, 16, 0, 1, 1);

        lineEdit_learnRate = new QLineEdit(groupBox_7);
        lineEdit_learnRate->setObjectName(QStringLiteral("lineEdit_learnRate"));
        sizePolicy1.setHeightForWidth(lineEdit_learnRate->sizePolicy().hasHeightForWidth());
        lineEdit_learnRate->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(lineEdit_learnRate, 11, 1, 1, 1);

        lineEdit_NormalizedRange = new QLineEdit(groupBox_7);
        lineEdit_NormalizedRange->setObjectName(QStringLiteral("lineEdit_NormalizedRange"));

        gridLayout_8->addWidget(lineEdit_NormalizedRange, 8, 1, 1, 1);

        pushButton_startTraning = new QPushButton(groupBox_7);
        pushButton_startTraning->setObjectName(QStringLiteral("pushButton_startTraning"));
        QPalette palette1;
        QBrush brush7(QColor(245, 226, 255, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        QBrush brush8(QColor(85, 255, 0, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush8);
        QBrush brush9(QColor(170, 255, 127, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush9);
        QBrush brush10(QColor(127, 255, 63, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush10);
        QBrush brush11(QColor(42, 127, 0, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush11);
        QBrush brush12(QColor(56, 170, 0, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush12);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush11);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush12);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush11);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        pushButton_startTraning->setPalette(palette1);
        pushButton_startTraning->setStyleSheet(QStringLiteral("color: rgb(245, 226, 255);"));

        gridLayout_8->addWidget(pushButton_startTraning, 16, 1, 1, 1);

        comboBox_normalizedMethd = new QComboBox(groupBox_7);
        comboBox_normalizedMethd->setObjectName(QStringLiteral("comboBox_normalizedMethd"));

        gridLayout_8->addWidget(comboBox_normalizedMethd, 7, 1, 1, 1);

        lineEdit_maxIters = new QLineEdit(groupBox_7);
        lineEdit_maxIters->setObjectName(QStringLiteral("lineEdit_maxIters"));
        sizePolicy1.setHeightForWidth(lineEdit_maxIters->sizePolicy().hasHeightForWidth());
        lineEdit_maxIters->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(lineEdit_maxIters, 14, 1, 1, 1);

        label_26 = new QLabel(groupBox_7);
        label_26->setObjectName(QStringLiteral("label_26"));

        gridLayout_8->addWidget(label_26, 15, 0, 1, 1);

        label_39 = new QLabel(groupBox_7);
        label_39->setObjectName(QStringLiteral("label_39"));

        gridLayout_8->addWidget(label_39, 14, 0, 1, 1);

        lineEdit_ImgNumbers = new QLineEdit(groupBox_7);
        lineEdit_ImgNumbers->setObjectName(QStringLiteral("lineEdit_ImgNumbers"));
        sizePolicy1.setHeightForWidth(lineEdit_ImgNumbers->sizePolicy().hasHeightForWidth());
        lineEdit_ImgNumbers->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(lineEdit_ImgNumbers, 0, 1, 1, 1);

        label_28 = new QLabel(groupBox_7);
        label_28->setObjectName(QStringLiteral("label_28"));

        gridLayout_8->addWidget(label_28, 10, 0, 1, 1);

        spinBox_batchSize = new QSpinBox(groupBox_7);
        spinBox_batchSize->setObjectName(QStringLiteral("spinBox_batchSize"));
        spinBox_batchSize->setMaximum(128);
        spinBox_batchSize->setValue(1);

        gridLayout_8->addWidget(spinBox_batchSize, 10, 1, 1, 1);

        comboBox_useSimd = new QComboBox(groupBox_7);
        comboBox_useSimd->setObjectName(QStringLiteral("comboBox_useSimd"));
        comboBox_useSimd->setStyleSheet(QStringLiteral("background-color: rgb(218, 203, 218);"));

        gridLayout_8->addWidget(comboBox_useSimd, 15, 1, 1, 1);


        gridLayout->addWidget(groupBox_7, 1, 6, 4, 1);

        FNNClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FNNClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1668, 26));
        FNNClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FNNClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FNNClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FNNClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FNNClass->setStatusBar(statusBar);

        retranslateUi(FNNClass);

        comboBox_normalizedMethd->setCurrentIndex(2);
        comboBox_useSimd->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(FNNClass);
    } // setupUi

    void retranslateUi(QMainWindow *FNNClass)
    {
        FNNClass->setWindowTitle(QApplication::translate("FNNClass", "FNN", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("FNNClass", "kernal", Q_NULLPTR));
        label_34->setText(QApplication::translate("FNNClass", "LayerType", Q_NULLPTR));
        label_2->setText(QApplication::translate("FNNClass", "Row/Dim1", Q_NULLPTR));
        label_14->setText(QApplication::translate("FNNClass", "Pooling Fun", Q_NULLPTR));
        label_29->setText(QApplication::translate("FNNClass", "Set & Add", Q_NULLPTR));
        label_33->setText(QApplication::translate("FNNClass", "padding", Q_NULLPTR));
        pushButton_addLayer->setText(QString());
        label_4->setText(QApplication::translate("FNNClass", "Neuro Num", Q_NULLPTR));
        label_10->setText(QApplication::translate("FNNClass", "Pooling Row/Dim1", Q_NULLPTR));
        comboBox_LayerType->clear();
        comboBox_LayerType->insertItems(0, QStringList()
         << QApplication::translate("FNNClass", "CONVOLUTION", Q_NULLPTR)
         << QApplication::translate("FNNClass", "FULLYCONNECTION", Q_NULLPTR)
        );
        comboBox_padding->clear();
        comboBox_padding->insertItems(0, QStringList()
         << QApplication::translate("FNNClass", "smaller", Q_NULLPTR)
         << QApplication::translate("FNNClass", "same", Q_NULLPTR)
        );
        comboBox_padding->setCurrentText(QApplication::translate("FNNClass", "smaller", Q_NULLPTR));
        label_5->setText(QApplication::translate("FNNClass", "Stride", Q_NULLPTR));
        label_15->setText(QApplication::translate("FNNClass", "Activate Fun", Q_NULLPTR));
        label_3->setText(QApplication::translate("FNNClass", "Col/Dim2", Q_NULLPTR));
        lineEdit_initSd->setText(QApplication::translate("FNNClass", "1", Q_NULLPTR));
        label_24->setText(QApplication::translate("FNNClass", "Init SD", Q_NULLPTR));
        label_12->setText(QApplication::translate("FNNClass", "Pooling Stride", Q_NULLPTR));
        label_11->setText(QApplication::translate("FNNClass", "Pooling Col/Dim2", Q_NULLPTR));
        label_22->setText(QApplication::translate("FNNClass", "bias", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("FNNClass", "MODEL EDIT", Q_NULLPTR));
        label_27->setText(QApplication::translate("FNNClass", "Pop_back", Q_NULLPTR));
        lineEdit_depth->setText(QApplication::translate("FNNClass", "0", Q_NULLPTR));
        pushButton_lockModel->setText(QString());
        label_16->setText(QApplication::translate("FNNClass", "lock Model", Q_NULLPTR));
        pushButton_ClearModel->setText(QString());
        pushButton_popBackLayer->setText(QString());
        label_32->setText(QApplication::translate("FNNClass", "Clear Model", Q_NULLPTR));
        pushButton->setText(QApplication::translate("FNNClass", "PushButton", Q_NULLPTR));
        label_23->setText(QApplication::translate("FNNClass", "Detpth", Q_NULLPTR));
        label_21->setText(QApplication::translate("FNNClass", "init model", Q_NULLPTR));
        label_30->setText(QApplication::translate("FNNClass", "save Model", Q_NULLPTR));
        pushButton_saveModel->setText(QString());
        lineEdit_modelname->setText(QApplication::translate("FNNClass", ".\\\\FNNModel.net", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("FNNClass", "Test Layer", Q_NULLPTR));
        label_18->setText(QApplication::translate("FNNClass", "view out Image", Q_NULLPTR));
        label_25->setText(QApplication::translate("FNNClass", "input image", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("FNNClass", "Set Input", Q_NULLPTR));
        label_6->setText(QApplication::translate("FNNClass", "OutLen", Q_NULLPTR));
        label_19->setText(QApplication::translate("FNNClass", "start col", Q_NULLPTR));
        label_7->setText(QApplication::translate("FNNClass", "Row", Q_NULLPTR));
        label_9->setText(QApplication::translate("FNNClass", "Channel", Q_NULLPTR));
        label_8->setText(QApplication::translate("FNNClass", "Col", Q_NULLPTR));
        label_17->setText(QApplication::translate("FNNClass", "start row", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("FNNClass", "Model Controller", Q_NULLPTR));
        label_13->setText(QApplication::translate("FNNClass", "Import Model", Q_NULLPTR));
        pushButton_ImportWts->setText(QString());
        label_20->setText(QApplication::translate("FNNClass", "test img num", Q_NULLPTR));
        pushButton_LaunchModel->setText(QApplication::translate("FNNClass", "Launch Model", Q_NULLPTR));
        label_44->setText(QApplication::translate("FNNClass", "Import test Img", Q_NULLPTR));
        lineEdit_testImgNum->setText(QApplication::translate("FNNClass", "10000", Q_NULLPTR));
        pushButton_ImportTestImg->setText(QString());
        pushButton_cifarTest->setText(QApplication::translate("FNNClass", "Import Cifar Test Group", Q_NULLPTR));
        label_pITURE->setText(QApplication::translate("FNNClass", "PICTURE", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("FNNClass", "train", Q_NULLPTR));
        label_37->setText(QApplication::translate("FNNClass", "Loss", Q_NULLPTR));
        label_42->setText(QApplication::translate("FNNClass", "Img Number", Q_NULLPTR));
        pushButton_ImportIdealOut->setText(QString());
        lineEdit_NormalizedSHif->setText(QApplication::translate("FNNClass", "1", Q_NULLPTR));
        label_36->setText(QApplication::translate("FNNClass", "learn rate", Q_NULLPTR));
        label_35->setText(QApplication::translate("FNNClass", "Normalized Mthd", Q_NULLPTR));
        pushButton_importImage->setText(QString());
        lineEdit_diffLoss->setText(QApplication::translate("FNNClass", "0.00000001", Q_NULLPTR));
        label_41->setText(QApplication::translate("FNNClass", "range", Q_NULLPTR));
        lineEdit_Loss->setText(QApplication::translate("FNNClass", "0.001", Q_NULLPTR));
        label_43->setText(QApplication::translate("FNNClass", "shift", Q_NULLPTR));
        label_38->setText(QApplication::translate("FNNClass", "Diff Loss", Q_NULLPTR));
        label_40->setText(QApplication::translate("FNNClass", "Import Ideal Out", Q_NULLPTR));
        pushButton_cifarTrains->setText(QApplication::translate("FNNClass", "Import Cifar Train Group", Q_NULLPTR));
        label->setText(QApplication::translate("FNNClass", "Import Img", Q_NULLPTR));
        label_Traingmode->setText(QApplication::translate("FNNClass", "start", Q_NULLPTR));
        lineEdit_learnRate->setText(QApplication::translate("FNNClass", "0.001", Q_NULLPTR));
        lineEdit_NormalizedRange->setText(QApplication::translate("FNNClass", "255", Q_NULLPTR));
        pushButton_startTraning->setText(QString());
        comboBox_normalizedMethd->clear();
        comboBox_normalizedMethd->insertItems(0, QStringList()
         << QApplication::translate("FNNClass", "None", Q_NULLPTR)
         << QApplication::translate("FNNClass", "global norm", Q_NULLPTR)
         << QApplication::translate("FNNClass", "self norm", Q_NULLPTR)
        );
        lineEdit_maxIters->setText(QApplication::translate("FNNClass", "20", Q_NULLPTR));
        label_26->setText(QApplication::translate("FNNClass", "use simd", Q_NULLPTR));
        label_39->setText(QApplication::translate("FNNClass", "Max iter", Q_NULLPTR));
        lineEdit_ImgNumbers->setText(QApplication::translate("FNNClass", "100", Q_NULLPTR));
        label_28->setText(QApplication::translate("FNNClass", "Batch Size", Q_NULLPTR));
        comboBox_useSimd->clear();
        comboBox_useSimd->insertItems(0, QStringList()
         << QApplication::translate("FNNClass", "Basic", Q_NULLPTR)
         << QApplication::translate("FNNClass", "Simd", Q_NULLPTR)
         << QApplication::translate("FNNClass", "MultiThread Simd", Q_NULLPTR)
         << QApplication::translate("FNNClass", "Cuda", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class FNNClass: public Ui_FNNClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FNN_H
