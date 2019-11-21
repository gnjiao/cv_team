#pragma once
#include <QDialog>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QTabWidget>
#include <QTableWidget>
#include <QSplitter>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QColorDialog>
#include <QCheckBox>

#include <opencv2\core\core.hpp>

#include <opencv2\highgui\highgui.hpp>

#include <opencv2\imgproc\imgproc.hpp>

#include "DataModel\FrameView.h"

#include "DataModel\InputBox.h"

#include "DataModel\ExtendedGroupBox.h"

#include "GraphicsItem\EraserItem.h"

#include "GraphicsItem\ROIItem.h"

#include "GraphicsItem\SearchTool.h"


#include "ShapeMatchOperator.h"


class ShapeMatchWidget : public QDialog
{
	Q_OBJECT

public :
	ShapeMatchWidget(QWidget *parent = Q_NULLPTR);

	~ShapeMatchWidget();

	void setRefImage(QImage Image);

	void setOperator(ShapeMatchOperator *opra);

	InputBox* getImageComboBox();

private slots:

	void on_action_openFile_triggered();

	void on_action_play_triggered();

	void on_action_edit_triggered(bool);

	void on_action_edit_finish_triggered();

	void on_action_findFeaturePoint_triggered();


	void on_image_cb_Changed(int);
	
	void on_MinScore_sl_Changed(int);
	
	void on_SearchCount_sl_Changed(int);
	
	void on_MinAngle_sl_Changed(int);
	
	void on_MaxAngle_sl_Changed(int);
	
	void on_MinScale_sl_Changed(int);
	
	void on_MaxScale_sl_Changed(int);
	
	void on_Greedness_sl_Changed(int);
	
	void on_MinThresh_sl_Changed(int);
	
	void on_MaxThresh_sl_Changed(int);
	
	void on_MinScore_Changed(int);
	
	void on_SearchCount_Changed(int);
	
	void on_MinAngle_Changed(int);
	
	void on_MaxAngle_Changed(int);
	
	void on_MinScale_Changed(int);
	
	void on_MaxScale_Changed(int);
	
	void on_Greedness_Changed(int);
	
	void on_MinThresh_Changed(int);
	
	void on_MaxThresh_Changed(int);


	void on_SearchTool_cb_Clicked(bool);
	
	void on_SearchTool_btn_Clicked(bool);
	
	void on_ROI_cb_Clicked(bool);
	
	void on_ROI_btn_Clicked(bool);
	
	void on_Eraser_cb_Clicked(bool);
	
	void on_Eraser_btn_Clicked(bool);

	void on_SearchTool_getcolor(QColor);

	void on_ROI_getcolor(QColor);

	void on_Eraser_getcolor(QColor);

	void on_OK_btn_Clicked(bool);

	void on_Cancel_btn_Clicked(bool);

protected:
	void doOpenFile(cv::Mat src);

private:

	void makeToolBar();
	
	void makeWidget();
	
	void makeLayout();


	void makeBaseWidget();
	
	void makeItemWidget();

	void initWidget();

	void updateOutput();

	void syncOperator(ShapeMatchOperator* AA, ShapeMatchOperator* BB);

	void DrawPointsOnPic(vector<cv::Point>pt, cv::Mat &img, cv::Scalar color);




	QToolBar *toolBar;
	
	QAction *action_openFile;
	
	QAction *action_play;
	
	QAction *action_edit;
	
	QAction *action_edit_finish;
	
	QAction *action_findFeaturePoint;

	QFileDialog *openFileDlg;

	FrameView *fr_view;


	QTabWidget *TabWidget;

	QWidget *BaseParamWidget;

	QWidget *SetItemWidget;

	QWidget *OutputWidget;

	QTableWidget *TableWidget;

	QLabel *botMsgBox;

	QPushButton *OK_btn;

	QPushButton *Cancel_btn;



	InputBox *image_cb;


	QDoubleSpinBox *MinScore_sp;

	QSlider *MinScore_sl;


	QSpinBox *SearchCount_sp;

	QSlider *SearchCount_sl;



	QSpinBox *MinAngle_sp;

	QSlider *MinAngle_sl;


	QSpinBox *MaxAngle_sp;

	QSlider *MaxAngle_sl;



	QDoubleSpinBox *MinScale_sp;

	QSlider *MinScale_sl;



	QDoubleSpinBox *MaxScale_sp;

	QSlider *MaxScale_sl;



	QDoubleSpinBox *Greedness_sp;

	QSlider *Greedness_sl;



	QSpinBox *MinThresh_sp;

	QSlider  *MinThresh_sl;


	QSpinBox *MaxThresh_sp;

	QSlider *MaxThresh_sl;



	QCheckBox *SearchTool_cb;

	QPushButton *SearchTool_btn;


	QCheckBox *ROI_cb;

	QPushButton *ROI_btn;



	QCheckBox *Eraser_cb;

	QPushButton *Eraser_btn;

	QSpinBox *Eraser_sp;

	QSlider *Eraser_sl;


	QColorDialog *m_pColorDialog;

	QColor SearchToolColor = QColor(0, 255, 0);
	
	QColor ROIColor = QColor(0, 0, 255);

	QColor EraserColor = QColor(255, 0, 0);

	//下面是数据部分
private:
	QImage RefImage;

	EraserItem *Eraser;

	ROIItem *ROI;

	SearchTool *Search_tool;


	ShapeMatchOperator *CalOperator;

	ShapeMatchOperator *SaveOperator;


	vector<cv::Point> m_featurePoints;

	cv::Mat m_imgShow;

	cv::Mat m_imgCurrent;

	cv::Scalar m_color_creatTemplate = cv::Scalar(0, 255, 255);

	cv::Scalar m_color_findTemplate = cv::Scalar(255, 255, 0);

};
