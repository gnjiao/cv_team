#pragma once



#include <QtWidgets/QWidget>

#include "BaseWidget\ShellWidgetBase.h"

#include "ShapeMatchShell.h"

#include "Eraser_item.h"

#include "geo_item\Rect_item.h"

#include "InputBox.h"

#include <QDebug>

#include <QMessageBox>

#include <QCheckBox>

#include <QVBoxLayout>

#include <QGridLayout>

#include <QSpinBox>

#include <QDoubleSpinBox>

#include <QLineEdit>

#include <QTableWidget>

#include <QBitmap>





class ShapeMatchWidget : public ShellWidgetBase

{

	Q_OBJECT



public:

	ShapeMatchWidget(QWidget *parent = Q_NULLPTR);

	~ShapeMatchWidget();



	InputBox *inputBox_srcImage;



	void setShell(ShapeMatchShell* shell);

	void doConnect();

	void doPlay();

	void doOk();

	void doCancel();

	void doOpenFile(cv::Mat img);

	void doEdit(bool b);

	void doEditSave();



private:

	void setupActions(); //????????§Ö?Action??????????????????????

	void setupInput(); //?????????????

	void setupFindTemplate();

	void setupCreateTemplate();

	void setupOutput(); //?????????????Widget

	void setupDisplays();

	void DrawPointsOnPic(vector<cv::Point>pt, cv::Mat &img, cv::Scalar color); //?????????

	void updateOutput();

	void updateTable();

	ShapeMatchShell* m_shell = nullptr; //?????????

	ShapeMatchShell* m_shell2 = nullptr;

	void syncShells(ShapeMatchShell* shell1, ShapeMatchShell* shell2);



	//cv::Mat m_imgTemplate;

	cv::Mat m_imgShow;

	cv::Mat m_imgCurrent;

	cv::Scalar m_color_creatTemplate;

	cv::Scalar m_color_findTemplate;



	vector<cv::Point> m_pointsFeature; //?????????

	cv::Point m_pointCenter; //??????????

	Rectitem* m_rectItem_template; //?????Rect????

	Eraser_item * m_itemEraser; //?????????

								//??????

	QAction *action_findFeaturePoints; //???????

	QAction *action_eraser;



	QDoubleSpinBox *spinBox_minScore; //??§³?????

	QSlider* slider_minScore;

	QSpinBox * spinBox_matchNum; //???????????

	QSlider* slider_matchNum;

	QDoubleSpinBox *spinBox_minAngle; //??§³???

	QSlider* slider_minAngle;

	QDoubleSpinBox *spinBox_maxAngle; //???????

	QSlider* slider_maxAngle;

	QDoubleSpinBox *spinBox_minScale; //??§³???

	QSlider* slider_minScale;

	QDoubleSpinBox *spinBox_maxScale; //???????

	QSlider* slider_maxScale;

	QDoubleSpinBox *spinBox_greediness;//????

	QSlider* slider_greediness;

	//???????

	QWidget* templatePage; //???????????

	QSpinBox* spinBox_minThreshold; //??§³???

	QSlider* slider_minThreshold;

	QSpinBox* spinBox_maxThreshold; //???????

	QSlider* slider_maxThreshold;

	QSpinBox* spinBox_eraser;

	QSlider* slider_eraser;

	//????????????

	QTableWidget * m_tableWidgetOutParam; //????????§Ò?

										  //???????????

	QCheckBox * m_checkBoxFindROI; //????ROI??????

	QCheckBox * m_checkBoxTemplateROI; //???ROI??????

	QCheckBox * m_checkBoxFeaturePoints; //????????????

	QCheckBox * m_checkBoxMatePoint; //???????????



	private slots:

	void on_inputBox_imageSrc_indexChanged(int index);

	void on_action_findFeaturePoints(); //????????????

	void on_action_eraser(bool b); //??????????

	void on_pointsFromTemplate_erased(); //?????????

	void on_slider_eraser_valueChanged(int value);

	void on_slider_minScore_valueChanged(int value);

	void on_spinBox_minScore_valueChanged(double value);

	void on_slider_minAngle_valueChanged(int value);

	void on_spinBox_minAngle_valueChanged(double value);

	void on_slider_maxAngle_valueChanged(int value);

	void on_spinBox_maxAngle_valueChanged(double value);

	void on_slider_minScale_valueChanged(int value);

	void on_spinBox_minScale_valueChanged(double value);

	void on_slider_maxScale_valueChanged(int value);

	void on_spinBox_maxScale_valueChanged(double value);

	void on_slider_greediness_valueChanged(int value);

	void on_spinBox_greediness_valueChanged(double value);

	void on_slider_minThreshold_valueChanged(int value);

	void on_spinBox_minThreshold_valueChanged(int value);

	void on_slider_maxThreshold_valueChanged(int value);

	void on_spinBox_maxThreshold_valueChanged(int value);



};

